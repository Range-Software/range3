/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolverheat.cpp                                          *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   12-th December 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Heat-transfer solver class definition               *
 *********************************************************************/

#include <rblib.h>

#include "rsolverheat.h"
#include "rconvection.h"
#include "rmatrixsolver.h"

void RSolverHeat::_init(const RSolverHeat *pHeatSolver)
{
    if (pHeatSolver)
    {
        this->elementCapacity = pHeatSolver->elementCapacity;
        this->elementConduction = pHeatSolver->elementConduction;
        this->elementDensity = pHeatSolver->elementDensity;
        this->nodeTemperature = pHeatSolver->nodeTemperature;
        this->elementHeat = pHeatSolver->elementHeat;
        this->elementRadiativeHeat = pHeatSolver->elementRadiativeHeat;
        this->elementJouleHeat = pHeatSolver->elementJouleHeat;
        this->elementHeatFlux = pHeatSolver->elementHeatFlux;
    }
}

RSolverHeat::RSolverHeat(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData)
    : RSolverGeneric(pModel,modelFileName,convergenceFileName,sharedData)
{
    this->problemType = R_PROBLEM_HEAT;
    this->_init();
}

RSolverHeat::RSolverHeat(const RSolverHeat &heatSolver)
    : RSolverGeneric(heatSolver)
{
    this->_init(&heatSolver);
}

RSolverHeat::~RSolverHeat()
{
}

RSolverHeat &RSolverHeat::operator =(const RSolverHeat &heatSolver)
{
    RSolverGeneric::operator =(heatSolver);
    this->_init(&heatSolver);
    return (*this);
}

bool RSolverHeat::hasConverged(void) const
{
    return true;
}

double RSolverHeat::findTemperatureScale(void) const
{
    return 1.0;
}

void RSolverHeat::updateScales(void)
{
    this->scales.setMetre(this->findMeshScale());
    this->scales.setKelvin(this->findTemperatureScale());
}

void RSolverHeat::recover(void)
{
    this->recoverVariable(R_VARIABLE_TEMPERATURE,
                          R_VARIABLE_APPLY_NODE,
                          this->pModel->getNNodes(),
                          0,
                          this->nodeTemperature,
                          RVariable::getInitValue(R_VARIABLE_TEMPERATURE));
    this->recoverVariable(R_VARIABLE_HEAT,
                          R_VARIABLE_APPLY_ELEMENT,
                          this->pModel->getNElements(),
                          0,
                          this->elementHeat,
                          0.0);
    this->recoverVariable(R_VARIABLE_HEAT_RADIATION,
                          R_VARIABLE_APPLY_ELEMENT,
                          this->pModel->getNElements(),
                          0,
                          this->elementRadiativeHeat,
                          0.0);
    this->recoverVariable(R_VARIABLE_JOULE_HEAT,
                          R_VARIABLE_APPLY_ELEMENT,
                          this->pModel->getNElements(),
                          0,
                          this->elementJouleHeat,
                          0.0);
}

