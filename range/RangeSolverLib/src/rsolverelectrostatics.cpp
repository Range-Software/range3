/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolverelectrostatics.cpp                                *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   19-th November 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Electrostatics solver class definition              *
 *********************************************************************/

#include "rsolverelectrostatics.h"
#include "rmatrixsolver.h"

void RSolverElectrostatics::_init(const RSolverElectrostatics *pSolver)
{
    if (pSolver)
    {
        this->nodeElectricPotential = pSolver->nodeElectricPotential;
        this->elementElectricField = pSolver->elementElectricField;
        this->elementCurrentDensity = pSolver->elementCurrentDensity;
        this->elementRelativePermittivity = pSolver->elementRelativePermittivity;
        this->elementElectricConductivity = pSolver->elementElectricConductivity;
        this->elementElectricEnergy = pSolver->elementElectricEnergy;
        this->elementElectricResistivity = pSolver->elementElectricResistivity;
        this->elementJouleHeat = pSolver->elementJouleHeat;
    }
}

RSolverElectrostatics::RSolverElectrostatics(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData)
    : RSolverGeneric(pModel,modelFileName,convergenceFileName,sharedData)
{
    this->problemType = R_PROBLEM_ELECTROSTATICS;
    this->_init();
}

RSolverElectrostatics::RSolverElectrostatics(const RSolverElectrostatics &solver)
    : RSolverGeneric(solver)
{
    this->_init(&solver);
}

RSolverElectrostatics::~RSolverElectrostatics()
{

}

RSolverElectrostatics &RSolverElectrostatics::operator =(const RSolverElectrostatics &solver)
{
    RSolverGeneric::operator =(solver);
    this->_init(&solver);
    return (*this);
}

bool RSolverElectrostatics::hasConverged(void) const
{
    return true;
}

void RSolverElectrostatics::updateScales(void)
{

}

void RSolverElectrostatics::recover(void)
{
    this->recoverVariable(R_VARIABLE_ELECTRIC_POTENTIAL,
                          R_VARIABLE_APPLY_NODE,
                          this->pModel->getNNodes(),
                          0,
                          this->nodeElectricPotential,
                          0.0);
//    this->syncShared("node-electric_potential",this->nodeElectricPotential);
}

void RSolverElectrostatics::prepare(void)
{
    RRVector elementElectricPotential;
    RBVector electricPotentialSetValues;

    RRVector elementChargeDensity;
    RBVector chargeDensitySetValues;

    this->generateNodeBook(R_PROBLEM_ELECTROSTATICS);

    this->generateVariableVector(R_VARIABLE_ELECTRIC_POTENTIAL,elementElectricPotential,electricPotentialSetValues,true,this->firstRun,this->firstRun);
    this->generateVariableVector(R_VARIABLE_CHARGE_DENSITY,elementChargeDensity,chargeDensitySetValues,true,this->firstRun,this->firstRun);

    this->generateMaterialVecor(R_MATERIAL_PROPERTY_RELATIVE_PERMITTIVITY,this->elementRelativePermittivity);
    this->generateMaterialVecor(R_MATERIAL_PROPERTY_ELECTRICAL_CONDUCTIVITY,this->elementElectricConductivity);

    this->pModel->convertElementToNodeVector(elementElectricPotential,electricPotentialSetValues,this->nodeElectricPotential,true);

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
                RRMatrix Ke(element.size(),element.size());
                RRVector fe(element.size());

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
                        // Force
                        fe[m] += elementChargeDensity[elementID] * N[m] * detJ * shapeFunc.getW();
                    }
                }
                #pragma omp critical
                {
                    this->assemblyMatrix(elementID,Ke,fe);
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
                RRMatrix Ke(element.size(),element.size());
                RRVector fe(element.size());

                RRMatrix B(element.size(),1);

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
                            Ke[m][n] += B[m][0] * B[n][0]
                                     * this->elementRelativePermittivity[elementID]
                                     * RSolverGeneric::e0
                                     * detJ
                                     * shapeFunc.getW()
                                     * line.getCrossArea();
                        }
                        // Force
                        fe[m] -= elementChargeDensity[elementID] * N[m] * detJ * shapeFunc.getW();
                    }
                }
                #pragma omp critical
                {
                    this->assemblyMatrix(elementID,Ke,fe);
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
                RRMatrix Ke(element.size(),element.size());
                RRVector fe(element.size());
                RRMatrix B(element.size(),2);

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
                            Ke[m][n] += (B[m][0] * B[n][0] + B[m][1] * B[n][1])
                                     * this->elementRelativePermittivity[elementID]
                                     * RSolverGeneric::e0
                                     * surface.getThickness()
                                     * detJ
                                     * shapeFunc.getW();
                        }
                        // Force
                        fe[m] -= elementChargeDensity[elementID] * N[m] * detJ * shapeFunc.getW();
                    }
                }
                #pragma omp critical
                {
                    this->assemblyMatrix(elementID,Ke,fe);
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
                RRMatrix Ke(element.size(),element.size());
                RRVector fe(element.size());
                RRMatrix B(element.size(),3);

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
                                     * this->elementRelativePermittivity[elementID]
                                     * RSolverGeneric::e0
                                     * detJ
                                     * shapeFunc.getW();
                        }
                        // Force
                        fe[m] -= elementChargeDensity[elementID] * N[m] * detJ * shapeFunc.getW();
                    }
                }
                #pragma omp critical
                {
                    this->assemblyMatrix(elementID,Ke,fe);
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

void RSolverElectrostatics::solve(void)
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
            this->nodeElectricPotential[i] = this->x[position];
        }
    }
}

