/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   solver_task.cpp                                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   25-th August 2013                                        *
 *                                                                   *
 *  DESCRIPTION: Solver task class definition                        *
 *********************************************************************/

#include <omp.h>
#include <unistd.h>

#include <algorithm>

#include <ralib.h>
#include <rmlib.h>
#include <rsolverlib.h>

#include "solver_task.h"

SolverTask::SolverTask(const SolverInput &solverInput, QCoreApplication *app, QObject *parent)
    : QObject(parent)
    , modelFileName(solverInput.modelFileName)
    , moduleLicenseFileName(solverInput.moduleLicenseFileName)
    , account(solverInput.account)
    , password(solverInput.password)
    , convergenceFileName(solverInput.convergenceFileName)
    , monitoringFileName(solverInput.monitoringFileName)
    , nThreads(solverInput.nThreads)
    , app(app)
{
    this->nThreads = std::max(this->nThreads,uint(1));
    this->nThreads = std::min(this->nThreads,uint(omp_get_max_threads()));
}

void SolverTask::run(void)
{
    omp_set_num_threads(this->nThreads);

    RModel model;
    RLicense license;

    // Load model
    try
    {
        model.read(this->modelFileName);
        RLogger::info("Model has been successfully loaded from file \'%s\'.\n", this->modelFileName.toUtf8().constData());
    }
    catch (const RError &error)
    {
        RLogger::error("Failed to load model from file \'%s\'. %s\n", this->modelFileName.toUtf8().constData(), error.getMessage().toUtf8().constData());

        emit this->finished();
        this->app->exit(1);
        return;
    }

    // Load license
    try
    {
        license.read(this->moduleLicenseFileName);
        RLogger::info("License has been successfully loaded from file \'%s\'.\n", this->moduleLicenseFileName.toUtf8().constData());
    }
    catch (const RError &error)
    {
        RLogger::error("Failed to load license from file \'%s\'. %s\n", this->moduleLicenseFileName.toUtf8().constData(), error.getMessage().toUtf8().constData());

        emit this->finished();
        this->app->exit(1);
        return;
    }

    // Validate problem types against licenses.
    std::vector<RProblemType> problemTypes(RProblem::getTypes(model.getProblemTaskTree().getProblemTypeMask()));
    bool licenseValid = true;
    for (uint i=0;i<problemTypes.size();i++)
    {
        if (!license.validateRecord(RProblem::getId(problemTypes[i]),this->account,this->password))
        {
            RLogger::warning("Missing license for \'%s\' (product-id: %s)\n",
                             RProblem::getName(problemTypes[i]).toUtf8().constData(),
                             RProblem::getId(problemTypes[i]).toUtf8().constData());
            licenseValid = false;
        }
    }
    if (!licenseValid)
    {
        RLogger::error("Failed to validate problem types against license file \'%s\'.\n", this->moduleLicenseFileName.toUtf8().constData());

        emit this->finished();
        this->app->exit(1);
        return;
    }

    model.getMatrixSolverConf(RMatrixSolverConf::CG).setOutputFileName(this->convergenceFileName);
    model.getMatrixSolverConf(RMatrixSolverConf::GMRES).setOutputFileName(this->convergenceFileName);
    model.getMonitoringPointManager().setOutputFileName(this->monitoringFileName);

    // Solve model
    try
    {
        RLogger::info("\n");
        RLogger::info("********* SOLVER - BEGIN *********\n");
        RLogger::info("\n");
        RLogger::indent();
        RSolver solver(model,this->modelFileName);
        solver.run();
        RLogger::unindent();
        RLogger::info("\n");
        RLogger::info("********** SOLVER - END **********\n");
        RLogger::info("\n");
    }
    catch (const RError &error)
    {
        RLogger::unindent();
        RLogger::error("Failed to solve model from file \'%s\'. %s\n", this->modelFileName.toUtf8().constData(), error.getMessage().toUtf8().constData());

        emit this->finished();
        this->app->exit(1);
        return;
    }

    // Save model
    try
    {
        this->modelFileName = model.write(this->modelFileName);
        RLogger::info("Model has been successfully saved to file \'%s\'.\n", this->modelFileName.toUtf8().constData());
    }
    catch (const RError &error)
    {
        RLogger::error("Failed to save model to file \'%s\'. %s\n", this->modelFileName.toUtf8().constData(), error.getMessage().toUtf8().constData());

        emit this->finished();
        this->app->exit(1);
        return;
    }

    emit this->finished();
    this->app->exit(0);
}