void RSolverHeat::prepare(void)
{
    RBVector temperatureSetValues;
    RBVector heatSetValues;

    this->generateNodeBook(R_PROBLEM_HEAT);

//    this->pModel->convertNodeToElementVector(this->nodeTemperature,this->elementTemperature);

    this->generateVariableVector(R_VARIABLE_TEMPERATURE,this->elementTemperature,temperatureSetValues,true,this->firstRun,this->firstRun);
    this->generateVariableVector(R_VARIABLE_HEAT,this->elementHeat,heatSetValues,true,this->firstRun,this->firstRun);
    this->generateMaterialVecor(R_MATERIAL_PROPERTY_THERMAL_CONDUCTIVITY,this->elementConduction);
    this->generateMaterialVecor(R_MATERIAL_PROPERTY_HEAT_CAPACITY,this->elementCapacity);
    this->generateMaterialVecor(R_MATERIAL_PROPERTY_DENSITY,this->elementDensity);

    this->pModel->convertElementToNodeVector(this->elementTemperature,temperatureSetValues,this->nodeTemperature,true);

    this->b.resize(this->nodeBook.getNEnabled());
    this->x.resize(this->nodeBook.getNEnabled());

    this->A.clear();
    this->b.fill(0.0);
    this->x.fill(0.0);

    // Prepare point elements.
    for (uint i=0;i<this->pModel->getNPoints();i++)
    {
        RPoint &point = this->pModel->getPoint(i);

        bool abort = false;
        #pragma omp parallel for default(shared)
        for (int64_t j=0;j<int64_t(point.size());j++)
        {
            try
            {
                uint elementID = point.get(j);

                if (!this->computableElements[elementID])
                {
                    continue;
                }

                const RElement &element = this->pModel->getElement(elementID);
                R_ERROR_ASSERT(R_ELEMENT_TYPE_IS_POINT(element.getType()));
                uint nInp = RElement::getNIntegrationPoints(element.getType());
                RRMatrix Me(element.size(),element.size());
                RRMatrix Ke(element.size(),element.size());
                RRVector fe(element.size());

                Me.fill(0.0);
                Ke.fill(0.0);
                fe.fill(0.0);

                for (uint k=0;k<nInp;k++)
                {
                    const RElementShapeFunction &shapeFunc = RElement::getShapeFunction(element.getType(),k);
                    const RRVector &N = shapeFunc.getN();
                    RRMatrix J, Rt;
                    double detJ = this->pModel->getElement(elementID).findJacobian(this->pModel->getNodes(),k,J,Rt);

                    for (unsigned m=0;m<element.size();m++)
                    {
                        for (unsigned n=0;n<element.size();n++)
                        {
                            // Mass
                            if (this->pModel->getTimeSolver().getEnabled())
                            {
                                Me[m][n] += N[m] * N[n]
                                         * this->elementDensity[elementID]
                                         * this->elementCapacity[elementID]
                                         * detJ
                                         * shapeFunc.getW()
                                         * point.getVolume();
                            }
                        }
                        // Force
                        fe[m] += (this->elementHeat[elementID] + this->elementJouleHeat[elementID]) * N[m] * detJ * shapeFunc.getW();
                    }
                }
                #pragma omp critical
                {
                    this->assemblyMatrix(elementID,Me,Ke,fe);
                }
            }
            catch (const RError &rError)
            {
                #pragma omp critical
                {
                    RLogger::error("%s\n",rError.getMessage().toUtf8().constData());
                    abort = true;
                }
            }
        }
        if (abort)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to prepare matrix system.");
        }
    }

    // Prepare line elements.
    for (uint i=0;i<this->pModel->getNLines();i++)
    {
        RLine &line = this->pModel->getLine(i);

        bool abort = false;
        #pragma omp parallel for default(shared)
        for (int64_t j=0;j<int64_t(line.size());j++)
        {
            try
            {
                uint elementID = line.get(j);

                if (!this->computableElements[elementID])
                {
                    continue;
                }

                const RElement &element = this->pModel->getElement(elementID);
                R_ERROR_ASSERT(R_ELEMENT_TYPE_IS_LINE(element.getType()));
                uint nInp = RElement::getNIntegrationPoints(element.getType());
                RRMatrix Me(element.size(),element.size());
                RRMatrix Ke(element.size(),element.size());
                RRVector fe(element.size());

                RRMatrix B(element.size(),1);

                Me.fill(0.0);
                Ke.fill(0.0);
                fe.fill(0.0);

                for (uint k=0;k<nInp;k++)
                {
                    const RElementShapeFunction &shapeFunc = RElement::getShapeFunction(element.getType(),k);
                    const RRVector &N = shapeFunc.getN();
                    const RRMatrix &dN = shapeFunc.getDN();
                    RRMatrix J, Rt;
                    double detJ = this->pModel->getElement(elementID).findJacobian(this->pModel->getNodes(),k,J,Rt);

                    for (uint m=0;m<dN.getNRows();m++)
                    {
                        B[m][0] += dN[m][0]*J[0][0];
                    }

                    for (unsigned m=0;m<element.size();m++)
                    {
                        for (unsigned n=0;n<element.size();n++)
                        {
                            // Conduction
                            double kcnd = (B[m][0]*B[n][0]) * line.getCrossArea() * this->elementConduction[elementID];

                            Ke[m][n] += kcnd * detJ * shapeFunc.getW();

                            // Mass
                            if (this->pModel->getTimeSolver().getEnabled())
                            {
                                Me[m][n] += N[m] * N[n]
                                         * this->elementDensity[elementID]
                                         * this->elementCapacity[elementID]
                                         * detJ
                                         * shapeFunc.getW()
                                         * line.getCrossArea();
                            }
                        }
                        // Force
                        fe[m] += (this->elementHeat[elementID] + this->elementJouleHeat[elementID]) * N[m] * detJ * shapeFunc.getW();
                    }
                }
                #pragma omp critical
                {
                    this->assemblyMatrix(elementID,Me,Ke,fe);
                }
            }
            catch (const RError &rError)
            {
                #pragma omp critical
                {
                    RLogger::error("%s\n",rError.getMessage().toUtf8().constData());
                    abort = true;
                }
            }
        }
        if (abort)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to prepare matrix system.");
        }
    }

    // Prepare surface elements.
    for (uint i=0;i<this->pModel->getNSurfaces();i++)
    {
        RSurface &surface = this->pModel->getSurface(i);

        double htc = 0.0;
        double htt = 0.0;

        this->getSimpleConvection(surface,htc,htt);
        this->getForcedConvection(surface,htc,htt);

        bool abort = false;
        #pragma omp parallel for default(shared)
        for (int64_t j=0;j<int64_t(surface.size());j++)
        {
            try
            {
                uint elementID = surface.get(j);

                if (!this->computableElements[elementID])
                {
                    continue;
                }

                const RElement &element = this->pModel->getElement(elementID);
                R_ERROR_ASSERT(R_ELEMENT_TYPE_IS_SURFACE(element.getType()));
                uint nInp = RElement::getNIntegrationPoints(element.getType());
                RRMatrix Me(element.size(),element.size());
                RRMatrix Ke(element.size(),element.size());
                RRVector fe(element.size());
                RRMatrix B(element.size(),2);

                this->getNaturalConvection(surface,elementID,htc,htt);

                Me.fill(0.0);
                Ke.fill(0.0);
                fe.fill(0.0);

                for (uint k=0;k<nInp;k++)
                {
                    const RElementShapeFunction &shapeFunc = RElement::getShapeFunction(element.getType(),k);
                    const RRVector &N = shapeFunc.getN();
                    const RRMatrix &dN = shapeFunc.getDN();
                    RRMatrix J, Rt;
                    double detJ = this->pModel->getElement(elementID).findJacobian(this->pModel->getNodes(),k,J,Rt);

                    B.fill(0.0);
                    for (uint m=0;m<dN.getNRows();m++)
                    {
                        B[m][0] += (dN[m][0]*J[0][0] + dN[m][1]*J[0][1]);
                        B[m][1] += (dN[m][0]*J[1][0] + dN[m][1]*J[1][1]);
                    }

                    for (unsigned m=0;m<element.size();m++)
                    {
                        for (unsigned n=0;n<element.size();n++)
                        {
                            // Conduction
                            double kcnd = (B[m][0]*B[n][0]+B[m][1]*B[n][1]) * surface.getThickness() * this->elementConduction[elementID];
                            // Convection
                            double kcnv = N[m] * N[n] * htc;

                            Ke[m][n] += (kcnd + kcnv) * detJ * shapeFunc.getW();

                            // Mass
                            if (this->pModel->getTimeSolver().getEnabled())
                            {
                                Me[m][n] += N[m] * N[n]
                                         * this->elementDensity[elementID]
                                         * this->elementCapacity[elementID]
                                         * detJ
                                         * shapeFunc.getW()
                                         * surface.getThickness();
                            }
                        }
                        // Force
                        fe[m] += (this->elementHeat[elementID] + this->elementRadiativeHeat[elementID] + this->elementJouleHeat[elementID]) * N[m] * detJ * shapeFunc.getW();
                    }
                }

                // Convection force
                double elementArea = 0.0;
                if (element.findArea(this->pModel->getNodes(),elementArea))
                {
                    for (unsigned m=0;m<element.size();m++)
                    {
                        fe[m] += htc * htt * elementArea / element.size();
                    }
                }
                #pragma omp critical
                {
                    this->assemblyMatrix(elementID,Me,Ke,fe);
                }
            }
            catch (const RError &rError)
            {
                #pragma omp critical
                {
                    RLogger::error("%s\n",rError.getMessage().toUtf8().constData());
                    abort = true;
                }
            }
        }
        if (abort)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to prepare matrix system.");
        }
    }

    // Prepare volume elements.
    for (uint i=0;i<this->pModel->getNVolumes();i++)
    {
        RVolume &volume = this->pModel->getVolume(i);

        bool abort = false;
        #pragma omp parallel for default(shared)
        for (int64_t j=0;j<int64_t(volume.size());j++)
        {
            #pragma omp flush (abort)
            if (abort)
            {
                continue;
            }
            try
            {
                uint elementID = volume.get(j);

                if (!this->computableElements[elementID])
                {
                    continue;
                }

                const RElement &element = this->pModel->getElement(elementID);
                R_ERROR_ASSERT(R_ELEMENT_TYPE_IS_VOLUME(element.getType()));
                uint nInp = RElement::getNIntegrationPoints(element.getType());
                RRMatrix Me(element.size(),element.size());
                RRMatrix Ke(element.size(),element.size());
                RRVector fe(element.size());
                RRMatrix B(element.size(),3);

                Me.fill(0.0);
                Ke.fill(0.0);
                fe.fill(0.0);

                // Conduction
                for (uint k=0;k<nInp;k++)
                {
                    const RElementShapeFunction &shapeFunc = RElement::getShapeFunction(element.getType(),k);
                    const RRVector &N = shapeFunc.getN();
                    const RRMatrix &dN = shapeFunc.getDN();
                    RRMatrix J, Rt;
                    double detJ = this->pModel->getElement(elementID).findJacobian(this->pModel->getNodes(),k,J,Rt);

                    B.fill(0.0);
                    for (uint m=0;m<dN.getNRows();m++)
                    {
                        B[m][0] += (dN[m][0]*J[0][0] + dN[m][1]*J[0][1] + dN[m][2]*J[0][2]);
                        B[m][1] += (dN[m][0]*J[1][0] + dN[m][1]*J[1][1] + dN[m][2]*J[1][2]);
                        B[m][2] += (dN[m][0]*J[2][0] + dN[m][1]*J[2][1] + dN[m][2]*J[2][2]);
                    }

                    for (unsigned m=0;m<element.size();m++)
                    {
                        for (unsigned n=0;n<element.size();n++)
                        {
                            // Conduction
                            Ke[m][n] += (B[m][0]*B[n][0] + B[m][1]*B[n][1] + B[m][2]*B[n][2])
                                     * this->elementConduction[elementID]
                                     * detJ
                                     * shapeFunc.getW();

                            // Mass
                            if (this->pModel->getTimeSolver().getEnabled())
                            {
                                Me[m][n] += N[m] * N[n]
                                         * this->elementDensity[elementID]
                                         * this->elementCapacity[elementID]
                                         * detJ
                                         * shapeFunc.getW();
                            }
                        }
                        // Force
                        fe[m] += (this->elementHeat[elementID] + this->elementJouleHeat[elementID]) * N[m] * detJ * shapeFunc.getW();
                    }
                }
                #pragma omp critical
                {
                    this->assemblyMatrix(elementID,Me,Ke,fe);
                }
            }
            catch (const RError &rError)
            {
                #pragma omp critical
                {
                    RLogger::error("%s\n",rError.getMessage().toUtf8().constData());
                    abort = true;
                }
            }
        }
        if (abort)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to prepare matrix system.");
        }
    }
}

