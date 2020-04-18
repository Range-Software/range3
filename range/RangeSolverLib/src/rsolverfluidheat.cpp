/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolverfluidheat.cpp                                     *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   28-th January 2016                                       *
 *                                                                   *
 *  DESCRIPTION: Fluid - heat transfer solver class definition       *
 *********************************************************************/

#include <omp.h>

#include "rsolverfluid.h"
#include "rsolverfluidheat.h"
#include "rmatrixsolver.h"

class FluidHeatMatrixContainer
{
    public:

        bool initialized;

        // Element level matricies
        RRMatrix me;    // m
        RRMatrix ce;    // c
        RRMatrix ke;    // k
        RRMatrix cte;   // c~
        RRMatrix kte;   // k~
        RRMatrix yte;   // y~

        // Element level vectors
        RRVector fv;    // f

    public:

        FluidHeatMatrixContainer() : initialized(false)
        {

        }

        void resize(uint nen)
        {
            this->me.resize(nen,nen,0.0);
            this->ce.resize(nen,nen,0.0);
            this->ke.resize(nen,nen,0.0);
            this->cte.resize(nen,nen,0.0);
            this->kte.resize(nen,nen,0.0);
            this->yte.resize(nen,nen,0.0);

            this->fv.resize(nen,0.0);

            this->initialized = true;
        }

        void clear(void)
        {
            this->me.fill(0.0);
            this->ce.fill(0.0);
            this->ke.fill(0.0);
            this->cte.fill(0.0);
            this->kte.fill(0.0);
            this->yte.fill(0.0);

            this->fv.fill(0.0);
        }
};

void RSolverFluidHeat::_init(const RSolverFluidHeat *pSolver)
{
    if (pSolver)
    {
        this->elementCapacity = pSolver->elementCapacity;
        this->elementConduction = pSolver->elementConduction;
        this->elementDensity = pSolver->elementDensity;
        this->nodeTemperature = pSolver->nodeTemperature;
        this->nodeHeat = pSolver->nodeHeat;
        this->elementHeat = pSolver->elementHeat;
        this->elementRadiativeHeat = pSolver->elementRadiativeHeat;
        this->elementJouleHeat = pSolver->elementJouleHeat;
        this->elementHeatFlux = pSolver->elementHeatFlux;
        this->elementVelocity = pSolver->elementVelocity;
        this->nodeVelocity = pSolver->nodeVelocity;
        this->streamVelocity = pSolver->streamVelocity;
    }
}

RSolverFluidHeat::RSolverFluidHeat(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData)
    : RSolverGeneric(pModel,modelFileName,convergenceFileName,sharedData)
{
    this->problemType = R_PROBLEM_FLUID_HEAT;
    this->_init();
}

RSolverFluidHeat::RSolverFluidHeat(const RSolverFluidHeat &solver)
    : RSolverGeneric(solver)
{
    this->_init(&solver);
}

RSolverFluidHeat::~RSolverFluidHeat()
{
    this->clearShapeDerivatives();
}

RSolverFluidHeat &RSolverFluidHeat::operator =(const RSolverFluidHeat &solver)
{
    RSolverGeneric::operator =(solver);
    this->_init(&solver);
    return (*this);
}

bool RSolverFluidHeat::hasConverged(void) const
{
    return true;
}

double RSolverFluidHeat::findTemperatureScale(void) const
{
    return 1.0;
}

void RSolverFluidHeat::generateNodeHeatVector(void)
{
    RBVector heatSetValues;
    this->generateVariableVector(R_VARIABLE_HEAT,this->elementHeat,heatSetValues,true,this->firstRun,this->firstRun);

    this->nodeHeat.fill(0.0); // Heat on node is meant as an input - needs to be cleared
    this->pModel->convertElementToNodeVector(this->elementHeat,heatSetValues,this->nodeHeat,true);

    RRVector qv(this->pModel->getNElements(),0.0);
    RUVector qc(this->pModel->getNElements(),0);

    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        double q = this->elementJouleHeat[i] + this->elementRadiativeHeat[i];

        if (q == 0.0)
        {
            continue;
        }

        const RElement &rElement(this->pModel->getElement(i));
        for (uint j=0;j<rElement.size();j++)
        {
            qv[rElement.getNodeId(j)] += q;
            qc[rElement.getNodeId(j)]++;
        }
    }

    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        if (qc[i] == 0)
        {
            continue;
        }
        this->nodeHeat[i] += qv[i] / double(qc[i]);
    }
}

