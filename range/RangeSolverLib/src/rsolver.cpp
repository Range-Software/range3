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
#include "rsolvercontaminant.h"
#include "rsolverelectrostatics.h"
#include "rsolvermagnetostatics.h"
#include "rsolverfluid.h"
#include "rsolverfluidheat.h"
#include "rsolverheat.h"
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

        if (problemTypeMask & R_PROBLEM_ACOUSTICS)
        {
            // NOT WORKING
            this->solvers[R_PROBLEM_ACOUSTICS] = new RSolverAcoustic(this->pModel,this->modelFileName,this->sharedData);
        }
        if (problemTypeMask & R_PROBLEM_CONTAMINANT)
        {
            this->solvers[R_PROBLEM_CONTAMINANT] = new RSolverContaminant(this->pModel,this->modelFileName,this->sharedData);
        }
        if (problemTypeMask & R_PROBLEM_ELECTROSTATICS)
        {
            this->solvers[R_PROBLEM_ELECTROSTATICS] = new RSolverElectrostatics(this->pModel,this->modelFileName,this->sharedData);
        }
        if (problemTypeMask & R_PROBLEM_MAGNETOSTATICS)
        {
            this->solvers[R_PROBLEM_MAGNETOSTATICS] = new RSolverMagnetostatics(this->pModel,this->modelFileName,this->sharedData);
        }
        if (problemTypeMask & R_PROBLEM_FLUID)
        {
            this->solvers[R_PROBLEM_FLUID] = new RSolverFluid(this->pModel,this->modelFileName,this->sharedData);
        }
        if (problemTypeMask & R_PROBLEM_FLUID_HEAT)
        {
            this->solvers[R_PROBLEM_FLUID_HEAT] = new RSolverFluidHeat(this->pModel,this->modelFileName,this->sharedData);
        }
        if (problemTypeMask & R_PROBLEM_RADIATIVE_HEAT)
        {
            this->solvers[R_PROBLEM_RADIATIVE_HEAT] = new RSolverRadiativeHeat(this->pModel,this->modelFileName,this->sharedData);
        }
        if (problemTypeMask & R_PROBLEM_HEAT)
        {
            this->solvers[R_PROBLEM_HEAT] = new RSolverHeat(this->pModel,this->modelFileName,this->sharedData);
        }
        if (problemTypeMask & R_PROBLEM_STRESS)
        {
            this->solvers[R_PROBLEM_STRESS] = new RSolverStress(this->pModel,this->modelFileName,this->sharedData,false);
        }
        if (problemTypeMask & R_PROBLEM_STRESS_MODAL)
        {
            this->solvers[R_PROBLEM_STRESS_MODAL] = new RSolverStress(this->pModel,this->modelFileName,this->sharedData,true);
        }
        if (problemTypeMask & R_PROBLEM_POTENTIAL)
        {
            // NOT IMPLEMENTED
        }
        if (problemTypeMask & R_PROBLEM_WAVE)
        {
            // NOT WORKING
            this->solvers[R_PROBLEM_WAVE] = new RSolverWave(this->pModel,this->modelFileName,this->sharedData);
        }

        // if not restarting clear results data.
        if (!this->pModel->getProblemSetup().getRestart())
        {
            this->pModel->removeAllVariables();
        }
    }
}

RSolver::RSolver(RModel &model, const QString &modelFileName)
    : pModel(&model)
    , modelFileName(modelFileName)
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
        } while (timeSolver.setNextTimeStep() != RConstants::eod
                 &&
                 RApplicationState::getInstance().getStateType() != R_APPLICATION_STATE_STOP);
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
            RLogger::unindent();

            this->solversExecutionCount[problemType]++;
        }
    }

    return converged;
}