void RSolverHeat::solve(void)
{
    try
    {
        RLogger::indent();
        RMatrixSolver matrixSolver(this->pModel->getMatrixSolverConf(RMatrixSolverConf::CG));
        matrixSolver.solve(this->A,this->b,this->x,R_MATRIX_PRECONDITIONER_JACOBI,1);
        RLogger::unindent();
    }
    catch (RError error)
    {
        RLogger::unindent();
        throw error;
    }

    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        uint position;
        if (this->nodeBook.getValue(i,position))
        {
            this->nodeTemperature[i] = this->x[position];
        }
    }

    this->pModel->convertNodeToElementVector(this->nodeTemperature,this->elementTemperature);
}

void RSolverHeat::process(void)
{
//    RLogger::info("Processing results\n");
    double Qx;
    double Qy;
    double Qz;

    // Initialize heat flux vector vector
    this->elementHeatFlux.resize(this->pModel->getNElements(),RR3Vector(0.0,0.0,0.0));

    // Process line elements.
    for (uint i=0;i<this->pModel->getNLines();i++)
    {
        RLine &line = this->pModel->getLine(i);

        for (uint j=0;j<line.size();j++)
        {
            uint elementID = line.get(j);

            if (!this->computableElements[elementID])
            {
                continue;
            }

            const RElement &element = this->pModel->getElement(elementID);
            uint nInp = RElement::getNIntegrationPoints(element.getType());
            RRVector B(element.size());

            Qx = Qy = Qz = 0.0;

            // Conduction
            if (line.getCrossArea() > 0.0)
            {
                double Qi = 0.0;

                for (uint k=0;k<nInp;k++)
                {
                    const RElementShapeFunction &shapeFunc = RElement::getShapeFunction(element.getType(),k);
                    const RRMatrix &dN = shapeFunc.getDN();
                    RRMatrix J, Rt;
                    double detJ = this->pModel->getElement(elementID).findJacobian(this->pModel->getNodes(),k,J,Rt);

                    if (line.getCrossArea() != 0.0)
                    {
                        for (uint m=0;m<dN.getNRows();m++)
                        {
                            B[m] += dN[m][0] * J[0][0] * detJ / double(nInp);
                        }
                    }
                }

                for (uint k=0;k<element.size();k++)
                {
                    uint nodeID = element.getNodeId(k);

                    Qi -= B[k] * this->elementConduction[elementID] * this->nodeTemperature[nodeID];
                }

                RRMatrix R;
                RRVector t;
                this->pModel->getElement(elementID).findTransformationMatrix(this->pModel->getNodes(),R,t);

                Qx += R[0][0]*Qi;
                Qy += R[1][0]*Qi;
                Qz += R[2][0]*Qi;
            }

            this->elementHeatFlux[elementID][0] = Qx;
            this->elementHeatFlux[elementID][1] = Qy;
            this->elementHeatFlux[elementID][2] = Qz;
        }
    }

    // Process surface elements.
    for (uint i=0;i<this->pModel->getNSurfaces();i++)
    {
        RSurface &surface = this->pModel->getSurface(i);

        double htc = 0.0;
        double htt = 0.0;

        this->getSimpleConvection(surface,htc,htt);
        this->getForcedConvection(surface,htc,htt);

        for (uint j=0;j<surface.size();j++)
        {
            uint elementID = surface.get(j);

            if (!this->computableElements[elementID])
            {
                continue;
            }

            const RElement &element = this->pModel->getElement(elementID);
            uint nInp = RElement::getNIntegrationPoints(element.getType());
            RRMatrix B(element.size(),2);

            this->getNaturalConvection(surface,elementID,htc,htt);

            Qx = Qy = Qz = 0.0;

            // Conduction
            if (surface.getThickness() > 0.0)
            {
                double Qi = 0.0;
                double Qj = 0.0;

                for (uint k=0;k<nInp;k++)
                {
                    const RElementShapeFunction &shapeFunc = RElement::getShapeFunction(element.getType(),k);
                    const RRMatrix &dN = shapeFunc.getDN();
                    RRMatrix J, Rt;
                    double detJ = this->pModel->getElement(elementID).findJacobian(this->pModel->getNodes(),k,J,Rt);

                    if (surface.getThickness() != 0.0)
                    {
                        for (uint m=0;m<dN.getNRows();m++)
                        {
                            B[m][0] += (dN[m][0]*J[0][0] + dN[m][1]*J[0][1]) * detJ / double(nInp);
                            B[m][1] += (dN[m][0]*J[1][0] + dN[m][1]*J[1][1]) * detJ / double(nInp);
                        }
                    }
                }

                for (uint k=0;k<element.size();k++)
                {
                    uint nodeID = element.getNodeId(k);

                    Qi -= B[k][0] * this->elementConduction[elementID] * this->nodeTemperature[nodeID];
                    Qj -= B[k][1] * this->elementConduction[elementID] * this->nodeTemperature[nodeID];
                }

                RRMatrix R;
                RRVector t;
                this->pModel->getElement(elementID).findTransformationMatrix(this->pModel->getNodes(),R,t);

                Qx += R[0][0]*Qi + R[0][1]*Qj;
                Qy += R[1][0]*Qi + R[1][1]*Qj;
                Qz += R[2][0]*Qi + R[2][1]*Qj;
            }

            // Convection
            if (htc > 0.0)
            {
                RR3Vector normal;
                double area;
                if (element.findNormal(this->pModel->getNodes(),normal[0],normal[1],normal[2]) &&
                    element.findArea(this->pModel->getNodes(),area))
                {
                    double Qhe = htc * area * (htt - this->elementTemperature[elementID]);
                    Qx += normal[0] * Qhe;
                    Qy += normal[1] * Qhe;
                    Qz += normal[2] * Qhe;
                }
            }


            this->elementHeatFlux[elementID][0] = Qx;
            this->elementHeatFlux[elementID][1] = Qy;
            this->elementHeatFlux[elementID][2] = Qz;
        }
    }

    // Process volume elements.
    for (uint i=0;i<this->pModel->getNVolumes();i++)
    {
        RVolume &volume = this->pModel->getVolume(i);

        for (uint j=0;j<volume.size();j++)
        {
            uint elementID = volume.get(j);

            if (!this->computableElements[elementID])
            {
                continue;
            }

            const RElement &element = this->pModel->getElement(elementID);
            uint nInp = RElement::getNIntegrationPoints(element.getType());
            RRMatrix B(element.size(),3);

            Qx = Qy = Qz = 0.0;

            B.fill(0.0);

            // Conduction
            for (uint k=0;k<nInp;k++)
            {
                const RElementShapeFunction &shapeFunc = RElement::getShapeFunction(element.getType(),k);
                const RRMatrix &dN = shapeFunc.getDN();
                RRMatrix J, Rt;
                double detJ = this->pModel->getElement(elementID).findJacobian(this->pModel->getNodes(),k,J,Rt);

                for (uint m=0;m<dN.getNRows();m++)
                {
                    B[m][0] += (dN[m][0]*J[0][0] + dN[m][1]*J[0][1] + dN[m][2]*J[0][2]) * detJ / double(nInp);
                    B[m][1] += (dN[m][0]*J[1][0] + dN[m][1]*J[1][1] + dN[m][2]*J[1][2]) * detJ / double(nInp);
                    B[m][2] += (dN[m][0]*J[2][0] + dN[m][1]*J[2][1] + dN[m][2]*J[2][2]) * detJ / double(nInp);
                }
            }

            for (uint m=0;m<element.size();m++)
            {
                uint nodeID = element.getNodeId(m);

                Qx -= B[m][0] * this->elementConduction[elementID] * this->nodeTemperature[nodeID];
                Qy -= B[m][1] * this->elementConduction[elementID] * this->nodeTemperature[nodeID];
                Qz -= B[m][2] * this->elementConduction[elementID] * this->nodeTemperature[nodeID];
            }

            this->elementHeatFlux[elementID][0] = Qx;
            this->elementHeatFlux[elementID][1] = Qy;
            this->elementHeatFlux[elementID][2] = Qz;
        }
    }
}