void RSolverFluidHeat::updateScales(void)
{
    this->scales.setMetre(this->findMeshScale());
    this->scales.setKelvin(this->findTemperatureScale());
}

void RSolverFluidHeat::recover(void)
{
    this->recoveryStopWatch.reset();
    this->recoveryStopWatch.resume();
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

    this->recoverVariable(R_VARIABLE_VELOCITY,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),0,this->nodeVelocity.x,0.0);
    this->recoverVariable(R_VARIABLE_VELOCITY,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),1,this->nodeVelocity.y,0.0);
    this->recoverVariable(R_VARIABLE_VELOCITY,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),2,this->nodeVelocity.z,0.0);
    this->recoveryStopWatch.pause();
}

void RSolverFluidHeat::prepare(void)
{
    RLogger::info("Building matrix system\n");
    RLogger::indent();

    this->buildStopWatch.reset();
    this->assemblyStopWatch.reset();

    RBVector temperatureSetValues;

    this->generateNodeBook(R_PROBLEM_FLUID_HEAT);

    this->generateVariableVector(R_VARIABLE_TEMPERATURE,this->elementTemperature,temperatureSetValues,true,this->firstRun,this->firstRun);
    this->generateMaterialVecor(R_MATERIAL_PROPERTY_THERMAL_CONDUCTIVITY,this->elementConduction);
    this->generateMaterialVecor(R_MATERIAL_PROPERTY_HEAT_CAPACITY,this->elementCapacity);
    this->generateMaterialVecor(R_MATERIAL_PROPERTY_DENSITY,this->elementDensity);

    this->generateNodeHeatVector();

    this->pModel->convertElementToNodeVector(this->elementTemperature,temperatureSetValues,this->nodeTemperature,true);

    this->pModel->convertNodeToElementVector(this->nodeVelocity.x,this->elementVelocity.x);
    this->pModel->convertNodeToElementVector(this->nodeVelocity.y,this->elementVelocity.y);
    this->pModel->convertNodeToElementVector(this->nodeVelocity.z,this->elementVelocity.z);

    this->computeShapeDerivatives();
    this->streamVelocity = RSolverFluid::computeStreamVelocity(*this->pModel,this->nodeVelocity,false);

    this->b.resize(this->nodeBook.getNEnabled());
    this->x.resize(this->nodeBook.getNEnabled());

    this->A.clear();
    this->A.setNRows(this->nodeBook.getNEnabled());
    this->b.fill(0.0);
    this->x.fill(0.0);

    bool abort = false;

    RMatrixManager<FluidHeatMatrixContainer> matrixManager;

    // Compute element matrices
    #pragma omp parallel for default(shared) private(matrixManager)
    for (int64_t i=0;i<int64_t(this->pModel->getNElements());i++)
    {
        uint elementID = i;

        const RElement &element = this->pModel->getElement(elementID);

        #pragma omp flush (abort)
        if (abort)
        {
            continue;
        }
        try
        {
            uint nen = element.size();

            RRMatrix Ae(nen,nen,0.0);
            RRVector be(nen,0.0);

            RStopWatch localStopWatch;
            localStopWatch.reset();

            if (R_ELEMENT_TYPE_IS_VOLUME(element.getType()))
            {
                if (!this->computableElements[elementID])
                {
                    continue;
                }
                localStopWatch.resume();
                this->computeElement(elementID,Ae,be,matrixManager);
                localStopWatch.pause();
            }
            #pragma omp critical
            {
                this->buildStopWatch.addElapsedTime(localStopWatch.getMiliSeconds());

                this->assemblyStopWatch.resume();
                this->assemblyMatrix(elementID,Ae,be);
                this->assemblyStopWatch.pause();
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
        RLogger::unindent();
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to prepare matrix system.");
    }

    RLogger::unindent();
}

void RSolverFluidHeat::solve(void)
{
    RLogger::info("Solving matrix system\n");
    RLogger::indent();

    this->solverStopWatch.reset();
    this->solverStopWatch.resume();

    try
    {
        RLogger::indent();
        RMatrixSolver matrixSolver(this->pModel->getMatrixSolverConf(RMatrixSolverConf::GMRES));
        matrixSolver.solve(this->A,this->b,this->x,R_MATRIX_PRECONDITIONER_JACOBI,1);
        RLogger::unindent();
    }
    catch (RError error)
    {
        RLogger::unindent();
        throw error;
    }

    this->solverStopWatch.pause();

    this->nodeTemperature.resize(this->pModel->getNNodes(),0.0);

    this->updateStopWatch.reset();
    this->updateStopWatch.resume();

    double tOld = RRVector::norm(this->nodeTemperature);

    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        uint position = 0;
        if (this->nodeBook.getValue(i,position))
        {
            this->nodeTemperature[i] = std::max(this->x[position],0.0);
        }
    }

    double t = RRVector::norm(this->nodeTemperature);

    this->cvgT = (t - tOld) / this->scales.findScaleFactor(R_VARIABLE_TEMPERATURE);

    this->updateStopWatch.pause();

    RLogger::unindent();
}

void RSolverFluidHeat::process(void)
{
    double Qx;
    double Qy;
    double Qz;

    // Initialize heat flux vector vector
    this->elementHeatFlux.resize(this->pModel->getNElements(),RR3Vector(0.0,0.0,0.0));

    // Process volume elements.
    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        uint elementID = i;
        const RElement &element = this->pModel->getElement(elementID);
        if (!R_ELEMENT_TYPE_IS_VOLUME(element.getType()) || !this->computableElements[elementID])
        {
            continue;
        }

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

void RSolverFluidHeat::store(void)
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

void RSolverFluidHeat::statistics(void)
{
    static uint counter = 0;
    static double oldResidual = 0.0;

    double scale = std::pow(this->scales.getSecond(),2) / this->scales.getKilogram();
    double residual = RRVector::norm(this->b)*scale;
    double convergence = residual - oldResidual;
    oldResidual = residual;

    std::vector<RIterationInfoValue> cvgValues;
    cvgValues.push_back(RIterationInfoValue("Solver residual",residual));
    cvgValues.push_back(RIterationInfoValue("Solver convergence",convergence));
    cvgValues.push_back(RIterationInfoValue("Temperature convergence",this->cvgT));

    RIterationInfo::writeToFile(this->convergenceFileName,counter,cvgValues);

    this->printStats(R_VARIABLE_TEMPERATURE);
    this->printStats(R_VARIABLE_HEAT_FLUX);
    this->processMonitoringPoints();

    RLogger::info("Convergence:   %-13g\n",residual);
    RLogger::info("Build time:    %9u [ms]\n",this->buildStopWatch.getMiliSeconds());
    RLogger::info("Assembly time: %9u [ms]\n",this->assemblyStopWatch.getMiliSeconds());
    RLogger::info("Solver time:   %9u [ms]\n",this->solverStopWatch.getMiliSeconds());
    RLogger::info("Update time:   %9u [ms]\n",this->updateStopWatch.getMiliSeconds());

    counter++;
}

void RSolverFluidHeat::computeShapeDerivatives(void)
{
    this->shapeDerivations.resize(this->pModel->getNElements(),0);

    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        uint elementID = i;

        const RElement &rElement = this->pModel->getElement(elementID);
        if (R_ELEMENT_TYPE_IS_VOLUME(rElement.getType()))
        {
            if (!this->computableElements[elementID])
            {
                continue;
            }
            if (!this->shapeDerivations[elementID])
            {
                this->shapeDerivations[elementID] = new RElementShapeDerivation(rElement,this->pModel->getNodes(),R_PROBLEM_FLUID);
            }
        }
    }
}

void RSolverFluidHeat::clearShapeDerivatives(void)
{
    for (uint i=0;i<this->shapeDerivations.size();i++)
    {
        delete this->shapeDerivations[i];
    }
}

void RSolverFluidHeat::computeElement(unsigned int elementID, RRMatrix &Ae, RRVector &be, RMatrixManager<FluidHeatMatrixContainer> &matrixManager)
{
    if (RElement::hasConstantDerivative(this->pModel->getElement(elementID).getType()))
    {
        this->computeElementConstantDerivative(elementID,Ae,be,matrixManager);
    }
    else
    {
        this->computeElementGeneral(elementID,Ae,be,matrixManager);
    }
}

void RSolverFluidHeat::computeElementGeneral(unsigned int elementID, RRMatrix &Ae, RRVector &be, RMatrixManager<FluidHeatMatrixContainer> &matrixManager)
{
    bool unsteady = (this->pModel->getTimeSolver().getEnabled());

    const RElement &element = this->pModel->getElement(elementID);
    uint nen = element.size();
    uint nInp = RElement::getNIntegrationPoints(element.getType());

    double ro = this->elementDensity[elementID];
    double c = this->elementCapacity[elementID];
    double k = this->elementConduction[elementID];

    double ca = ro*c;

    Ae.fill(0.0);
    be.fill(0.0);

    FluidHeatMatrixContainer &matrixCotainer = matrixManager.getMatricies(element.getType());
    matrixCotainer.clear();

    // Element level matricies
    RRMatrix &me = matrixCotainer.me;
    RRMatrix &ce = matrixCotainer.ce;
    RRMatrix &ke = matrixCotainer.ke;
    RRMatrix &cte = matrixCotainer.cte;
    RRMatrix &kte = matrixCotainer.kte;
    RRMatrix &yte = matrixCotainer.yte;

    // Element level vectors
    RRVector &fv = matrixCotainer.fv;

    double alpha = this->pModel->getTimeSolver().getTimeMarchApproximationCoefficient();
    double dt = this->pModel->getTimeSolver().getCurrentTimeStepSize();

    // Element level input -------------------------------------------
    RR3Vector ve(this->elementVelocity.x[elementID],
                 this->elementVelocity.y[elementID],
                 this->elementVelocity.z[elementID]);
    // element level velocity magnitude
    double mvh = ve.length();
    // element level velocity direction
    RR3Vector s(ve);
    s.normalize();

    for (uint intPoint=0;intPoint<nInp;intPoint++)
    {
        const RElementShapeFunction &shapeFunc = RElement::getShapeFunction(element.getType(),intPoint);
        const RRVector &N = shapeFunc.getN();
        const RRMatrix &B = this->shapeDerivations[elementID]->getDerivative(intPoint);
        double detJ = this->shapeDerivations[elementID]->getJacobian(intPoint);

        double integValue = detJ * shapeFunc.getW();

        // velocity divergence
        RRVector vdiv(nen,0.0);
        // element length scale
        double h = 0.0;

        for (uint m=0;m<nen;m++)
        {
            vdiv[m] += ve[0] * B[m][0] + ve[1] * B[m][1] + ve[2] * B[m][2];
            h += std::fabs(s[0]*B[m][0] + s[1]*B[m][1] + s[2]*B[m][2]);
        }
        if (h != 0.0)
        {
            h = 2.0/h;
        }
        // Reynolds numbers
        double Re(k == 0.0 ? 0.0 : mvh * h / (2.0 * k));

        // SUPG stabilization parameter
        double Tsupg = 0.0;
        if (mvh > 0.0)
        {
            Tsupg = h / (2.0 * mvh);
            if (Re > 0.0 && Re <= 3.0)
            {
                Tsupg *= Re / 3.0;
            }
        }

        for (uint m=0;m<nen;m++)
        {
            for (uint n=0;n<nen;n++)
            {
                // m matrix
                if (unsteady)
                {
                    me[m][n] = ca * N[m] * N[n];
                }
                // c matrix
                ce[m][n] = ca * N[m] * vdiv[n];
                // k matrix
                ke[m][n] = -k * (B[m][0] * B[n][0] + B[m][1] * B[n][1] + B[m][2] * B[n][2]);
                // k~ matrix
                kte[m][n] = Tsupg * ca * vdiv[m] * vdiv[n];
                // y~ matrix
                yte[m][n] = Tsupg * vdiv[m];
            }
            // f vector
            fv[m] = this->nodeHeat[element.getNodeId(m)] * N[m];
        }

        // Assembly element level matrixes
        for (uint m=0;m<nen;m++)
        {
            for (uint n=0;n<nen;n++)
            {
                if (unsteady)
                {
                    Ae[m][n] += me[m][n] + cte[m][n]
                              + alpha * dt * (  ce[m][n]  + ke[m][n]
                                             + kte[m][n] + yte[m][n] );
                }
                else
                {
                    Ae[m][n] += ce[m][n] + ke[m][n]
                              + kte[m][n] + yte[m][n];
                }
            }
            if (unsteady)
            {
                be[m] += dt * fv[m];
                for (uint n=0;n<nen;n++)
                {
                    be[m] += (me[m][n] + cte[m][n] - (1.0 - alpha) * dt * (ce[m][n] + ke[m][n] + kte[m][n] + yte[m][n]))
                           * this->nodeTemperature[element.getNodeId(n)];
                }
            }
            else
            {
                be[m] = fv[m];
            }
        }
        for (uint m=0;m<nen;m++)
        {
            for (uint n=0;n<nen;n++)
            {
                Ae[m][n] *= integValue;
            }
            be[m] *= integValue;
        }
    }
}

void RSolverFluidHeat::computeElementConstantDerivative(unsigned int elementID, RRMatrix &Ae, RRVector &be, RMatrixManager<FluidHeatMatrixContainer> &matrixManager)
{
    bool unsteady = (this->pModel->getTimeSolver().getEnabled());

    const RElement &element = this->pModel->getElement(elementID);
    uint nen = element.size();

    double ro = this->elementDensity[elementID];
    double c = this->elementCapacity[elementID];
    double k = this->elementConduction[elementID];

    double ca = ro*c;

    Ae.fill(0.0);
    be.fill(0.0);

    FluidHeatMatrixContainer &matrixCotainer = matrixManager.getMatricies(element.getType());
    matrixCotainer.clear();

    // Element level matricies
    RRMatrix &me = matrixCotainer.me;
    RRMatrix &ce = matrixCotainer.ce;
    RRMatrix &ke = matrixCotainer.ke;
    RRMatrix &cte = matrixCotainer.cte;
    RRMatrix &kte = matrixCotainer.kte;
    RRMatrix &yte = matrixCotainer.yte;

    // Element level vectors
    RRVector &fv = matrixCotainer.fv;

    double alpha = this->pModel->getTimeSolver().getTimeMarchApproximationCoefficient();
    double dt = this->pModel->getTimeSolver().getCurrentTimeStepSize();

    // Element level input -------------------------------------------
    RR3Vector ve(this->elementVelocity.x[elementID],
                 this->elementVelocity.y[elementID],
                 this->elementVelocity.z[elementID]);
    // element level velocity magnitude
    double mvh = ve.length();
    // element level velocity direction
    RR3Vector s(ve);
    s.normalize();

    const RRVector &iN = RElement::getMassVector(element.getType());
    const RRMatrix &iNiN = RElement::getMassMatrix(element.getType());
    double wt = RElement::getTotalWeightFactor(element.getType());
    const RRMatrix &B = this->shapeDerivations[elementID]->getDerivative(0);

    // velocity divergence
    RRVector vdiv(nen,0.0);
    // element length scale
    double h = 0.0;

    for (uint m=0;m<nen;m++)
    {
        vdiv[m] += ve[0] * B[m][0] + ve[1] * B[m][1] + ve[2] * B[m][2];
        h += std::fabs(s[0]*B[m][0] + s[1]*B[m][1] + s[2]*B[m][2]);
    }
    if (h != 0.0)
    {
        h = 2.0/h;
    }
    // Reynolds numbers
    double Re(k == 0.0 ? 0.0 : mvh * h / (2.0 * k));

    // SUPG stabilization parameter
    double Tsupg = 0.0;
    if (mvh > 0.0)
    {
        Tsupg = h / (2.0 * mvh);
        if (Re > 0.0 && Re <= 3.0)
        {
            Tsupg *= Re / 3.0;
        }
    }

    for (uint m=0;m<nen;m++)
    {
        for (uint n=0;n<nen;n++)
        {
            // m matrix
            if (unsteady)
            {
                me[m][n] = ca * iNiN[m][n];
            }
            // c matrix
            ce[m][n] = ca * iN[m] * vdiv[n];
            // k matrix
            ke[m][n] = -k * wt * (B[m][0] * B[n][0] + B[m][1] * B[n][1] + B[m][2] * B[n][2]);
            // k~ matrix
            kte[m][n] = Tsupg * ca * vdiv[m] * vdiv[n] * wt;
            // y~ matrix
            yte[m][n] = Tsupg * vdiv[m] * wt;
        }
        // f vector
        fv[m] = this->nodeHeat[element.getNodeId(m)] * iN[m];
    }

    // Assembly element level matrixes
    for (uint m=0;m<nen;m++)
    {
        for (uint n=0;n<nen;n++)
        {
            if (unsteady)
            {
                Ae[m][n] = me[m][n] + cte[m][n]
                         + alpha * dt * (  ce[m][n]  + ke[m][n]
                                         + kte[m][n] + yte[m][n] );
            }
            else
            {
                Ae[m][n] = ce[m][n] + ke[m][n]
                         + kte[m][n] + yte[m][n];
            }
        }
        if (unsteady)
        {
            be[m] = dt * fv[m];
            for (uint n=0;n<nen;n++)
            {
                be[m] += (me[m][n] + cte[m][n] - (1.0 - alpha) * dt * (ce[m][n] + ke[m][n] + kte[m][n] + yte[m][n]))
                       * this->nodeTemperature[element.getNodeId(n)];
            }
        }
        else
        {
            be[m] = fv[m];
        }
    }

    double detJ = this->shapeDerivations[elementID]->getJacobian(0);
    Ae *= detJ;
    be *= detJ;
}

void RSolverFluidHeat::assemblyMatrix(unsigned int elementID, const RRMatrix &Ae, const RRVector &be)
{
    const RElement &rElement = this->pModel->getElement(elementID);
    RRVector fe(be);

    // Apply explicit boundary conditions.
    for (uint m=0;m<rElement.size();m++)
    {
        uint mp = 0;
        if (!this->nodeBook.getValue(rElement.getNodeId(m),mp))
        {
            for (uint n=0;n<rElement.size();n++)
            {
                fe[n] -= Ae[n][m] * this->nodeTemperature[rElement.getNodeId(m)];
            }
        }
    }

    // Assembly final matrix system
    for (uint m=0;m<rElement.size();m++)
    {
        uint mp = 0;

        if (this->nodeBook.getValue(rElement.getNodeId(m),mp))
        {
            this->b[mp] += fe[m];

            for (uint n=0;n<rElement.size();n++)
            {
                uint np = 0;

                if (this->nodeBook.getValue(rElement.getNodeId(n),np))
                {
                    this->A.addValue(mp,np,Ae[m][n]);
                }
            }
        }
    }
}
