/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolverstress.cpp                                        *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   27-th May 2015                                           *
 *                                                                   *
 *  DESCRIPTION: Stress-strain solver class definition               *
 *********************************************************************/

#include <cmath>

#include "rsolverstress.h"
#include "rmatrixsolver.h"
#include "reigenvaluesolver.h"

void RSolverStress::_init(const RSolverStress *pStressSolver)
{
    if (pStressSolver)
    {
        this->problemType = pStressSolver->problemType;
        this->elementElasticityModulus = pStressSolver->elementElasticityModulus;
        this->elementPoissonRatio = pStressSolver->elementPoissonRatio;
        this->elementDensity = pStressSolver->elementDensity;
        this->elementThermalExpansion = pStressSolver->elementThermalExpansion;
        this->elementEnvironmentTemperature = pStressSolver->elementEnvironmentTemperature;
        this->nodeDisplacement = pStressSolver->nodeDisplacement;
        this->nodeInitialDisplacement = pStressSolver->nodeInitialDisplacement;
        this->nodeForce = pStressSolver->nodeForce;
        this->nodeAcceleration = pStressSolver->nodeAcceleration;
        this->elementNormalStress = pStressSolver->elementNormalStress;
        this->elementShearStress = pStressSolver->elementShearStress;
        this->elementVonMisses = pStressSolver->elementVonMisses;
    }
}

RSolverStress::RSolverStress(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData, bool modalAnalysis)
    : RSolverGeneric(pModel,modelFileName,convergenceFileName,sharedData)
{
    this->problemType = modalAnalysis ? R_PROBLEM_STRESS_MODAL : R_PROBLEM_STRESS;
    this->_init();
}

RSolverStress::RSolverStress(const RSolverStress &stressSolver)
    : RSolverGeneric(stressSolver)
{
    this->_init(&stressSolver);
}

RSolverStress::~RSolverStress()
{

}

RSolverStress &RSolverStress::operator =(const RSolverStress &stressSolver)
{
    RSolverGeneric::operator =(stressSolver);
    this->_init(&stressSolver);
    return (*this);
}

bool RSolverStress::hasConverged(void) const
{
    return true;
}

void RSolverStress::updateScales(void)
{
    this->scales.setMetre(this->findMeshScale());
}

void RSolverStress::recover(void)
{
    this->recoverVariable(R_VARIABLE_DISPLACEMENT,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),0,this->nodeDisplacement.x,0.0);
    this->recoverVariable(R_VARIABLE_DISPLACEMENT,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),1,this->nodeDisplacement.y,0.0);
    this->recoverVariable(R_VARIABLE_DISPLACEMENT,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),2,this->nodeDisplacement.z,0.0);
    this->recoverVariable(R_VARIABLE_FORCE,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),0,this->nodeForce.x,0.0);
    this->recoverVariable(R_VARIABLE_FORCE,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),1,this->nodeForce.y,0.0);
    this->recoverVariable(R_VARIABLE_FORCE,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),2,this->nodeForce.z,0.0);
    this->recoverVariable(R_VARIABLE_ACCELERATION,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),0,this->nodeAcceleration.x,0.0);
    this->recoverVariable(R_VARIABLE_ACCELERATION,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),1,this->nodeAcceleration.y,0.0);
    this->recoverVariable(R_VARIABLE_ACCELERATION,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),2,this->nodeAcceleration.z,0.0);
    this->recoverVariable(R_VARIABLE_PRESSURE,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),0,this->nodePressure,0.0);

//    this->syncShared("node-displacement-x",this->nodeDisplacement.x);
//    this->syncShared("node-displacement-y",this->nodeDisplacement.y);
//    this->syncShared("node-displacement-z",this->nodeDisplacement.z);

//    this->syncShared("node-acceleration-x",this->nodeAcceleration.x);
//    this->syncShared("node-acceleration-y",this->nodeAcceleration.y);
//    this->syncShared("node-acceleration-z",this->nodeAcceleration.z);
}

