/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolveracoustic.cpp                                      *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   20-th August 2015                                        *
 *                                                                   *
 *  DESCRIPTION: Acoustic solver class definition                    *
 *********************************************************************/

#include "rsolveracoustic.h"
#include "rmatrixsolver.h"

void RSolverAcoustic::_init(const RSolverAcoustic *pAcousticSolver)
{
    if (pAcousticSolver)
    {
        this->elementElasticityModulus = pAcousticSolver->elementElasticityModulus;
        this->elementDensity = pAcousticSolver->elementDensity;
        this->elementDampingFactor = pAcousticSolver->elementDampingFactor;
        this->nodeVelocityPotential = pAcousticSolver->nodeVelocityPotential;
        this->nodeVelocityPotentialVelocity = pAcousticSolver->nodeVelocityPotentialVelocity;
        this->nodeVelocityPotentialAcceleration = pAcousticSolver->nodeVelocityPotentialAcceleration;
        this->nodeAcousticPressure = pAcousticSolver->nodeAcousticPressure;
        this->elementAcousticParticleVelocity = pAcousticSolver->elementAcousticParticleVelocity;
    }
    else
    {
        this->nodeVelocityPotentialVelocity.resize(this->pModel->getNNodes(),0.0);
        this->nodeVelocityPotentialAcceleration.resize(this->pModel->getNNodes(),0.0);
    }
}

RSolverAcoustic::RSolverAcoustic(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData)
    : RSolverGeneric(pModel,modelFileName,convergenceFileName,sharedData)
{
    this->problemType = R_PROBLEM_ACOUSTICS;
    this->_init();
}

RSolverAcoustic::RSolverAcoustic(const RSolverAcoustic &acousticSolver)
    : RSolverGeneric(acousticSolver)
{
    this->_init(&acousticSolver);
}

RSolverAcoustic::~RSolverAcoustic()
{

}

RSolverAcoustic &RSolverAcoustic::operator =(const RSolverAcoustic &acousticSolver)
{
    RSolverGeneric::operator =(acousticSolver);
    this->_init(&acousticSolver);
    return (*this);
}

bool RSolverAcoustic::hasConverged(void) const
{
    return true;
}

double RSolverAcoustic::findSoundSpeedScale(void) const
{
    if (this->pModel->getNElements() == 0)
    {
        return 1.0;
    }

    RRVector E, ro;

    this->generateMaterialVecor(R_MATERIAL_PROPERTY_MODULUS_OF_ELASTICITY,E);
    this->generateMaterialVecor(R_MATERIAL_PROPERTY_DENSITY,ro);

    double cavg = 0.0;
    uint ne = 0;
    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        if (this->computableElements[i])
        {
            cavg += std::sqrt(E[i]/ro[i]);
            ne++;
        }
    }
    cavg /= double(ne);
    if (cavg < RConstants::eps)
    {
        cavg = 1.0;
    }
    return 1.0 / cavg;
}

void RSolverAcoustic::updateScales(void)
{
//    double sScale = this->findSoundSpeedScale();
//    double eScale = this->findElementScale(true);

//    this->scales.setMetre(eScale*sScale);
//    double dt = this->pModel->getTimeSolver().getCurrentTimeStepSize();
//    this->scales.setMetre(1.0/eScale);
//    this->scales.setSecond(1.0/dt);
//    this->scales.print(false);
//    this->scales.setMetre(eScale*sScale);
    this->scales.setMetre(this->findMeshScale());
    //    this->scales.setSecond(eScale/sScale);
}

void RSolverAcoustic::recover(void)
{
    this->recoverVariable(R_VARIABLE_POTENTIAL,
                          R_VARIABLE_APPLY_NODE,
                          this->pModel->getNNodes(),
                          0,
                          this->nodeVelocityPotential,
                          0.0);
    this->recoverVariable(R_VARIABLE_POTENTIAL_VELOCITY,
                          R_VARIABLE_APPLY_NODE,
                          this->pModel->getNNodes(),
                          0,
                          this->nodeVelocityPotentialVelocity,
                          0.0);
    this->recoverVariable(R_VARIABLE_POTENTIAL_ACCELERATION,
                          R_VARIABLE_APPLY_NODE,
                          this->pModel->getNNodes(),
                          0,
                          this->nodeVelocityPotentialAcceleration,
                          0.0);
}