void RSolverHeat::store(void)
{
    RLogger::info("Storing results\n");
    RLogger::indent();

    // Temperature
    uint temperaturePos = this->pModel->findVariable(R_VARIABLE_TEMPERATURE);
    if (temperaturePos == RConstants::eod)
    {
        temperaturePos = this->pModel->addVariable(R_VARIABLE_TEMPERATURE);
        this->pModel->getVariable(temperaturePos).getVariableData().setMinMaxDisplayValue(
                    RStatistics::findMinimumValue(this->nodeTemperature),
                    RStatistics::findMaximumValue(this->nodeTemperature));
    }
    RVariable &temperature =  this->pModel->getVariable(temperaturePos);

    temperature.setApplyType(R_VARIABLE_APPLY_NODE);
    temperature.resize(1,this->pModel->getNNodes());
    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        temperature.setValue(0,i,this->nodeTemperature[i]);
    }

    // Heat flux
    uint heatFluxPos = this->pModel->findVariable(R_VARIABLE_HEAT_FLUX);
    if (heatFluxPos == RConstants::eod)
    {
        heatFluxPos = this->pModel->addVariable(R_VARIABLE_HEAT_FLUX);
        this->pModel->getVariable(heatFluxPos).getVariableData().setMinMaxDisplayValue(
                    RStatistics::findMinimumMagnitude(this->elementHeatFlux),
                    RStatistics::findMaximumMagnitude(this->elementHeatFlux));
    }
    RVariable &heatFlux =  this->pModel->getVariable(heatFluxPos);

    heatFlux.setApplyType(R_VARIABLE_APPLY_ELEMENT);
    heatFlux.resize(3,this->pModel->getNElements());
    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        heatFlux.setValue(0,i,this->elementHeatFlux[i][0]);
        heatFlux.setValue(1,i,this->elementHeatFlux[i][1]);
        heatFlux.setValue(2,i,this->elementHeatFlux[i][2]);
    }

    RLogger::unindent();
}