void RSolverStress::prepare(void)
{
    //! Element displacement vector.
    struct { RRVector x, y, z, n; } elementDisplacement;
    struct { RBVector x, y, z, n; } displacementSetValues;

    //! Element force vector.
    RSolverCartesianVector<RRVector> elementForce;
    RSolverCartesianVector<RBVector> forceSetValues;

    //! Element acceleration vector.
    RSolverCartesianVector<RRVector> elementAcceleration;
    RSolverCartesianVector<RBVector> accelerationSetValues;

    //! Element gravity vector.
    RSolverCartesianVector<RRVector> elementGravity;
    RSolverCartesianVector<RBVector> gGravitySetValues;

    //! Element pressure.
    RRVector elementPressure;
    RBVector pressureSetValues;

    //! Element traction per unit area.
    RSolverCartesianVector<RRVector> elementForceUnitArea;
    RSolverCartesianVector<RBVector> forceUnitAreaSetValues;

    //! Element weight.
    RRVector elementWeight;
    RBVector weightSetValues;

    RBVector temperatureSetValues;

    this->generateNodeBook();
    this->generateVariableVector(R_VARIABLE_DISPLACEMENT_X,elementDisplacement.x,displacementSetValues.x,true,this->firstRun,this->firstRun);
    this->generateVariableVector(R_VARIABLE_DISPLACEMENT_Y,elementDisplacement.y,displacementSetValues.y,true,this->firstRun,this->firstRun);
    this->generateVariableVector(R_VARIABLE_DISPLACEMENT_Z,elementDisplacement.z,displacementSetValues.z,true,this->firstRun,this->firstRun);
    this->generateVariableVector(R_VARIABLE_DISPLACEMENT,elementDisplacement.n,displacementSetValues.n,true,this->firstRun,this->firstRun);
    for (uint i=0;i<displacementSetValues.n.size();i++)
    {
        if (displacementSetValues.n[i])
        {
            displacementSetValues.x[i] = true;
            elementDisplacement.x[i] = elementDisplacement.n[i];
        }
    }
    this->generateVariableVector(R_VARIABLE_FORCE_X,elementForce.x,forceSetValues.x,true,this->firstRun,this->firstRun);
    this->generateVariableVector(R_VARIABLE_FORCE_Y,elementForce.y,forceSetValues.y,true,this->firstRun,this->firstRun);
    this->generateVariableVector(R_VARIABLE_FORCE_Z,elementForce.z,forceSetValues.z,true,this->firstRun,this->firstRun);
    this->generateVariableVector(R_VARIABLE_ACCELERATION_X,elementAcceleration.x,accelerationSetValues.x,true,this->firstRun,this->firstRun);
    this->generateVariableVector(R_VARIABLE_ACCELERATION_Y,elementAcceleration.y,accelerationSetValues.y,true,this->firstRun,this->firstRun);
    this->generateVariableVector(R_VARIABLE_ACCELERATION_Z,elementAcceleration.z,accelerationSetValues.z,true,this->firstRun,this->firstRun);
    this->generateVariableVector(R_VARIABLE_G_ACCELERATION_X,elementGravity.x,gGravitySetValues.x,true,true,true);
    this->generateVariableVector(R_VARIABLE_G_ACCELERATION_Y,elementGravity.y,gGravitySetValues.y,true,true,true);
    this->generateVariableVector(R_VARIABLE_G_ACCELERATION_Z,elementGravity.z,gGravitySetValues.z,true,true,true);
    this->generateVariableVector(R_VARIABLE_PRESSURE,elementPressure,pressureSetValues,true,true,true);
    this->generateVariableVector(R_VARIABLE_FORCE_UNIT_AREA_X,elementForceUnitArea.x,forceUnitAreaSetValues.x,true,true,true);
    this->generateVariableVector(R_VARIABLE_FORCE_UNIT_AREA_Y,elementForceUnitArea.y,forceUnitAreaSetValues.y,true,true,true);
    this->generateVariableVector(R_VARIABLE_FORCE_UNIT_AREA_Z,elementForceUnitArea.z,forceUnitAreaSetValues.z,true,true,true);
    this->generateVariableVector(R_VARIABLE_WEIGHT,elementWeight,weightSetValues,true,true,true);
    this->generateMaterialVecor(R_MATERIAL_PROPERTY_MODULUS_OF_ELASTICITY,this->elementElasticityModulus);
    this->generateMaterialVecor(R_MATERIAL_PROPERTY_POISSON_RATIO,this->elementPoissonRatio);
    this->generateMaterialVecor(R_MATERIAL_PROPERTY_DENSITY,this->elementDensity);
    this->generateMaterialVecor(R_MATERIAL_PROPERTY_THERMAL_EXPANSION_COEFFICIENT,this->elementThermalExpansion);
    this->generateVariableVector(R_VARIABLE_TEMPERATURE,this->elementEnvironmentTemperature,temperatureSetValues,false,false,true);

    this->b.resize(this->nodeBook.getNEnabled());
    this->x.resize(this->nodeBook.getNEnabled());

    this->M.clear();
    this->A.clear();
    this->b.fill(0.0);
    this->x.fill(0.0);

    this->pModel->convertElementToNodeVector(elementDisplacement.x,displacementSetValues.x,this->nodeDisplacement.x,true);
    this->pModel->convertElementToNodeVector(elementDisplacement.y,displacementSetValues.y,this->nodeDisplacement.y,true);
    this->pModel->convertElementToNodeVector(elementDisplacement.z,displacementSetValues.z,this->nodeDisplacement.z,true);
    this->pModel->convertElementToNodeVector(elementForce.x,forceSetValues.x,this->nodeForce.x,true);
    this->pModel->convertElementToNodeVector(elementForce.y,forceSetValues.y,this->nodeForce.y,true);
    this->pModel->convertElementToNodeVector(elementForce.z,forceSetValues.z,this->nodeForce.z,true);
    this->pModel->convertElementToNodeVector(elementAcceleration.x,accelerationSetValues.x,this->nodeAcceleration.x,true);
    this->pModel->convertElementToNodeVector(elementAcceleration.y,accelerationSetValues.y,this->nodeAcceleration.y,true);
    this->pModel->convertElementToNodeVector(elementAcceleration.z,accelerationSetValues.z,this->nodeAcceleration.z,true);
    this->pModel->convertElementToNodeVector(elementPressure,pressureSetValues,this->nodePressure,true);

    // Convert node pressure to element pressure
    for (uint i=0;i<elementPressure.size();i++)
    {
        if (!pressureSetValues[i])
        {
            const RElement &rElement = this->pModel->getElement(i);
            uint nne = rElement.size();
            if (nne > 0)
            {
                elementPressure[i] = 0.0;
                for (uint j=0;j<nne;j++)
                {
                    elementPressure[i] += this->nodePressure[rElement.getNodeId(j)];
                }
                elementPressure[i] /= double(nne);
            }
        }
    }

    std::vector<RNode> nodesBkp;
    if (this->problemType == R_PROBLEM_STRESS_MODAL)
    {
        this->nodeInitialDisplacement.x = this->nodeDisplacement.x;
        this->nodeInitialDisplacement.y = this->nodeDisplacement.y;
        this->nodeInitialDisplacement.z = this->nodeDisplacement.z;
        RLogger::info("Moving prestressed nodes\n");
        nodesBkp = this->pModel->getNodes();
        for (uint i=0;i<this->pModel->getNNodes();i++)
        {
            this->pModel->getNode(i).move(RR3Vector(this->nodeDisplacement.x[i],this->nodeDisplacement.y[i],this->nodeDisplacement.z[i]));
        }
    }

    // Prepare point elements.
    for (uint i=0;i<this->pModel->getNPoints();i++)
    {
        RPoint &point = this->pModel->getPoint(i);
        double pointVolume = point.getVolume();

        bool abort = false;
        #pragma omp parallel for default(shared)
        for (int64_t j=0;j<int64_t(point.size());j++)
        {
            #pragma omp flush (abort)
            if (abort)
            {
                continue;
            }
            try
            {
                uint elementID = point.get(uint(j));

                if (!this->computableElements[elementID])
                {
                    continue;
                }

                R_ERROR_ASSERT(R_ELEMENT_TYPE_IS_POINT(this->pModel->getElement(elementID).getType()));
                RRMatrix Me(3,3);
                RRMatrix Ke(3,3);
                RRVector fe(3);

                Me.fill(0.0);
                Ke.fill(0.0);
                fe.fill(0.0);

                // Force
                fe[0] += elementForce.x[elementID];
                fe[1] += elementForce.y[elementID];
                fe[2] += elementForce.z[elementID];
                // Weight
                fe[0] += elementWeight[elementID] * elementGravity.x[elementID];
                fe[1] += elementWeight[elementID] * elementGravity.y[elementID];
                fe[2] += elementWeight[elementID] * elementGravity.z[elementID];
                // Own weight
                if (pointVolume > 0.0)
                {
                    fe[0] += elementGravity.x[elementID] * this->elementDensity[elementID] * pointVolume;
                    fe[1] += elementGravity.y[elementID] * this->elementDensity[elementID] * pointVolume;
                    fe[2] += elementGravity.z[elementID] * this->elementDensity[elementID] * pointVolume;
                }

                // Mass
                if (this->pModel->getTimeSolver().getEnabled() || this->problemType == R_PROBLEM_STRESS_MODAL)
                {
                    Me.setIdentity(3);
                    Me *= this->elementDensity[elementID] * pointVolume;
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
                #pragma omp flush (abort)
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
        double lineCrossArea = line.getCrossArea();

        bool abort = false;
        #pragma omp parallel for default(shared)
        for (int64_t j=0;j<int64_t(line.size());j++)
        {
            #pragma omp flush (abort)
            if (abort)
            {
                continue;
            }
            try
            {
                uint elementID = line.get(uint(j));

                if (!this->computableElements[elementID])
                {
                    continue;
                }

                const RElement &element = this->pModel->getElement(elementID);
                R_ERROR_ASSERT(R_ELEMENT_TYPE_IS_LINE(element.getType()));
                uint nInp = RElement::getNIntegrationPoints(element.getType());
                RRMatrix Me(element.size()*3,element.size()*3,0.0);
                RRMatrix Ke(element.size()*3,element.size()*3,0.0);
                RRVector fe(element.size()*3,0.0);

                RRMatrix Be(3*element.size(),1);
                RRMatrix BeT(1,3*element.size());

                double lineLength = 0.0;
                element.findLength(this->pModel->getNodes(),lineLength);

                double E = this->elementElasticityModulus[elementID];
                double De = E * lineCrossArea;

                double dT = this->elementTemperature[elementID] - this->elementEnvironmentTemperature[elementID];

                for (uint k=0;k<nInp;k++)
                {
                    const RElementShapeFunction &shapeFunc = RElement::getShapeFunction(element.getType(),k);
                    const RRVector &N = shapeFunc.getN();
                    const RRMatrix &dN = shapeFunc.getDN();
                    RRMatrix J, Rt;
                    double detJ = element.findJacobian(this->pModel->getNodes(),k,J,Rt);
                    if (lineCrossArea > 0.0)
                    {
                        Be.fill(0.0);
                        for (uint m=0;m<dN.getNRows();m++)
                        {
                            Be[3*m+0][0] += Rt[3*m+0][0]*dN[m][0]*J[0][0];
                            Be[3*m+1][0] += Rt[3*m+1][0]*dN[m][0]*J[0][0];
                            Be[3*m+2][0] += Rt[3*m+2][0]*dN[m][0]*J[0][0];
                        }
                        BeT.transpose(Be);
                        Be *= De;
                        RRMatrix::mlt(Be,BeT,Ke);
                    }

                    for (uint m=0;m<element.size();m++)
                    {
                        if (lineCrossArea > 0.0)
                        {
                            // Mass
                            if (this->pModel->getTimeSolver().getEnabled() || this->problemType == R_PROBLEM_STRESS_MODAL)
                            {
                                for (uint n=0;n<element.size();n++)
                                {
                                    double value = N[m] * N[n]
                                                 * this->elementDensity[elementID]
                                                 * detJ
                                                 * shapeFunc.getW()
                                                 * lineCrossArea;
                                    Me[3*m+0][3*n+0] += std::pow(Rt[0][0],2.0)*value;
                                    Me[3*m+1][3*n+1] += std::pow(Rt[1][0],2.0)*value;
                                    Me[3*m+2][3*n+2] += std::pow(Rt[2][0],2.0)*value;
                                }
                            }
                        }

                        double integValue = N[m] * detJ * shapeFunc.getW();

                        // Force
                        fe[3*m+0] += (elementForce.x[elementID] / lineLength) * integValue;
                        fe[3*m+1] += (elementForce.y[elementID] / lineLength) * integValue;
                        fe[3*m+2] += (elementForce.z[elementID] / lineLength) * integValue;
                        // Weight
                        fe[3*m+0] += (elementWeight[elementID] * elementGravity.x[elementID] / lineLength) * integValue;
                        fe[3*m+1] += (elementWeight[elementID] * elementGravity.y[elementID] / lineLength) * integValue;
                        fe[3*m+2] += (elementWeight[elementID] * elementGravity.z[elementID] / lineLength) * integValue;
                        // Own weight
                        if (lineCrossArea > 0.0)
                        {
                            fe[3*m+0] += elementGravity.x[elementID] * this->elementDensity[elementID] * lineCrossArea * integValue;
                            fe[3*m+1] += elementGravity.y[elementID] * this->elementDensity[elementID] * lineCrossArea * integValue;
                            fe[3*m+2] += elementGravity.z[elementID] * this->elementDensity[elementID] * lineCrossArea * integValue;
                        }

                        // Thermal expansion
                        if (lineCrossArea > 0.0)
                        {
                            double fet = this->elementThermalExpansion[elementID] * dT * De * Be[m][0] * lineCrossArea * detJ * shapeFunc.getW();

                            fe[3*m+0] += Rt[3*m+0][0]*fet;
                            fe[3*m+1] += Rt[3*m+1][0]*fet;
                            fe[3*m+2] += Rt[3*m+2][0]*fet;
                        }
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
                #pragma omp flush (abort)
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
        double surfaceArea = surface.findArea(this->pModel->getNodes(),this->pModel->getElements());
        double surfaceThickness = surface.getThickness();

        bool abort = false;
        #pragma omp parallel for default(shared)
        for (int64_t j=0;j<int64_t(surface.size());j++)
        {
            #pragma omp flush (abort)
            if (abort)
            {
                continue;
            }
            try
            {
                uint elementID = surface.get(uint(j));

                if (!this->computableElements[elementID] && !this->includableElements[elementID])
                {
                    continue;
                }

                const RElement &element = this->pModel->getElement(elementID);
                R_ERROR_ASSERT(R_ELEMENT_TYPE_IS_SURFACE(element.getType()));
                uint nInp = RElement::getNIntegrationPoints(element.getType());
                RRMatrix Me(element.size()*3,element.size()*3);
                RRMatrix Ke(element.size()*3,element.size()*3);
                RRVector fe(element.size()*3);

                Me.fill(0.0);
                Ke.fill(0.0);
                fe.fill(0.0);

                RRMatrix B(element.size(),3);
                RRMatrix Be(element.size()*2,3);
                RRMatrix BeT(3,element.size()*2);
                RRMatrix BeD(element.size()*2,3);
                RRMatrix Met(element.size()*2,element.size()*2);
                RRMatrix MeRt(element.size()*3,element.size()*2);
                RRMatrix Ket(element.size()*2,element.size()*2);
                RRMatrix KeRt(element.size()*3,element.size()*2);
                RRVector fet(element.size()*2);

                RRMatrix De(3,3,0.0);

                double E = this->elementElasticityModulus[elementID];
                double v = this->elementPoissonRatio[elementID];

                De[0][0] = 1-v;   De[0][1] = v;
                De[1][0] = v;     De[1][1] = 1-v;
                De[2][2] = (1-2*v)/2;
                De *= E/((1+v)*(1-2*v));

                double dT = this->elementTemperature[elementID] - this->elementEnvironmentTemperature[elementID];

                RR3Vector normal;
                element.findNormal(this->pModel->getNodes(),normal[0],normal[1],normal[2]);

                for (uint k=0;k<nInp;k++)
                {
                    const RElementShapeFunction &shapeFunc = RElement::getShapeFunction(element.getType(),k);
                    const RRVector &N = shapeFunc.getN();
                    const RRMatrix &dN = shapeFunc.getDN();
                    RRMatrix J, Rt, RtT;
                    double detJ = element.findJacobian(this->pModel->getNodes(),k,J,Rt);
                    RtT.transpose(Rt);

                    if (surfaceThickness > 0.0)
                    {
                        B.fill(0.0);
                        for (uint m=0;m<dN.getNRows();m++)
                        {
                            B[m][0] += (dN[m][0]*J[0][0] + dN[m][1]*J[0][1]);
                            B[m][1] += (dN[m][0]*J[1][0] + dN[m][1]*J[1][1]);
                        }

                        for (uint m=0;m<element.size();m++)
                        {
                            Be[2*m][0] = B[m][0];   Be[2*m+1][0] = 0.0;
                            Be[2*m][1] = 0.0;       Be[2*m+1][1] = B[m][1];
                            Be[2*m][2] = B[m][1];   Be[2*m+1][2] = B[m][0];
                        }
                        BeT.transpose(Be);

                        RRMatrix::mlt(Be,De,BeD);
                        RRMatrix::mlt(BeD,BeT,Ket);
                        RRMatrix::mlt(Rt,Ket,KeRt);
                        RRMatrix::mlt(KeRt,RtT,Ke);
                        Ke *= detJ * shapeFunc.getW();
                    }

                    for (uint m=0;m<element.size();m++)
                    {
                        if (surfaceThickness > 0.0)
                        {
                            // Mass
                            if (this->pModel->getTimeSolver().getEnabled() || this->problemType == R_PROBLEM_STRESS_MODAL)
                            {
                                for (uint n=0;n<element.size();n++)
                                {
                                    double value = N[m] * N[n]
                                                 * this->elementDensity[elementID]
                                                 * detJ
                                                 * shapeFunc.getW()
                                                 * surfaceThickness;
                                    Met[2*m+0][2*n+0] += value;
                                    Met[2*m+1][2*n+1] += value;
                                }
                            }
                        }

                        double integValue = N[m] * detJ * shapeFunc.getW();

                        // Pressure vector
                        fe[3*m+0] += elementPressure[elementID] * normal[0] * integValue * (this->inwardElements[elementID] ? 1.0 : -1.0);
                        fe[3*m+1] += elementPressure[elementID] * normal[1] * integValue * (this->inwardElements[elementID] ? 1.0 : -1.0);
                        fe[3*m+2] += elementPressure[elementID] * normal[2] * integValue * (this->inwardElements[elementID] ? 1.0 : -1.0);
                        // Force per unit area
                        fe[3*m+0] += elementForceUnitArea.x[elementID] * integValue;
                        fe[3*m+1] += elementForceUnitArea.y[elementID] * integValue;
                        fe[3*m+2] += elementForceUnitArea.z[elementID] * integValue;
                        // Force
                        fe[3*m+0] += (elementForce.x[elementID] / surfaceArea) * integValue;
                        fe[3*m+1] += (elementForce.y[elementID] / surfaceArea) * integValue;
                        fe[3*m+2] += (elementForce.z[elementID] / surfaceArea) * integValue;
                        // Weight
                        fe[3*m+0] += (elementWeight[elementID] * elementGravity.x[elementID] / surfaceArea) * integValue;
                        fe[3*m+1] += (elementWeight[elementID] * elementGravity.y[elementID] / surfaceArea) * integValue;
                        fe[3*m+2] += (elementWeight[elementID] * elementGravity.z[elementID] / surfaceArea) * integValue;
                        // Own weight
                        if (surfaceThickness > 0.0)
                        {
                            fe[3*m+0] += elementGravity.x[elementID] * this->elementDensity[elementID] * surfaceThickness * integValue;
                            fe[3*m+1] += elementGravity.y[elementID] * this->elementDensity[elementID] * surfaceThickness * integValue;
                            fe[3*m+2] += elementGravity.z[elementID] * this->elementDensity[elementID] * surfaceThickness * integValue;
                        }

                        // Thermal expansion
                        if (surfaceThickness > 0.0)
                        {
                            fet.fill(0.0);
                            for (uint n=0;n<3;n++)
                            {
                                fet[2*m+0] += this->elementThermalExpansion[elementID] * dT * BeD[2*m+0][n] * surfaceThickness * detJ * shapeFunc.getW();
                                fet[2*m+1] += this->elementThermalExpansion[elementID] * dT * BeD[2*m+1][n] * surfaceThickness * detJ * shapeFunc.getW();
                            }

                            fe[3*m+0] += Rt[3*m+0][0]*fet[2*m+0] + Rt[3*m+0][1]*fet[2*m+1];
                            fe[3*m+1] += Rt[3*m+1][0]*fet[2*m+0] + Rt[3*m+1][1]*fet[2*m+1];
                            fe[3*m+2] += Rt[3*m+2][0]*fet[2*m+0] + Rt[3*m+2][1]*fet[2*m+1];
                        }
                    }

                    // Mass
                    if (surfaceThickness > 0.0 && (this->pModel->getTimeSolver().getEnabled() || this->problemType == R_PROBLEM_STRESS_MODAL))
                    {
                        RRMatrix::mlt(Rt,Met,MeRt);
                        RRMatrix::mlt(MeRt,RtT,Me,true);
                    }
                    if (!this->computableElements[elementID])
                    {
                        Me.fill(0.0);
                        Ke.fill(0.0);
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
                #pragma omp flush (abort)
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
                uint elementID = volume.get(uint(j));

                if (!this->computableElements[elementID])
                {
                    continue;
                }

                const RElement &element = this->pModel->getElement(elementID);
                R_ERROR_ASSERT(R_ELEMENT_TYPE_IS_VOLUME(element.getType()));
                uint nInp = RElement::getNIntegrationPoints(element.getType());
                RRMatrix Me(element.size()*3,element.size()*3);
                RRMatrix Ke(element.size()*3,element.size()*3);
                RRVector fe(element.size()*3);

                Me.fill(0.0);
                Ke.fill(0.0);
                fe.fill(0.0);

                RRMatrix B(element.size(),3);
                RRMatrix Be(element.size()*3,6);
                RRMatrix BeT(6,element.size()*3);
                RRMatrix BeD(element.size()*3,6);
                RRMatrix Ket(element.size()*3,element.size()*3);

                RRMatrix De(6,6);
                De.fill(0.0);

                double E = this->elementElasticityModulus[elementID];
                double v = this->elementPoissonRatio[elementID];

                De[0][0] = 1.0-v; De[0][1] = v;     De[0][2] = v;
                De[1][0] = v;     De[1][1] = 1.0-v; De[1][2] = v;
                De[2][0] = v;     De[2][1] = v;     De[2][2] = 1.0-v;
                De[3][3] = De[4][4] = De[5][5] = (1.0-2.0*v)/2.0;
                De *= E/((1.0+v)*(1.0-2.0*v));

                double dT = this->elementTemperature[elementID] - this->elementEnvironmentTemperature[elementID];

                for (uint k=0;k<nInp;k++)
                {
                    const RElementShapeFunction &shapeFunc = RElement::getShapeFunction(element.getType(),k);
                    const RRVector &N = shapeFunc.getN();
                    const RRMatrix &dN = shapeFunc.getDN();
                    RRMatrix J, Rt;
                    double detJ = element.findJacobian(this->pModel->getNodes(),k,J,Rt);

                    B.fill(0.0);
                    for (uint m=0;m<dN.getNRows();m++)
                    {
                        B[m][0] += (dN[m][0]*J[0][0] + dN[m][1]*J[0][1] + dN[m][2]*J[0][2]);
                        B[m][1] += (dN[m][0]*J[1][0] + dN[m][1]*J[1][1] + dN[m][2]*J[1][2]);
                        B[m][2] += (dN[m][0]*J[2][0] + dN[m][1]*J[2][1] + dN[m][2]*J[2][2]);
                    }

                    for (uint m=0;m<element.size();m++)
                    {
                        Be[3*m+0][0] = B[m][0];   Be[3*m+1][0] = 0.0;       Be[3*m+2][0] = 0.0;
                        Be[3*m+0][1] = 0.0;       Be[3*m+1][1] = B[m][1];   Be[3*m+2][1] = 0.0;
                        Be[3*m+0][2] = 0.0;       Be[3*m+1][2] = 0.0;       Be[3*m+2][2] = B[m][2];
                        Be[3*m+0][3] = 0.0;       Be[3*m+1][3] = B[m][2];   Be[3*m+2][3] = B[m][1];
                        Be[3*m+0][4] = B[m][2];   Be[3*m+1][4] = 0.0;       Be[3*m+2][4] = B[m][0];
                        Be[3*m+0][5] = B[m][1];   Be[3*m+1][5] = B[m][0];   Be[3*m+2][5] = 0.0;
                    }
                    BeT.transpose(Be);

                    RRMatrix::mlt(Be,De,BeD);
                    RRMatrix::mlt(BeD,BeT,Ket);
                    for (uint m=0;m<3*element.size();m++)
                    {
                        for (uint n=0;n<3*element.size();n++)
                        {
                            // Stiffness matrix
                            Ke[m][n] += Ket[m][n] * detJ * shapeFunc.getW();
                        }
                    }

                    for (uint m=0;m<element.size();m++)
                    {
                        for (uint n=0;n<element.size();n++)
                        {
                            // Mass
                            if (this->pModel->getTimeSolver().getEnabled() || this->problemType == R_PROBLEM_STRESS_MODAL)
                            {
                                double value = N[m] * N[n]
                                             * this->elementDensity[elementID]
                                             * detJ
                                             * shapeFunc.getW();
                                Me[3*m+0][3*n+0] += value;
                                Me[3*m+1][3*n+1] += value;
                                Me[3*m+2][3*n+2] += value;
                            }
                        }

                        // Own weight
                        fe[3*m+0] += elementGravity.x[elementID] * this->elementDensity[elementID] * N[m] * detJ * shapeFunc.getW();
                        fe[3*m+1] += elementGravity.y[elementID] * this->elementDensity[elementID] * N[m] * detJ * shapeFunc.getW();
                        fe[3*m+2] += elementGravity.z[elementID] * this->elementDensity[elementID] * N[m] * detJ * shapeFunc.getW();

                        // Thermal expansion
                        for (uint n=0;n<3;n++)
                        {
                            fe[3*m+0] += this->elementThermalExpansion[elementID] * dT * BeD[3*m+0][n] * detJ * shapeFunc.getW();
                            fe[3*m+1] += this->elementThermalExpansion[elementID] * dT * BeD[3*m+1][n] * detJ * shapeFunc.getW();
                            fe[3*m+2] += this->elementThermalExpansion[elementID] * dT * BeD[3*m+2][n] * detJ * shapeFunc.getW();
                        }
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
                #pragma omp flush (abort)
            }
        }
        if (abort)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to prepare matrix system.");
        }
    }
    if (this->problemType == R_PROBLEM_STRESS_MODAL)
    {
        RLogger::info("Restoring prestressed nodes\n");
        for (uint i=0;i<this->pModel->getNNodes();i++)
        {
            this->pModel->setNode(i,nodesBkp[i]);
        }
    }
}

void RSolverStress::solve(void)
{
    try
    {
        if (this->problemType == R_PROBLEM_STRESS_MODAL)
        {
            this->solveEigenValue();
        }
        else
        {
            this->solveStressStrain();
        }
    }
    catch (const RError &error)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to solve problem. %s", error.getMessage().toUtf8().constData());
    }
}

void RSolverStress::solveStressStrain(void)
{
    RLogger::info("Solving stress-strain problem.\n");

    try
    {
        RLogger::indent();
        RMatrixSolver matrixSolver(this->pModel->getMatrixSolverConf(RMatrixSolverConf::CG));
        matrixSolver.solve(this->A,this->b,this->x,R_MATRIX_PRECONDITIONER_JACOBI,3);
        RLogger::unindent();
    }
    catch (const RError &error)
    {
        RLogger::unindent();
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to solve stress-strain problem. %s", error.getMessage().toUtf8().constData());
    }

    this->setDisplacement(this->x);
}

void RSolverStress::solveEigenValue(void)
{
    RLogger::info("Solving eigen-value problem.\n");

    REigenValueSolverConf conf;

    if (this->pModel->getProblemSetup().getModalSetup().getMethod() == R_MODAL_MULTIPLE_MODES)
    {
        conf.setMethod(REigenValueSolverConf::Arnoldi);
    }
    else
    {
        conf.setMethod(REigenValueSolverConf::Rayleigh);
    }
    conf.setNEigenValues(this->pModel->getProblemSetup().getModalSetup().getNModesToExtract());
    conf.setNIterations(this->pModel->getProblemSetup().getModalSetup().getNIterations());
    conf.setSolverCvgValue(this->pModel->getProblemSetup().getModalSetup().getConvergenceValue());
    conf.setOutputFrequency(this->pModel->getMatrixSolverConf(RMatrixSolverConf::CG).getOutputFrequency());
    conf.setOutputFileName(this->pModel->getMatrixSolverConf(RMatrixSolverConf::CG).getOutputFileName());

    REigenValueSolver solver(conf,this->pModel->getMatrixSolverConf(RMatrixSolverConf::CG));

    try
    {
        RLogger::indent();
        solver.solve(this->M,this->A,this->d,this->ev);
        RLogger::unindent();
    }
    catch (const RError &error)
    {
        RLogger::unindent();
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to solve eigen-value problem. %s", error.getMessage().toUtf8().constData());
    }
}

void RSolverStress::setDisplacement(const RRVector &v)
{
    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        uint position;
        RR3Vector du(0.0,0.0,0.0);

        if (this->nodeBook.getValue(3*i+0,position))
        {
            du[0] = v[position];
        }
        if (this->nodeBook.getValue(3*i+1,position))
        {
            du[1] = v[position];
        }
        if (this->nodeBook.getValue(3*i+2,position))
        {
            du[2] = v[position];
        }

        if (this->localRotations[i].isActive())
        {
            this->localRotations[i].rotateResultsVector(du);
        }

        if (this->problemType == R_PROBLEM_STRESS_MODAL)
        {
            du[0] += this->nodeInitialDisplacement.x[i];
            du[1] += this->nodeInitialDisplacement.y[i];
            du[2] += this->nodeInitialDisplacement.z[i];
        }

        this->nodeDisplacement.x[i] = du[0];
        this->nodeDisplacement.y[i] = du[1];
        this->nodeDisplacement.z[i] = du[2];
    }
}

void RSolverStress::process(void)
{
    if (this->problemType == R_PROBLEM_STRESS_MODAL)
    {
        uint modeNum = this->pModel->getProblemSetup().getModalSetup().getMode();
        this->pModel->getProblemSetup().getModalSetup().setFrequency(this->d[modeNum]);

        RLogger::info("Eigen-value = %g\n",this->d[modeNum]);

        RRVector v(this->ev.getNColumns(),0.0);
        for (uint j=0;j<this->ev.getNColumns();j++)
        {
            v[j] = this->ev[modeNum][j];
        }
        this->setDisplacement(v);
    }

    // Initialize force vector
    this->nodeForce.x.resize(this->pModel->getNNodes());
    this->nodeForce.y.resize(this->pModel->getNNodes());
    this->nodeForce.z.resize(this->pModel->getNNodes());

    this->nodeForce.x.fill(0.0);
    this->nodeForce.y.fill(0.0);
    this->nodeForce.z.fill(0.0);

    // Initialize stress vectors
    this->elementNormalStress.resize(this->pModel->getNElements(),0.0);
    this->elementShearStress.resize(this->pModel->getNElements(),0.0);
    this->elementVonMisses.resize(this->pModel->getNElements(),0.0);

    // Process line elements.
    for (uint i=0;i<this->pModel->getNLines();i++)
    {
        RLine &line = this->pModel->getLine(i);
        double lineCrossArea = line.getCrossArea();

        if (lineCrossArea == 0.0)
        {
            continue;
        }

        bool abort = false;
        #pragma omp parallel for default(shared)
        for (int64_t j=0;j<int64_t(line.size());j++)
        {
            #pragma omp flush (abort)
            if (abort)
            {
                continue;
            }
            try
            {
                uint elementID = line.get(uint(j));

                if (!this->computableElements[elementID])
                {
                    continue;
                }

                const RElement &element = this->pModel->getElement(elementID);
                R_ERROR_ASSERT(R_ELEMENT_TYPE_IS_LINE(element.getType()));
                uint nInp = RElement::getNIntegrationPoints(element.getType());
                RRMatrix Me(element.size()*3,element.size()*3,0.0);
                RRMatrix Ke(element.size()*3,element.size()*3,0.0);
                RRVector fe(element.size()*3,0.0);
                RRVector ae(element.size()*3,0.0);
                RRVector xe(element.size()*3,0.0);
                double QeN = 0.0;

                RRMatrix Be(3*element.size(),1);
                RRMatrix BeT(1,3*element.size());

                double E = this->elementElasticityModulus[elementID];
                double De = E * lineCrossArea;

                RRMatrix Rl;
                RRVector tl;
                element.findTransformationMatrix(this->pModel->getNodes(),Rl,tl);
                Rl.invert();

                RRVector lxe(element.size(),0.0);
                for (uint k=0;k<element.size();k++)
                {
                    RR3Vector xg(this->nodeDisplacement.x[element.getNodeId(k)],
                                 this->nodeDisplacement.y[element.getNodeId(k)],
                                 this->nodeDisplacement.z[element.getNodeId(k)]);
                    RR3Vector xl;
                    RRMatrix::mlt(Rl,xg,xl);
                    lxe[k] = xl[0];
                }

                for (uint k=0;k<element.size();k++)
                {
                    ae[3*k+0] = this->nodeAcceleration.x[element.getNodeId(k)];
                    ae[3*k+1] = this->nodeAcceleration.y[element.getNodeId(k)];
                    ae[3*k+2] = this->nodeAcceleration.z[element.getNodeId(k)];

                    xe[3*k+0] = this->nodeDisplacement.x[element.getNodeId(k)];
                    xe[3*k+1] = this->nodeDisplacement.y[element.getNodeId(k)];
                    xe[3*k+2] = this->nodeDisplacement.z[element.getNodeId(k)];
                }

                double dT = this->elementTemperature[elementID] - this->elementEnvironmentTemperature[elementID];

                for (uint k=0;k<nInp;k++)
                {
                    const RElementShapeFunction &shapeFunc = RElement::getShapeFunction(element.getType(),k);
                    const RRVector &N = shapeFunc.getN();
                    const RRMatrix &dN = shapeFunc.getDN();
                    RRMatrix J, Rt, RtT;
                    double detJ = element.findJacobian(this->pModel->getNodes(),k,J,Rt);
                    RtT.transpose(Rt);

                    Be.fill(0.0);
                    for (uint m=0;m<dN.getNRows();m++)
                    {
                        Be[3*m+0][0] += Rt[0][0]*dN[m][0]*J[0][0];
                        Be[3*m+1][0] += Rt[1][0]*dN[m][0]*J[0][0];
                        Be[3*m+2][0] += Rt[2][0]*dN[m][0]*J[0][0];
                    }
                    BeT.transpose(Be);

                    Be *= De;
                    RRMatrix::mlt(Be,BeT,Ke);
                    Ke *= detJ * shapeFunc.getW();

                    for (uint m=0;m<element.size();m++)
                    {
                        if (lineCrossArea > 0.0)
                        {
                            // Mass
                            if (this->pModel->getTimeSolver().getEnabled() || this->problemType == R_PROBLEM_STRESS_MODAL)
                            {
                                for (uint n=0;n<element.size();n++)
                                {
                                    double value = N[m] * N[n]
                                                 * this->elementDensity[elementID]
                                                 * detJ
                                                 * shapeFunc.getW()
                                                 * lineCrossArea;
                                    Me[3*m+0][3*n+0] += std::pow(Rt[0][0],2.0)*value;
                                    Me[3*m+1][3*n+1] += std::pow(Rt[1][0],2.0)*value;
                                    Me[3*m+2][3*n+2] += std::pow(Rt[2][0],2.0)*value;
                                }
                            }
                        }
                    }

                    double integValue = 1.0/double(nInp);

                    // Element level stress.
                    for (uint m=0;m<element.size();m++)
                    {
                        QeN += dN[m][0]*J[0][0] * De * lxe[m] * integValue;
                        QeN -= dN[m][0]*J[0][0] * De * this->elementThermalExpansion[elementID] * dT * lineCrossArea * integValue;
                    }
                }

                RRVector fae, fxe;
                RRMatrix::mlt(Me,ae,fae);
                RRMatrix::mlt(Ke,xe,fxe);
                RRVector::add(fae,fxe,fe);

                #pragma omp critical
                {
                    for (uint m=0;m<element.size();m++)
                    {
                        this->nodeForce.x[element.getNodeId(m)] += fe[3*m+0];
                        this->nodeForce.y[element.getNodeId(m)] += fe[3*m+1];
                        this->nodeForce.z[element.getNodeId(m)] += fe[3*m+2];
                    }

                    this->elementNormalStress[elementID] = QeN;
                    this->elementShearStress[elementID] = 0.0;
                    this->elementVonMisses[elementID] = QeN;
                }
            }
            catch (const RError &rError)
            {
                #pragma omp critical
                {
                    RLogger::error("%s\n",rError.getMessage().toUtf8().constData());
                    abort = true;
                }
                #pragma omp flush (abort)
            }
        }
        if (abort)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to process results.");
        }
    }

    // Process surface elements.
    for (uint i=0;i<this->pModel->getNSurfaces();i++)
    {
        RSurface &surface = this->pModel->getSurface(i);
        double surfaceThickness = surface.getThickness();

        if (surfaceThickness == 0.0)
        {
            continue;
        }

        bool abort = false;
        #pragma omp parallel for default(shared)
        for (int64_t j=0;j<int64_t(surface.size());j++)
        {
            #pragma omp flush (abort)
            if (abort)
            {
                continue;
            }
            try
            {
                uint elementID = surface.get(uint(j));

                if (!this->computableElements[elementID])
                {
                    continue;
                }

                const RElement &element = this->pModel->getElement(elementID);
                R_ERROR_ASSERT(R_ELEMENT_TYPE_IS_SURFACE(element.getType()));
                uint nInp = RElement::getNIntegrationPoints(element.getType());
                RRMatrix Me(element.size()*3,element.size()*3,0.0);
                RRMatrix Ke(element.size()*2,element.size()*2,0.0);
                RRVector fe(element.size()*3,0.0);
                RRVector ae(element.size()*3,0.0);
                RRVector xe(element.size()*3,0.0);
                RRVector Qe(3,0.0);
                double QeN = 0.0;
                double QeS = 0.0;
                double QeVM = 0.0;

                RRMatrix B(element.size(),3);
                RRMatrix Be(element.size()*2,3);
                RRMatrix BeT(3,element.size()*2);
                RRMatrix BeD(element.size()*2,3);
                RRMatrix Met(element.size()*2,element.size()*2);
                RRMatrix MeRt(element.size()*2,element.size()*2);
                RRMatrix Ket(element.size()*2,element.size()*2);
                RRMatrix KeRt(element.size()*2,element.size()*2);
                RRVector fet(element.size()*2);

                RRMatrix De(3,3,0.0);

                double E = this->elementElasticityModulus[elementID];
                double v = this->elementPoissonRatio[elementID];

                De[0][0] = 1-v;   De[0][1] = v;
                De[1][0] = v;     De[1][1] = 1-v;
                De[2][2] = (1-2*v)/2;
                De *= E/((1+v)*(1-2*v));

                RRMatrix Rl;
                RRVector tl;
                element.findTransformationMatrix(this->pModel->getNodes(),Rl,tl);
                Rl.invert();

                RRVector lxe(element.size()*2);
                for (uint k=0;k<element.size();k++)
                {
                    RR3Vector xg(this->nodeDisplacement.x[element.getNodeId(k)],
                                 this->nodeDisplacement.y[element.getNodeId(k)],
                                 this->nodeDisplacement.z[element.getNodeId(k)]);
                    RR3Vector xl;
                    RRMatrix::mlt(Rl,xg,xl);
                    lxe[2*k+0] = xl[0];
                    lxe[2*k+1] = xl[1];
                }

                for (uint k=0;k<element.size();k++)
                {
                    ae[3*k+0] = this->nodeAcceleration.x[element.getNodeId(k)];
                    ae[3*k+1] = this->nodeAcceleration.y[element.getNodeId(k)];
                    ae[3*k+2] = this->nodeAcceleration.z[element.getNodeId(k)];

                    xe[3*k+0] = this->nodeDisplacement.x[element.getNodeId(k)];
                    xe[3*k+1] = this->nodeDisplacement.y[element.getNodeId(k)];
                    xe[3*k+2] = this->nodeDisplacement.z[element.getNodeId(k)];
                }

                double dT = this->elementTemperature[elementID] - this->elementEnvironmentTemperature[elementID];

                for (uint k=0;k<nInp;k++)
                {
                    const RElementShapeFunction &shapeFunc = RElement::getShapeFunction(element.getType(),k);
                    const RRVector &N = shapeFunc.getN();
                    const RRMatrix &dN = shapeFunc.getDN();
                    RRMatrix J, Rt, RtT;
                    double detJ = element.findJacobian(this->pModel->getNodes(),k,J,Rt);
                    RtT.transpose(Rt);

                    B.fill(0.0);
                    for (uint m=0;m<dN.getNRows();m++)
                    {
                        B[m][0] += (dN[m][0]*J[0][0] + dN[m][1]*J[0][1]);
                        B[m][1] += (dN[m][0]*J[1][0] + dN[m][1]*J[1][1]);
                    }

                    for (uint m=0;m<element.size();m++)
                    {
                        Be[2*m][0] = B[m][0];   Be[2*m+1][0] = 0.0;
                        Be[2*m][1] = 0.0;       Be[2*m+1][1] = B[m][1];
                        Be[2*m][2] = B[m][1];   Be[2*m+1][2] = B[m][0];
                    }
                    BeT.transpose(Be);

                    RRMatrix::mlt(Be,De,BeD);
                    RRMatrix::mlt(BeD,BeT,Ket);
                    RRMatrix::mlt(Rt,Ket,KeRt);
                    RRMatrix::mlt(KeRt,RtT,Ke);
                    Ke *= detJ * shapeFunc.getW();

                    for (uint m=0;m<element.size();m++)
                    {
                        // Mass
                        if (this->pModel->getTimeSolver().getEnabled() || this->problemType == R_PROBLEM_STRESS_MODAL)
                        {
                            for (uint n=0;n<element.size();n++)
                            {
                                double value = N[m] * N[n]
                                             * this->elementDensity[elementID]
                                             * detJ
                                             * shapeFunc.getW()
                                             * surfaceThickness;
                                Met[2*m+0][2*n+0] += value;
                                Met[2*m+1][2*n+1] += value;
                            }
                        }
                    }

                    // Mass
                    if (this->pModel->getTimeSolver().getEnabled() || this->problemType == R_PROBLEM_STRESS_MODAL)
                    {
                        RRMatrix::mlt(Rt,Met,MeRt);
                        RRMatrix::mlt(MeRt,RtT,Me,true);
                    }

                    double integValue = 1.0/double(nInp);

                    // Element level stress.
                    for (uint m=0;m<element.size();m++)
                    {
                        for (uint n=0;n<3;n++)
                        {
                            Qe[n] += BeD[2*m+0][n] * lxe[2*m+0] * integValue
                                  +  BeD[2*m+1][n] * lxe[2*m+1] * integValue;
                        }
                        for (uint n=0;n<2;n++)
                        {
                            Qe[0] -= BeD[2*m+0][n] * this->elementThermalExpansion[elementID] * dT * surfaceThickness * integValue;
                            Qe[1] -= BeD[2*m+1][n] * this->elementThermalExpansion[elementID] * dT * surfaceThickness * integValue;
                        }
                    }
                }

                RRVector fae, fxe;
                RRMatrix::mlt(Me,ae,fae);
                RRMatrix::mlt(Ke,xe,fxe);
                RRVector::add(fae,fxe,fe);

                QeN = std::sqrt(Qe[0] * Qe[0] + Qe[1] * Qe[1] - Qe[0] * Qe[1]);
                QeS = std::sqrt(3.0) * Qe[2];
                QeVM = QeN + QeS;

                #pragma omp critical
                {
                    for (uint m=0;m<element.size();m++)
                    {
                        this->nodeForce.x[element.getNodeId(m)] += fe[3*m+0];
                        this->nodeForce.y[element.getNodeId(m)] += fe[3*m+1];
                        this->nodeForce.z[element.getNodeId(m)] += fe[3*m+2];
                    }

                    this->elementNormalStress[elementID] = QeN;
                    this->elementShearStress[elementID] = QeS;
                    this->elementVonMisses[elementID] = QeVM;
                }
            }
            catch (const RError &rError)
            {
                #pragma omp critical
                {
                    RLogger::error("%s\n",rError.getMessage().toUtf8().constData());
                    abort = true;
                }
                #pragma omp flush (abort)
            }
        }
        if (abort)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to process results.");
        }
    }

    // Process volume elements.
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
                uint elementID = volume.get(uint(j));

                if (!this->computableElements[elementID])
                {
                    continue;
                }

                const RElement &element = this->pModel->getElement(elementID);
                R_ERROR_ASSERT(R_ELEMENT_TYPE_IS_VOLUME(element.getType()));
                uint nInp = RElement::getNIntegrationPoints(element.getType());
                RRMatrix Me(element.size()*3,element.size()*3,0.0);
                RRMatrix Ke(element.size()*3,element.size()*3,0.0);
                RRVector fe(element.size()*3,0.0);
                RRVector ae(element.size()*3,0.0);
                RRVector xe(element.size()*3,0.0);
                RRVector Qe(6,0.0);

                RRMatrix B(element.size(),3);
                RRMatrix Be(element.size()*3,6);
                RRMatrix BeT(6,element.size()*3);
                RRMatrix BeD(element.size()*3,6);
                RRMatrix Ket(element.size()*3,element.size()*3);

                RRMatrix De(6,6,0.0);

                double E = this->elementElasticityModulus[elementID];
                double v = this->elementPoissonRatio[elementID];

                De[0][0] = 1-v;   De[0][1] = v;     De[0][2] = v;
                De[1][0] = v;     De[1][1] = 1-v;   De[1][2] = v;
                De[2][0] = v;     De[2][1] = v;     De[2][2] = 1-v;
                De[3][3] = De[4][4] = De[5][5] = (1-2*v)/2;
                De *= E/((1+v)*(1-2*v));

                double dT = this->elementTemperature[elementID] - this->elementEnvironmentTemperature[elementID];

                for (uint k=0;k<element.size();k++)
                {
                    ae[3*k+0] = this->nodeAcceleration.x[element.getNodeId(k)];
                    ae[3*k+1] = this->nodeAcceleration.y[element.getNodeId(k)];
                    ae[3*k+2] = this->nodeAcceleration.z[element.getNodeId(k)];

                    xe[3*k+0] = this->nodeDisplacement.x[element.getNodeId(k)];
                    xe[3*k+1] = this->nodeDisplacement.y[element.getNodeId(k)];
                    xe[3*k+2] = this->nodeDisplacement.z[element.getNodeId(k)];
                }

                for (uint k=0;k<nInp;k++)
                {
                    const RElementShapeFunction &shapeFunc = RElement::getShapeFunction(element.getType(),k);
                    const RRVector &N = shapeFunc.getN();
                    const RRMatrix &dN = shapeFunc.getDN();
                    RRMatrix J, Rt;
                    double detJ = element.findJacobian(this->pModel->getNodes(),k,J,Rt);

                    B.fill(0.0);
                    for (uint m=0;m<dN.getNRows();m++)
                    {
                        B[m][0] += (dN[m][0]*J[0][0] + dN[m][1]*J[0][1] + dN[m][2]*J[0][2]);
                        B[m][1] += (dN[m][0]*J[1][0] + dN[m][1]*J[1][1] + dN[m][2]*J[1][2]);
                        B[m][2] += (dN[m][0]*J[2][0] + dN[m][1]*J[2][1] + dN[m][2]*J[2][2]);
                    }

                    for (uint m=0;m<element.size();m++)
                    {
                        Be[3*m+0][0] = B[m][0];   Be[3*m+1][0] = 0.0;       Be[3*m+2][0] = 0.0;
                        Be[3*m+0][1] = 0.0;       Be[3*m+1][1] = B[m][1];   Be[3*m+2][1] = 0.0;
                        Be[3*m+0][2] = 0.0;       Be[3*m+1][2] = 0.0;       Be[3*m+2][2] = B[m][2];
                        Be[3*m+0][3] = 0.0;       Be[3*m+1][3] = B[m][2];   Be[3*m+2][3] = B[m][1];
                        Be[3*m+0][4] = B[m][2];   Be[3*m+1][4] = 0.0;       Be[3*m+2][4] = B[m][0];
                        Be[3*m+0][5] = B[m][1];   Be[3*m+1][5] = B[m][0];   Be[3*m+2][5] = 0.0;
                    }
                    BeT.transpose(Be);

                    RRMatrix::mlt(Be,De,BeD);
                    RRMatrix::mlt(BeD,BeT,Ket);
                    for (uint m=0;m<3*element.size();m++)
                    {
                        for (uint n=0;n<3*element.size();n++)
                        {
                            // Stiffness matrix
                            Ke[m][n] += Ket[m][n] * detJ * shapeFunc.getW();
                        }
                    }

                    for (uint m=0;m<element.size();m++)
                    {
                        for (uint n=0;n<element.size();n++)
                        {
                            // Mass
                            if (this->pModel->getTimeSolver().getEnabled() || this->problemType == R_PROBLEM_STRESS_MODAL)
                            {
                                double value = N[m] * N[n]
                                             * this->elementDensity[elementID]
                                             * detJ
                                             * shapeFunc.getW();
                                Me[3*m+0][3*n+0] += value;
                                Me[3*m+1][3*n+1] += value;
                                Me[3*m+2][3*n+2] += value;
                            }
                        }
                    }

                    double integValue = 1.0/double(nInp);

                    // Element level stress.
                    for (uint m=0;m<element.size();m++)
                    {
                        for (uint n=0;n<6;n++)
                        {
                            Qe[n] += BeD[3*m+0][n] * this->nodeDisplacement.x[element.getNodeId(m)] * integValue
                                  +  BeD[3*m+1][n] * this->nodeDisplacement.y[element.getNodeId(m)] * integValue
                                  +  BeD[3*m+2][n] * this->nodeDisplacement.z[element.getNodeId(m)] * integValue;
                        }
                        for (uint n=0;n<3;n++)
                        {
                            Qe[0] -= BeD[3*m+0][n] * this->elementThermalExpansion[elementID] * dT * integValue;
                            Qe[1] -= BeD[3*m+1][n] * this->elementThermalExpansion[elementID] * dT * integValue;
                            Qe[2] -= BeD[3*m+2][n] * this->elementThermalExpansion[elementID] * dT * integValue;
                        }
                    }
                }

                RRVector fae, fxe;
                RRMatrix::mlt(Me,ae,fae);
                RRMatrix::mlt(Ke,xe,fxe);
                RRVector::add(fae,fxe,fe);

                double QeN = std::sqrt(Qe[0]*Qe[0] + Qe[1]*Qe[1] + Qe[2]*Qe[2] - (Qe[0]*Qe[1] + Qe[1]*Qe[2] + Qe[2]*Qe[0]));
                double QeS = std::sqrt(3.0 * (Qe[3]*Qe[3] + Qe[4]*Qe[4] + Qe[5]*Qe[5]));
                double QeVM = QeN + QeS;

                #pragma omp critical
                {
                    for (uint m=0;m<element.size();m++)
                    {
                        this->nodeForce.x[element.getNodeId(m)] += fe[3*m+0];
                        this->nodeForce.y[element.getNodeId(m)] += fe[3*m+1];
                        this->nodeForce.z[element.getNodeId(m)] += fe[3*m+2];
                    }

                    this->elementNormalStress[elementID] = QeN;
                    this->elementShearStress[elementID] = QeS;
                    this->elementVonMisses[elementID] = QeVM;
                }
            }
            catch (const RError &rError)
            {
                #pragma omp critical
                {
                    RLogger::error("%s\n",rError.getMessage().toUtf8().constData());
                    abort = true;
                }
                #pragma omp flush (abort)
            }
        }
        if (abort)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to process results.");
        }
    }
}

void RSolverStress::store(void)
{
    RLogger::info("Storing results\n");
    RLogger::indent();

    // Displacement
    uint displacementPos = this->pModel->findVariable(R_VARIABLE_DISPLACEMENT);
    if (displacementPos == RConstants::eod)
    {
        displacementPos = this->pModel->addVariable(R_VARIABLE_DISPLACEMENT);

        double umin = 0.0;
        double umax = 0.0;
        for (uint i=0;i<this->nodeDisplacement.x.size();i++)
        {
            double u = RR3Vector(this->nodeDisplacement.x[i],
                                 this->nodeDisplacement.y[i],
                                 this->nodeDisplacement.z[i]).length();
            if (i == 0)
            {
                umin = umax = u;
            }
            else
            {
                umin = std::min(umin,u);
                umax = std::max(umax,u);
            }
        }

        this->pModel->getVariable(displacementPos).getVariableData().setMinMaxDisplayValue(umin,umax);
    }
    RVariable &displacement =  this->pModel->getVariable(displacementPos);

    displacement.setApplyType(R_VARIABLE_APPLY_NODE);
    displacement.resize(3,this->pModel->getNNodes());
    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        displacement.setValue(0,i,this->nodeDisplacement.x[i]);
        displacement.setValue(1,i,this->nodeDisplacement.y[i]);
        displacement.setValue(2,i,this->nodeDisplacement.z[i]);
    }

    // VonMises Stress
    uint vonMisesStressPos = this->pModel->findVariable(R_VARIABLE_STRESS_VON_MISES);
    if (vonMisesStressPos == RConstants::eod)
    {
        vonMisesStressPos = this->pModel->addVariable(R_VARIABLE_STRESS_VON_MISES);

        this->pModel->getVariable(vonMisesStressPos).getVariableData().setMinMaxDisplayValue(
                    RStatistics::findMinimumValue(this->elementVonMisses),
                    RStatistics::findMaximumValue(this->elementVonMisses));
    }
    RVariable &vonMisesStress =  this->pModel->getVariable(vonMisesStressPos);

    vonMisesStress.setApplyType(R_VARIABLE_APPLY_ELEMENT);
    vonMisesStress.resize(1,this->pModel->getNElements());
    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        vonMisesStress.setValue(0,i,this->elementVonMisses[i]);
    }

    // Normal Stress
    uint normalStressPos = this->pModel->findVariable(R_VARIABLE_STRESS_NORMAL);
    if (normalStressPos == RConstants::eod)
    {
        normalStressPos = this->pModel->addVariable(R_VARIABLE_STRESS_NORMAL);

        this->pModel->getVariable(normalStressPos).getVariableData().setMinMaxDisplayValue(
                    RStatistics::findMinimumValue(this->elementNormalStress),
                    RStatistics::findMaximumValue(this->elementNormalStress));
    }
    RVariable &normalStress =  this->pModel->getVariable(normalStressPos);

    normalStress.setApplyType(R_VARIABLE_APPLY_ELEMENT);
    normalStress.resize(1,this->pModel->getNElements());
    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        normalStress.setValue(0,i,this->elementNormalStress[i]);
    }

    // Shear Stress
    uint shearStressPos = this->pModel->findVariable(R_VARIABLE_STRESS_SHEAR);
    if (shearStressPos == RConstants::eod)
    {
        shearStressPos = this->pModel->addVariable(R_VARIABLE_STRESS_SHEAR);

        this->pModel->getVariable(shearStressPos).getVariableData().setMinMaxDisplayValue(
                    RStatistics::findMinimumValue(this->elementShearStress),
                    RStatistics::findMaximumValue(this->elementShearStress));
    }
    RVariable &shearStress =  this->pModel->getVariable(shearStressPos);

    shearStress.setApplyType(R_VARIABLE_APPLY_ELEMENT);
    shearStress.resize(1,this->pModel->getNElements());
    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        shearStress.setValue(0,i,this->elementShearStress[i]);
    }

    // Force
    uint forcePos = this->pModel->findVariable(R_VARIABLE_FORCE);
    if (forcePos == RConstants::eod)
    {
        forcePos = this->pModel->addVariable(R_VARIABLE_FORCE);

        double fmin = 0.0;
        double fmax = 0.0;
        for (uint i=0;i<this->nodeForce.x.size();i++)
        {
            double f = RR3Vector(this->nodeForce.x[i],
                                 this->nodeForce.y[i],
                                 this->nodeForce.z[i]).length();
            if (i == 0)
            {
                fmin = fmax = f;
            }
            else
            {
                fmin = std::min(fmin,f);
                fmax = std::max(fmax,f);
            }
        }

        this->pModel->getVariable(displacementPos).getVariableData().setMinMaxDisplayValue(fmin,fmax);
    }
    RVariable &force =  this->pModel->getVariable(forcePos);

    force.setApplyType(R_VARIABLE_APPLY_NODE);
    force.resize(3,this->pModel->getNNodes());
    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        force.setValue(0,i,this->nodeForce.x[i]);
        force.setValue(1,i,this->nodeForce.y[i]);
        force.setValue(2,i,this->nodeForce.z[i]);
    }

    RLogger::unindent();
}

