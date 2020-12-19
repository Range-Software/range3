/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   action.cpp                                               *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   8-th August 2013                                         *
 *                                                                   *
 *  DESCRIPTION: Action class definition                             *
 *********************************************************************/

#include <QString>
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>

#include <rblib.h>
#include <rmlib.h>

#include "action.h"
#include "application_settings_dialog.h"
#include "bool_difference_dialog.h"
#include "bool_intersection_dialog.h"
#include "bool_union_dialog.h"
#include "break_intersected_elements_dialog.h"
#include "convergence_graph_dialog.h"
#include "drop_results_dialog.h"
#include "new_model_dialog.h"
#include "coarsen_surface_dialog.h"
#include "cut_dialog.h"
#include "help_center_dialog.h"
#include "iso_dialog.h"
#include "job_manager.h"
#include "log_browser_dialog.h"
#include "find_sliver_elements_dialog.h"
#include "fix_sliver_elements_dialog.h"
#include "main_window.h"
#include "mark_entity_dialog.h"
#include "merge_entity_dialog.h"
#include "merge_near_nodes_dialog.h"
#include "remove_entity_dialog.h"
#include "material_list.h"
#include "matrix_solver_config_dialog.h"
#include "main_window.h"
#include "mesh_generator_dialog.h"
#include "model_action.h"
#include "model_io.h"
#include "model_statistics_dialog.h"
#include "monitoring_points_dialog.h"
#include "monitoring_point_graph_dialog.h"
#include "move_node_dialog.h"
#include "point_inside_surface_dialog.h"
#include "problem_selector_dialog.h"
#include "problem_task_dialog.h"
#include "rename_model_dialog.h"
#include "report_dialog.h"
#include "rra_session.h"
#include "session_entity_id.h"
#include "session.h"
#include "stream_line_dialog.h"
#include "solver_start_dialog.h"
#include "solver_manager.h"
#include "scalar_field_dialog.h"
#include "text_browser_dialog.h"
#include "update_dialog.h"
#include "vector_field_dialog.h"

Action::Action(ActionType type, MainWindow *mainWindow, QObject *parent) :
    QAction(parent),
    type(type),
    mainWindow(mainWindow)
{
    const ActionDefinition *actionDefinition = MainSettings::getInstance().getApplicationSettings()->getActionDefinition();

    switch (this->type)
    {
        case ACTION_NONE:
        {
            break;
        }
        case ACTION_SEPARATOR:
        {
            this->setSeparator(true);
            break;
        }
        default:
        {
            this->setText(tr(actionDefinition->getText(this->type).toUtf8().constData()));
            this->setToolTip(tr(actionDefinition->getDescription(this->type).toUtf8().constData()));
            this->setShortcut(tr(actionDefinition->getShortCut(this->type).toUtf8().constData()));
            if (!actionDefinition->getIcon(this->type).isEmpty())
            {
                this->setIcon(QIcon(actionDefinition->getIcon(this->type).toUtf8().constData()));
                this->setIconVisibleInMenu(true);
            }
            this->setMenuRole(actionDefinition->getMenuRole(this->type));
            QObject::connect(this, &Action::triggered, this, actionDefinition->getSlot(this->type));
            break;
        }
    }

    if (this->type == ACTION_SOLVER_START)
    {
        QObject::connect(&SolverManager::getInstance(), &SolverManager::jobStarted, this, &Action::disable);
        QObject::connect(&SolverManager::getInstance(), &SolverManager::jobFinished, this, &Action::enable);
    }
    if (this->type == ACTION_SOLVER_STOP || this->type == ACTION_SOLVER_KILL)
    {
        QObject::connect(&SolverManager::getInstance(), &SolverManager::jobStarted, this, &Action::enable);
        QObject::connect(&SolverManager::getInstance(), &SolverManager::jobFinished, this, &Action::disable);
    }
}

void Action::enable()
{
    this->setEnabled(true);
}

void Action::disable()
{
    this->setDisabled(true);
}

void Action::onSessionNew()
{
    R_LOG_TRACE_IN;
    int response = QMessageBox::question(this->mainWindow,
                                         tr("Close session"),
                                         tr("Are you sure you want to close current session?"),
                                         QMessageBox::Yes | QMessageBox::No,
                                         QMessageBox::No);
    if (response == QMessageBox::Yes)
    {
        Session::getInstance().clear();
        this->onSessionSaveAs();
    }
    R_LOG_TRACE_OUT;
}

void Action::onSessionOpen()
{
    R_LOG_TRACE_IN;
    QString fileExtension = Session::getDefaultFileExtension();
    QString dialogDesc = "Range model files (*." + fileExtension + ");;Any files (*)";
    QString fileName = QFileDialog::getOpenFileName(this->mainWindow,
                                                    tr("Open session"),
                                                    MainSettings::getInstance().getDataDir(),
                                                    dialogDesc);
    if (!fileName.isEmpty())
    {
        try
        {
            Session::getInstance().read(fileName);
        }
        catch (const RError &error)
        {
            RLogger::error("Failed to read the session file \'%s\'. ERROR: %s\n",fileName.toUtf8().constData(),error.getMessage().toUtf8().constData());
        }
    }
    R_LOG_TRACE_OUT;
}

void Action::onSessionSave()
{
    R_LOG_TRACE_IN;
    QDir dataDir(MainSettings::getInstance().getDataDir());
    QString dialogDesc = "Range session files (*.ras);;Any files (*)";

    QString fileName = Session::getInstance().getFileName();
    if (fileName.isEmpty())
    {
        fileName = dataDir.filePath("default.ras");
        fileName = QFileDialog::getSaveFileName(this->mainWindow,
                                                tr("Save session"),
                                                fileName,
                                                dialogDesc);
    }
    if (!fileName.isEmpty())
    {
        Session::getInstance().write(fileName,true);
    }
    R_LOG_TRACE_OUT;
}

void Action::onSessionSaveAs()
{
    R_LOG_TRACE_IN;
    QDir sessionDir(MainSettings::getInstance().getSessionDir());
    QString dialogDesc = "Range session files (*.ras);;Any files (*)";

    QString fileName;
    if (fileName.isEmpty())
    {
        fileName = sessionDir.filePath("default.ras");
    }
    fileName = QFileDialog::getSaveFileName(this->mainWindow,
                                            tr("Save session"),
                                            fileName,
                                            dialogDesc);
    if (!fileName.isEmpty())
    {
        Session::getInstance().write(fileName,true);
    }
    R_LOG_TRACE_OUT;
}

void Action::onSessionClose()
{
    R_LOG_TRACE_IN;
    int response = QMessageBox::question(this->mainWindow,
                                         tr("Close session"),
                                         tr("Are you sure you want to close current session?"),
                                         QMessageBox::Yes | QMessageBox::No,
                                         QMessageBox::No);
    if (response == QMessageBox::Yes)
    {
        Session::getInstance().clear();
    }
    R_LOG_TRACE_OUT;
}

void Action::onModelNew()
{
    R_LOG_TRACE_IN;
    NewModelDialog(this->mainWindow).exec();
    R_LOG_TRACE_OUT;
}