void RSolverElectrostatics::process(void)
{
    // Initialize vectors
    this->elementElectricField.resize(this->pModel->getNElements(),RR3Vector(0.0,0.0,0.0));
    this->elementCurrentDensity.resize(this->pModel->getNElements(),RR3Vector(0.0,0.0,0.0));
    this->elementElectricEnergy.resize(this->pModel->getNElements(),0.0);
    this->elementElectricResistivity.resize(this->pModel->getNElements(),0.0);
    this->elementJouleHeat.resize(this->pModel->getNElements(),0.0);

    RR3Vector electricField;
    RR3Vector currentDensity;

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

            electricField.fill(0.0);
            currentDensity.fill(0.0);

            // Conduction
            if (line.getCrossArea() > 0.0)
            {
                double Ei = 0.0;

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

                    Ei -= B[k] * this->nodeElectricPotential[nodeID];
                }

                RRMatrix R;
                RRVector t;
                this->pModel->getElement(elementID).findTransformationMatrix(this->pModel->getNodes(),R,t);

                electricField[0] += R[0][0]*Ei;
                electricField[1] += R[1][0]*Ei;
                electricField[2] += R[2][0]*Ei;

                double dElectricField = RRVector::dot(electricField,electricField);
                double ere = std::sqrt(dElectricField);

                currentDensity[0] = electricField[0] * this->elementElectricConductivity[elementID];
                currentDensity[1] = electricField[1] * this->elementElectricConductivity[elementID];
                currentDensity[2] = electricField[2] * this->elementElectricConductivity[elementID];

                double jre = RRVector::norm(currentDensity);

                double elementLength = 0.0;
                element.findLength(this->pModel->getNodes(),elementLength);

                this->elementElectricField[elementID] = electricField;
                this->elementCurrentDensity[elementID] = currentDensity;
                this->elementElectricEnergy[elementID] = this->elementRelativePermittivity[elementID] * RSolverGeneric::e0 * dElectricField / 2.0;
                this->elementElectricResistivity[elementID] = (jre > RConstants::eps ? ere / jre : 0.0);
                this->elementJouleHeat[elementID] = this->elementElectricConductivity[elementID] * dElectricField * elementLength;
            }
        }
    }

    // Process surface elements.
    for (uint i=0;i<this->pModel->getNSurfaces();i++)
    {
        RSurface &surface = this->pModel->getSurface(i);

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

            electricField.fill(0.0);
            currentDensity.fill(0.0);

            // Conduction
            if (surface.getThickness() > 0.0)
            {
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

                double Ei = 0.0;
                double Ej = 0.0;

                for (uint k=0;k<element.size();k++)
                {
                    uint nodeID = element.getNodeId(k);

                    Ei -= B[k][0] * this->nodeElectricPotential[nodeID] * surface.getThickness();
                    Ej -= B[k][1] * this->nodeElectricPotential[nodeID] * surface.getThickness();
                }

                RRMatrix R;
                RRVector t;
                this->pModel->getElement(elementID).findTransformationMatrix(this->pModel->getNodes(),R,t);

                electricField[0] += R[0][0]*Ei + R[0][1]*Ej;
                electricField[1] += R[1][0]*Ei + R[1][1]*Ej;
                electricField[2] += R[2][0]*Ei + R[2][1]*Ej;

                double dElectricField = RRVector::dot(electricField,electricField);
                double ere = std::sqrt(dElectricField);

                currentDensity[0] = electricField[0] * this->elementElectricConductivity[elementID];
                currentDensity[1] = electricField[1] * this->elementElectricConductivity[elementID];
                currentDensity[2] = electricField[2] * this->elementElectricConductivity[elementID];

                double jre = RRVector::norm(currentDensity);

                double elementLength = 0.0;

                RRVector s(2,0.0);
                if (ere > RConstants::eps)
                {
                    s[0] = Ei / ere;
                    s[1] = Ej / ere;
                }
                for (uint m=0;m<element.size();m++)
                {
                    elementLength += std::fabs(s[0] * B[m][0] + s[1] * B[m][1]);
                }
                if (elementLength > RConstants::eps)
                {
                    elementLength = 2.0 / elementLength;
                }

                this->elementElectricField[elementID] = electricField;
                this->elementCurrentDensity[elementID] = currentDensity;
                this->elementElectricEnergy[elementID] = this->elementRelativePermittivity[elementID] * RSolverGeneric::e0 * dElectricField / 2.0;
                this->elementElectricResistivity[elementID] = (jre > RConstants::eps ? ere / jre : 0.0);
                this->elementJouleHeat[elementID] = this->elementElectricConductivity[elementID] * dElectricField * elementLength;
            }
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

            electricField.fill(0.0);
            currentDensity.fill(0.0);

            for (uint m=0;m<element.size();m++)
            {
                uint nodeID = element.getNodeId(m);

                electricField[0] -= B[m][0] * this->nodeElectricPotential[nodeID];
                electricField[1] -= B[m][1] * this->nodeElectricPotential[nodeID];
                electricField[2] -= B[m][2] * this->nodeElectricPotential[nodeID];
            }

            double dElectricField = RRVector::dot(electricField,electricField);
            double ere = std::sqrt(dElectricField);

            currentDensity[0] = electricField[0] * this->elementElectricConductivity[elementID];
            currentDensity[1] = electricField[1] * this->elementElectricConductivity[elementID];
            currentDensity[2] = electricField[2] * this->elementElectricConductivity[elementID];

            double jre = RRVector::norm(currentDensity);

            double elementLength = 0.0;

            RR3Vector s(0.0,0.0,0.0);
            if (ere > RConstants::eps)
            {
                s[0] = electricField[0] / ere;
                s[1] = electricField[1] / ere;
                s[2] = electricField[2] / ere;
            }
            for (uint m=0;m<element.size();m++)
            {
                elementLength += std::fabs(s[0] * B[m][0] + s[1] * B[m][1] + s[2] * B[m][2]);
            }
            if (elementLength > RConstants::eps)
            {
                elementLength = 2.0 / elementLength;
            }

            this->elementElectricField[elementID] = electricField;
            this->elementCurrentDensity[elementID] = currentDensity;
            this->elementElectricEnergy[elementID] = this->elementRelativePermittivity[elementID] * RSolverGeneric::e0 * dElectricField / 2.0;
            this->elementElectricResistivity[elementID] = (jre > RConstants::eps ? ere / jre : 0.0);
            this->elementJouleHeat[elementID] = this->elementElectricConductivity[elementID] * dElectricField * elementLength;
        }
    }
}

