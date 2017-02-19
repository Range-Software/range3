/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   session.cpp                                              *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   17-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Session class definition                            *
 *********************************************************************/

#include <QDesktopServices>

#include "session.h"
#include "main_settings.h"
#include "model_io.h"
#include "job_manager.h"

Session::Session()
{
    this->downloadManager = new DownloadManager(this);
    this->drawEngine = new DrawEngine(this);

    QObject::connect(&this->pickList,&PickList::pickListChanged,this,&Session::onPickListChanged);
    QObject::connect(this,&Session::modelAdded,this,&Session::onModelAdded);
    QObject::connect(this,&Session::modelChanged,this,&Session::onModelChanged);
    QObject::connect(MainSettings::getInstance().getApplicationSettings(),
                     &ApplicationSettings::nHistoryRecordsChanged,
                     this,
                     &Session::onNHistoryRecordsChanged);
}

Session & Session::getInstance(void)
{
    static Session session;
    return session;
}

void Session::lock(void)
{
    this->mutex.lock();
}

bool Session::trylock(int timeout)
{
    return this->mutex.tryLock(timeout);
}

void Session::unlock(void)
{
    this->mutex.unlock();
}

DownloadManager *Session::getDownloadManager(void)
{
    return this->downloadManager;
}

DrawEngine *Session::getDrawEngine(void)
{
    return this->drawEngine;
}

const QString &Session::getFileName(void) const
{
    return this->fileName;
}

uint Session::getNModels(void) const
{
    return (uint)this->models.size();
}

const Model & Session::getModel(uint position) const
{
    return this->models[position];
}

Model & Session::getModel(uint position)
{
    return this->models[position];
}

const Model * Session::getModelPtr(uint position) const
{
    return &this->models[position];
}

Model * Session::getModelPtr(uint position)
{
    return &this->models[position];
}

void Session::addModel(const Model &model)
{
    this->models.push_back(model);
    this->models.last().initializeMeshInput();
    emit this->modelAdded(this->models.size()-1);
}

void Session::setModel(uint position, const Model &model)
{
    this->models[position] = model;
    this->setModelChanged(position);
}

void Session::removeModel(uint position)
{
    this->models.removeAt(int(position));
    this->pickList.removeItems(position);
    emit this->modelRemoved(position);
}

void Session::setModelSelected(uint modelID, bool selected)
{
    for (uint i=0;i<this->getModel(modelID).getNPoints();i++)
    {
        this->setEntitySelected(modelID,R_ENTITY_GROUP_POINT,i,selected,false);
    }
    for (uint i=0;i<this->getModel(modelID).getNLines();i++)
    {
        this->setEntitySelected(modelID,R_ENTITY_GROUP_LINE,i,selected,false);
    }
    for (uint i=0;i<this->getModel(modelID).getNSurfaces();i++)
    {
        this->setEntitySelected(modelID,R_ENTITY_GROUP_SURFACE,i,selected,false);
    }
    for (uint i=0;i<this->getModel(modelID).getNVolumes();i++)
    {
        this->setEntitySelected(modelID,R_ENTITY_GROUP_VOLUME,i,selected,false);
    }
    for (uint i=0;i<this->getModel(modelID).getNVectorFields();i++)
    {
        this->setEntitySelected(modelID,R_ENTITY_GROUP_VECTOR_FIELD,i,selected,false);
    }
    for (uint i=0;i<this->getModel(modelID).getNScalarFields();i++)
    {
        this->setEntitySelected(modelID,R_ENTITY_GROUP_SCALAR_FIELD,i,selected,false);
    }
    for (uint i=0;i<this->getModel(modelID).getNStreamLines();i++)
    {
        this->setEntitySelected(modelID,R_ENTITY_GROUP_STREAM_LINE,i,selected,false);
    }
    for (uint i=0;i<this->getModel(modelID).getNCuts();i++)
    {
        this->setEntitySelected(modelID,R_ENTITY_GROUP_CUT,i,selected,false);
    }
    for (uint i=0;i<this->getModel(modelID).getNIsos();i++)
    {
        this->setEntitySelected(modelID,R_ENTITY_GROUP_ISO,i,selected,false);
    }
    this->getModel(modelID).setSelected(selected);
    this->setModelSelectionChanged(modelID);
}

