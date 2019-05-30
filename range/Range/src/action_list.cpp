/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   action_list.cpp                                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   16-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Action list class definition                        *
 *********************************************************************/

#include <QKeySequence>

#include "action_list.h"
#include "main_window.h"
#include "session.h"
#include "solver_manager.h"
#include "material_list.h"
#include "mark_entity_dialog.h"
#include "merge_entity_dialog.h"

ActionList::ActionList(QObject *parent) : QObject(parent)
{
    this->actions.resize(ACTION_N_TYPES);

    for (uint i=0;i<ACTION_N_TYPES;i++)
    {
        if (ActionType(i) != ACTION_SEPARATOR)
        {
            this->actions[i] = new Action(ActionType(i),(MainWindow*)this->parent(),this);
        }
    }

    this->processAvailability();
}

uint ActionList::getNActions(void) const
{
    return this->actions.size();
}

Action * ActionList::getAction (ActionType type)
{
    if (type == ACTION_SEPARATOR)
    {
        return new Action(ACTION_SEPARATOR,(MainWindow*)this->parent(),this);
    }
    else
    {
        return this->actions[type];
    }
}

Action * ActionList::getAction (const QString &name)
{
    for (QVector<Action*>::size_type i = 0; i != this->actions.size(); i++)
    {
        if (this->actions[i] == 0)
        {
            continue;
        }
        if (this->actions[i]->objectName() == name)
        {
            return this->getAction(ActionType(i));
        }
    }
    return 0;
}