void RSolverAcoustic::prepare(void)
{
    //! Element velocity potential - displacement.
    RRVector elementVelocityPotential;
    RBVector velocityPotentialSetValues;
    //! Element velocity normal.
    RRVector elementVelocityNormal;
    RBVector velocityNormalSetValues;

    this->generateNodeBook(R_PROBLEM_ACOUSTICS);

    this->generateVariableVector(R_VARIABLE_POTENTIAL,elementVelocityPotential,velocityPotentialSetValues,true,this->firstRun,this->firstRun);
    this->generateVariableVector(R_VARIABLE_VELOCITY,elementVelocityNormal,velocityNormalSetValues,true,this->firstRun,this->firstRun);

    this->generateMaterialVecor(R_MATERIAL_PROPERTY_MODULUS_OF_ELASTICITY,this->elementElasticityModulus);
    this->generateMaterialVecor(R_MATERIAL_PROPERTY_DENSITY,this->elementDensity);

    this->elementDampingFactor.resize(this->pModel->getNElements(),0.0);
    this->pModel->convertElementToNodeVector(elementVelocityPotential,velocityPotentialSetValues,this->nodeVelocityPotential,true);
    this->nodeVelocityPotentialOld = this->nodeVelocityPotential;

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
                RRMatrix Ce(element.size(),element.size());
                RRMatrix Ke(element.size(),element.size());
                RRVector fe(element.size());

                Me.fill(0.0);
                Ce.fill(0.0);
                Ke.fill(0.0);
                fe.fill(0.0);

                for (uint k=0;k<nInp;k++)
                {
                    const RElementShapeFunction &shapeFunc = RElement::getShapeFunction(element.getType(),k);
                    const RRVector &N = shapeFunc.getN();
                    RRMatrix J, Rt;
                    double detJ = this->pModel->getElement(elementID).findJacobian(this->pModel->getNodes(),k,J,Rt);

                    for (uint m=0;m<element.size();m++)
                    {
                        for (uint n=0;n<element.size();n++)
                        {
                            // Mass
                            if (this->pModel->getTimeSolver().getEnabled())
                            {
                                Me[m][n] += (-1.0) * N[m] * N[n] * detJ * shapeFunc.getW() * point.getVolume();
                            }
                        }
                        // Velocity / force
                        fe[m] += elementVelocityNormal[elementID] * N[m] * detJ * shapeFunc.getW();
                    }
                }
                #pragma omp critical
                {
                    this->assemblyMatrix(elementID,Me,Ce,Ke,fe);
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
                RRMatrix Ce(element.size(),element.size());
                RRMatrix Ke(element.size(),element.size());
                RRVector fe(element.size());
                RRMatrix B(element.size(),1);

                Me.fill(0.0);
                Ce.fill(0.0);
                Ke.fill(0.0);
                fe.fill(0.0);

                double c = std::sqrt(this->elementElasticityModulus[elementID]/this->elementDensity[elementID]);

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
                        B[m][0] += dN[m][0]*J[0][0];
                    }

                    for (uint m=0;m<element.size();m++)
                    {
                        for (uint n=0;n<element.size();n++)
                        {
                            // Stiffness
                            Ke[m][n] += (B[m][0]*B[n][0]) * line.getCrossArea() * c * c * detJ * shapeFunc.getW();

                            // Mass
                            if (this->pModel->getTimeSolver().getEnabled())
                            {
                                Me[m][n] += (-1.0) * N[m] * N[n] * detJ * shapeFunc.getW() * line.getCrossArea();
                            }
                        }
                        // Velocity / force
                        fe[m] += elementVelocityNormal[elementID] * N[m] * detJ * shapeFunc.getW();
                    }
                }
                #pragma omp critical
                {
                    this->assemblyMatrix(elementID,Me,Ce,Ke,fe);
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
                RRMatrix Me(element.size(),element.size());
                RRMatrix Ce(element.size(),element.size());
                RRMatrix Ke(element.size(),element.size());
                RRVector fe(element.size());
                RRMatrix B(element.size(),2);

                Me.fill(0.0);
                Ce.fill(0.0);
                Ke.fill(0.0);
                fe.fill(0.0);

                double c = std::sqrt(this->elementElasticityModulus[elementID]/this->elementDensity[elementID]);

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

                    for (uint m=0;m<element.size();m++)
                    {
                        for (uint n=0;n<element.size();n++)
                        {
                            // Stiffness
                            Ke[m][n] += (B[m][0]*B[n][0]+B[m][1]*B[n][1]) * surface.getThickness() * c * c * detJ * shapeFunc.getW();

                            // Mass
                            if (this->pModel->getTimeSolver().getEnabled())
                            {
                                Me[m][n] += (-1.0) * N[m] * N[n] * detJ * shapeFunc.getW() * surface.getThickness();
                            }
                        }
                        // Velocity / force
                        fe[m] += elementVelocityNormal[elementID] * N[m] * detJ * shapeFunc.getW();
                    }
                }
                #pragma omp critical
                {
                    this->assemblyMatrix(elementID,Me,Ce,Ke,fe);
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
                RRMatrix Ce(element.size(),element.size());
                RRMatrix Ke(element.size(),element.size());
                RRVector fe(element.size());
                RRMatrix B(element.size(),3);

                Me.fill(0.0);
                Ce.fill(0.0);
                Ke.fill(0.0);
                fe.fill(0.0);

                double c = std::sqrt(this->elementElasticityModulus[elementID]/this->elementDensity[elementID]);

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

                    for (uint m=0;m<element.size();m++)
                    {
                        for (uint n=0;n<element.size();n++)
                        {
                            // Stiffness
                            Ke[m][n] += (B[m][0]*B[n][0] + B[m][1]*B[n][1] + B[m][2]*B[n][2])
                                     * c * c
                                     * detJ
                                     * shapeFunc.getW();

                            // Mass
                            if (this->pModel->getTimeSolver().getEnabled())
                            {
                                Me[m][n] += (-1.0) * N[m] * N[n] * detJ * shapeFunc.getW();
                            }
                        }
                    }
                }
                #pragma omp critical
                {
                    this->assemblyMatrix(elementID,Me,Ce,Ke,fe);
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

void RSolverAcoustic::solve(void)
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
            this->nodeVelocityPotential[i] = this->x[position];
        }
    }

    double alpha = 1.0 / 2.0;
    double beta = this->pModel->getTimeSolver().getTimeMarchApproximationCoefficient() / 2.0;
    double dt = this->pModel->getTimeSolver().getCurrentTimeStepSize();

    double a0 = 1.0 / (beta * std::pow(dt,2));
    double a2 = 1.0 / (beta * dt);
    double a3 = (1.0 / (2.0 * beta)) - 1.0;
    double a6 = dt * (1.0 - alpha);
    double a7 = dt * alpha;

    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        double puo = this->nodeVelocityPotentialOld[i];
        double pvo = this->nodeVelocityPotentialVelocity[i];
        double pao = this->nodeVelocityPotentialAcceleration[i];

        double pu = this->nodeVelocityPotential[i];
        double pa = a0 * (pu - puo) - a2 * pvo - a3 * pao;
        double pv = pvo + a6 * pao + a7 * pa;

        this->nodeVelocityPotentialVelocity[i] = pv;
        this->nodeVelocityPotentialAcceleration[i] = pa;
    }
}