void Action::onModelOpen()
{
    R_LOG_TRACE_IN;
    QString binaryExtension = RModel::getDefaultFileExtension(true);
    QString asciiExtension = RModel::getDefaultFileExtension(false);
    QString dialogDesc = "All supported files (*." + binaryExtension + " *." + asciiExtension + " *.tmsh *.bmsh *.raw *.stl);;"
                       + "Range model files (*." + binaryExtension + " *." + asciiExtension + ");;" +
                       + "Old Range model mesh files (*.tmsh *.bmsh);;"
                       + "RAW triangle files (*.raw);;"
                       + "STL triangle files (*.stl);;"
                       + "Any files (*)";
    QString fileName = QFileDialog::getOpenFileName(this->mainWindow,
                                                    tr("Open model"),
                                                    MainSettings::getInstance().getDataDir(),
                                                    dialogDesc);
    if (!fileName.isEmpty())
    {
        ModelIOType modelIOType;

        QString extension = RFileManager::getExtension(fileName).toLower();
        if (extension == "tmsh" || extension == "bmsh")
        {
            modelIOType = MODEL_IO_MSH_IMPORT;
        }
        else if (extension == "raw")
        {
            modelIOType = MODEL_IO_RAW_IMPORT;
        }
        else if (extension == "stl")
        {
            modelIOType = MODEL_IO_STL_IMPORT;
        }
        else
        {
            modelIOType = MODEL_IO_OPEN;
        }

        JobManager::getInstance().submit(new ModelIO(modelIOType, fileName));
    }
    R_LOG_TRACE_OUT;
}

void Action::onModelSave()
{
    R_LOG_TRACE_IN;
    foreach (uint selectedModelID,Session::getInstance().getSelectedModelIDs())
    {
        Model &rModel = Session::getInstance().getModel(selectedModelID);

        QString fileName = ModelIO::getModelSaveName(MainSettings::getInstance(),this->mainWindow,rModel);

        if (!fileName.isEmpty())
        {
            JobManager::getInstance().submit(new ModelIO(MODEL_IO_SAVE, fileName, &rModel));
        }
    }
    R_LOG_TRACE_OUT;
}

void Action::onModelSaveAs()
{
    R_LOG_TRACE_IN;
    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<selectedModelIDs.size();i++)
    {
        Model &model = Session::getInstance().getModel(selectedModelIDs[i]);

        QString fileName = ModelIO::getModelSaveName(MainSettings::getInstance(),this->mainWindow,model,true);

        if (!fileName.isEmpty())
        {
            JobManager::getInstance().submit(new ModelIO(MODEL_IO_SAVE, fileName, &model));
        }
    }
    R_LOG_TRACE_OUT;
}

void Action::onModelExportMsh()
{
    R_LOG_TRACE_IN;
    QDir dataDir(MainSettings::getInstance().getDataDir());
    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();
    for (int i=0;i<selectedModelIDs.size();i++)
    {
        Model *pModel = Session::getInstance().getModelPtr(selectedModelIDs[i]);
        QString fileName = dataDir.filePath(pModel->getName() + ".tmsh");
        fileName = QFileDialog::getSaveFileName(this->mainWindow,
                                                tr("Export model to MSH file"),
                                                fileName,
                                                "Range mesh files (*.tmsh *.bmsh);;Any files (*)");
        if (!fileName.isEmpty())
        {
            JobManager::getInstance().submit(new ModelIO(MODEL_IO_MSH_EXPORT, fileName, pModel));
        }
    }
    R_LOG_TRACE_OUT;
}

void Action::onModelExportRaw()
{
    R_LOG_TRACE_IN;
    QDir dataDir(MainSettings::getInstance().getDataDir());
    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();
    for (int i=0;i<selectedModelIDs.size();i++)
    {
        Model *pModel = Session::getInstance().getModelPtr(selectedModelIDs[i]);
        QString fileName = dataDir.filePath(pModel->getName() + ".raw");
        fileName = QFileDialog::getSaveFileName(this->mainWindow,
                                                tr("Export model to RAW file"),
                                                fileName,
                                                "RAW triangle files (*.raw);;Any files (*)");
        if (!fileName.isEmpty())
        {
            JobManager::getInstance().submit(new ModelIO(MODEL_IO_RAW_EXPORT, fileName, pModel));
        }
    }
    R_LOG_TRACE_OUT;
}

void Action::onModelExportStlAscii()
{
    R_LOG_TRACE_IN;
    QDir dataDir(MainSettings::getInstance().getDataDir());
    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();
    for (int i=0;i<selectedModelIDs.size();i++)
    {
        Model *pModel = Session::getInstance().getModelPtr(selectedModelIDs[i]);
        QString fileName = dataDir.filePath(pModel->getName() + ".stl");
        fileName = QFileDialog::getSaveFileName(this->mainWindow,
                                                tr("Export model to STL (ascii) file"),
                                                fileName,
                                                "STL triangle files (*.stl);;Any files (*)");
        if (!fileName.isEmpty())
        {
            JobManager::getInstance().submit(new ModelIO(MODEL_IO_STL_ASCII_EXPORT, fileName, pModel));
        }
    }
    R_LOG_TRACE_OUT;
}

void Action::onModelExportStlBinary()
{
    R_LOG_TRACE_IN;
    QDir dataDir(MainSettings::getInstance().getDataDir());
    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();
    for (int i=0;i<selectedModelIDs.size();i++)
    {
        Model *pModel = Session::getInstance().getModelPtr(selectedModelIDs[i]);
        QString fileName = dataDir.filePath(pModel->getName() + ".stl");
        fileName = QFileDialog::getSaveFileName(this->mainWindow,
                                                tr("Export model to STL (binary) file"),
                                                fileName,
                                                "STL triangle files (*.stl);;Any files (*)");
        if (!fileName.isEmpty())
        {
            JobManager::getInstance().submit(new ModelIO(MODEL_IO_STL_BINARY_EXPORT, fileName, pModel));
        }
    }
    R_LOG_TRACE_OUT;
}

void Action::onModelClose()
{
    R_LOG_TRACE_IN;
    int response = QMessageBox::question(this->mainWindow,
                                         tr("Close model?"),
                                         tr("Are you sure you want to close selected models?"));
    if (response == QMessageBox::Yes)
    {
        QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();
        for (int i=0;i<modelIDs.size();i++)
        {
            RLogger::info("Closing model \'%s\'\n",Session::getInstance().getModel(uint(modelIDs.size()-i-1)).getName().toUtf8().constData());
            Session::getInstance().removeModel(modelIDs[modelIDs.size()-i-1]);
        }
    }
    R_LOG_TRACE_OUT;
}

void Action::onModelReloadResults()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();
    for (int i=0;i<modelIDs.size();i++)
    {
        Job *updateJob = new ModelIO(MODEL_IO_UPDATE, Session::getInstance().getModel(modelIDs[i]).getFileName(), &Session::getInstance().getModel(modelIDs[i]));
        JobManager::getInstance().submit(updateJob);
    }
    R_LOG_TRACE_OUT;
}