void RSolverHeat::statistics(void)
{
    this->printStats(R_VARIABLE_TEMPERATURE);
    this->printStats(R_VARIABLE_HEAT_FLUX);
    this->processMonitoringPoints();
}

void RSolverHeat::assemblyMatrix(uint elementID, const RRMatrix &Me, const RRMatrix &Ke, const RRVector &fe)
{
    double alpha = this->pModel->getTimeSolver().getTimeMarchApproximationCoefficient();
    double dt = this->pModel->getTimeSolver().getCurrentTimeStepSize();

    const RElement &element = this->pModel->getElement(elementID);

    RRMatrix Ae(element.size(),element.size());
    RRVector be(element.size());

    Ae.fill(0.0);
    be.fill(0.0);

    if (this->pModel->getTimeSolver().getEnabled())
    {
        for (unsigned m=0;m<element.size();m++)
        {
            be[m] = dt * fe[m];
            for (unsigned n=0;n<element.size();n++)
            {
                Ae[m][n] = Me[m][n] + alpha * dt * Ke[m][n];
                be[m] += (Me[m][n] - (1.0 - alpha) * dt * Ke[m][n]) * this->nodeTemperature[element.getNodeId(n)];
            }
        }
    }
    else
    {
        Ae = Ke;
        be = fe;
    }

    // Apply explicit boundary conditions.
    for (uint m=0;m<element.size();m++)
    {
        uint position;
        uint nodeID = element.getNodeId(m);
        if (!this->nodeBook.getValue(nodeID,position))
        {
            for (uint n=0;n<element.size();n++)
            {
                be[n] -= Ae[n][m] * this->nodeTemperature[nodeID];
            }
        }
    }


    // Assembly final matrix system
    for (uint m=0;m<element.size();m++)
    {
        uint mp;

        if (this->nodeBook.getValue(element.getNodeId(m),mp))
        {
            this->b[mp] += be[m];
            for (uint n=0;n<element.size();n++)
            {
                uint np = 0;

                if (this->nodeBook.getValue(element.getNodeId(n),np))
                {
                    this->A.addValue(mp,np,Ae[m][n]);
                }
            }
        }
    }
}