void RSolverAcoustic::process(void)
{
    // Process absorbing boundary
    this->processAbsorbingBoundary();

    // Acoustic pressure
    this->processAcousticPressure();

    // Acoustic particle velocity
    this->processAcousticParticleVelocity();
}

void RSolverAcoustic::processAbsorbingBoundary(void)
{
    // Find nodes which are on absorbing boundary.
    std::vector<bool> absorbingBoundaryNodes = this->findAbsorbingBoundaryNodes();

    // Find absorbing boundary normals for each node.
    std::vector<RR3Vector> absorbingBoundaryNormals = this->findAbsorbingBoundaryNormals(absorbingBoundaryNodes);

    double dt = this->pModel->getTimeSolver().getCurrentTimeStepSize();

    RRVector t(this->pModel->getNNodes(),0.0);
    RUVector ncount(this->pModel->getNNodes(),0);

    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        if (!this->computableElements[i])
        {
            continue;
        }

        const RElement &rElement = this->pModel->getElement(i);
        double c = std::sqrt(this->elementElasticityModulus[i]/this->elementDensity[i]);

        for (uint j=0;j<rElement.size();j++)
        {
            if (!absorbingBoundaryNodes[rElement.getNodeId(j)])
            {
                continue;
            }
            for (uint k=j+1;k<rElement.size();k++)
            {
                if (absorbingBoundaryNodes[rElement.getNodeId(k)])
                {
                    continue;
                }
                RR3Vector distance;
                RR3Vector::subtract(this->pModel->getNode(rElement.getNodeId(k)).toVector(),
                                    this->pModel->getNode(rElement.getNodeId(j)).toVector(),
                                    distance);
                distance[0] *= absorbingBoundaryNormals[rElement.getNodeId(j)][0];
                distance[1] *= absorbingBoundaryNormals[rElement.getNodeId(j)][1];
                distance[2] *= absorbingBoundaryNormals[rElement.getNodeId(j)][2];

                double dl = distance.length();

                if (dl < RConstants::eps)
                {
                    continue;
                }

                double B = 1.0;
                double b = 0.4;
                double V = c * dt / dl;
                double qx = (b*(B+V)-V)/((B+V)*(1.0-b));
                double qt = (b*(B+V)-B)/((B+V)*(1.0-b));
                double qtx = b/(b-1.0);

                t[rElement.getNodeId(j)] += -qx * this->nodeVelocityPotential[rElement.getNodeId(k)]
                                          -  qt * this->nodeVelocityPotentialOld[rElement.getNodeId(j)]
                                          -  qtx * this->nodeVelocityPotentialOld[rElement.getNodeId(k)];
                ncount[rElement.getNodeId(j)] ++;
            }
        }
    }
    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        if (ncount[i] > 0)
        {
            this->nodeVelocityPotential[i] = t[i] / ncount[i];
        }
    }
}