void ActionList::processAvailability(void)
{
    this->setDisabled(false);

    this->getAction(ACTION_GEOMETRY_UNDO)->setDisabled(!Session::getInstance().isUndoAvailable());
    this->getAction(ACTION_GEOMETRY_UNDO)->setToolTip(Session::getInstance().getUndoTooltip());
    this->getAction(ACTION_GEOMETRY_REDO)->setDisabled(!Session::getInstance().isRedoAvailable());
    this->getAction(ACTION_GEOMETRY_REDO)->setToolTip(Session::getInstance().getRedoTooltip());

    this->getAction(ACTION_SESSION_NEW)->setEnabled(true);
    this->getAction(ACTION_SESSION_OPEN)->setEnabled(true);
    this->getAction(ACTION_SESSION_SAVE)->setEnabled(true);
    this->getAction(ACTION_SESSION_SAVE_AS)->setEnabled(true);
    this->getAction(ACTION_SESSION_CLOSE)->setEnabled(true);
    this->getAction(ACTION_MODEL_NEW)->setEnabled(true);
    this->getAction(ACTION_MODEL_OPEN)->setEnabled(true);

    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();
    if (!selectedModelIDs.empty())
    {
        this->getAction(ACTION_MODEL_SAVE)->setEnabled(true);
        this->getAction(ACTION_MODEL_SAVE_AS)->setEnabled(true);
        this->getAction(ACTION_MODEL_EXPORT_MSH)->setEnabled(true);
        this->getAction(ACTION_MODEL_EXPORT_RAW)->setEnabled(true);
        this->getAction(ACTION_MODEL_EXPORT_STL_ASCII)->setEnabled(true);
        this->getAction(ACTION_MODEL_EXPORT_STL_BINARY)->setEnabled(true);
        this->getAction(ACTION_MODEL_CLOSE)->setEnabled(true);
        this->getAction(ACTION_MODEL_RELOAD_RESULTS)->setEnabled(true);
        this->getAction(ACTION_MODEL_DROP_RESULTS)->setEnabled(true);
        this->getAction(ACTION_MODEL_RENAME)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DRAW_POINT)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DRAW_LINE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DRAW_TRIANGLE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DRAW_QUADRILATERAL)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DRAW_CIRCLE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DRAW_ELLIPSE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DRAW_TETRAHEDRON)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DRAW_HEXAHEDRON)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DRAW_CYLINDER)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DRAW_SPHERE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DRAW_RAW)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_CREATE_ELEMENT)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_FIND_SLIVER_ELEMENTS)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_FIX_SLIVER_ELEMENTS)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_FIND_INTERSECTED_ELEMENTS)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_BREAK_INTERSECTED_ELEMENTS)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_BOOL_UNION)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_BOOL_DIFFERENCE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_BOOL_INTERSECTION)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_MERGE_NEAR_NODES)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_MOVE_NODE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_REMOVE_NODE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_REMOVE_NODE_AND_CLOSE_HOLE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_REMOVE_ELEMENT)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_LINE_GENERATE_FROM_EDGES)->setEnabled(Session::selectedModelsHasEntities(R_ENTITY_GROUP_SURFACE));
        this->getAction(ACTION_GEOMETRY_SURFACE_MARK)->setEnabled(Session::selectedModelsHasEntities(R_ENTITY_GROUP_SURFACE));
        this->getAction(ACTION_GEOMETRY_SURFACE_SWAP_ELEMENT_NORMAL)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_SURFACE_SWAP_NORMALS)->setEnabled(Session::selectedModelsHasEntities(R_ENTITY_GROUP_SURFACE));
        this->getAction(ACTION_GEOMETRY_SURFACE_SYNC_NORMALS)->setEnabled(Session::selectedModelsHasEntities(R_ENTITY_GROUP_SURFACE));
        this->getAction(ACTION_GEOMETRY_SURFACE_CLOSE_HOLE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_SURFACE_COARSEN)->setEnabled(Session::selectedModelsHasEntities(R_ENTITY_GROUP_SURFACE));
        this->getAction(ACTION_GEOMETRY_VECTOR_FIELD_CREATE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_VECTOR_FIELD_EDIT)->setEnabled(Session::selectedModelsHasEntitySelected(R_ENTITY_GROUP_VECTOR_FIELD));
        this->getAction(ACTION_GEOMETRY_SCALAR_FIELD_CREATE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_SCALAR_FIELD_EDIT)->setEnabled(Session::selectedModelsHasEntitySelected(R_ENTITY_GROUP_SCALAR_FIELD));
        this->getAction(ACTION_GEOMETRY_STREAM_LINE_CREATE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_STREAM_LINE_EDIT)->setEnabled(Session::selectedModelsHasEntitySelected(R_ENTITY_GROUP_STREAM_LINE));
        this->getAction(ACTION_GEOMETRY_CUT_CREATE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_CUT_EDIT)->setEnabled(Session::selectedModelsHasEntitySelected(R_ENTITY_GROUP_CUT));
        this->getAction(ACTION_GEOMETRY_ISO_CREATE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_ISO_EDIT)->setEnabled(Session::selectedModelsHasEntitySelected(R_ENTITY_GROUP_ISO));
        this->getAction(ACTION_GEOMETRY_ENTITY_MERGE)->setEnabled(MergeEntityDialog::entitiesSelected());
        this->getAction(ACTION_GEOMETRY_ENTITY_REMOVE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_TRANSFORM)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_VOLUME_GENERATE_TETRAHEDRA)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DEV_EXPORT_SLIVER_ELEMENTS)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DEV_EXPORT_INTERSECTED_ELEMENTS)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DEV_PURGE_UNUSED_NODES)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DEV_PURGE_UNUSED_ELEMENTS)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DEV_REMOVE_DUPLICATE_NODES)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DEV_REMOVE_DUPLICATE_ELEMENTS)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DEV_POINT_INSIDE_SURFACE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DEV_TETRAHEDRALIZE_SURFACE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DEV_CONSOLIDATE)->setEnabled(true);
        this->getAction(ACTION_PROBLEM_TASK_FLOW)->setEnabled(true);
        this->getAction(ACTION_PROBLEM_SOLVER_SETUP)->setEnabled(true);
        this->getAction(ACTION_PROBLEM_DEFINE_MONITORING_POINTS)->setEnabled(true);
        this->getAction(ACTION_PROBLEM_RESET)->setEnabled(true);
        this->getAction(ACTION_SOLVER_START)->setEnabled(true);
        this->getAction(ACTION_REPORT_MODEL_STATISTICS)->setEnabled(true);
        this->getAction(ACTION_REPORT_SOLVER_LOG)->setEnabled(true);
        this->getAction(ACTION_REPORT_CONVERGENCE_GRAPH)->setEnabled(true);
        this->getAction(ACTION_REPORT_MONITORING_POINT_GRAPH)->setEnabled(true);
        this->getAction(ACTION_REPORT_CREATE_REPORT)->setEnabled(true);
    }

    if (MaterialList::getInstance().getSelectedMaterialName().length() > 0)
    {
        this->getAction(ACTION_MATERIAL_DELETE)->setEnabled(true);
    }

    if (SolverManager::getInstance().getNRunning() > 0)
    {
        this->getAction(ACTION_SOLVER_STOP)->setEnabled(true);
        this->getAction(ACTION_SOLVER_KILL)->setEnabled(true);
    }
}

void ActionList::enable(void)
{
    this->processAvailability();
}

void ActionList::disable(void)
{
    this->setDisabled(true);
}