void Session::setEntitySelected(uint modelID, REntityGroupType elementGrpType, uint entityID, bool selected, bool notifyModel)
{
    if (this->getModel(modelID).getSelected(elementGrpType,entityID) != selected)
    {
        this->getModel(modelID).setSelected(elementGrpType,entityID,selected);
        emit this->entitySelectionChanged(modelID,elementGrpType,entityID,selected);
        if (notifyModel)
        {
            this->setModelSelectionChanged(modelID);
        }
    }
}

void Session::setEntityVisible(uint modelID, REntityGroupType elementGrpType, uint entityID, bool visible)
{
    if (this->getModel(modelID).getVisible(elementGrpType,entityID) != visible)
    {
        this->getModel(modelID).setVisible(elementGrpType,entityID,visible);
        emit this->entityVisibilityChanged(modelID,elementGrpType,entityID,visible);
        emit this->modelVisibilityChanged(modelID);
    }
}

void Session::setModelVisible(uint modelID, bool visible)
{
    for (uint i=0;i<this->getModel(modelID).getNPoints();i++)
    {
        this->setEntityVisible(modelID,R_ENTITY_GROUP_POINT,i,visible);
    }
    for (uint i=0;i<this->getModel(modelID).getNLines();i++)
    {
        this->setEntityVisible(modelID,R_ENTITY_GROUP_LINE,i,visible);
    }
    for (uint i=0;i<this->getModel(modelID).getNSurfaces();i++)
    {
        this->setEntityVisible(modelID,R_ENTITY_GROUP_SURFACE,i,visible);
    }
    for (uint i=0;i<this->getModel(modelID).getNVolumes();i++)
    {
        this->setEntityVisible(modelID,R_ENTITY_GROUP_VOLUME,i,visible);
    }
    for (uint i=0;i<this->getModel(modelID).getNVectorFields();i++)
    {
        this->setEntityVisible(modelID,R_ENTITY_GROUP_VECTOR_FIELD,i,visible);
    }
    for (uint i=0;i<this->getModel(modelID).getNScalarFields();i++)
    {
        this->setEntityVisible(modelID,R_ENTITY_GROUP_SCALAR_FIELD,i,visible);
    }
    for (uint i=0;i<this->getModel(modelID).getNStreamLines();i++)
    {
        this->setEntityVisible(modelID,R_ENTITY_GROUP_STREAM_LINE,i,visible);
    }
    for (uint i=0;i<this->getModel(modelID).getNCuts();i++)
    {
        this->setEntityVisible(modelID,R_ENTITY_GROUP_CUT,i,visible);
    }
    for (uint i=0;i<this->getModel(modelID).getNIsos();i++)
    {
        this->setEntityVisible(modelID,R_ENTITY_GROUP_ISO,i,visible);
    }
}

QList<uint> Session::getSelectedModelIDs(void) const
{
    QList<uint> selectedModelIDs;

    for (uint i=0;i<this->getNModels();i++)
    {
        // Check if model is partialy selected (NOT completely unselected)
        if (!this->getModel(i).isSelected(false))
        {
            selectedModelIDs.push_back(i);
        }
    }

    return selectedModelIDs;
}

bool Session::isModelSelected(uint modelID)
{
    return (!this->getModel(modelID).isSelected(false));
}

QList<SessionEntityID> Session::getAllEntityIDs(void) const
{
    QList<SessionEntityID> allEntityIDs;

    for (uint i=0;i<this->getNModels();i++)
    {
        allEntityIDs += this->getAllEntityIDs(i);
    }
    return allEntityIDs;
}