void RSolverAcoustic::processAcousticPressure(void)
{
    this->nodeAcousticPressure.resize(this->pModel->getNNodes(),0.0);

    RRVector nodeDensity;
    RBVector nodeDensitySetValues(this->pModel->getNNodes(),false);
    this->pModel->convertElementToNodeVector(this->elementDensity,nodeDensitySetValues,nodeDensity);

    //double dt = this->pModel->getTimeSolver().getCurrentTimeStepSize();
    //double ct = this->pModel->getTimeSolver().getCurrentTime();

    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        //this->nodeAcousticPressure[i] = (-1.0) * nodeDensity[i] * (this->nodeVelocityPotentialDisplacement[i] - this->nodeVelocityPotentialDisplacementOld[i]) / dt;
        // Per second
        this->nodeAcousticPressure[i] = nodeDensity[i] * this->nodeVelocityPotential[i];
    }
}

void RSolverAcoustic::processAcousticParticleVelocity(void)
{
    this->elementAcousticParticleVelocity.x.resize(this->pModel->getNElements(),0.0);
    this->elementAcousticParticleVelocity.y.resize(this->pModel->getNElements(),0.0);
    this->elementAcousticParticleVelocity.z.resize(this->pModel->getNElements(),0.0);

    // Process line elements.
    for (uint i=0;i<this->pModel->getNLines();i++)
    {
        RLine &line = this->pModel->getLine(i);

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
                uint elementID = line.get(j);

                if (!this->computableElements[elementID])
                {
                    continue;
                }

                const RElement &element = this->pModel->getElement(elementID);
                R_ERROR_ASSERT(R_ELEMENT_TYPE_IS_LINE(element.getType()));
                uint nInp = RElement::getNIntegrationPoints(element.getType());
                RRVector B(element.size());

                RR3Vector ve(0.0,0.0,0.0);

                if (line.getCrossArea() > 0.0)
                {
                    double vi = 0.0;

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

                        vi -= B[k] * this->nodeVelocityPotential[nodeID];
                    }

                    RRMatrix R;
                    RRVector t;
                    this->pModel->getElement(elementID).findTransformationMatrix(this->pModel->getNodes(),R,t);

                    ve[0] += R[0][0]*vi;
                    ve[1] += R[1][0]*vi;
                    ve[2] += R[2][0]*vi;
                }

                this->elementAcousticParticleVelocity.x[elementID] = ve[0];
                this->elementAcousticParticleVelocity.y[elementID] = ve[1];
                this->elementAcousticParticleVelocity.z[elementID] = ve[2];
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
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to process acoustic particle velocity.");
        }
    }

    // Process surface elements.
    for (uint i=0;i<this->pModel->getNSurfaces();i++)
    {
        RSurface &surface = this->pModel->getSurface(i);

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
                uint elementID = surface.get(j);

                if (!this->computableElements[elementID])
                {
                    continue;
                }

                const RElement &element = this->pModel->getElement(elementID);
                R_ERROR_ASSERT(R_ELEMENT_TYPE_IS_SURFACE(element.getType()));
                uint nInp = RElement::getNIntegrationPoints(element.getType());
                RRMatrix B(element.size(),2);

                RR3Vector ve(0.0,0.0,0.0);

                if (surface.getThickness() > 0.0)
                {
                    double vi = 0.0;
                    double vj = 0.0;

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

                        vi -= B[k][0] * this->nodeVelocityPotential[nodeID];
                        vj -= B[k][1] * this->nodeVelocityPotential[nodeID];
                    }

                    RRMatrix R;
                    RRVector t;
                    this->pModel->getElement(elementID).findTransformationMatrix(this->pModel->getNodes(),R,t);

                    ve[0] += R[0][0]*vi + R[0][1]*vj;
                    ve[1] += R[1][0]*vi + R[1][1]*vj;
                    ve[2] += R[2][0]*vi + R[2][1]*vj;
                }

                this->elementAcousticParticleVelocity.x[elementID] = ve[0];
                this->elementAcousticParticleVelocity.y[elementID] = ve[1];
                this->elementAcousticParticleVelocity.z[elementID] = ve[2];
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
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to process acoustic particle velocity.");
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
                uint elementID = volume.get(j);

                if (!this->computableElements[elementID])
                {
                    continue;
                }

                const RElement &element = this->pModel->getElement(elementID);
                R_ERROR_ASSERT(R_ELEMENT_TYPE_IS_VOLUME(element.getType()));
                uint nInp = RElement::getNIntegrationPoints(element.getType());
                RRMatrix B(element.size(),3);
                RR3Vector ve(0.0,0.0,0.0);

                for (uint k=0;k<nInp;k++)
                {
                    const RElementShapeFunction &shapeFunc = RElement::getShapeFunction(element.getType(),k);
                    const RRMatrix &dN = shapeFunc.getDN();
                    RRMatrix J, Rt;
                    double detJ = this->pModel->getElement(elementID).findJacobian(this->pModel->getNodes(),k,J,Rt);

                    B.fill(0.0);
                    for (uint m=0;m<dN.getNRows();m++)
                    {
                        B[m][0] += (dN[m][0]*J[0][0] + dN[m][1]*J[0][1] + dN[m][2]*J[0][2]) * detJ / double(nInp);
                        B[m][1] += (dN[m][0]*J[1][0] + dN[m][1]*J[1][1] + dN[m][2]*J[1][2]) * detJ / double(nInp);
                        B[m][2] += (dN[m][0]*J[2][0] + dN[m][1]*J[2][1] + dN[m][2]*J[2][2]) * detJ / double(nInp);
                    }
                }

                for (uint m=0;m<element.size();m++)
                {
                    uint nodeId = element.getNodeId(m);
                    ve[0] -= B[m][0] * this->nodeVelocityPotential[nodeId];
                    ve[1] -= B[m][1] * this->nodeVelocityPotential[nodeId];
                    ve[2] -= B[m][2] * this->nodeVelocityPotential[nodeId];
                }

                this->elementAcousticParticleVelocity.x[elementID] = ve[0];
                this->elementAcousticParticleVelocity.y[elementID] = ve[1];
                this->elementAcousticParticleVelocity.z[elementID] = ve[2];
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
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to process acoustic particle velocity.");
        }
    }
}

