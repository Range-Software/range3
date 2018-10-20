/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolver.cpp                                              *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   11-th December 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Range solver class definition                       *
 *********************************************************************/

#include "rsolver.h"
#include "rsolveracoustic.h"
#include "rsolverfluidparticle.h"
#include "rsolverelectrostatics.h"
#include "rsolvermagnetostatics.h"
#include "rsolverfluid.h"
#include "rsolverfluidheat.h"
#include "rsolverheat.h"
#include "rsolvermesh.h"
#include "rsolverradiativeheat.h"
#include "rsolverstress.h"
#include "rsolverwave.h"

void RSolver::_init(const RSolver *pSolver)
{
    if (pSolver)
    {
        this->pModel = pSolver->pModel;
        this->sharedData = pSolver->sharedData;
        this->solvers = pSolver->solvers;
        // TODO - implement copy by value for solvers map
//        QMap<RProblemType,RGenericSolver*>::iterator iter = pSolver->solvers.begin();
//        for (iter = pSolver->solvers.begin();
//             iter != pSolver->solvers.end();
//             ++iter)
//        {
//            iter.key();
//            iter.value();
//        }
        this->solversExecutionCount = pSolver->solversExecutionCount;
    }
    else
    {
        this->sharedData.addData("element-temperature",RRVector(this->pModel->getNElements(),RVariable::getInitValue(R_VARIABLE_TEMPERATURE)));
        RProblemTypeMask problemTypeMask = this->pModel->getProblemTaskTree().getProblemTypeMask();

        std::vector<RProblemType> problemTypes = RProblem::getTypes(problemTypeMask);
        for (uint i=0;i<problemTypes.size();i++)
        {
            QString problemConvergenceFileName(RFileManager::getFileNameWithSuffix(this->convergenceFileName,RProblem::getId(problemTypes[i])));
            if (problemTypes[i] == R_PROBLEM_ACOUSTICS)
            {
                // NOT WORKING
                this->solvers[problemTypes[i]] = new RSolverAcoustic(this->pModel,this->modelFileName,problemConvergenceFileName,this->sharedData);
            }
            else if (problemTypes[i] == R_PROBLEM_FLUID_PARTICLE)
            {
                this->solvers[problemTypes[i]] = new RSolverFluidParticle(this->pModel,this->modelFileName,problemConvergenceFileName,this->sharedData);
            }
            else if (problemTypes[i] == R_PROBLEM_ELECTROSTATICS)
            {
                this->solvers[problemTypes[i]] = new RSolverElectrostatics(this->pModel,this->modelFileName,problemConvergenceFileName,this->sharedData);
            }
            else if (problemTypes[i] == R_PROBLEM_MAGNETOSTATICS)
            {
                this->solvers[problemTypes[i]] = new RSolverMagnetostatics(this->pModel,this->modelFileName,problemConvergenceFileName,this->sharedData);
            }
            else if (problemTypes[i] == R_PROBLEM_FLUID)
            {
                this->solvers[problemTypes[i]] = new RSolverFluid(this->pModel,this->modelFileName,problemConvergenceFileName,this->sharedData);
            }
            else if (problemTypes[i] == R_PROBLEM_FLUID_HEAT)
            {
                this->solvers[problemTypes[i]] = new RSolverFluidHeat(this->pModel,this->modelFileName,problemConvergenceFileName,this->sharedData);
            }
            else if (problemTypes[i] == R_PROBLEM_RADIATIVE_HEAT)
            {
                this->solvers[problemTypes[i]] = new RSolverRadiativeHeat(this->pModel,this->modelFileName,problemConvergenceFileName,this->sharedData);
            }
            else if (problemTypes[i] == R_PROBLEM_HEAT)
            {
                this->solvers[problemTypes[i]] = new RSolverHeat(this->pModel,this->modelFileName,problemConvergenceFileName,this->sharedData);
            }
            else if (problemTypes[i] == R_PROBLEM_STRESS)
            {
                this->solvers[problemTypes[i]] = new RSolverStress(this->pModel,this->modelFileName,problemConvergenceFileName,this->sharedData,false);
            }
            else if (problemTypes[i] == R_PROBLEM_STRESS_MODAL)
            {
                this->solvers[problemTypes[i]] = new RSolverStress(this->pModel,this->modelFileName,problemConvergenceFileName,this->sharedData,true);
            }
            else if (problemTypes[i] == R_PROBLEM_POTENTIAL)
            {
                // NOT IMPLEMENTED
            }
            else if (problemTypes[i] == R_PROBLEM_WAVE)
            {
                // NOT WORKING
                this->solvers[problemTypes[i]] = new RSolverWave(this->pModel,this->modelFileName,problemConvergenceFileName,this->sharedData);
            }
            else if (problemTypes[i] == R_PROBLEM_MESH)
            {
                this->solvers[problemTypes[i]] = new RSolverMesh(this->pModel,this->modelFileName,problemConvergenceFileName,this->sharedData);
            }
        }

        // if not restarting clear results data.
        if (!this->pModel->getProblemSetup().getRestart())
        {
            this->pModel->removeAllVariables();
        }
    }
}