void RSolverStress::statistics(void)
{
    this->printStats(R_VARIABLE_DISPLACEMENT);
    this->printStats(R_VARIABLE_STRESS_VON_MISES);
    this->printStats(R_VARIABLE_FORCE);
    this->processMonitoringPoints();
}

void RSolverStress::generateNodeBook(void)
{
    this->nodeBook.resize(this->pModel->getNNodes()*3);
    this->nodeBook.initialize();

    for (uint i=0;i<this->pModel->getNElementGroups();i++)
    {
        REntityGroupType entityType = this->pModel->getEntityGroupType(i);
        const RElementGroup *pElementGroup = this->pModel->getElementGroupPtr(i);
        if (!pElementGroup)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Element group could not be found (%u of %u).",i,this->pModel->getNElementGroups());
        }
        bool hasDisplacementX = false;
        bool hasDisplacementY = false;
        bool hasDisplacementZ = false;
        for (uint j=0;j<pElementGroup->getNBoundaryConditions();j++)
        {
            const RBoundaryCondition &bc = pElementGroup->getBoundaryCondition(j);
            if (RBoundaryCondition::getProblemTypeMask(bc.getType()) & R_PROBLEM_STRESS)
            {
                if (bc.getType() == R_BOUNDARY_CONDITION_DISPLACEMENT || bc.getType() == R_BOUNDARY_CONDITION_DISPLACEMENT_NORMAL)
                {
                    hasDisplacementX = true;
                    hasDisplacementY = true;
                    hasDisplacementZ = true;
                }
                if (bc.getType() == R_BOUNDARY_CONDITION_DISPLACEMENT_ROLLER)
                {
                    if (entityType == R_ENTITY_GROUP_LINE)
                    {
                        hasDisplacementY = true;
                        hasDisplacementZ = true;
                    }
                    else
                    {
                        hasDisplacementX = true;
                    }
                }
            }
        }
        if (!hasDisplacementX && !hasDisplacementY && !hasDisplacementZ)
        {
            continue;
        }
        for (uint j=0;j<pElementGroup->size();j++)
        {
            uint elementID = pElementGroup->get(j);
            const RElement &element = this->pModel->getElement(elementID);
            for (uint k=0;k<element.size();k++)
            {
                uint nodeId = element.getNodeId(k);
                if (hasDisplacementX)
                {
                    this->nodeBook.disable(3*nodeId+0,true);
                }
                if (hasDisplacementY)
                {
                    this->nodeBook.disable(3*nodeId+1,true);
                }
                if (hasDisplacementZ)
                {
                    this->nodeBook.disable(3*nodeId+2,true);
                }
            }
        }
    }
    RBVector computableNodes(this->pModel->getNNodes(),false);
    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        if (this->computableElements[i])
        {
            const RElement &rElement = this->pModel->getElement(i);
            for (uint j=0;j<rElement.size();j++)
            {
                computableNodes[rElement.getNodeId(j)] = true;
            }
        }
    }
    for (uint i=0;i<computableNodes.size();i++)
    {
        if (!computableNodes[i])
        {
            this->nodeBook.disable(3*i+0,true);
            this->nodeBook.disable(3*i+1,true);
            this->nodeBook.disable(3*i+2,true);
        }
    }
}