void RSolverAcoustic::store(void)
{
    RLogger::info("Storing results\n");
    RLogger::indent();

    // Velocity potential
    uint velocityPotentialPos = this->pModel->findVariable(R_VARIABLE_POTENTIAL);
    if (velocityPotentialPos == RConstants::eod)
    {
        velocityPotentialPos = this->pModel->addVariable(R_VARIABLE_POTENTIAL);
        this->pModel->getVariable(velocityPotentialPos).getVariableData().setMinMaxDisplayValue(
                    RStatistics::findMinimumValue(this->nodeVelocityPotential),
                    RStatistics::findMaximumValue(this->nodeVelocityPotential));
    }
    RVariable &velocityPotential =  this->pModel->getVariable(velocityPotentialPos);

    velocityPotential.setApplyType(R_VARIABLE_APPLY_NODE);
    velocityPotential.resize(1,this->pModel->getNNodes());
    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        velocityPotential.setValue(0,i,this->nodeVelocityPotential[i]);
    }

    // Acoustic pressure
    uint acousticPressurePos = this->pModel->findVariable(R_VARIABLE_ACOUSTIC_PRESSURE);
    if (acousticPressurePos == RConstants::eod)
    {
        acousticPressurePos = this->pModel->addVariable(R_VARIABLE_ACOUSTIC_PRESSURE);
        this->pModel->getVariable(acousticPressurePos).getVariableData().setMinMaxDisplayValue(
                    RStatistics::findMinimumValue(this->nodeAcousticPressure),
                    RStatistics::findMaximumValue(this->nodeAcousticPressure));
    }
    RVariable &acousticPressure =  this->pModel->getVariable(acousticPressurePos);

    acousticPressure.setApplyType(R_VARIABLE_APPLY_NODE);
    acousticPressure.resize(1,this->pModel->getNNodes());
    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        acousticPressure.setValue(0,i,this->nodeAcousticPressure[i]);
    }

    // Acoustic particle velocity
    uint acousticParticleVelocityPos = this->pModel->findVariable(R_VARIABLE_ACOUSTIC_PARTICLE_VELOCITY);
    if (acousticParticleVelocityPos == RConstants::eod)
    {
        acousticParticleVelocityPos = this->pModel->addVariable(R_VARIABLE_ACOUSTIC_PARTICLE_VELOCITY);

        double umin = 0.0;
        double umax = 0.0;
        for (uint i=0;i<this->elementAcousticParticleVelocity.x.size();i++)
        {
            double u = RR3Vector(this->elementAcousticParticleVelocity.x[i],
                                 this->elementAcousticParticleVelocity.y[i],
                                 this->elementAcousticParticleVelocity.z[i]).length();
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

        this->pModel->getVariable(acousticParticleVelocityPos).getVariableData().setMinMaxDisplayValue(umin,umax);
    }
    RVariable &acousticParticleVelocity =  this->pModel->getVariable(acousticParticleVelocityPos);

    acousticParticleVelocity.setApplyType(R_VARIABLE_APPLY_ELEMENT);
    acousticParticleVelocity.resize(3,this->pModel->getNElements());
    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        acousticParticleVelocity.setValue(0,i,this->elementAcousticParticleVelocity.x[i]);
        acousticParticleVelocity.setValue(1,i,this->elementAcousticParticleVelocity.y[i]);
        acousticParticleVelocity.setValue(2,i,this->elementAcousticParticleVelocity.z[i]);
    }

    RLogger::unindent();
}

