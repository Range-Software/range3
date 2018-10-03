/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   solver_task.cpp                                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   10-th December 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Solver task class definition                        *
 *********************************************************************/

#include <rblib.h>

#include "model_io.h"
#include "solver_manager.h"
#include "solver_process.h"
#include "solver_task.h"
#include "session.h"
#include "main_settings.h"

SolverTask::SolverTask(const ApplicationSettings *applicationSettings, uint modelID, QObject *parent)
    : Job(parent)
    , applicationSettings(applicationSettings)
    , modelID(modelID)
    , solverProcess(0)
{
    this->taskID.generate();
    this->solverExecutable = applicationSettings->getSolverPath();

    Model &rModel = Session::getInstance().getModel(this->modelID);

    QFileInfo modelFileInfo(rModel.getFileName());

    this->modelFileName = modelFileInfo.absoluteFilePath();
    this->logFileName = rModel.buildTmpFileName("log",this->taskID.toString());
    this->convergenceFileName = rModel.buildTmpFileName("cvg",this->taskID.toString());
    this->monitoringFileName = rModel.buildTmpFileName("mon",this->taskID.toString());

    this->solverArguments.append("--file=" + this->modelFileName);
    this->solverArguments.append("--log-file=" + this->logFileName);
    this->solverArguments.append("--convergence-file=" + this->convergenceFileName);
    this->solverArguments.append("--monitoring-file=" + this->monitoringFileName);
    this->solverArguments.append("--nthreads=" + QString::number(this->applicationSettings->getNThreads()));
    this->solverArguments.append("--task-id=" + this->taskID.toString());
    this->solverArguments.append("--task-server=" + SolverManager::getInstance().getTaskServerName());
}

uint SolverTask::getModelID(void) const
{
    return this->modelID;
}

const SolverTaskID &SolverTask::getTaskID(void) const
{
    return this->taskID;
}

SolverTaskID &SolverTask::getTaskID(void)
{
    return this->taskID;
}

const QString SolverTask::getStopCommand(void) const
{
    return QString(this->taskID.toString() + ":STOP");
}

void SolverTask::kill(void)
{
    RLogger::info("Killing solver task (#%s).\n",this->taskID.toString().toUtf8().constData());
    this->solverProcess->kill();
}

const QString &SolverTask::getLogFileName(void) const
{
    return this->logFileName;
}

QString SolverTask::getCommandLine(void) const
{
    QString cmdLine = this->solverExecutable;
    for (int i=0;i<this->solverArguments.size();i++)
    {
        cmdLine += QString(" \"") + this->solverArguments.at(i) + QString("\"");
    }
    return cmdLine;
}

void SolverTask::run(void)
{
    try
    {
        Session::getInstance().storeCurentModelVersion(this->modelID,tr("Execute solver task"));

        RLogger::info("Solver task (#%s) - Begin\n",this->taskID.toString().toUtf8().constData());
        RLogger::indent();

        Model &rModel = Session::getInstance().getModel(this->modelID);
        QString modelFileName(RFileManager::getFileNameWithOutTimeStep(rModel.getFileName()));

        // Save model first.
        try
        {
            emit this->isBlocking(true);
            ModelIO::save(rModel,modelFileName,false);
            emit this->isBlocking(false);
        }
        catch (const RError &error)
        {
            RLogger::error("%s File: '%s'\n",
                           error.getMessage().toUtf8().constData(),
                           modelFileName.toUtf8().constData());
            emit this->isBlocking(false);
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to start the solver because model could not be saved.");
        }

        // Execute solver
        QString cmdLine = this->getCommandLine();

        RLogger::info("Executing \'%s\'\n",cmdLine.toUtf8().constData());

        this->solverProcess = new SolverProcess(this);

        QObject::connect(this->solverProcess,
                         &SolverProcess::readyReadStandardOutput,
                         this,
                         &SolverTask::onProcessReadyReadStandardOutput);
        QObject::connect(this->solverProcess,
                         &SolverProcess::readyReadStandardError,
                         this,
                         &SolverTask::onProcessReadyReadStandardError);

        this->solverProcess->start(this->solverExecutable,this->solverArguments);

        if (!this->solverProcess->waitForFinished(-1) || this->solverProcess->exitCode() != 0)
        {
            RLogger::warning("Command \'%s\' failed with exit code = %d.\n",
                             cmdLine.toUtf8().constData(),
                             this->solverProcess->exitCode());

            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Solver execution failed.");
        }

        RLogger::info("Command \'%s\' succesfully finished.\n",cmdLine.toUtf8().constData());

        // Update model.
        try
        {
            emit this->isBlocking(true);
            ModelIO::update(rModel,modelFileName);
            emit this->isBlocking(false);
        }
        catch (const RError &error)
        {
            RLogger::error("%s File: '%s'\n",
                           error.getMessage().toUtf8().constData(),
                           modelFileName.toUtf8().constData());
            emit this->isBlocking(false);
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to finish the solver because model could not be opened.");
        }

        RLogger::unindent();
        RLogger::info("Solver task (#%s) - End\n",this->taskID.toString().toUtf8().constData());

        Session::getInstance().setModelChanged(this->modelID);
    }
    catch (const RError &error)
    {
        RLogger::unindent();
        RLogger::error("Solver task (#%s) failed: %s\n",this->taskID.toString().toUtf8().constData(),error.getMessage().toUtf8().constData());
    }
}

void SolverTask::onProcessReadyReadStandardOutput(void)
{
    emit this->readyReadStandardOutput(QString::fromLocal8Bit(this->solverProcess->readAllStandardOutput()));
}

void SolverTask::onProcessReadyReadStandardError(void)
{
    emit this->readyReadStandardError(QString::fromLocal8Bit(this->solverProcess->readAllStandardError()));
}