bool RSolverHeat::getSimpleConvection(const RElementGroup &elementGroup, double &htc, double &htt)
{
    if (!elementGroup.hasBoundaryCondition(R_BOUNDARY_CONDITION_CONVECTION_SIMPLE))
    {
        return false;
    }

    RBoundaryCondition bc = elementGroup.getBoundaryCondition(R_BOUNDARY_CONDITION_CONVECTION_SIMPLE);
    uint cPos = 0;

    cPos = bc.findComponentPosition(R_VARIABLE_CONVECTION_COEFFICIENT);
    if (cPos == RConstants::eod)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,
                     "Failed to find \'%s\' component in \'%s\' boundary condition.",
                     RVariable::getName(R_VARIABLE_CONVECTION_COEFFICIENT).toUtf8().constData(),
                     RBoundaryCondition::getName(R_BOUNDARY_CONDITION_CONVECTION_SIMPLE).toUtf8().constData());
    }
    htc = bc.getComponent(cPos).get(this->pModel->getTimeSolver().getCurrentTime());

    cPos = bc.findComponentPosition(R_VARIABLE_FLUID_TEMPERATURE);
    if (cPos == RConstants::eod)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,
                     "Failed to find \'%s\' component in \'%s\' boundary condition.",
                     RVariable::getName(R_VARIABLE_FLUID_TEMPERATURE).toUtf8().constData(),
                     RBoundaryCondition::getName(R_BOUNDARY_CONDITION_CONVECTION_SIMPLE).toUtf8().constData());
    }
    htt = bc.getComponent(cPos).get(this->pModel->getTimeSolver().getCurrentTime());

    return true;
}