void RSolverStress::assemblyMatrix(uint elementID, const RRMatrix &Me, const RRMatrix &Ke, const RRVector &fe)
{
    double alpha = this->pModel->getTimeSolver().getTimeMarchApproximationCoefficient();
    double dt = this->pModel->getTimeSolver().getCurrentTimeStepSize();

    const RElement &rElement = this->pModel->getElement(elementID);

    RRMatrix Ae(3*rElement.size(),3*rElement.size());
    RRMatrix Be(3*rElement.size(),3*rElement.size());
    RRVector be(3*rElement.size());

    Ae.fill(0.0);
    Be.fill(0.0);
    be.fill(0.0);

    if (this->pModel->getTimeSolver().getEnabled())
    {
        for (unsigned m=0;m<rElement.size();m++)
        {
            be[3*m+0] = dt * fe[3*m+0];
            be[3*m+1] = dt * fe[3*m+1];
            be[3*m+2] = dt * fe[3*m+2];
            for (unsigned n=0;n<rElement.size();n++)
            {
                Ae[3*m+0][3*n+0] = Me[3*m+0][3*n+0] + alpha * dt * Ke[3*m+0][3*n+0];
                Ae[3*m+1][3*n+0] = Me[3*m+1][3*n+0] + alpha * dt * Ke[3*m+1][3*n+0];
                Ae[3*m+2][3*n+0] = Me[3*m+2][3*n+0] + alpha * dt * Ke[3*m+2][3*n+0];

                Ae[3*m+0][3*n+1] = Me[3*m+0][3*n+1] + alpha * dt * Ke[3*m+0][3*n+1];
                Ae[3*m+1][3*n+1] = Me[3*m+1][3*n+1] + alpha * dt * Ke[3*m+1][3*n+1];
                Ae[3*m+2][3*n+1] = Me[3*m+2][3*n+1] + alpha * dt * Ke[3*m+2][3*n+1];

                Ae[3*m+0][3*n+2] = Me[3*m+0][3*n+2] + alpha * dt * Ke[3*m+0][3*n+2];
                Ae[3*m+1][3*n+2] = Me[3*m+1][3*n+2] + alpha * dt * Ke[3*m+1][3*n+2];
                Ae[3*m+2][3*n+2] = Me[3*m+2][3*n+2] + alpha * dt * Ke[3*m+2][3*n+2];

                be[3*m+0] += (Me[3*m+0][3*n+0] - (1.0 - alpha) * dt * Ke[3*m+0][3*n+0]) * this->nodeDisplacement.x[rElement.getNodeId(n)]
                          +  (Me[3*m+0][3*n+1] - (1.0 - alpha) * dt * Ke[3*m+0][3*n+1]) * this->nodeDisplacement.y[rElement.getNodeId(n)]
                          +  (Me[3*m+0][3*n+2] - (1.0 - alpha) * dt * Ke[3*m+0][3*n+2]) * this->nodeDisplacement.z[rElement.getNodeId(n)];

                be[3*m+1] += (Me[3*m+1][3*n+0] - (1.0 - alpha) * dt * Ke[3*m+1][3*n+0]) * this->nodeDisplacement.x[rElement.getNodeId(n)]
                          +  (Me[3*m+1][3*n+1] - (1.0 - alpha) * dt * Ke[3*m+1][3*n+1]) * this->nodeDisplacement.y[rElement.getNodeId(n)]
                          +  (Me[3*m+1][3*n+2] - (1.0 - alpha) * dt * Ke[3*m+1][3*n+2]) * this->nodeDisplacement.z[rElement.getNodeId(n)];

                be[3*m+2] += (Me[3*m+2][3*n+0] - (1.0 - alpha) * dt * Ke[3*m+2][3*n+0]) * this->nodeDisplacement.x[rElement.getNodeId(n)]
                          +  (Me[3*m+2][3*n+1] - (1.0 - alpha) * dt * Ke[3*m+2][3*n+1]) * this->nodeDisplacement.y[rElement.getNodeId(n)]
                          +  (Me[3*m+2][3*n+2] - (1.0 - alpha) * dt * Ke[3*m+2][3*n+2]) * this->nodeDisplacement.z[rElement.getNodeId(n)];
            }
        }
    }
    else if (this->problemType == R_PROBLEM_STRESS_MODAL)
    {
        Ae = Ke;
        Be = Me;
        be = fe;
    }
    else
    {
        Ae = Ke;
        be = fe;
    }
    this->applyLocalRotations(elementID,Ae);
    this->applyLocalRotations(elementID,Be);
    this->applyLocalRotations(elementID,be);

    // Apply explicit boundary conditions.
    for (uint m=0;m<rElement.size();m++)
    {
        uint position;
        uint nodeID = rElement.getNodeId(m);
        for (uint n=0;n<3*rElement.size();n++)
        {
            if (!this->nodeBook.getValue(3*nodeID+0,position))
            {
                be[n] -= (Ae[n][3*m+0] + Be[n][3*m+0]) * this->nodeDisplacement.x[nodeID];
            }
            if (!this->nodeBook.getValue(3*nodeID+1,position))
            {
                be[n] -= (Ae[n][3*m+1] + Be[n][3*m+1]) * this->nodeDisplacement.y[nodeID];
            }
            if (!this->nodeBook.getValue(3*nodeID+2,position))
            {
                be[n] -= (Ae[n][3*m+2] + Be[n][3*m+2]) * this->nodeDisplacement.z[nodeID];
            }
        }
    }

    // Assembly final matrix system
    uint dims = 3;
    for (uint m=0;m<rElement.size();m++)
    {
        for (uint i=0;i<dims;i++)
        {
            uint mp = 0;

            if (this->nodeBook.getValue(dims*rElement.getNodeId(m)+i,mp))
            {
                this->b[mp] += be[dims*m+i];

                for (uint n=0;n<rElement.size();n++)
                {
                    for (uint j=0;j<dims;j++)
                    {
                        uint np = 0;

                        if (this->nodeBook.getValue(dims*rElement.getNodeId(n)+j,np))
                        {
                            this->A.addValue(mp,np,Ae[dims*m+i][dims*n+j]);
                            if (this->problemType == R_PROBLEM_STRESS_MODAL)
                            {
                                this->M.addValue(mp,np,Me[dims*m+i][dims*n+j]);
                            }
                        }
                    }
                }
            }
        }
    }
}