void RSolverElectrostatics::store(void)
{
    RLogger::info("Storing results\n");
    RLogger::indent();

    // Electric potential
    uint electricPotentialPos = this->pModel->findVariable(R_VARIABLE_ELECTRIC_POTENTIAL);
    if (electricPotentialPos == RConstants::eod)
    {
        electricPotentialPos = this->pModel->addVariable(R_VARIABLE_ELECTRIC_POTENTIAL);
        this->pModel->getVariable(electricPotentialPos).getVariableData().setMinMaxDisplayValue(
                    RStatistics::findMinimumValue(this->nodeElectricPotential),
                    RStatistics::findMaximumValue(this->nodeElectricPotential));
    }
    RVariable &electricPotential =  this->pModel->getVariable(electricPotentialPos);

    electricPotential.setApplyType(R_VARIABLE_APPLY_NODE);
    electricPotential.resize(1,this->pModel->getNNodes());
    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        electricPotential.setValue(0,i,this->nodeElectricPotential[i]);
    }

    // Electric field
    uint electricFieldPos = this->pModel->findVariable(R_VARIABLE_ELECTRIC_FIELD);
    if (electricFieldPos == RConstants::eod)
    {
        electricFieldPos = this->pModel->addVariable(R_VARIABLE_ELECTRIC_FIELD);
        this->pModel->getVariable(electricFieldPos).getVariableData().setMinMaxDisplayValue(
                    RStatistics::findMinimumMagnitude(this->elementElectricField),
                    RStatistics::findMaximumMagnitude(this->elementElectricField));
    }
    RVariable &electricField =  this->pModel->getVariable(electricFieldPos);

    electricField.setApplyType(R_VARIABLE_APPLY_ELEMENT);
    electricField.resize(3,this->pModel->getNElements());
    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        electricField.setValue(0,i,this->elementElectricField[i][0]);
        electricField.setValue(1,i,this->elementElectricField[i][1]);
        electricField.setValue(2,i,this->elementElectricField[i][2]);
    }

    // Current density
    uint currentDensityPos = this->pModel->findVariable(R_VARIABLE_CURRENT_DENSITY);
    if (currentDensityPos == RConstants::eod)
    {
        currentDensityPos = this->pModel->addVariable(R_VARIABLE_CURRENT_DENSITY);
        this->pModel->getVariable(currentDensityPos).getVariableData().setMinMaxDisplayValue(
                    RStatistics::findMinimumMagnitude(this->elementCurrentDensity),
                    RStatistics::findMaximumMagnitude(this->elementCurrentDensity));
    }
    RVariable &currentDensity =  this->pModel->getVariable(currentDensityPos);

    currentDensity.setApplyType(R_VARIABLE_APPLY_ELEMENT);
    currentDensity.resize(3,this->pModel->getNElements());
    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        currentDensity.setValue(0,i,this->elementCurrentDensity[i][0]);
        currentDensity.setValue(1,i,this->elementCurrentDensity[i][1]);
        currentDensity.setValue(2,i,this->elementCurrentDensity[i][2]);
    }

    // Electric energy
    uint electricEnergyPos = this->pModel->findVariable(R_VARIABLE_ELECTRIC_ENERGY);
    if (electricEnergyPos == RConstants::eod)
    {
        electricEnergyPos = this->pModel->addVariable(R_VARIABLE_ELECTRIC_ENERGY);
        this->pModel->getVariable(electricEnergyPos).getVariableData().setMinMaxDisplayValue(
                    RStatistics::findMinimumValue(this->elementElectricEnergy),
                    RStatistics::findMaximumValue(this->elementElectricEnergy));
    }
    RVariable &electricEnergy =  this->pModel->getVariable(electricEnergyPos);

    electricEnergy.setApplyType(R_VARIABLE_APPLY_ELEMENT);
    electricEnergy.resize(1,this->pModel->getNElements());
    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        electricEnergy.setValue(0,i,this->elementElectricEnergy[i]);
    }

    // Electric resistivity
    uint electricResistivityPos = this->pModel->findVariable(R_VARIABLE_ELECTRICAL_RESISTIVITY);
    if (electricResistivityPos == RConstants::eod)
    {
        electricResistivityPos = this->pModel->addVariable(R_VARIABLE_ELECTRICAL_RESISTIVITY);
        this->pModel->getVariable(electricResistivityPos).getVariableData().setMinMaxDisplayValue(
                    RStatistics::findMinimumValue(this->elementElectricResistivity),
                    RStatistics::findMaximumValue(this->elementElectricResistivity));
    }
    RVariable &electricResistivity =  this->pModel->getVariable(electricResistivityPos);

    electricResistivity.setApplyType(R_VARIABLE_APPLY_ELEMENT);
    electricResistivity.resize(1,this->pModel->getNElements());
    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        electricResistivity.setValue(0,i,this->elementElectricResistivity[i]);
    }

    // Joule heat
    uint jouleHeatPos = this->pModel->findVariable(R_VARIABLE_JOULE_HEAT);
    if (jouleHeatPos == RConstants::eod)
    {
        jouleHeatPos = this->pModel->addVariable(R_VARIABLE_JOULE_HEAT);
        this->pModel->getVariable(jouleHeatPos).getVariableData().setMinMaxDisplayValue(
                    RStatistics::findMinimumValue(this->elementJouleHeat),
                    RStatistics::findMaximumValue(this->elementJouleHeat));
    }
    RVariable &jouleHeat =  this->pModel->getVariable(jouleHeatPos);

    jouleHeat.setApplyType(R_VARIABLE_APPLY_ELEMENT);
    jouleHeat.resize(1,this->pModel->getNElements());
    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        jouleHeat.setValue(0,i,this->elementJouleHeat[i]);
    }

    RLogger::unindent();
}

void RSolverElectrostatics::statistics(void)
{
    this->printStats(R_VARIABLE_ELECTRIC_POTENTIAL);
    this->printStats(R_VARIABLE_ELECTRIC_FIELD);
    this->printStats(R_VARIABLE_CURRENT_DENSITY);
    this->printStats(R_VARIABLE_ELECTRIC_ENERGY);
    this->printStats(R_VARIABLE_ELECTRICAL_RESISTIVITY);
    this->printStats(R_VARIABLE_JOULE_HEAT);
    this->processMonitoringPoints();
}

void RSolverElectrostatics::assemblyMatrix(unsigned int elementID, const RRMatrix &Ke, const RRVector &fe)
{
    const RElement &element = this->pModel->getElement(elementID);

    RRMatrix Ae(Ke);
    RRVector be(fe);

    // Apply explicit boundary conditions.
    for (uint m=0;m<element.size();m++)
    {
        uint position;
        uint nodeID = element.getNodeId(m);
        if (!this->nodeBook.getValue(nodeID,position))
        {
            for (uint n=0;n<element.size();n++)
            {
                be[n] -= Ae[n][m] * this->nodeElectricPotential[nodeID];
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