bool RSolverHeat::getForcedConvection(const RElementGroup &elementGroup, double &htc, double &htt)
{
    if (!elementGroup.hasBoundaryCondition(R_BOUNDARY_CONDITION_CONVECTION_FORCED))
    {
        return false;
    }

    RBoundaryCondition bc = elementGroup.getBoundaryCondition(R_BOUNDARY_CONDITION_CONVECTION_FORCED);
    uint cPos = 0;

    // Fluid temperature
    cPos = bc.findComponentPosition(R_VARIABLE_FLUID_TEMPERATURE);
    if (cPos == RConstants::eod)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,
                     "Failed to find \'%s\' component in \'%s\' boundary condition.",
                     RVariable::getName(R_VARIABLE_FLUID_TEMPERATURE).toUtf8().constData(),
                     RBoundaryCondition::getName(R_BOUNDARY_CONDITION_CONVECTION_FORCED).toUtf8().constData());
    }
    htt = bc.getComponent(cPos).get(this->pModel->getTimeSolver().getCurrentTime());

    // Density
    cPos = bc.findComponentPosition(R_VARIABLE_DENSITY);
    if (cPos == RConstants::eod)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,
                     "Failed to find \'%s\' component in \'%s\' boundary condition.",
                     RVariable::getName(R_VARIABLE_DENSITY).toUtf8().constData(),
                     RBoundaryCondition::getName(R_BOUNDARY_CONDITION_CONVECTION_FORCED).toUtf8().constData());
    }
    double ro = bc.getComponent(cPos).get(this->pModel->getTimeSolver().getCurrentTime());

    // Dynamic viscosity
    cPos = bc.findComponentPosition(R_VARIABLE_DYNAMIC_VISCOSITY);
    if (cPos == RConstants::eod)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,
                     "Failed to find \'%s\' component in \'%s\' boundary condition.",
                     RVariable::getName(R_VARIABLE_DYNAMIC_VISCOSITY).toUtf8().constData(),
                     RBoundaryCondition::getName(R_BOUNDARY_CONDITION_CONVECTION_FORCED).toUtf8().constData());
    }
    double mu = bc.getComponent(cPos).get(this->pModel->getTimeSolver().getCurrentTime());

    // Heat capacity
    cPos = bc.findComponentPosition(R_VARIABLE_HEAT_CAPACITY);
    if (cPos == RConstants::eod)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,
                     "Failed to find \'%s\' component in \'%s\' boundary condition.",
                     RVariable::getName(R_VARIABLE_HEAT_CAPACITY).toUtf8().constData(),
                     RBoundaryCondition::getName(R_BOUNDARY_CONDITION_CONVECTION_FORCED).toUtf8().constData());
    }
    double c = bc.getComponent(cPos).get(this->pModel->getTimeSolver().getCurrentTime());

    // Hydraulic diameter
    cPos = bc.findComponentPosition(R_VARIABLE_HYDRAULIC_DIAMETER);
    if (cPos == RConstants::eod)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,
                     "Failed to find \'%s\' component in \'%s\' boundary condition.",
                     RVariable::getName(R_VARIABLE_HYDRAULIC_DIAMETER).toUtf8().constData(),
                     RBoundaryCondition::getName(R_BOUNDARY_CONDITION_CONVECTION_FORCED).toUtf8().constData());
    }
    double d = bc.getComponent(cPos).get(this->pModel->getTimeSolver().getCurrentTime());

    // Thermal conductivity
    cPos = bc.findComponentPosition(R_VARIABLE_THERMAL_CONDUCTIVITY);
    if (cPos == RConstants::eod)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,
                     "Failed to find \'%s\' component in \'%s\' boundary condition.",
                     RVariable::getName(R_VARIABLE_THERMAL_CONDUCTIVITY).toUtf8().constData(),
                     RBoundaryCondition::getName(R_BOUNDARY_CONDITION_CONVECTION_FORCED).toUtf8().constData());
    }
    double k = bc.getComponent(cPos).get(this->pModel->getTimeSolver().getCurrentTime());

    // Mean velocity
    cPos = bc.findComponentPosition(R_VARIABLE_VELOCITY);
    if (cPos == RConstants::eod)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,
                     "Failed to find \'%s\' component in \'%s\' boundary condition.",
                     RVariable::getName(R_VARIABLE_VELOCITY).toUtf8().constData(),
                     RBoundaryCondition::getName(R_BOUNDARY_CONDITION_CONVECTION_FORCED).toUtf8().constData());
    }
    double v = bc.getComponent(cPos).get(this->pModel->getTimeSolver().getCurrentTime());

    RConvection convection;

    convection.setType(R_CONVECTION_FORCED_EXTERNAL);
    convection.setMaterial("Custom material",mu,ro,k,c,0.0);
    convection.setDiameter(d);
    convection.setVelocity(v);
    convection.setFluidTemp(htt);

    htc = convection.calculateHtc();

    return true;
}