void RSolverAcoustic::statistics(void)
{
    this->printStats(R_VARIABLE_POTENTIAL);
    this->printStats(R_VARIABLE_ACOUSTIC_PARTICLE_VELOCITY);
    this->printStats(R_VARIABLE_ACOUSTIC_PRESSURE);
    this->processMonitoringPoints();
}

std::vector<bool> RSolverAcoustic::findAbsorbingBoundaryNodes(void) const
{
    std::vector<bool> absorbingBoundaryNodes;
    absorbingBoundaryNodes.resize(this->pModel->getNNodes(),false);

    for (uint i=0;i<this->pModel->getNPoints();i++)
    {
        const RPoint &rPoint(this->pModel->getPoint(i));
        if (rPoint.hasBoundaryCondition(R_BOUNDARY_CONDITION_ABSORBING_BOUNDARY))
        {
            for (uint j=0;j<rPoint.size();j++)
            {
                const RElement &rElement(this->pModel->getElement(rPoint.get(j)));
                for (uint k=0;k<rElement.size();k++)
                {
                    absorbingBoundaryNodes[rElement.getNodeId(k)] = true;
                }
            }
        }
    }
    for (uint i=0;i<this->pModel->getNLines();i++)
    {
        const RLine &rLine(this->pModel->getLine(i));
        if (rLine.hasBoundaryCondition(R_BOUNDARY_CONDITION_ABSORBING_BOUNDARY))
        {
            for (uint j=0;j<rLine.size();j++)
            {
                const RElement &rElement(this->pModel->getElement(rLine.get(j)));
                for (uint k=0;k<rElement.size();k++)
                {
                    absorbingBoundaryNodes[rElement.getNodeId(k)] = true;
                }
            }
        }
    }
    for (uint i=0;i<this->pModel->getNSurfaces();i++)
    {
        const RSurface &rSurface(this->pModel->getSurface(i));
        if (rSurface.hasBoundaryCondition(R_BOUNDARY_CONDITION_ABSORBING_BOUNDARY))
        {
            for (uint j=0;j<rSurface.size();j++)
            {
                const RElement &rElement(this->pModel->getElement(rSurface.get(j)));
                for (uint k=0;k<rElement.size();k++)
                {
                    absorbingBoundaryNodes[rElement.getNodeId(k)] = true;
                }
            }
        }
    }
    for (uint i=0;i<this->pModel->getNVolumes();i++)
    {
        const RVolume &rVolume(this->pModel->getVolume(i));
        if (rVolume.hasBoundaryCondition(R_BOUNDARY_CONDITION_ABSORBING_BOUNDARY))
        {
            for (uint j=0;j<rVolume.size();j++)
            {
                const RElement &rElement(this->pModel->getElement(rVolume.get(j)));
                for (uint k=0;k<rElement.size();k++)
                {
                    absorbingBoundaryNodes[rElement.getNodeId(k)] = true;
                }
            }
        }
    }
    return absorbingBoundaryNodes;
}