QList<SessionEntityID> Session::getAllEntityIDs(uint modelID) const
{
    QList<SessionEntityID> allEntityIDs;
    SessionEntityID entityID;

    entityID.setMid(modelID);

    entityID.setType(R_ENTITY_GROUP_POINT);
    for (uint j=0;j<this->getModel(modelID).getNPoints();j++)
    {
        entityID.setEid(j);
        allEntityIDs.push_back(entityID);
    }

    entityID.setType(R_ENTITY_GROUP_LINE);
    for (uint j=0;j<this->getModel(modelID).getNLines();j++)
    {
        entityID.setEid(j);
        allEntityIDs.push_back(entityID);
    }

    entityID.setType(R_ENTITY_GROUP_SURFACE);
    for (uint j=0;j<this->getModel(modelID).getNSurfaces();j++)
    {
        entityID.setEid(j);
        allEntityIDs.push_back(entityID);
    }

    entityID.setType(R_ENTITY_GROUP_VOLUME);
    for (uint j=0;j<this->getModel(modelID).getNVolumes();j++)
    {
        entityID.setEid(j);
        allEntityIDs.push_back(entityID);
    }

    entityID.setType(R_ENTITY_GROUP_VECTOR_FIELD);
    for (uint j=0;j<this->getModel(modelID).getNVectorFields();j++)
    {
        entityID.setEid(j);
        allEntityIDs.push_back(entityID);
    }

    entityID.setType(R_ENTITY_GROUP_SCALAR_FIELD);
    for (uint j=0;j<this->getModel(modelID).getNScalarFields();j++)
    {
        entityID.setEid(j);
        allEntityIDs.push_back(entityID);
    }

    entityID.setType(R_ENTITY_GROUP_STREAM_LINE);
    for (uint j=0;j<this->getModel(modelID).getNStreamLines();j++)
    {
        entityID.setEid(j);
        allEntityIDs.push_back(entityID);
    }

    entityID.setType(R_ENTITY_GROUP_CUT);
    for (uint j=0;j<this->getModel(modelID).getNCuts();j++)
    {
        entityID.setEid(j);
        allEntityIDs.push_back(entityID);
    }

    entityID.setType(R_ENTITY_GROUP_ISO);
    for (uint j=0;j<this->getModel(modelID).getNIsos();j++)
    {
        entityID.setEid(j);
        allEntityIDs.push_back(entityID);
    }

    return allEntityIDs;
}

QList<SessionEntityID> Session::getSelectedEntityIDs(void) const
{
    QList<SessionEntityID> selectedEntityIDs;

    for (uint i=0;i<this->getNModels();i++)
    {
        selectedEntityIDs += this->getModel(i).getSelectedEntityIDs(i);
    }
    return selectedEntityIDs;
}

QList<SessionEntityID> Session::getVisibleEntityIDs(void) const
{
    QList<SessionEntityID> visibleEntityIDs;

    for (uint i=0;i<this->getNModels();i++)
    {
        visibleEntityIDs += this->getModel(i).getVisibleEntityIDs(i);
    }
    return visibleEntityIDs;
}

QList<SessionEntityID> Session::filterSelectedEntityIDs(const QList<SessionEntityID> selectedEntityIDs, uint modelID, REntityGroupTypeMask entityTypeMask)
{
    QList<SessionEntityID> selectedEntities(selectedEntityIDs);

    for (int i=selectedEntities.size()-1;i>=0;i--)
    {
        if (selectedEntities[i].getMid() == modelID
            &&
            selectedEntities[i].getType() & entityTypeMask
            &&
            R_ENTITY_GROUP_TYPE_IS_ELEMENT(selectedEntities[i].getType()))
        {
            continue;
        }
        selectedEntities.erase(selectedEntities.begin()+i);
    }

    return selectedEntities;
}

uint Session::findModelByName(const QString &modelName)
{
    for (uint i=0;i<this->getNModels();i++)
    {
        if (this->getModel(i).getName() == modelName)
        {
            return i;
        }
    }
    return RConstants::eod;
}

uint Session::findModelByPtr(const Model *pModel)
{
    for (uint i=0;i<this->getNModels();i++)
    {
        if (this->getModelPtr(i) == pModel)
        {
            return i;
        }
    }
    return RConstants::eod;
}

void Session::setModelSelectionChanged(uint modelID)
{
    emit this->modelSelectionChanged(modelID);
}

void Session::setDisplayPropertiesChanged(uint modelID, REntityGroupType elementGrpType, uint entityID)
{
    emit this->displayPropertiesChanged(modelID,elementGrpType,entityID);
}

void Session::setModelChanged(uint modelID)
{
    emit this->modelChanged(modelID);
}

void Session::setModelRenamed(uint modelID)
{
    emit this->modelRenamed(modelID);
}

void Session::setProblemChanged(uint modelID)
{
    emit this->problemChanged(modelID);
}

void Session::setProblemSelectionChanged(uint modelID)
{
    emit this->problemSelectionChanged(modelID);
}

void Session::setResultsChanged(uint modelID)
{
    emit this->resultsChanged(modelID);
}

void Session::setBoundaryConditionChanged(uint modelID, REntityGroupType elementGrpType, uint entityID)
{
    emit this->boundaryConditionChanged(modelID,elementGrpType,entityID);
}

void Session::setInitialConditionChanged(uint modelID, REntityGroupType elementGrpType, uint entityID)
{
    emit this->initialConditionChanged(modelID,elementGrpType,entityID);
}

void Session::setEnvironmentConditionChanged(uint modelID, REntityGroupType elementGrpType, uint entityID)
{
    emit this->environmentConditionChanged(modelID,elementGrpType,entityID);
}