RSolver::RSolver(RModel &model, const QString &modelFileName, const QString &convergenceFileName)
    : pModel(&model)
    , modelFileName(modelFileName)
    , convergenceFileName(convergenceFileName)
{
    this->_init();
}

RSolver::RSolver(const RSolver &solver)
{
    this->_init(&solver);
}

RSolver::~RSolver()
{
    foreach (RSolverGeneric *solver, this->solvers)
    {
        delete solver;
    }
}

RSolver &RSolver::operator =(const RSolver &solver)
{
    this->_init(&solver);
    return (*this);
}

void RSolver::run(void)
{
    RTimeSolver &timeSolver = this->pModel->getTimeSolver();
    timeSolver.harmonizeTimesWithInput(this->pModel->getProblemSetup().getRestart());

    if (timeSolver.getEnabled() && RProblem::getTimeSolverEnabled(this->pModel->getProblemTaskTree().getProblemTypeMask()))
    {
        if (this->pModel->getProblemSetup().getRestart())
        {
            timeSolver.setNextTimeStep();
        }
        RSolverGeneric::updateOldRecords(timeSolver,this->modelFileName);

        do {
            RLogger::info("time step: %9u of %-9u | time = % 12e [sec] | dt = % 12e [sec]\n",
                          timeSolver.getCurrentTimeStep()+1,
                          timeSolver.getNTimeSteps(),
                          timeSolver.getCurrentTime(),
                          timeSolver.getCurrentTimeStepSize());
            timeSolver.setComputedTime(timeSolver.getCurrentTime());

            RLogger::indent();
            this->runSingle();
            RLogger::unindent();

            if (RApplicationState::getInstance().getStateType() == R_APPLICATION_STATE_STOP)
            {
                break;
            }
        } while (timeSolver.setNextTimeStep() != RConstants::eod);
    }
    else
    {
        timeSolver.setCurrentTimeStep(0);
        timeSolver.setComputedTime(0.0);
        timeSolver.setTimes(RTimeSolver::findTimesVector(1, 0.0, 0.0));
        this->runSingle();
    }
}

void RSolver::runSingle(void)
{
    this->runProblemTask(this->pModel->getProblemTaskTree(),0);
}

bool RSolver::runProblemTask(const RProblemTaskItem &problemTaskItem, uint taskIteration)
{
    bool converged = false;

    if (problemTaskItem.getProblemType() == R_PROBLEM_NONE)
    {
        for (uint i=0;i<problemTaskItem.getNIterations();i++)
        {
            uint nConverged = 0;
            RLogger::info("Problem task iteration: %u of %u\n",i+1,problemTaskItem.getNIterations());
            RLogger::indent();
            for (uint j=0;j<problemTaskItem.getNChildren();j++)
            {
                if (this->runProblemTask(problemTaskItem.getChild(j),i))
                {
                    nConverged++;
                }
            }
            if (nConverged == problemTaskItem.getNChildren())
            {
                converged = true;
            }
            RLogger::unindent();
            if (converged)
            {
                RLogger::info("All sub-tasks have converged.\n");
                break;
            }

            if (RApplicationState::getInstance().getStateType() == R_APPLICATION_STATE_STOP)
            {
                break;
            }
        }
    }
    else
    {
        if (this->solvers.contains(problemTaskItem.getProblemType()))
        {
            RProblemType problemType = problemTaskItem.getProblemType();
            if (!this->solversExecutionCount.contains(problemType))
            {
                this->solversExecutionCount[problemType] = 0;
            }
            uint executionCount = this->solversExecutionCount[problemType];

            bool firstRun = (executionCount == 0 && !this->pModel->getProblemSetup().getRestart());

            RLogger::info("Solving problem task: %s\n",RProblem::getName(problemTaskItem.getProblemType()).toUtf8().constData());
            RLogger::indent();

            this->solvers[problemType]->run(firstRun,taskIteration);
            converged = this->solvers[problemTaskItem.getProblemType()]->hasConverged();
            if (this->solvers[problemTaskItem.getProblemType()]->getMeshChanged())
            {
                RProblemTypeMask problemTypeMask = this->pModel->getProblemTaskTree().getProblemTypeMask();
                std::vector<RProblemType> problemTypes = RProblem::getTypes(problemTypeMask);
                for (uint i=0;i<problemTypes.size();i++)
                {
                    this->solvers[problemTypes[i]]->setMeshChanged(true);
                }
            }

            RLogger::unindent();

            this->solversExecutionCount[problemType]++;
        }
    }

    return converged;
}