void Action::onModelRename()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();
    for (int i=0;i<modelIDs.size();i++)
    {
        RenameModelDialog(modelIDs.at(i)).exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onModelDropResults()
{
    R_LOG_TRACE_IN;
    DropResultsDialog dropResultsDialog(this->mainWindow);
    dropResultsDialog.exec();
    R_LOG_TRACE_OUT;
}

void Action::onMaterialNew()
{
    R_LOG_TRACE_IN;
    QString materialNameBase = tr("New material");
    QString materialName = materialNameBase;
    uint nTries = 0;
    while (MaterialList::getInstance().containName(materialName))
    {
        materialName = materialNameBase + " - " + QString::number(++nTries);
    }

    RMaterial material = RMaterial::generateDefault();
    material.setName(materialName);
    MaterialList::getInstance().addMaterial(material,true);
    R_LOG_TRACE_OUT;
}

void Action::onMaterialImport()
{
    R_LOG_TRACE_IN;
    QString fileName = QFileDialog::getOpenFileName(this->mainWindow,
                                                    tr("Select material file"),
                                                    MainSettings::getInstance().getHomeDir(),
                                                    "Material files (*.tmat);;Any files (*)");
    if (!fileName.isEmpty())
    {
        RLogger::info("Importing legacy material file \'%s\'\n",fileName.toUtf8().constData());

        RMaterial material;
        try
        {
            material.import(fileName);
        }
        catch (const RError &error)
        {
            RLogger::error("Failed to import legacy material file \'%s\'. %s",
                           fileName.toUtf8().constData(),
                           error.getMessage().toUtf8().constData());
            return;
        }
        bool isNew = true;
        if (MaterialList::getInstance().containName(material.getName()))
        {
            int response = QMessageBox::question(this->mainWindow,
                                                 tr("Overwrite?"),
                                                 tr("Material with such name already exists. Do you want to overwrite it?"));
            if (response != QMessageBox::Yes)
            {
                return;
            }
            isNew = false;
        }
        MaterialList::getInstance().addMaterial(material,isNew);
    }
    R_LOG_TRACE_OUT;
}

void Action::onMaterialDelete()
{
    R_LOG_TRACE_IN;
    if (MaterialList::getInstance().getSelectedMaterialName().length() > 0)
    {
        QString question = QString("Are you sure you want to delete material <b>")
                         + MaterialList::getInstance().getSelectedMaterialName()
                         + QString("</b>?");

        int response = QMessageBox::question(this->mainWindow,
                                             tr("Delete selected material?"),
                                             question,
                                             QMessageBox::Yes | QMessageBox::No,
                                             QMessageBox::No);
        if (response == QMessageBox::Yes)
        {
            MaterialList::getInstance().remove(MaterialList::getInstance().getSelectedMaterialName());
        }
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryUndo()
{
    R_LOG_TRACE_IN;
    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();
    for (int i=0;i<selectedModelIDs.size();i++)
    {
        Model &rModel = Session::getInstance().getModel(selectedModelIDs[i]);
        if (rModel.getUndoStackSize() > 0)
        {
            Session::getInstance().getPickList().removeItems(selectedModelIDs[i]);
            rModel.undo(0);
        }
        Session::getInstance().setModelChanged(selectedModelIDs[i]);
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryRedo()
{
    R_LOG_TRACE_IN;
    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();
    for (int i=0;i<selectedModelIDs.size();i++)
    {
        Model &rModel = Session::getInstance().getModel(selectedModelIDs[i]);
        if (rModel.getRedoStackSize() > 0)
        {
            Session::getInstance().getPickList().removeItems(selectedModelIDs[i]);
            rModel.redo(0);
        }
        Session::getInstance().setModelChanged(selectedModelIDs[i]);
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryDrawPoint()
{
    R_LOG_TRACE_IN;
    Session::getInstance().getDrawEngine()->addObject(new DrawEnginePoint);
    R_LOG_TRACE_OUT;
}

void Action::onGeometryDrawLine()
{
    R_LOG_TRACE_IN;
    Session::getInstance().getDrawEngine()->addObject(new DrawEngineLine);
    R_LOG_TRACE_OUT;
}

void Action::onGeometryDrawTriangle()
{
    R_LOG_TRACE_IN;
    Session::getInstance().getDrawEngine()->addObject(new DrawEngineTriangle);
    R_LOG_TRACE_OUT;
}

void Action::onGeometryDrawQuadrilateral()
{
    R_LOG_TRACE_IN;
    Session::getInstance().getDrawEngine()->addObject(new DrawEngineQuadrilateral);
    R_LOG_TRACE_OUT;
}

void Action::onGeometryDrawCircle()
{
    R_LOG_TRACE_IN;
    Session::getInstance().getDrawEngine()->addObject(new DrawEngineCircle);
    R_LOG_TRACE_OUT;
}

void Action::onGeometryDrawEllipse()
{
    R_LOG_TRACE_IN;
    Session::getInstance().getDrawEngine()->addObject(new DrawEngineEllipse);
    R_LOG_TRACE_OUT;
}

void Action::onGeometryDrawTetrahedron()
{
    R_LOG_TRACE_IN;
    Session::getInstance().getDrawEngine()->addObject(new DrawEngineTetrahedron);
    R_LOG_TRACE_OUT;
}

void Action::onGeometryDrawHexahedron()
{
    R_LOG_TRACE_IN;
    Session::getInstance().getDrawEngine()->addObject(new DrawEngineHexahedron);
    R_LOG_TRACE_OUT;
}

void Action::onGeometryDrawCylinder()
{
    R_LOG_TRACE_IN;
    Session::getInstance().getDrawEngine()->addObject(new DrawEngineCylinder);
    R_LOG_TRACE_OUT;
}

void Action::onGeometryDrawSphere()
{
    R_LOG_TRACE_IN;
    Session::getInstance().getDrawEngine()->addObject(new DrawEngineSphere);
    R_LOG_TRACE_OUT;
}

void Action::onGeometryDrawRaw()
{
    R_LOG_TRACE_IN;
    Session::getInstance().getDrawEngine()->addObject(new DrawEngineRaw);
    R_LOG_TRACE_OUT;
}

void Action::onGeometryCreateElement()
{
    R_LOG_TRACE_IN;
    PickList &pickList = Session::getInstance().getPickList();

    if (pickList.getNItems(PICK_ITEM_NODE) == 0)
    {
        QMessageBox::information(this->mainWindow,
                                 tr("Create element"),
                                 tr("No node selected."),
                                 QMessageBox::Close);
        R_LOG_TRACE_OUT;
        return;
    }

    QList<uint> modelIDs = pickList.getModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        uint nNodes = pickList.getNItems(PICK_ITEM_NODE,modelIDs[i]);

        RElementType elementType = R_ELEMENT_NONE;

        switch (nNodes)
        {
            case 0:
            {
                break;
            }
            case 1:
            {
                // Create point element.
                elementType = R_ELEMENT_POINT;
                break;
            }
            case 2:
            {
                // Create line element.
                elementType = R_ELEMENT_TRUSS1;
                break;
            }
            case 3:
            {
                // Create triangle element.
                elementType = R_ELEMENT_TRI1;
                break;
            }
            case 4:
            {
                // Create rectangle or tetrahedra element.
                QMessageBox msgBox(this->mainWindow);
                QPushButton *quadButton = msgBox.addButton(tr("Quadrilateral"), QMessageBox::YesRole);
                QPushButton *tetrButton = msgBox.addButton(tr("Tetrahedra"), QMessageBox::NoRole);

                msgBox.setWindowTitle(tr("Choose element type"));
                msgBox.setIcon(QMessageBox::Question);
                msgBox.setText(tr("Which element type to create?"));
                msgBox.exec();

                if (msgBox.clickedButton() == quadButton)
                {
                    // Create quadrilateral element.
                    elementType = R_ELEMENT_QUAD1;
                }
                else if (msgBox.clickedButton() == tetrButton)
                {
                    // Create tetrahedra element.
                    elementType = R_ELEMENT_TETRA1;
                }
                break;
            }
            default:
            {
                QMessageBox::information(this->mainWindow,
                                         tr("Create element"),
                                         tr("Too many nodes selected."),
                                         QMessageBox::Close);
                R_LOG_TRACE_OUT;
                return;
            }
        }

        if (elementType == R_ELEMENT_NONE)
        {
            continue;
        }

        QList<uint> nodeIDs;

        QVector<PickItem> pickItems = pickList.getItems(modelIDs[i]);
        for (int j=0;j<pickItems.size();j++)
        {
            if (pickItems[j].getItemType() == PICK_ITEM_NODE)
            {
                uint elementID = pickItems[j].getElementID();
                uint nodeID = pickItems[j].getNodeID();
                if (elementID != RConstants::eod && nodeID != RConstants::eod)
                {
                    nodeIDs.append(nodeID);
                    pickList.removeItem(pickItems[j]);
                }
            }
        }

        ModelActionInput modelActionInput(modelIDs[i]);
        modelActionInput.setCreateElement(nodeIDs,elementType);

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryFindSliverElements()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        FindSliverElementsDialog findliverElementsDialog(modelIDs[i],this->mainWindow);
        findliverElementsDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryFixSliverElements()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        FixSliverElementsDialog fixSliverElementsDialog(modelIDs[i],this->mainWindow);
        fixSliverElementsDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryFindIntersectedElements()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        ModelActionInput modelActionInput(modelIDs[i]);
        modelActionInput.setFindIntersectedElements();

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryBreakIntersectedElements()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
//        // Questionable whether to remove volume elements
//        // It should not be possible to generate volume mesh if intersected elements are present
//        const Model &rModel = Session::getInstance().getModel(modelIDs[i]);
//        if (rModel.getNVolumes() > 0)
//        {
//            if (QMessageBox::question(this->mainWindow,
//                                      tr("Volume elements found"),
//                                      tr("To break intersected elements all volume elements must be removed.<br/>Are you sure you want to continue and remove all volume elements?"),
//                                      QMessageBox::No,
//                                      QMessageBox::Yes) == QMessageBox::No)
//            {
//                R_LOG_TRACE_OUT;
//                return;
//            }
//        }

        BreakIntersectedElementsDialog bieDialog(modelIDs[i],this->mainWindow);
        bieDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryBoolUnion()
{
    R_LOG_TRACE_IN;
    QList<SessionEntityID> selectedEntityIDs = Session::getInstance().getSelectedEntityIDs();
    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<selectedModelIDs.size();i++)
    {
        QList<SessionEntityID> entityIDs;

        for (int j=0;j<selectedEntityIDs.size();j++)
        {
            if (selectedEntityIDs.at(j).getMid() == selectedModelIDs.at(i) && selectedEntityIDs.at(j).getType() == R_ENTITY_GROUP_SURFACE)
            {
                entityIDs.push_back(selectedEntityIDs.at(j));
            }
        }

        if (entityIDs.size() < 2)
        {
            QMessageBox::warning(this->mainWindow,
                              tr("Too few surfaces selected"),
                              tr("To perform union boolean operation at least two surfaces must be selected."));
            R_LOG_TRACE_OUT;
            return;
        }

        for (int j=0;j<entityIDs.size();j++)
        {
            const Model &rModel = Session::getInstance().getModel(selectedModelIDs[i]);
            if (!rModel.checkIfSurfaceIsClosed(entityIDs[j].getEid()))
            {
                QMessageBox::warning(this->mainWindow,
                                     tr("Surface is not closed"),
                                     tr("Surface") + QString(" <b>") + rModel.getSurface(entityIDs[j].getEid()).getName()  + QString("</b> ") + tr("is not closed.<br/>")
                                     + tr("To perform union boolean operation all selected surfaces must be closed."));
                R_LOG_TRACE_OUT;
                return;
            }
        }

        BoolUnionDialog boolUnionDialog(selectedModelIDs[i],entityIDs,this->mainWindow);
        boolUnionDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryBoolDifference()
{
    R_LOG_TRACE_IN;
    QList<SessionEntityID> selectedEntityIDs = Session::getInstance().getSelectedEntityIDs();
    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<selectedModelIDs.size();i++)
    {
        QList<SessionEntityID> entityIDs;

        for (int j=0;j<selectedEntityIDs.size();j++)
        {
            if (selectedEntityIDs.at(j).getMid() == selectedModelIDs.at(i) && selectedEntityIDs.at(j).getType() == R_ENTITY_GROUP_SURFACE)
            {
                entityIDs.push_back(selectedEntityIDs.at(j));
            }
        }

        if (entityIDs.size() < 2)
        {
            QMessageBox::warning(this->mainWindow,
                              tr("Too few surfaces selected"),
                              tr("To perform difference boolean operation at least two surfaces must be selected."));
            R_LOG_TRACE_OUT;
            return;
        }

        for (int j=0;j<entityIDs.size();j++)
        {
            const Model &rModel = Session::getInstance().getModel(selectedModelIDs[i]);
            if (!rModel.checkIfSurfaceIsClosed(entityIDs[j].getEid()))
            {
                QMessageBox::warning(this->mainWindow,
                                     tr("Surface is not closed"),
                                     tr("Surface") + QString(" <b>") + rModel.getSurface(entityIDs[j].getEid()).getName()  + QString("</b> ") + tr("is not closed.<br/>")
                                     + tr("To perform difference boolean operation all selected surfaces must be closed."));
                R_LOG_TRACE_OUT;
                return;
            }
        }

        BoolDifferenceDialog boolDifferenceDialog(selectedModelIDs[i],entityIDs,this->mainWindow);
        boolDifferenceDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryBoolIntersection()
{
    R_LOG_TRACE_IN;
    QList<SessionEntityID> selectedEntityIDs = Session::getInstance().getSelectedEntityIDs();
    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<selectedModelIDs.size();i++)
    {
        QList<SessionEntityID> entityIDs;

        for (int j=0;j<selectedEntityIDs.size();j++)
        {
            if (selectedEntityIDs.at(j).getMid() == selectedModelIDs.at(i) && selectedEntityIDs.at(j).getType() == R_ENTITY_GROUP_SURFACE)
            {
                entityIDs.push_back(selectedEntityIDs.at(j));
            }
        }

        if (entityIDs.size() < 2)
        {
            QMessageBox::warning(this->mainWindow,
                              tr("Too few surfaces selected"),
                              tr("To perform intersection boolean operation at least two surfaces must be selected."));
            R_LOG_TRACE_OUT;
            return;
        }

        for (int j=0;j<entityIDs.size();j++)
        {
            const Model &rModel = Session::getInstance().getModel(selectedModelIDs[i]);
            if (!rModel.checkIfSurfaceIsClosed(entityIDs[j].getEid()))
            {
                QMessageBox::warning(this->mainWindow,
                                     tr("Surface is not closed"),
                                     tr("Surface") + QString(" <b>") + rModel.getSurface(entityIDs[j].getEid()).getName()  + QString("</b> ") + tr("is not closed.<br/>")
                                     + tr("To perform intersection boolean operation all selected surfaces must be closed."));
                R_LOG_TRACE_OUT;
                return;
            }
        }

        BoolIntersectionDialog boolIntersectionDialog(selectedModelIDs[i],entityIDs,this->mainWindow);
        boolIntersectionDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryMergeNearNodes()
{
    R_LOG_TRACE_IN;
    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<selectedModelIDs.size();i++)
    {
        MergeNearNodesDialog mergeNearNodesDialog(selectedModelIDs[i],this->mainWindow);
        mergeNearNodesDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryMoveNode()
{
    R_LOG_TRACE_IN;
    PickList &pickList = Session::getInstance().getPickList();

    if (pickList.getNItems(PICK_ITEM_NODE) == 0)
    {
        QMessageBox::information(this->mainWindow,
                                 tr("Move node"),
                                 tr("No node selected."),
                                 QMessageBox::Close);
        R_LOG_TRACE_OUT;
        return;
    }

    QList<uint> modelIDs = pickList.getModelIDs();
    for (int i=0;i<modelIDs.size();i++)
    {
        QList<uint> nodeIDs;

        QVector<PickItem> pickItems = pickList.getItems(modelIDs[i]);
        for (int j=0;j<pickItems.size();j++)
        {
            if (pickItems[j].getItemType() == PICK_ITEM_NODE)
            {
                uint elementID = pickItems[j].getElementID();
                uint lNodeID = pickItems[j].getNodeID();
                if (elementID != RConstants::eod && lNodeID != RConstants::eod)
                {
                    nodeIDs.append(lNodeID);
                }
            }
        }

        if (nodeIDs.size() > 0)
        {
            std::sort(nodeIDs.begin(),nodeIDs.end());

            MoveNodeDialog moveNodeDialog(modelIDs[i],nodeIDs,this->mainWindow);
            moveNodeDialog.exec();
        }
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryRemoveNode()
{
    R_LOG_TRACE_IN;
    PickList &pickList = Session::getInstance().getPickList();

    if (pickList.getNItems(PICK_ITEM_NODE) == 0)
    {
        QMessageBox::information(this->mainWindow,
                                 tr("Remove nodes"),
                                 tr("No node selected."),
                                 QMessageBox::Close);
        R_LOG_TRACE_OUT;
        return;
    }
    if (QMessageBox::question(this->mainWindow,
                               tr("Remove nodes"),
                               tr("Are you sure you want to remove picked nodes?"),
                               QMessageBox::No,
                               QMessageBox::Yes) == QMessageBox::No)
    {
        R_LOG_TRACE_OUT;
        return;
    }

    QList<uint> modelIDs = pickList.getModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        QList<uint> nodeIDs;

        QVector<PickItem> pickItems = pickList.getItems(modelIDs[i]);
        for (int j=0;j<pickItems.size();j++)
        {
            if (pickItems[j].getItemType() == PICK_ITEM_NODE)
            {
                uint elementID = pickItems[j].getElementID();
                uint nodeID = pickItems[j].getNodeID();
                if (elementID != RConstants::eod && nodeID != RConstants::eod)
                {
                    nodeIDs.append(nodeID);
                    pickList.removeItem(pickItems[j]);
                }
            }
        }

        ModelActionInput modelActionInput(modelIDs[i]);
        modelActionInput.setRemoveNodes(nodeIDs,false);

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryRemoveNodeAndCloseHole()
{
    R_LOG_TRACE_IN;
    PickList &pickList = Session::getInstance().getPickList();

    if (pickList.getNItems(PICK_ITEM_NODE) == 0)
    {
        QMessageBox::information(this->mainWindow,
                                 tr("Remove nodes"),
                                 tr("No node selected."),
                                 QMessageBox::Close);
        R_LOG_TRACE_OUT;
        return;
    }
    if (QMessageBox::question(this->mainWindow,
                               tr("Remove nodes"),
                               tr("Are you sure you want to remove picked nodes?"),
                               QMessageBox::No,
                               QMessageBox::Yes) == QMessageBox::No)
    {
        R_LOG_TRACE_OUT;
        return;
    }

    QList<uint> modelIDs = pickList.getModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        QList<uint> nodeIDs;

        QVector<PickItem> pickItems = pickList.getItems(modelIDs[i]);
        for (int j=0;j<pickItems.size();j++)
        {
            if (pickItems[j].getItemType() == PICK_ITEM_NODE)
            {
                uint elementID = pickItems[j].getElementID();
                uint nodeID = pickItems[j].getNodeID();
                if (elementID != RConstants::eod && nodeID != RConstants::eod)
                {
                    nodeIDs.append(nodeID);
                    pickList.removeItem(pickItems[j]);
                }
            }
        }

        ModelActionInput modelActionInput(modelIDs[i]);
        modelActionInput.setRemoveNodes(nodeIDs,true);

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryRemoveElement()
{
    R_LOG_TRACE_IN;
    PickList &pickList = Session::getInstance().getPickList();

    if (pickList.getNItems(PICK_ITEM_ELEMENT) == 0)
    {
        QMessageBox::information(this->mainWindow,
                                 tr("Remove elements"),
                                 tr("No element selected."),
                                 QMessageBox::Close);
        R_LOG_TRACE_OUT;
        return;
    }
    if (QMessageBox::question(this->mainWindow,
                               tr("Remove elements"),
                               tr("Are you sure you want to remove picked elements?"),
                               QMessageBox::No,
                               QMessageBox::Yes) == QMessageBox::No)
    {
        R_LOG_TRACE_OUT;
        return;
    }

    QList<uint> modelIDs = pickList.getModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        QList<uint> elementIDs;

        QVector<PickItem> pickItems = pickList.getItems(modelIDs[i]);
        for (int j=0;j<pickItems.size();j++)
        {
            if (pickItems[j].getItemType() == PICK_ITEM_ELEMENT)
            {
                uint elementID = pickItems[j].getElementID();
                if (elementID != RConstants::eod)
                {
                    elementIDs.append(elementID);
                    pickList.removeItem(pickItems[j]);
                }
            }
        }

        ModelActionInput modelActionInput(modelIDs[i]);
        modelActionInput.setRemoveElements(elementIDs,false);

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryLineGenerateFromEdges()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        ModelActionInput modelActionInput(modelIDs[i]);
        modelActionInput.setGenerateLinesFromEdges(40.0);

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometrySurfaceMark()
{
    R_LOG_TRACE_IN;
    MarkEntityDialog markEntityDialog(R_ENTITY_GROUP_SURFACE,this->mainWindow);
    markEntityDialog.exec();
    R_LOG_TRACE_OUT;
}

void Action::onGeometrySurfaceSwapElementNormal()
{
    R_LOG_TRACE_IN;
    PickList &pickList = Session::getInstance().getPickList();

    if (pickList.getNItems(PICK_ITEM_ELEMENT) == 0)
    {
        QMessageBox::information(this->mainWindow,
                                 tr("Swap element normal"),
                                 tr("No element selected."),
                                 QMessageBox::Close);
        R_LOG_TRACE_OUT;
        return;
    }
    QList<uint> modelIDs = pickList.getModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        QList<uint> elementIDs;
        QVector<PickItem> pickItems = pickList.getItems(modelIDs[i]);
        for (int j=0;j<pickItems.size();j++)
        {
            if (pickItems[j].getItemType() == PICK_ITEM_ELEMENT)
            {
                uint elementID = pickItems[j].getElementID();
                if (elementID != RConstants::eod)
                {
                    elementIDs.append(elementID);
                    pickList.removeItem(pickItems[j]);
                }
            }
        }

        ModelActionInput modelActionInput(modelIDs[i]);
        modelActionInput.setSwapSurfaceElementNormal(elementIDs);

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometrySurfaceSwapNormals()
{
    R_LOG_TRACE_IN;
    QList<SessionEntityID> selectedEntityIDs = Session::getInstance().getSelectedEntityIDs();
    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<selectedModelIDs.size();i++)
    {
        QList<SessionEntityID> entityIDs;

        for (int j=0;j<selectedEntityIDs.size();j++)
        {
            if (selectedEntityIDs.at(j).getMid() == selectedModelIDs.at(i))
            {
                entityIDs.push_back(selectedEntityIDs.at(j));
            }
        }

        ModelActionInput modelActionInput(selectedModelIDs.at(i));
        modelActionInput.setSwapSurfaceNormals(entityIDs);

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometrySurfaceSyncNormals()
{
    R_LOG_TRACE_IN;
    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<selectedModelIDs.size();i++)
    {
        ModelActionInput modelActionInput(selectedModelIDs.at(i));
        modelActionInput.setSyncSurfaceNormals();

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometrySurfaceCloseHole()
{
    R_LOG_TRACE_IN;
    PickList &pickList = Session::getInstance().getPickList();

    if (pickList.getNItems(PICK_ITEM_HOLE_ELEMENT) == 0)
    {
        QMessageBox::information(this->mainWindow,
                                 tr("Close surface hole"),
                                 tr("No edge selected."),
                                 QMessageBox::Close);
        R_LOG_TRACE_OUT;
        return;
    }

    foreach (uint modelID,pickList.getModelIDs())
    {
        QList<uint> holeIDs;

        QVector<PickItem> pickItems = pickList.getItems(modelID);
        for (int j=0;j<pickItems.size();j++)
        {
            if (pickItems[j].getItemType() == PICK_ITEM_HOLE_ELEMENT)
            {
                uint elementID = pickItems[j].getElementID();
                if (elementID != RConstants::eod)
                {
                    holeIDs.append(elementID);
                    pickList.removeItem(pickItems[j]);
                }
            }
        }

        ModelActionInput modelActionInput(modelID);
        modelActionInput.setCloseSurfaceHole(holeIDs);

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometrySurfaceCoarsen()
{
    R_LOG_TRACE_IN;
    QList<SessionEntityID> selectedEntityIDs = Session::getInstance().getSelectedEntityIDs();
    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<selectedModelIDs.size();i++)
    {
        QList<SessionEntityID> entityIDs;

        for (int j=0;j<selectedEntityIDs.size();j++)
        {
            if (selectedEntityIDs.at(j).getMid() == selectedModelIDs.at(i))
            {
                entityIDs.push_back(selectedEntityIDs.at(j));
            }
        }

        CoarsenSurfaceDialog coarsenSurfaceDialog(selectedModelIDs[i],entityIDs,this->mainWindow);
        coarsenSurfaceDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryVectorFieldCreate()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        VectorFieldDialog vectorFieldDialog(modelIDs[i],this->mainWindow);
        vectorFieldDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryVectorFieldEdit()
{
    R_LOG_TRACE_IN;
    QList<SessionEntityID> entityIDs = Session::getInstance().getSelectedEntityIDs();

    for (int i=0;i<entityIDs.size();i++)
    {
        if (entityIDs[i].getType() == R_ENTITY_GROUP_VECTOR_FIELD)
        {
            VectorFieldDialog vectorFieldDialog(entityIDs[i].getMid(),entityIDs[i].getEid(),this->mainWindow);
            vectorFieldDialog.exec();
        }
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryScalarFieldCreate()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        ScalarFieldDialog scalarFieldDialog(modelIDs[i],this->mainWindow);
        scalarFieldDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryScalarFieldEdit()
{
    R_LOG_TRACE_IN;
    QList<SessionEntityID> entityIDs = Session::getInstance().getSelectedEntityIDs();

    for (int i=0;i<entityIDs.size();i++)
    {
        if (entityIDs[i].getType() != R_ENTITY_GROUP_SCALAR_FIELD)
        {
            continue;
        }
        ScalarFieldDialog scalarFieldDialog(entityIDs[i].getMid(),entityIDs[i].getEid(),this->mainWindow);
        scalarFieldDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryStreamLineCreate()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        StreamLineDialog streamLineDialog(modelIDs[i],this->mainWindow);
        streamLineDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryStreamLineEdit()
{
    R_LOG_TRACE_IN;
    QList<SessionEntityID> entityIDs = Session::getInstance().getSelectedEntityIDs();

    for (int i=0;i<entityIDs.size();i++)
    {
        if (entityIDs[i].getType() != R_ENTITY_GROUP_STREAM_LINE)
        {
            continue;
        }
        StreamLineDialog streamLineDialog(entityIDs[i].getMid(),entityIDs[i].getEid(),this->mainWindow);
        streamLineDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryCutCreate()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        CutDialog *cutDialog = new CutDialog(modelIDs[i],RConstants::eod,this->mainWindow);
        cutDialog->show();
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryCutEdit()
{
    R_LOG_TRACE_IN;
    QList<SessionEntityID> entityIDs = Session::getInstance().getSelectedEntityIDs();

    for (int i=0;i<entityIDs.size();i++)
    {
        if (entityIDs[i].getType() != R_ENTITY_GROUP_CUT)
        {
            continue;
        }
        CutDialog *cutDialog = new CutDialog(entityIDs[i].getMid(),entityIDs[i].getEid(),this->mainWindow);
        cutDialog->show();
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryIsoCreate()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        IsoDialog isoDialog(modelIDs[i],this->mainWindow);
        isoDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryIsoEdit()
{
    R_LOG_TRACE_IN;
    QList<SessionEntityID> entityIDs = Session::getInstance().getSelectedEntityIDs();

    for (int i=0;i<entityIDs.size();i++)
    {
        if (entityIDs[i].getType() != R_ENTITY_GROUP_ISO)
        {
            continue;
        }
        IsoDialog isoDialog(entityIDs[i].getMid(),entityIDs[i].getEid(),this->mainWindow);
        isoDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryEntityMerge()
{
    R_LOG_TRACE_IN;
    MergeEntityDialog mergeEntityDialog(R_ENTITY_GROUP_ELEMENT,this->mainWindow);
    mergeEntityDialog.exec();
    R_LOG_TRACE_OUT;
}

void Action::onGeometryEntityRemove()
{
    R_LOG_TRACE_IN;
    RemoveEntityDialog removeEntityDialog(R_ENTITY_GROUP_ALL,this->mainWindow);
    removeEntityDialog.exec();
    R_LOG_TRACE_OUT;
}

void Action::onGeometryGenerateTetrahedra()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        MeshGeneratorDialog meshGeneratorDialog(modelIDs[i],this->mainWindow);
        meshGeneratorDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryTransform()
{
    R_LOG_TRACE_IN;
    MainWindow::getInstance()->showTransformGeometryWidget();
    R_LOG_TRACE_OUT;
}

void Action::onGeometryDevExportSliverElements()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        ModelActionInput modelActionInput(modelIDs[i]);
        modelActionInput.setExportSliverElements();

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryDevExportIntersectedElements()
{
    R_LOG_TRACE_IN;
    foreach (uint modelID, Session::getInstance().getSelectedModelIDs())
    {
        ModelActionInput modelActionInput(modelID);
        modelActionInput.setExportIntersectedElements();

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryDevPurgeUnusedNodes()
{
    R_LOG_TRACE_IN;
    foreach (uint modelID, Session::getInstance().getSelectedModelIDs())
    {
        ModelActionInput modelActionInput(modelID);
        modelActionInput.setPurgeUnusedNodes();

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryDevPurgeUnusedElements()
{
    R_LOG_TRACE_IN;
    foreach (uint modelID, Session::getInstance().getSelectedModelIDs())
    {
        ModelActionInput modelActionInput(modelID);
        modelActionInput.setPurgeUnusedElements();

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryDevRemoveDuplicateNodes()
{
    R_LOG_TRACE_IN;
    foreach (uint modelID, Session::getInstance().getSelectedModelIDs())
    {
        ModelActionInput modelActionInput(modelID);
        modelActionInput.setRemoveDuplicateNodes();

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryDevRemoveDuplicateElements()
{
    R_LOG_TRACE_IN;
    foreach (uint modelID, Session::getInstance().getSelectedModelIDs())
    {
        ModelActionInput modelActionInput(modelID);
        modelActionInput.setRemoveDuplicateElements();

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryDevPointInsideSurface()
{
    R_LOG_TRACE_IN;
    QList<SessionEntityID> selectedEntityIDs = Session::getInstance().getSelectedEntityIDs();
    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<selectedModelIDs.size();i++)
    {
        QList<SessionEntityID> entityIDs;
        QList<uint> surfaceIDs;

        for (int j=0;j<selectedEntityIDs.size();j++)
        {
            if (selectedEntityIDs.at(j).getMid() == selectedModelIDs.at(i) && selectedEntityIDs.at(j).getType() == R_ENTITY_GROUP_SURFACE)
            {
                entityIDs.append(selectedEntityIDs.at(j));
                surfaceIDs.append(selectedEntityIDs.at(j).getEid());
            }
        }

        if (entityIDs.size() == 0)
        {
            QMessageBox::warning(this->mainWindow,
                              tr("Too few surfaces selected"),
                              tr("To perform point inside check at least one surface must be selected."));
            R_LOG_TRACE_OUT;
            return;
        }

        const Model &rModel = Session::getInstance().getModel(selectedModelIDs[i]);
        if (!rModel.checkIfSurfacesAreClosed(surfaceIDs))
        {
            QMessageBox::warning(this->mainWindow,
                              tr("Unclosed surfaces"),
                              tr("Selected surfaces do not form closed surface."));
            R_LOG_TRACE_OUT;
            return;
        }

        PointInsideSurfaceDialog pointInsideSurfaceDialog(selectedModelIDs[i],entityIDs,this->mainWindow);
        pointInsideSurfaceDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryDevTetrahedralizeeSurface()
{
    R_LOG_TRACE_IN;
    QList<SessionEntityID> selectedEntityIDs = Session::getInstance().getSelectedEntityIDs();
    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<selectedModelIDs.size();i++)
    {
        QList<SessionEntityID> entityIDs;
        QList<uint> surfaceIDs;

        for (int j=0;j<selectedEntityIDs.size();j++)
        {
            if (selectedEntityIDs.at(j).getMid() == selectedModelIDs.at(i) && selectedEntityIDs.at(j).getType() == R_ENTITY_GROUP_SURFACE)
            {
                entityIDs.append(selectedEntityIDs.at(j));
                surfaceIDs.append(selectedEntityIDs.at(j).getEid());
            }
        }

        if (entityIDs.size() == 0)
        {
            QMessageBox::warning(this->mainWindow,
                              tr("Too few surfaces selected"),
                              tr("To perform point inside check at least one surface must be selected."));
            R_LOG_TRACE_OUT;
            return;
        }

        const Model &rModel = Session::getInstance().getModel(selectedModelIDs[i]);
        if (!rModel.checkIfSurfacesAreClosed(surfaceIDs))
        {
            QMessageBox::warning(this->mainWindow,
                              tr("Unclosed surfaces"),
                              tr("Selected surfaces do not form closed surface."));
            R_LOG_TRACE_OUT;
            return;
        }

        ModelActionInput modelActionInput(selectedModelIDs[i]);
        modelActionInput.setTetrahedralizeSurface(entityIDs);

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }
    R_LOG_TRACE_OUT;
}

void Action::onGeometryDevConsolidate()
{
    R_LOG_TRACE_IN;
    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<selectedModelIDs.size();i++)
    {
        ModelActionInput modelActionInput(selectedModelIDs[i]);
        modelActionInput.setConsolidate();

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }
    R_LOG_TRACE_OUT;
}

void Action::onProblemTaskFlow()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        ProblemTaskDialog problemTaskDialog(modelIDs[i],this->mainWindow);
        problemTaskDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onProblemSolverSetup()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        MatrixSolverConfigDialog matrixSolverConfigDialog(modelIDs[i],this->mainWindow);
        matrixSolverConfigDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onProblemDefineMonitoringPoints()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        MonitoringPointsDialog monitoringPointsDialog(modelIDs[i],this->mainWindow);
        monitoringPointsDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onProblemReset()
{
    R_LOG_TRACE_IN;
    if (QMessageBox::question(this->mainWindow,
                               tr("Reset problem setup"),
                               tr("Are you sure you want to reset all problem setup including boundary, initial and environment conditions?"),
                               QMessageBox::No,
                               QMessageBox::Yes) == QMessageBox::No)
    {
        R_LOG_TRACE_OUT;
        return;
    }

    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();
    for (int i=0;i<modelIDs.size();i++)
    {
        Session::getInstance().storeCurentModelVersion(modelIDs[i],tr("Problem reset"));
        Session::getInstance().getModel(modelIDs[i]).RProblem::operator =(RProblem());
        Session::getInstance().getModel(modelIDs[i]).clearBoundaryConditions();
        Session::getInstance().getModel(modelIDs[i]).clearInitialConditions();
        Session::getInstance().getModel(modelIDs[i]).clearEnvironmentConditions();
        Session::getInstance().getModel(modelIDs[i]).clearEntityVariableData();
        Session::getInstance().setModelChanged(modelIDs[i]);
    }
    R_LOG_TRACE_OUT;
}

void Action::onSolverStart()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    // First perform checks whether the setup is correct.
    for (int i=0;i<modelIDs.size();i++)
    {
        Model &rModel = Session::getInstance().getModel(modelIDs[i]);
        QString modelFileName = ModelIO::getModelSaveName(MainSettings::getInstance(),this->mainWindow,rModel);
        if (!modelFileName.isEmpty())
        {
            rModel.setFileName(modelFileName);
            SolverStartDialog solverStartDialog(modelIDs[i],this->mainWindow);
            solverStartDialog.exec();
        }
    }
    R_LOG_TRACE_OUT;
}

void Action::onSolverStop()
{
    R_LOG_TRACE_IN;
    if (QMessageBox::question(this->mainWindow,
                               tr("Stop solver"),
                               tr("Are you sure you want to stop all running solvers?"),
                               QMessageBox::No,
                               QMessageBox::Yes) == QMessageBox::No)
    {
        R_LOG_TRACE_OUT;
        return;
    }

    try
    {
        SolverManager::getInstance().stopRunningTasks();
    }
    catch (RError &error)
    {
        RLogger::error("Could not stop the solver (%s).\n",error.getMessage().toUtf8().constData());
    }
    R_LOG_TRACE_OUT;
}

void Action::onSolverKill()
{
    R_LOG_TRACE_IN;
    if (QMessageBox::question(this->mainWindow,
                               tr("Kill solver"),
                               tr("Are you sure you want to kill all running solvers?"),
                               QMessageBox::No,
                               QMessageBox::Yes) == QMessageBox::No)
    {
        R_LOG_TRACE_OUT;
        return;
    }

    try
    {
        SolverManager::getInstance().killRunningTasks();
    }
    catch (RError &error)
    {
        RLogger::error("Could not kill the solver (%s).\n",error.getMessage().toUtf8().constData());
    }
    R_LOG_TRACE_OUT;
}

void Action::onReportModelStatistics()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        ModelStatisticsDialog modelStatisticsDialog(modelIDs[i],this->mainWindow);
        modelStatisticsDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onReportSolverLog()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        QString fileName(RFileManager::findLastFile(Session::getInstance().getModel(modelIDs[i]).buildTmpFileName("log",QString("*"))));
        if (fileName.isEmpty())
        {
            QMessageBox::information(this->mainWindow,tr("No solver log file"),tr("There is no file containing solver log."));
        }
        else
        {
            LogBrowserDialog logBrowserDialog(fileName,this->mainWindow);
            logBrowserDialog.exec();
        }
    }
    R_LOG_TRACE_OUT;
}

void Action::onReportConvergenceGraph()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        QStringList fileNames;

        QString filePattern(Session::getInstance().getModel(modelIDs[i]).buildTmpFileName("cvg",QString("*")));

        std::vector<RProblemType> problemTypes = RProblem::getTypes(R_PROBLEM_ALL);
        for (uint i=0;i<problemTypes.size();i++)
        {
            QString fileName(RFileManager::findLastFile(RFileManager::getFileNameWithSuffix(filePattern,RProblem::getId(problemTypes[i]))));
            if (!fileName.isEmpty())
            {
                fileNames.append(fileName);
            }
        }

        for (RMatrixSolverType type=RMatrixSolverConf::None;type<RMatrixSolverConf::NTypes;type++)
        {
            QString fileName(RFileManager::findLastFile(RFileManager::getFileNameWithSuffix(filePattern,RMatrixSolverConf::getId(type))));
            if (!fileName.isEmpty())
            {
                fileNames.append(fileName);
            }
        }

        if (fileNames.isEmpty())
        {
            QMessageBox::information(this->mainWindow,tr("No convergence file"),tr("There is no file containing convergence values."));
        }
        else
        {
            ConvergenceGraphDialog *convergenceGraphDialog = new ConvergenceGraphDialog(fileNames,this->mainWindow);
            convergenceGraphDialog->setModal(false);
            convergenceGraphDialog->show();
        }
    }
    R_LOG_TRACE_OUT;
}

void Action::onReportMonitoringPointGraph()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        QString fileName(RFileManager::findLastFile(Session::getInstance().getModel(modelIDs[i]).buildTmpFileName("mon",QString("*"))));
        if (fileName.isEmpty())
        {
            QMessageBox::information(this->mainWindow,tr("No monitoring file"),tr("There is no file containing monitoring points."));
        }
        else
        {
            MonitoringPointGraphDialog monitoringPointGraphDialog(fileName,this->mainWindow);
            monitoringPointGraphDialog.exec();
        }
    }
    R_LOG_TRACE_OUT;
}

void Action::onReportCreateReport()
{
    R_LOG_TRACE_IN;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        ReportDialog reportDialog(modelIDs[i],this->mainWindow);
        reportDialog.exec();
    }
    R_LOG_TRACE_OUT;
}

void Action::onHelp()
{
    R_LOG_TRACE_IN;
    HelpCenterDialog *pHelpCenterDialog = new HelpCenterDialog(this->mainWindow);
    pHelpCenterDialog->setModal(false);
    pHelpCenterDialog->show();
    R_LOG_TRACE_OUT;
}

void Action::onQuit()
{
    R_LOG_TRACE_IN;
    this->mainWindow->close();
    R_LOG_TRACE_OUT;
}

void Action::onApplicationSettings()
{
    R_LOG_TRACE_IN;
    ApplicationSettingsDialog applicationSettingsDialog(MainSettings::getInstance().getApplicationSettings(),
                                                        this->mainWindow);
    applicationSettingsDialog.exec();
    R_LOG_TRACE_OUT;
}

void Action::onUpdate()
{
    R_LOG_TRACE_IN;
    UpdateDialog updateDialog(RRASession::getInstance().getAvailableSoftwareVersion(),
                              RRASession::getInstance().getAvailableSoftwareLink(),
                              this->mainWindow);
    updateDialog.exec();
    R_LOG_TRACE_OUT;
}

void Action::onAbout()
{
    R_LOG_TRACE_IN;
    QString title = tr("About");
    QString body = QString("<h1>") + RVendor::name + QString(" ") + QString::number(RVendor::version.getMajor()) + QString("</h1>")
                 + QString("<h2>") + RVendor::description + QString("</h2>")
                 + RVendor::version.toString()
                 + QString("<br/>")
                 + RVendor::author.toHtmlEscaped() + QString(" &copy; ") + QString::number(RVendor::year) + QString("<br/>")
                 + QString("<a href=\"mailto:") + RVendor::email + QString("\">") + RVendor::email + QString("</a><br/>")
                 + QString("<a href=\"") + RVendor::www + QString("\">") + RVendor::www + QString("</a>");

    QMessageBox::about(this->mainWindow,title,body);
    R_LOG_TRACE_OUT;
}

void Action::onAboutQt()
{
    R_LOG_TRACE_IN;
    QMessageBox::aboutQt(this->mainWindow,tr("About Qt"));
    R_LOG_TRACE_OUT;
}

void Action::onLicense()
{
    R_LOG_TRACE_IN;
    QString licenseFileName(MainSettings::getInstance().findLicenseFileName());

    try
    {
        QFile file(licenseFileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",licenseFileName.toUtf8().constData());
        }
        QString licenseText(file.readAll());
        file.close();

        TextBrowserDialog textBrowserDialog(tr("License"),
                                            licenseFileName,
                                            licenseText);
        textBrowserDialog.exec();
    }
    catch (const RError &rError)
    {
        RLogger::error("Failed to display license from file \'%s\'. %s\n",licenseFileName.toUtf8().constData(),rError.getMessage().toUtf8().constData());
    }
    catch (...)
    {
        RLogger::error("Failed to display license from file \'%s\'.\n",licenseFileName.toUtf8().constData());
    }
    R_LOG_TRACE_OUT;
}

void Action::onReleaseNotes()
{
    R_LOG_TRACE_IN;
    QString releaseNotesFileName(MainSettings::getInstance().findReleaseNotesFileName());

    try
    {
        QFile file(releaseNotesFileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",releaseNotesFileName.toUtf8().constData());
        }
        QString releaseNotesText(file.readAll());
        file.close();

        TextBrowserDialog textBrowserDialog(tr("Release notes"),
                                            releaseNotesFileName,
                                            releaseNotesText);
        textBrowserDialog.exec();
    }
    catch (const RError &rError)
    {
        RLogger::error("Failed to display release notes from file \'%s\'. %s\n",releaseNotesFileName.toUtf8().constData(),rError.getMessage().toUtf8().constData());
    }
    catch (...)
    {
        RLogger::error("Failed to display release notes from file \'%s\'.\n",releaseNotesFileName.toUtf8().constData());
    }
    R_LOG_TRACE_OUT;
}