void Session::setMaterialChanged(uint modelID, REntityGroupType elementGrpType, uint entityID)
{
    emit this->materialChanged(modelID,elementGrpType,entityID);
}

void Session::setVariableDataChanged(uint modelID, RVariableType variableType)
{
    emit this->variableDataChanged(modelID,variableType);
}

void Session::setVariableDataChanged(const SessionEntityID &entityID, RVariableType variableType)
{
    emit this->variableDataChanged(entityID,variableType);
}

const PickList &Session::getPickList(void) const
{
    return this->pickList;
}

PickList &Session::getPickList(void)
{
    return this->pickList;
}

void Session::setBeginDrawStreamLinePosition(const RR3Vector &position)
{
    emit this->beginDrawStreamLinePosition(position);
}

void Session::setEndDrawStreamLinePosition(void)
{
    emit this->endDrawStreamLinePosition();
}

void Session::setBeginDrawCutPlane(const RPlane &plane)
{
    emit this->beginDrawCutPlane(plane);
}

void Session::setEndDrawCutPlane(void)
{
    emit this->endDrawCutPlane();
}

void Session::setBeginDrawMoveNodes(const QMap<SessionNodeID, RR3Vector> &nodesToMove)
{
    emit this->beginDrawMoveNodes(nodesToMove);
}

void Session::setEndDrawMoveNodes(void)
{
    emit this->endDrawMoveNodes();
}

void Session::setTakeScreenShot(uint modelID, const QString &screenShotFileName)
{
    emit this->takeScreenShot(modelID, screenShotFileName);
}

void Session::read(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    RLogger::info("Reading session file \'%s\'.\n",fileName.toUtf8().constData());
    RLogger::indent();

    this->fileName = fileName;

    RFile sessionFile(this->fileName,RFile::ASCII);

    if (!sessionFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        RLogger::unindent();
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",this->fileName.toUtf8().constData());
    }

    uint nModels = 0;
    RFileIO::readAscii(sessionFile,nModels);
    RLogger::info("Found %u files\n",nModels);
    for (uint i=0;i<nModels;i++)
    {
        QString modelFileName;
        RFileIO::readAscii(sessionFile,modelFileName);
        RLogger::info("Found %u: \'%s\'\n",i+1,modelFileName.toUtf8().constData());

        if (!modelFileName.isEmpty())
        {
            // Read model.
            ModelIO *modelIO = new ModelIO(MODEL_IO_OPEN, modelFileName);
            modelIO->setAutoDelete(true);

            JobManager::getInstance().submit(modelIO);
        }
    }

    sessionFile.close();
    RLogger::unindent();
}

void Session::write(const QString &fileName, bool writeModels)
{
    RLogger::info("Writing session file \'%s\'.\n",fileName.toUtf8().constData());

    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    this->fileName = fileName;

    RFile sessionFile(this->fileName,RFile::ASCII);

    if (!sessionFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",this->fileName.toUtf8().constData());
    }

    RFileIO::writeAscii(sessionFile,this->getNModels(),true);
    for (uint i=0;i<this->getNModels();i++)
    {
        RFileIO::writeAscii(sessionFile,"\"" + this->getModel(i).getFileName() + "\"",true);

        if (writeModels)
        {
            if (this->getModel(i).getFileName().isEmpty())
            {
                QDir dataDir(MainSettings::getInstancePtr()->getDataDir());
                QString binaryExtension = RModel::getDefaultFileExtension(true);
                QString modelName = this->getModel(i).getName();
                if (modelName.length() == 0)
                {
                    modelName = "Model_" + QString::number(i+1);
                }
                QString modelFileName = dataDir.filePath(modelName + "." + binaryExtension);
                this->getModel(i).setFileName(modelFileName);
            }

            // Write model.
            ModelIO *modelIO = new ModelIO(MODEL_IO_SAVE, this->getModel(i).getFileName(), this->getModelPtr(i));
            modelIO->setAutoDelete(true);

            JobManager::getInstance().submit(modelIO);
        }
    }

    sessionFile.close();
}

void Session::clear(void)
{
    RLogger::info("Closing session\n");
    this->fileName.clear();

    while (this->getNModels() > 0)
    {
        RLogger::indent();
        RLogger::info("Closing model \'%s\'\n",this->getModel(this->getNModels()-1).getName().toUtf8().constData());

        Session::getInstance().removeModel(this->getNModels()-1);

        RLogger::unindent();
    }
}