void ActionList::setDisabled(bool allActions)
{
    bool enabled = false;

    if (allActions)
    {
        this->getAction(ACTION_SESSION_NEW)->setEnabled(enabled);
        this->getAction(ACTION_SESSION_OPEN)->setEnabled(enabled);
        this->getAction(ACTION_SESSION_SAVE)->setEnabled(enabled);
        this->getAction(ACTION_SESSION_SAVE_AS)->setEnabled(enabled);
        this->getAction(ACTION_SESSION_CLOSE)->setEnabled(enabled);
        this->getAction(ACTION_MODEL_NEW)->setEnabled(enabled);
        this->getAction(ACTION_MODEL_OPEN)->setEnabled(enabled);
    }
    this->getAction(ACTION_MODEL_SAVE)->setEnabled(enabled);
    this->getAction(ACTION_MODEL_SAVE_AS)->setEnabled(enabled);
    this->getAction(ACTION_MODEL_EXPORT_MSH)->setEnabled(enabled);
    this->getAction(ACTION_MODEL_EXPORT_RAW)->setEnabled(enabled);
    this->getAction(ACTION_MODEL_EXPORT_STL_ASCII)->setEnabled(enabled);
    this->getAction(ACTION_MODEL_EXPORT_STL_BINARY)->setEnabled(enabled);
    this->getAction(ACTION_MODEL_CLOSE)->setEnabled(enabled);
    this->getAction(ACTION_MODEL_RELOAD_RESULTS)->setEnabled(enabled);
    this->getAction(ACTION_MODEL_DROP_RESULTS)->setEnabled(enabled);
    this->getAction(ACTION_MODEL_RENAME)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_UNDO)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_REDO)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_DRAW_POINT)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_DRAW_LINE)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_DRAW_TRIANGLE)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_DRAW_QUADRILATERAL)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_DRAW_CIRCLE)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_DRAW_ELLIPSE)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_DRAW_TETRAHEDRON)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_DRAW_HEXAHEDRON)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_DRAW_CYLINDER)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_DRAW_SPHERE)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_DRAW_RAW)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_CREATE_ELEMENT)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_FIND_SLIVER_ELEMENTS)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_FIX_SLIVER_ELEMENTS)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_FIND_INTERSECTED_ELEMENTS)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_BREAK_INTERSECTED_ELEMENTS)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_BOOL_UNION)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_BOOL_DIFFERENCE)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_BOOL_INTERSECTION)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_MERGE_NEAR_NODES)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_MOVE_NODE)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_REMOVE_NODE)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_REMOVE_NODE_AND_CLOSE_HOLE)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_REMOVE_ELEMENT)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_LINE_GENERATE_FROM_EDGES)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_SURFACE_MARK)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_SURFACE_SWAP_ELEMENT_NORMAL)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_SURFACE_SWAP_NORMALS)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_SURFACE_SYNC_NORMALS)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_SURFACE_CLOSE_HOLE)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_SURFACE_COARSEN)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_VECTOR_FIELD_CREATE)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_VECTOR_FIELD_EDIT)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_SCALAR_FIELD_CREATE)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_SCALAR_FIELD_EDIT)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_STREAM_LINE_CREATE)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_STREAM_LINE_EDIT)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_CUT_CREATE)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_CUT_EDIT)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_ISO_CREATE)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_ISO_EDIT)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_ENTITY_MERGE)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_ENTITY_REMOVE)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_TRANSFORM)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_VOLUME_GENERATE_TETRAHEDRA)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_DEV_EXPORT_SLIVER_ELEMENTS)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_DEV_EXPORT_INTERSECTED_ELEMENTS)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_DEV_PURGE_UNUSED_NODES)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_DEV_PURGE_UNUSED_ELEMENTS)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_DEV_POINT_INSIDE_SURFACE)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_DEV_TETRAHEDRALIZE_SURFACE)->setEnabled(enabled);
    this->getAction(ACTION_GEOMETRY_DEV_CONSOLIDATE)->setEnabled(enabled);
    this->getAction(ACTION_PROBLEM_TASK_FLOW)->setEnabled(enabled);
    this->getAction(ACTION_PROBLEM_SOLVER_SETUP)->setEnabled(enabled);
    this->getAction(ACTION_PROBLEM_DEFINE_MONITORING_POINTS)->setEnabled(enabled);
    this->getAction(ACTION_PROBLEM_RESET)->setEnabled(enabled);
    this->getAction(ACTION_SOLVER_START)->setEnabled(enabled);
    this->getAction(ACTION_SOLVER_STOP)->setEnabled(enabled);
    this->getAction(ACTION_SOLVER_KILL)->setEnabled(enabled);
    this->getAction(ACTION_REPORT_MODEL_STATISTICS)->setEnabled(enabled);
    this->getAction(ACTION_REPORT_SOLVER_LOG)->setEnabled(enabled);
    this->getAction(ACTION_REPORT_CONVERGENCE_GRAPH)->setEnabled(enabled);
    this->getAction(ACTION_REPORT_MONITORING_POINT_GRAPH)->setEnabled(enabled);
    this->getAction(ACTION_REPORT_CREATE_REPORT)->setEnabled(enabled);
    this->getAction(ACTION_MATERIAL_DELETE)->setEnabled(enabled);
}

void ActionList::changeShortcut(ActionType actionType, const QString &shortcut)
{
    this->getAction(actionType)->setShortcut(QKeySequence(shortcut));
}
