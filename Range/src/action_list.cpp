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
    this->getAction(ACTION_MODEL_SAVE)->setDisabled(true);
    this->getAction(ACTION_MODEL_SAVE_AS)->setDisabled(true);
    this->getAction(ACTION_MODEL_EXPORT_MSH)->setDisabled(true);
    this->getAction(ACTION_MODEL_EXPORT_RAW)->setDisabled(true);
    this->getAction(ACTION_MODEL_EXPORT_STL_ASCII)->setDisabled(true);
    this->getAction(ACTION_MODEL_EXPORT_STL_BINARY)->setDisabled(true);
    this->getAction(ACTION_MODEL_CLOSE)->setDisabled(true);
    this->getAction(ACTION_MODEL_RELOAD_RESULTS)->setDisabled(true);
    this->getAction(ACTION_MODEL_DROP_RESULTS)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_UNDO)->setDisabled(!Session::getInstance().isUndoAvailable());
    this->getAction(ACTION_GEOMETRY_UNDO)->setToolTip(Session::getInstance().getUndoTooltip());
    this->getAction(ACTION_GEOMETRY_REDO)->setDisabled(!Session::getInstance().isRedoAvailable());
    this->getAction(ACTION_GEOMETRY_REDO)->setToolTip(Session::getInstance().getRedoTooltip());
    this->getAction(ACTION_GEOMETRY_DRAW_POINT)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_DRAW_LINE)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_DRAW_TRIANGLE)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_DRAW_QUADRILATERAL)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_DRAW_CIRCLE)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_DRAW_ELLIPSE)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_DRAW_TETRAHEDRON)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_DRAW_HEXAHEDRON)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_DRAW_CYLINDER)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_DRAW_SPHERE)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_CREATE_ELEMENT)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_FIX_SLIVER_ELEMENTS)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_FIND_INTERSECTED_ELEMENTS)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_BREAK_INTERSECTED_ELEMENTS)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_BOOL_UNION)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_BOOL_DIFFERENCE)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_BOOL_INTERSECTION)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_MERGE_NEAR_NODES)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_MOVE_NODE)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_REMOVE_NODE)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_REMOVE_NODE_AND_CLOSE_HOLE)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_REMOVE_ELEMENT)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_LINE_GENERATE_FROM_EDGES)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_SURFACE_MARK)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_SURFACE_SWAP_ELEMENT_NORMAL)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_SURFACE_SWAP_NORMALS)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_SURFACE_SYNC_NORMALS)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_SURFACE_CLOSE_HOLE)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_SURFACE_COARSEN)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_VECTOR_FIELD_CREATE)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_VECTOR_FIELD_EDIT)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_SCALAR_FIELD_CREATE)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_SCALAR_FIELD_EDIT)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_STREAM_LINE_CREATE)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_STREAM_LINE_EDIT)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_CUT_CREATE)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_CUT_EDIT)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_ISO_CREATE)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_ISO_EDIT)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_ENTITY_MERGE)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_ENTITY_REMOVE)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_TRANSFORM)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_VOLUME_GENERATE_TETRAHEDRA)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_DEV_EXPORT_INTERSECTED_ELEMENTS)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_DEV_PURGE_UNUSED_NODES)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_DEV_PURGE_UNUSED_ELEMENTS)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_DEV_POINT_INSIDE_SURFACE)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_DEV_TETRAHEDRALIZE_SURFACE)->setDisabled(true);
    this->getAction(ACTION_GEOMETRY_DEV_CONSOLIDATE)->setDisabled(true);
    this->getAction(ACTION_PROBLEM_TASK_FLOW)->setDisabled(true);
    this->getAction(ACTION_PROBLEM_SOLVER_SETUP)->setDisabled(true);
    this->getAction(ACTION_PROBLEM_DEFINE_MONITORING_POINTS)->setDisabled(true);
    this->getAction(ACTION_PROBLEM_RESET)->setDisabled(true);
    this->getAction(ACTION_SOLVER_START)->setDisabled(true);
    this->getAction(ACTION_SOLVER_STOP)->setDisabled(true);
    this->getAction(ACTION_SOLVER_KILL)->setDisabled(true);
    this->getAction(ACTION_SOLVER_VIEW)->setEnabled(true);
    this->getAction(ACTION_SOLVER_QUEUE)->setEnabled(true);
    this->getAction(ACTION_REPORT_MODEL_STATISTICS)->setDisabled(true);
    this->getAction(ACTION_REPORT_SOLVER_LOG)->setDisabled(true);
    this->getAction(ACTION_REPORT_CONVERGENCE_GRAPH)->setDisabled(true);
    this->getAction(ACTION_REPORT_MONITORING_POINT_GRAPH)->setDisabled(true);
    this->getAction(ACTION_REPORT_CREATE_REPORT)->setDisabled(true);
    this->getAction(ACTION_MATERIAL_DELETE)->setDisabled(true);

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
        this->getAction(ACTION_GEOMETRY_CREATE_ELEMENT)->setEnabled(true);
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
        this->getAction(ACTION_GEOMETRY_VECTOR_FIELD_EDIT)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_SCALAR_FIELD_CREATE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_SCALAR_FIELD_EDIT)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_STREAM_LINE_CREATE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_STREAM_LINE_EDIT)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_CUT_CREATE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_CUT_EDIT)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_ISO_CREATE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_ISO_EDIT)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_ENTITY_MERGE)->setEnabled(MergeEntityDialog::entitiesSelected());
        this->getAction(ACTION_GEOMETRY_ENTITY_REMOVE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_TRANSFORM)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_VOLUME_GENERATE_TETRAHEDRA)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DEV_EXPORT_INTERSECTED_ELEMENTS)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DEV_PURGE_UNUSED_NODES)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DEV_PURGE_UNUSED_ELEMENTS)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DEV_POINT_INSIDE_SURFACE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DEV_TETRAHEDRALIZE_SURFACE)->setEnabled(true);
        this->getAction(ACTION_GEOMETRY_DEV_CONSOLIDATE)->setEnabled(true);
        this->getAction(ACTION_PROBLEM_TASK_FLOW)->setEnabled(true);
        this->getAction(ACTION_PROBLEM_SOLVER_SETUP)->setEnabled(true);
        this->getAction(ACTION_PROBLEM_DEFINE_MONITORING_POINTS)->setEnabled(true);
        this->getAction(ACTION_PROBLEM_RESET)->setEnabled(true);
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
    else
    {
        this->getAction(ACTION_SOLVER_START)->setEnabled(true);
    }
}

void ActionList::changeShortcut(ActionType actionType, const QString &shortcut)
{
    this->getAction(actionType)->setShortcut(QKeySequence(shortcut));
}