void Session::storeCurentModelVersion(uint modelID, const QString &message)
{
    this->getModel(modelID).storeCurentVersion(MainSettings::getInstance().getApplicationSettings()->getNHistoryRecords(),message);
}

bool Session::isUndoAvailable(void) const
{
    QList<uint> selectedModelIDs = this->getSelectedModelIDs();
    for (int i=0;i<selectedModelIDs.size();i++)
    {
        if (this->getModel(selectedModelIDs[i]).getUndoStackSize() > 0)
        {
            return true;
        }
    }
    return false;
}

bool Session::isRedoAvailable(void) const
{
    QList<uint> selectedModelIDs = this->getSelectedModelIDs();
    for (int i=0;i<selectedModelIDs.size();i++)
    {
        if (this->getModel(selectedModelIDs[i]).getRedoStackSize() > 0)
        {
            return true;
        }
    }
    return false;
}

QString Session::getUndoTooltip(void) const
{
    QString undoMessage;

    QList<uint> selectedModelIDs = this->getSelectedModelIDs();
    for (int i=0;i<selectedModelIDs.size();i++)
    {
        if (!undoMessage.isEmpty())
        {
            undoMessage += "; ";
        }
        undoMessage += this->getModel(selectedModelIDs[i]).getUndoActionMessage();
    }
    undoMessage = tr("Undo") + (undoMessage.isEmpty() ? QString() : ": " + undoMessage);
    return undoMessage;
}

QString Session::getRedoTooltip(void) const
{
    QString redoMessage;

    QList<uint> selectedModelIDs = this->getSelectedModelIDs();
    for (int i=0;i<selectedModelIDs.size();i++)
    {
        if (!redoMessage.isEmpty())
        {
            redoMessage += "; ";
        }
        redoMessage += this->getModel(selectedModelIDs[i]).getRedoActionMessage();
    }
    redoMessage = tr("Redo") + (redoMessage.isEmpty() ? QString() : ": " + redoMessage);
    return redoMessage;
}

QString Session::getDefaultFileExtension(void)
{
    return "ras";
}

QString Session::getDefaultFileName(void)
{
    QDir sesionDir = MainSettings::getInstancePtr()->getSessionDir();
    return sesionDir.filePath(QString("default.") + Session::getDefaultFileExtension());
}

bool Session::selectedModelsHasEntities(REntityGroupTypeMask entityTypeMask)
{
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        if (((entityTypeMask & R_ENTITY_GROUP_POINT) && Session::getInstance().getModel(modelIDs[i]).getNPoints())
            ||
            ((entityTypeMask & R_ENTITY_GROUP_LINE) && Session::getInstance().getModel(modelIDs[i]).getNLines())
            ||
            ((entityTypeMask & R_ENTITY_GROUP_SURFACE) && Session::getInstance().getModel(modelIDs[i]).getNSurfaces())
            ||
            ((entityTypeMask & R_ENTITY_GROUP_VOLUME) && Session::getInstance().getModel(modelIDs[i]).getNVolumes()))
        {
            return true;
        }
    }
    return false;
}

void Session::onPickListChanged(void)
{
    emit this->pickListChanged();
}

void Session::onModelAdded(uint modelID)
{
    this->getModel(modelID).createDependentEntities();
}

void Session::onModelChanged(uint modelID)
{
    this->getModel(modelID).createDependentEntities();
}

void Session::onNHistoryRecordsChanged(uint nHistoryRecords)
{
    for (uint i=0;i<this->getNModels();i++)
    {
        this->getModel(i).updateHistoryStackSize(nHistoryRecords);
    }
    emit this->nHistoryRecordsChanged(nHistoryRecords);
}

void Session::onApplicationStarted(void)
{
    QString sessionFileName = MainSettings::getInstancePtr()->getSessionFileName();
    if (!sessionFileName.isEmpty())
    {
        try
        {
            this->read(sessionFileName);
        }
        catch (const RError &rError)
        {
            RLogger::warning("Failed to read the session file \'%s\'. ERROR: %s\n",sessionFileName.toUtf8().constData(),rError.getMessage().toUtf8().constData());
        }
    }
}

void Session::onSoftwareUpdateFileSaved(uint downloadID, const QString &fileName)
{
    RLogger::info("Opening downloaded file \'%s\'\n",fileName.toUtf8().constData());

    if (!QDesktopServices::openUrl(QUrl("file:///" + fileName)))
    {
        RLogger::error("Failed to open the file \'%s\'\n",fileName.toUtf8().constData());
    }
}
