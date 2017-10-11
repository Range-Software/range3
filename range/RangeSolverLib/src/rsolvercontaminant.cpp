/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolvercontaminant.cpp                                   *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   28-th January 2016                                       *
 *                                                                   *
 *  DESCRIPTION: Contaminant solver class definition                 *
 *********************************************************************/

#include <omp.h>

#include "rsolvercontaminant.h"
#include "rmatrixsolver.h"

class MatrixContainer
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

        MatrixContainer() : initialized(false)
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

class MatrixManager
{
    public:

        std::vector<MatrixContainer> c;

    public:

        MatrixManager()
        {
            this->c.resize(R_ELEMENT_N_TYPES);
        }

        MatrixContainer &getMatricies(RElementType type)
        {
            if (!this->c[type].initialized)
            {
                this->c[type].resize(RElement::getNNodes(type));
            }
            return this->c[type];
        }

};

void RSolverContaminant::_init(const RSolverContaminant *pSolver)
{
    if (pSolver)
    {
        this->elementDiffusion = pSolver->elementDiffusion;
        this->cvgC = pSolver->cvgC;
    }
}

RSolverContaminant::RSolverContaminant(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData)
    : RSolverFluid(pModel,modelFileName,convergenceFileName,sharedData)
    , cvgC(0.0)
{
    this->problemType = R_PROBLEM_CONTAMINANT;
    this->_init();
}

RSolverContaminant::RSolverContaminant(const RSolverContaminant &solver)
    : RSolverFluid(solver)
{
    this->_init(&solver);
}

RSolverContaminant::~RSolverContaminant()
{
    this->clearShapeDerivatives();
}

RSolverContaminant &RSolverContaminant::operator =(const RSolverContaminant &solver)
{
    RSolverGeneric::operator =(solver);
    this->_init(&solver);
    return (*this);
}

bool RSolverContaminant::hasConverged(void) const
{
    return true;
}

void RSolverContaminant::updateScales(void)
{

}

void RSolverContaminant::recover(void)
{
    this->recoveryStopWatch.reset();
    this->recoveryStopWatch.resume();
    this->recoverVariable(R_VARIABLE_PARTICLE_CONCENTRATION,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),0,this->nodeConcentration,RVariable::getInitValue(R_VARIABLE_PARTICLE_CONCENTRATION));
    this->recoverVariable(R_VARIABLE_VELOCITY,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),0,this->nodeVelocity.x,RVariable::getInitValue(R_VARIABLE_VELOCITY));
    this->recoverVariable(R_VARIABLE_VELOCITY,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),1,this->nodeVelocity.y,RVariable::getInitValue(R_VARIABLE_VELOCITY));
    this->recoverVariable(R_VARIABLE_VELOCITY,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),2,this->nodeVelocity.z,RVariable::getInitValue(R_VARIABLE_VELOCITY));
    this->recoveryStopWatch.pause();
}

void RSolverContaminant::prepare(void)
{
    RLogger::info("Building matrix system\n");
    RLogger::indent();

    this->buildStopWatch.reset();
    this->assemblyStopWatch.reset();

    this->pModel->convertNodeToElementVector(this->nodeVelocity.x,this->elementVelocity.x);
    this->pModel->convertNodeToElementVector(this->nodeVelocity.y,this->elementVelocity.y);
    this->pModel->convertNodeToElementVector(this->nodeVelocity.z,this->elementVelocity.z);

    if (this->taskIteration == 0)
    {
        RSolverGeneric::generateNodeBook(this->problemType);

        this->generateMaterialVecor(R_MATERIAL_PROPERTY_DENSITY,this->elementDensity);
        this->elementDiffusion.resize(this->pModel->getNElements());
        this->elementDiffusion.fill(0.0);

        this->computeElementScales();
        this->computeShapeDerivatives();
        this->streamVelocity = this->computeStreamVelocity(false);
    }

    this->b.resize(this->nodeBook.getNEnabled());
    this->x.resize(this->nodeBook.getNEnabled());

    this->A.clear();
    this->A.setNRows(this->nodeBook.getNEnabled());
    this->b.fill(0.0);
    this->x.fill(0.0);

    bool abort = false;

    MatrixManager matrixManager;

    // Compute element matrices
    #pragma omp parallel for default(shared) private(matrixManager)
    for (uint i=0;i<this->pModel->getNElements();i++)
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
                this->buildStopWatch.addElapsedTime(localStopWatch.getClock());

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

void RSolverContaminant::solve(void)
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

    this->nodeConcentration.resize(this->pModel->getNNodes(),0.0);

    this->updateStopWatch.reset();
    this->updateStopWatch.resume();

    double cOld = RRVector::norm(this->nodeConcentration);

    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        uint position = 0;
        if (this->nodeBook.getValue(i,position))
        {
            this->nodeConcentration[i] = std::max(this->x[position],0.0);
        }
    }

    double c = RRVector::norm(this->nodeConcentration);

    this->cvgC = (c - cOld) / this->scales.findScaleFactor(R_VARIABLE_PARTICLE_CONCENTRATION);

    this->updateStopWatch.pause();

    RLogger::unindent();
}