std::vector<bool> RSolverAcoustic::findAbsorbingBoundaryElements() const
{
    std::vector<bool> absorbingBoundaryElements;
    absorbingBoundaryElements.resize(this->pModel->getNElements(),false);

    for (uint i=0;i<this->pModel->getNPoints();i++)
    {
        const RPoint &rPoint(this->pModel->getPoint(i));
        if (rPoint.getVolume() < RConstants::eps)
        {
            continue;
        }
        if (rPoint.hasBoundaryCondition(R_BOUNDARY_CONDITION_ABSORBING_BOUNDARY))
        {
            for (uint j=0;j<rPoint.size();j++)
            {
                absorbingBoundaryElements[rPoint.get(j)] = true;
            }
        }
    }
    for (uint i=0;i<this->pModel->getNLines();i++)
    {
        const RLine &rLine(this->pModel->getLine(i));
        if (rLine.getCrossArea() < RConstants::eps)
        {
            continue;
        }
        if (rLine.hasBoundaryCondition(R_BOUNDARY_CONDITION_ABSORBING_BOUNDARY))
        {
            for (uint j=0;j<rLine.size();j++)
            {
                absorbingBoundaryElements[rLine.get(j)] = true;
            }
        }
    }
    for (uint i=0;i<this->pModel->getNSurfaces();i++)
    {
        const RSurface &rSurface(this->pModel->getSurface(i));
        if (rSurface.getThickness() < RConstants::eps)
        {
            continue;
        }
        if (rSurface.hasBoundaryCondition(R_BOUNDARY_CONDITION_ABSORBING_BOUNDARY))
        {
            for (uint j=0;j<rSurface.size();j++)
            {
                absorbingBoundaryElements[rSurface.get(j)] = true;
            }
        }
    }
    for (uint i=0;i<this->pModel->getNVolumes();i++)
    {
        const RVolume &rVolume(this->pModel->getVolume(i));
        if (rVolume.hasBoundaryCondition(R_BOUNDARY_CONDITION_ABSORBING_BOUNDARY))
        {
            for (uint j=0;j<rVolume.size();j++)
            {
                absorbingBoundaryElements[rVolume.get(j)] = true;
            }
        }
    }
    return absorbingBoundaryElements;
}