bool RSolverHeat::getNaturalConvection(const RElementGroup &elementGroup, uint elementId, double &htc, double &htt)
{
    if (!elementGroup.hasBoundaryCondition(R_BOUNDARY_CONDITION_CONVECTION_NATURAL))
    {
        return false;
    }

    RBoundaryCondition bc = elementGroup.getBoundaryCondition(R_BOUNDARY_CONDITION_CONVECTION_NATURAL);
    uint cPos = 0;

    // Density
    cPos = bc.findComponentPosition(R_VARIABLE_DENSITY);
    if (cPos == RConstants::eod)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,
                     "Failed to find \'%s\' component in \'%s\' boundary condition.",
                     RVariable::getName(R_VARIABLE_DENSITY).toUtf8().constData(),
                     RBoundaryCondition::getName(R_BOUNDARY_CONDITION_CONVECTION_NATURAL).toUtf8().constData());
    }
    double ro = bc.getComponent(cPos).get(this->pModel->getTimeSolver().getCurrentTime());

    // Dynamic viscosity
    cPos = bc.findComponentPosition(R_VARIABLE_DYNAMIC_VISCOSITY);
    if (cPos == RConstants::eod)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,
                     "Failed to find \'%s\' component in \'%s\' boundary condition.",
                     RVariable::getName(R_VARIABLE_DYNAMIC_VISCOSITY).toUtf8().constData(),
                     RBoundaryCondition::getName(R_BOUNDARY_CONDITION_CONVECTION_NATURAL).toUtf8().constData());
    }
    double mu = bc.getComponent(cPos).get(this->pModel->getTimeSolver().getCurrentTime());

    // Fluid temperature
    cPos = bc.findComponentPosition(R_VARIABLE_FLUID_TEMPERATURE);
    if (cPos == RConstants::eod)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,
                     "Failed to find \'%s\' component in \'%s\' boundary condition.",
                     RVariable::getName(R_VARIABLE_FLUID_TEMPERATURE).toUtf8().constData(),
                     RBoundaryCondition::getName(R_BOUNDARY_CONDITION_CONVECTION_NATURAL).toUtf8().constData());
    }
    htt = bc.getComponent(cPos).get(this->pModel->getTimeSolver().getCurrentTime());

    // Heat capacity
    cPos = bc.findComponentPosition(R_VARIABLE_HEAT_CAPACITY);
    if (cPos == RConstants::eod)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,
                     "Failed to find \'%s\' component in \'%s\' boundary condition.",
                     RVariable::getName(R_VARIABLE_HEAT_CAPACITY).toUtf8().constData(),
                     RBoundaryCondition::getName(R_BOUNDARY_CONDITION_CONVECTION_NATURAL).toUtf8().constData());
    }
    double c = bc.getComponent(cPos).get(this->pModel->getTimeSolver().getCurrentTime());

    // Hydraulic diameter
    cPos = bc.findComponentPosition(R_VARIABLE_HYDRAULIC_DIAMETER);
    if (cPos == RConstants::eod)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,
                     "Failed to find \'%s\' component in \'%s\' boundary condition.",
                     RVariable::getName(R_VARIABLE_HYDRAULIC_DIAMETER).toUtf8().constData(),
                     RBoundaryCondition::getName(R_BOUNDARY_CONDITION_CONVECTION_NATURAL).toUtf8().constData());
    }
    double d = bc.getComponent(cPos).get(this->pModel->getTimeSolver().getCurrentTime());

    // Thermal conductivity
    cPos = bc.findComponentPosition(R_VARIABLE_THERMAL_CONDUCTIVITY);
    if (cPos == RConstants::eod)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,
                     "Failed to find \'%s\' component in \'%s\' boundary condition.",
                     RVariable::getName(R_VARIABLE_THERMAL_CONDUCTIVITY).toUtf8().constData(),
                     RBoundaryCondition::getName(R_BOUNDARY_CONDITION_CONVECTION_NATURAL).toUtf8().constData());
    }
    double k = bc.getComponent(cPos).get(this->pModel->getTimeSolver().getCurrentTime());

    // Thermal expansion coefficient
    cPos = bc.findComponentPosition(R_VARIABLE_THERMAL_EXPANSION_COEFFICIENT);
    if (cPos == RConstants::eod)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,
                     "Failed to find \'%s\' component in \'%s\' boundary condition.",
                     RVariable::getName(R_VARIABLE_THERMAL_EXPANSION_COEFFICIENT).toUtf8().constData(),
                     RBoundaryCondition::getName(R_BOUNDARY_CONDITION_CONVECTION_NATURAL).toUtf8().constData());
    }
    double b = bc.getComponent(cPos).get(this->pModel->getTimeSolver().getCurrentTime());

    RConvection convection;

    convection.setType(R_CONVECTION_NATURAL_EXTERNAL_HORIZONTAL_PLATES);
    convection.setMaterial("Custom material",mu,ro,k,c,b);
    convection.setDiameter(d);
    convection.setSurfTemp(this->elementTemperature[elementId]);
    convection.setFluidTemp(htt);

    htc = convection.calculateHtc();

    return true;
}