void RSolverContaminant::process(void)
{

}

void RSolverContaminant::store(void)
{
    RLogger::info("Storing results\n");
    RLogger::indent();

    // Concentration
    uint concentrationPos = this->pModel->findVariable(R_VARIABLE_PARTICLE_CONCENTRATION);
    if (concentrationPos == RConstants::eod)
    {
        concentrationPos = this->pModel->addVariable(R_VARIABLE_PARTICLE_CONCENTRATION);

        this->pModel->getVariable(concentrationPos).getVariableData().setMinMaxDisplayValue(
                    RStatistics::findMinimumValue(this->nodeConcentration),
                    RStatistics::findMaximumValue(this->nodeConcentration));
    }
    RVariable &concentration =  this->pModel->getVariable(concentrationPos);

    concentration.setApplyType(R_VARIABLE_APPLY_NODE);
    concentration.resize(1,this->pModel->getNNodes());
    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        concentration.setValue(0,i,this->nodeConcentration[i]);
    }

    RLogger::unindent();
}

void RSolverContaminant::statistics(void)
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
    cvgValues.push_back(RIterationInfoValue("Concentration convergence",this->cvgC));

    RIterationInfo::writeToFile(this->convergenceFileName,counter,cvgValues);

    this->printStats(R_VARIABLE_VELOCITY);
    this->printStats(R_VARIABLE_PRESSURE);
    this->processMonitoringPoints();

    RLogger::info("Convergence:   %-13g\n",residual);

    RLogger::info("Build time:    %9u [ms]\n",this->buildStopWatch.getMiliSeconds());
    RLogger::info("Assembly time: %9u [ms]\n",this->assemblyStopWatch.getMiliSeconds());
    RLogger::info("Solver time:   %9u [ms]\n",this->solverStopWatch.getMiliSeconds());
    RLogger::info("Update time:   %9u [ms]\n",this->updateStopWatch.getMiliSeconds());

    counter++;
}

void RSolverContaminant::computeElement(unsigned int elementID, RRMatrix &Ae, RRVector &be, MatrixManager &matrixManager)
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

void RSolverContaminant::computeElementGeneral(unsigned int elementID, RRMatrix &Ae, RRVector &be, MatrixManager &matrixManager)
{

}

void RSolverContaminant::computeElementConstantDerivative(unsigned int elementID, RRMatrix &Ae, RRVector &be, MatrixManager &matrixManager)
{
    bool unsteady = (this->pModel->getTimeSolver().getEnabled());

    const RElement &element = this->pModel->getElement(elementID);
    uint nen = element.size();

    double ro = this->elementDensity[elementID];
    double k = this->elementDiffusion[elementID];

    Ae.fill(0.0);
    be.fill(0.0);

    MatrixContainer &matrixCotainer = matrixManager.getMatricies(element.getType());
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
                me[m][n] = ro * iNiN[m][n];
            }
            // c matrix
            ce[m][n] = ro * iN[m] * vdiv[n];
            // k matrix
            ke[m][n] = -k * wt * (B[m][0] * B[n][0] + B[m][1] * B[n][1] + B[m][2] * B[n][2]);
            // k~ matrix
            kte[m][n] = Tsupg * ro * vdiv[m] * vdiv[n] * wt;
            // y~ matrix
            yte[m][n] = Tsupg * vdiv[m] * wt;
        }
        // f vector
        fv[m] = 0.0;
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
                       * this->nodeConcentration[element.getNodeId(n)];
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

void RSolverContaminant::assemblyMatrix(unsigned int elementID, const RRMatrix &Ae, const RRVector &be)
{
    const RElement &rElement = this->pModel->getElement(elementID);

    // Assembly final matrix system
    for (uint m=0;m<rElement.size();m++)
    {
        uint mp = 0;

        if (this->nodeBook.getValue(rElement.getNodeId(m),mp))
        {
            this->b[mp] += be[m];

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