void RSolverStress::applyLocalRotations(unsigned int elementID, RRMatrix &Ae)
{
    const RElement &rElement = this->pModel->getElement(elementID);
    RRMatrix T;

    bool first = true;

    for (uint i=0;i<rElement.size();i++)
    {
        uint nodeId = rElement.getNodeId(i);
        if (this->localRotations[nodeId].isActive())
        {
            if (first)
            {
                T.setIdentity(Ae.getNRows());
                first = false;
            }
            T.setBlock(this->localRotations[nodeId].getR(),3*i,3*i);
        }
    }
    if (!first)
    {
        RRMatrix Tt(T);
        Tt.transpose();

        RRMatrix Aetmp;

        RRMatrix::mlt(Tt,Ae,Aetmp);
        RRMatrix::mlt(Aetmp,T,Ae);
    }
}

void RSolverStress::applyLocalRotations(unsigned int elementID, RRVector &fe)
{
    const RElement &rElement = this->pModel->getElement(elementID);
    RRMatrix T;

    bool first = true;

    for (uint i=0;i<rElement.size();i++)
    {
        uint nodeId = rElement.getNodeId(i);
        if (this->localRotations[nodeId].isActive())
        {
            if (first)
            {
                T.setIdentity(fe.getNRows());
                first = false;
            }
            T.setBlock(this->localRotations[nodeId].getR(),3*i,3*i);
        }
    }
    if (!first)
    {
        RRVector fetmp;

        RRMatrix::mlt(T,fe,fetmp);
        fe = fetmp;
    }
}