std::vector<RR3Vector> RSolverAcoustic::findAbsorbingBoundaryNormals(const std::vector<bool> &absorbingBoundaryNodes) const
{
    std::vector<RR3Vector> normals;
    normals.resize(this->pModel->getNNodes(),RR3Vector(0.0,0.0,0.0));

    RUVector ncount(this->pModel->getNNodes(),0);

    // Find elements which belong to absorbing boundary.
    std::vector<bool> absorbingBoundaryElements = this->findAbsorbingBoundaryElements();

    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        if (!absorbingBoundaryElements[i])
        {
            continue;
        }

        const RElement &rElement(this->pModel->getElement(i));
        std::vector<RElement> edgeElements = rElement.generateEdgeElements();
        for (uint j=0;j<edgeElements.size();j++)
        {
            const RElement &rEdgeElement = edgeElements[i];
            bool isAbsorbing = true;

            for (uint k=0;k<rEdgeElement.size();k++)
            {
                if (!absorbingBoundaryNodes[rEdgeElement.getNodeId(k)])
                {
                    isAbsorbing = false;
                    break;
                }
            }
            if (!isAbsorbing)
            {
                continue;
            }

            RR3Vector edgeElementNormal;
            bool normalFound = false;

            if (R_ELEMENT_TYPE_IS_POINT(rEdgeElement.getType()))
            {
                uint n1 = 1, n2 = 0;

                if (rEdgeElement.getNodeId(0) == rElement.getNodeId(0))
                {
                    n1 = 0;
                    n2 = 1;
                }

                RR3Vector::subtract(this->pModel->getNode(rEdgeElement.getNodeId(n1)).toVector(),
                                    this->pModel->getNode(rEdgeElement.getNodeId(n2)).toVector(),
                                    edgeElementNormal);

                edgeElementNormal.normalize();
                normalFound = true;
            }
            else if (R_ELEMENT_TYPE_IS_LINE(rEdgeElement.getType()))
            {
                RR3Vector elementNormal;
                if (rElement.findNormal(this->pModel->getNodes(),elementNormal[0],elementNormal[1],elementNormal[2]))
                {
                    RR3Vector direction;

                    RR3Vector::subtract(this->pModel->getNode(rEdgeElement.getNodeId(1)).toVector(),
                                        this->pModel->getNode(rEdgeElement.getNodeId(0)).toVector(),
                                        direction);

                    RR3Vector::cross(elementNormal,direction,edgeElementNormal);
                    edgeElementNormal.normalize();
                    normalFound = true;
                }
            }
            else if (R_ELEMENT_TYPE_IS_SURFACE(rEdgeElement.getType()))
            {
                normalFound = rEdgeElement.findNormal(this->pModel->getNodes(),edgeElementNormal[0],edgeElementNormal[1],edgeElementNormal[2]);
            }

            if (normalFound)
            {
                for (uint l=0;l<rEdgeElement.size();l++)
                {
                    uint nodeID = rEdgeElement.getNodeId(l);
                    normals[nodeID][0] += edgeElementNormal[0];
                    normals[nodeID][1] += edgeElementNormal[1];
                    normals[nodeID][2] += edgeElementNormal[2];
                    ncount[nodeID] ++;
                }
            }
        }
    }

    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        if (ncount[i] > 0)
        {
            normals[i][0] /= double(ncount[i]);
            normals[i][1] /= double(ncount[i]);
            normals[i][2] /= double(ncount[i]);
        }
    }

    return normals;
}

void RSolverAcoustic::assemblyMatrix(uint elementID, const RRMatrix &Me, const RRMatrix &Ce, const RRMatrix &Ke, const RRVector &fe)
{
    double alpha = 1.0 / 2.0;
    double beta = this->pModel->getTimeSolver().getTimeMarchApproximationCoefficient() / 2.0;
    double dt = this->pModel->getTimeSolver().getCurrentTimeStepSize();

    const RElement &element = this->pModel->getElement(elementID);

    RRMatrix Ae(element.size(),element.size());
    RRVector be(element.size());

    Ae.fill(0.0);
    be.fill(0.0);

    double a0 = 1.0 / (beta * std::pow(dt,2));
    double a1 = alpha / (beta * dt);
    double a2 = 1.0 / (beta * dt);
    double a3 = (1.0 / (2.0 * beta)) - 1.0;
    double a4 = (alpha / beta) - 1.0;
    double a5 = (1.0 / 2.0) * dt * ((alpha / beta) - 2.0);

    if (this->pModel->getTimeSolver().getEnabled())
    {
        for (uint m=0;m<element.size();m++)
        {
            be[m] = fe[m];
            for (uint n=0;n<element.size();n++)
            {
                double pu = this->nodeVelocityPotential[element.getNodeId(n)];
                double pv = this->nodeVelocityPotentialVelocity[element.getNodeId(n)];
                double pa = this->nodeVelocityPotentialAcceleration[element.getNodeId(n)];

                Ae[m][n] = Ke[m][n] + a0 * Me[m][n] + a1 * Ce[m][n];
                be[m] += Me[m][n] * (a0 * pu + a2 * pv + a3 * pa) + Ce[m][n] * (a1 * pu + a4 * pv + a5 * pa);
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
                be[n] -= Ae[n][m] * this->nodeVelocityPotential[nodeID];
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
