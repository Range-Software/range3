/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   action_definition.cpp                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   9-th September 2014                                      *
 *                                                                   *
 *  DESCRIPTION: Action definition class definition                  *
 *********************************************************************/

#include <rblib.h>

#include "action_definition.h"
#include "action.h"

ActionDefinition::ActionDefinition(QObject *parent)
    : QObject(parent)
{
    ActionDefinition::generateActionDescList(this->actionDesc);
}

ActionType ActionDefinition::findShortcutActionType(const QString &shortCut) const
{
    for (int i=0;i<int(ACTION_N_TYPES);i++)
    {
        if (this->actionDesc[i].shortCut == shortCut)
        {
            return ActionType(i);
        }
    }
    return ACTION_NONE;
}

const QString &ActionDefinition::getText(ActionType type) const
{
    if (!ACTION_TYPE_IS_VALID(type))
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Invalid action type: &d\n",int(type));
    }
    return this->actionDesc[type].text;
}

const QString &ActionDefinition::getDescription(ActionType type) const
{
    if (!ACTION_TYPE_IS_VALID(type))
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Invalid action type: &d\n",int(type));
    }
    return this->actionDesc[type].desc;
}

const QString &ActionDefinition::getShortCut(ActionType type) const
{
    if (!ACTION_TYPE_IS_VALID(type))
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Invalid action type: &d\n",int(type));
    }
    return this->actionDesc[type].shortCut;
}

void ActionDefinition::setShortcut(ActionType type, const QString &shortCut)
{
    if (!ACTION_TYPE_IS_VALID(type))
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Invalid action type: &d\n",int(type));
    }
    this->actionDesc[type].shortCut = shortCut;
    emit this->shortcutChanged(type,shortCut);
}

const QString &ActionDefinition::getIcon(ActionType type) const
{
    if (!ACTION_TYPE_IS_VALID(type))
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Invalid action type: &d\n",int(type));
    }
    return this->actionDesc[type].icon;
}

PointerToMemberFunction ActionDefinition::getSlot(ActionType type) const
{
    if (!ACTION_TYPE_IS_VALID(type))
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Invalid action type: &d\n",int(type));
    }
    return this->actionDesc[type].slot;
}

QList<ActionType> ActionDefinition::getActionTypes(ActionGroupTypeMask actionGroupTypeMask) const
{
    QList<ActionType> actionTypes;

    foreach (ActionDefinitionItem item, this->actionDesc)
    {
        if (item.getGroupType() & actionGroupTypeMask)
        {
            actionTypes.push_back(item.getType());
        }
    }

    return actionTypes;
}

void ActionDefinition::generateActionDescList(QList<ActionDefinitionItem> &actionDesc)
{
    actionDesc.clear();
    actionDesc.push_back(ActionDefinitionItem(ACTION_NONE, ACTION_GROUP_NONE, "", "", "", "", nullptr ));
    actionDesc.push_back(ActionDefinitionItem(ACTION_SESSION_NEW, ACTION_GROUP_SESSION, "New Session", "Create a new session.", "Ctrl+Alt+N", ":/icons/file/pixmaps/range-session_new.svg", &Action::onSessionNew));
    actionDesc.push_back(ActionDefinitionItem(ACTION_SESSION_OPEN, ACTION_GROUP_SESSION, "Open Session", "Open previously saved session.", "Ctrl+Alt+O", ":/icons/file/pixmaps/range-session_open.svg", &Action::onSessionOpen));
    actionDesc.push_back(ActionDefinitionItem(ACTION_SESSION_SAVE, ACTION_GROUP_SESSION, "Save Session", "Save current session.", "Ctrl+Alt+S", ":/icons/file/pixmaps/range-session_save.svg", &Action::onSessionSave));
    actionDesc.push_back(ActionDefinitionItem(ACTION_SESSION_SAVE_AS, ACTION_GROUP_SESSION, "Save Session As", "Save current session under different filename.", "Ctrl+Alt+Shift+S", ":/icons/file/pixmaps/range-session_save_as.svg", &Action::onSessionSaveAs));
    actionDesc.push_back(ActionDefinitionItem(ACTION_SESSION_CLOSE, ACTION_GROUP_SESSION, "Close Session", "Close current session.", "Ctrl+Alt+W", ":/icons/file/pixmaps/range-session_close.svg", &Action::onSessionClose));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_NEW, ACTION_GROUP_MODEL, "New Model", "Create a new empty model.", "Ctrl+N", ":/icons/file/pixmaps/range-model_new.svg", &Action::onModelNew));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_OPEN, ACTION_GROUP_MODEL, "Open Model", "Open previously saved model.", "Ctrl+O", ":/icons/file/pixmaps/range-model_open.svg", &Action::onModelOpen));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_SAVE, ACTION_GROUP_MODEL, "Save Model", "Save selected model. ", "Ctrl+S", ":/icons/file/pixmaps/range-model_save.svg", &Action::onModelSave));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_SAVE_AS, ACTION_GROUP_MODEL, "Save Model As", "Save selected model under a different filename.", "Ctrl+Shift+S", ":/icons/file/pixmaps/range-model_save_as.svg", &Action::onModelSaveAs));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_EXPORT_MSH, ACTION_GROUP_MODEL, "Export MSH model", "", "", ":/icons/file/pixmaps/range-model_export_msh.svg", &Action::onModelExportMsh));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_EXPORT_RAW, ACTION_GROUP_MODEL, "Export RAW model", "", "", ":/icons/file/pixmaps/range-model_export_raw.svg", &Action::onModelExportRaw));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_EXPORT_STL_ASCII, ACTION_GROUP_MODEL, "Export STL model (ascii)", "", "", ":/icons/file/pixmaps/range-model_export_stl.svg", &Action::onModelExportStlAscii));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_EXPORT_STL_BINARY, ACTION_GROUP_MODEL, "Export STL model (binary)", "", "", ":/icons/file/pixmaps/range-model_export_stl.svg", &Action::onModelExportStlBinary));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_CLOSE, ACTION_GROUP_MODEL, "Close Model", "", "Ctrl+W", ":/icons/file/pixmaps/range-model_close.svg", &Action::onModelClose));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_RELOAD_RESULTS, ACTION_GROUP_MODEL, "Reload Results", "", "", ":/icons/file/pixmaps/range-refresh.svg", &Action::onModelReloadResults));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_DROP_RESULTS, ACTION_GROUP_MODEL, "Drop Results", "", "", ":/icons/file/pixmaps/range-model_drop_results.svg", &Action::onModelDropResults));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_RENAME, ACTION_GROUP_MODEL, "Rename Model", "", "", ":/icons/file/pixmaps/range-model_rename.svg", &Action::onModelRename));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MATERIAL_NEW, ACTION_GROUP_MATERIAL, "New Material", "", "Ctrl+Alt+M", ":/icons/file/pixmaps/range-material_new.svg", &Action::onMaterialNew));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MATERIAL_IMPORT, ACTION_GROUP_MATERIAL, "Import Material", "", "", ":/icons/file/pixmaps/range-material_import.svg", &Action::onMaterialImport));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MATERIAL_DELETE, ACTION_GROUP_MATERIAL, "Delete Material", "", "Ctrl+Alt+D", ":/icons/file/pixmaps/range-material_delete.svg", &Action::onMaterialDelete));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_UNDO, ACTION_GROUP_GEOMETRY, "Undo", "", "Ctrl+Z", ":/icons/geometry/pixmaps/range-undo.svg", &Action::onGeometryUndo));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_REDO, ACTION_GROUP_GEOMETRY, "Redo", "", "Ctrl+Shift+Z", ":/icons/geometry/pixmaps/range-redo.svg", &Action::onGeometryRedo));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_POINT, ACTION_GROUP_GEOMETRY, "Draw point", "", "", ":/icons/geometry/pixmaps/range-draw_point.svg", &Action::onGeometryDrawPoint));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_LINE, ACTION_GROUP_GEOMETRY, "Draw line", "", "", ":/icons/geometry/pixmaps/range-draw_line.svg", &Action::onGeometryDrawLine));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_TRIANGLE, ACTION_GROUP_GEOMETRY, "Draw triangle", "", "", ":/icons/geometry/pixmaps/range-draw_triangle.svg", &Action::onGeometryDrawTriangle));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_QUADRILATERAL, ACTION_GROUP_GEOMETRY, "Draw quadrilateral", "", "", ":/icons/geometry/pixmaps/range-draw_quadrilateral.svg", &Action::onGeometryDrawQuadrilateral));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_CIRCLE, ACTION_GROUP_GEOMETRY, "Draw circle", "", "", ":/icons/geometry/pixmaps/range-draw_circle.svg", &Action::onGeometryDrawCircle));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_ELLIPSE, ACTION_GROUP_GEOMETRY, "Draw ellipse", "", "", ":/icons/geometry/pixmaps/range-draw_ellipse.svg", &Action::onGeometryDrawEllipse));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_TETRAHEDRON, ACTION_GROUP_GEOMETRY, "Draw tetrahedron", "", "", ":/icons/geometry/pixmaps/range-draw_tetrahedron.svg", &Action::onGeometryDrawTetrahedron));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_HEXAHEDRON, ACTION_GROUP_GEOMETRY, "Draw hexahedron", "", "", ":/icons/geometry/pixmaps/range-draw_hexahedron.svg", &Action::onGeometryDrawHexahedron));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_CYLINDER, ACTION_GROUP_GEOMETRY, "Draw cylinder", "", "", ":/icons/geometry/pixmaps/range-draw_cylinder.svg", &Action::onGeometryDrawCylinder));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_SPHERE, ACTION_GROUP_GEOMETRY, "Draw sphere", "", "", ":/icons/geometry/pixmaps/range-draw_sphere.svg", &Action::onGeometryDrawSphere));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_RAW, ACTION_GROUP_GEOMETRY, "Draw RAW object", "", "", ":/icons/geometry/pixmaps/range-draw_raw.svg", &Action::onGeometryDrawRaw));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_CREATE_ELEMENT, ACTION_GROUP_GEOMETRY, "Create element", "", "Ctrl+Alt+E", ":/icons/geometry/pixmaps/range-element_create.svg", &Action::onGeometryCreateElement));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_FIND_SLIVER_ELEMENTS, ACTION_GROUP_GEOMETRY, "Find sliver elements", "", "", ":/icons/geometry/pixmaps/range-sliver_find.svg", &Action::onGeometryFindSliverElements));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_FIX_SLIVER_ELEMENTS, ACTION_GROUP_GEOMETRY, "Fix sliver elements", "", "", ":/icons/geometry/pixmaps/range-sliver_fix.svg", &Action::onGeometryFixSliverElements));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_FIND_INTERSECTED_ELEMENTS, ACTION_GROUP_GEOMETRY, "Find intersected elements", "", "Ctrl+I", ":/icons/geometry/pixmaps/range-entity_surface_find_intersected.svg", &Action::onGeometryFindIntersectedElements));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_BREAK_INTERSECTED_ELEMENTS, ACTION_GROUP_GEOMETRY, "Break intersected elements", "", "Ctrl+J", ":/icons/geometry/pixmaps/range-entity_surface_break_intersected.svg", &Action::onGeometryBreakIntersectedElements));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_BOOL_UNION, ACTION_GROUP_GEOMETRY, "Union", "", "", ":/icons/geometry/pixmaps/range-entity_surface_bool_union.svg", &Action::onGeometryBoolUnion));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_BOOL_DIFFERENCE, ACTION_GROUP_GEOMETRY, "Difference", "", "", ":/icons/geometry/pixmaps/range-entity_surface_bool_difference.svg", &Action::onGeometryBoolDifference));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_BOOL_INTERSECTION, ACTION_GROUP_GEOMETRY, "Intersection", "", "", ":/icons/geometry/pixmaps/range-entity_surface_bool_intersection.svg", &Action::onGeometryBoolIntersection));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_MERGE_NEAR_NODES, ACTION_GROUP_GEOMETRY, "Merge near nodes", "", "", ":/icons/geometry/pixmaps/range-merge_near_nodes.svg", &Action::onGeometryMergeNearNodes));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_MOVE_NODE, ACTION_GROUP_GEOMETRY, "Move node", "", "", ":/icons/geometry/pixmaps/range-move_node.svg", &Action::onGeometryMoveNode));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_REMOVE_NODE, ACTION_GROUP_GEOMETRY, "Remove node", "", "Ctrl+Shift+X", ":/icons/geometry/pixmaps/range-node_remove.svg", &Action::onGeometryRemoveNode));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_REMOVE_NODE_AND_CLOSE_HOLE, ACTION_GROUP_GEOMETRY, "Remove node and close hole", "", "", ":/icons/geometry/pixmaps/range-node_remove_and_close.svg", &Action::onGeometryRemoveNodeAndCloseHole));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_REMOVE_ELEMENT, ACTION_GROUP_GEOMETRY, "Remove element", "", "Ctrl+Alt+X", ":/icons/geometry/pixmaps/range-element_remove.svg", &Action::onGeometryRemoveElement));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_LINE_GENERATE_FROM_EDGES, ACTION_GROUP_GEOMETRY, "Generate line(s) from surface edges", "", "", ":/icons/geometry/pixmaps/range-entity_line_from_surface.svg", &Action::onGeometryLineGenerateFromEdges));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_SURFACE_MARK, ACTION_GROUP_GEOMETRY, "Mark surface", "", "", ":/icons/geometry/pixmaps/range-entity_surface_mark.svg", &Action::onGeometrySurfaceMark));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_SURFACE_SWAP_ELEMENT_NORMAL, ACTION_GROUP_GEOMETRY, "Swap element normal", "", "", ":/icons/geometry/pixmaps/range-entity_surface_swap_element_normal.svg", &Action::onGeometrySurfaceSwapElementNormal));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_SURFACE_SWAP_NORMALS, ACTION_GROUP_GEOMETRY, "Swap normals", "", "", ":/icons/geometry/pixmaps/range-entity_surface_swap_normals.svg", &Action::onGeometrySurfaceSwapNormals));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_SURFACE_SYNC_NORMALS, ACTION_GROUP_GEOMETRY, "Synchronize normals", "", "", ":/icons/geometry/pixmaps/range-entity_surface_sync_normals.svg", &Action::onGeometrySurfaceSyncNormals));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_SURFACE_CLOSE_HOLE, ACTION_GROUP_GEOMETRY, "Close hole", "", "", ":/icons/geometry/pixmaps/range-entity_surface_close_hole.svg", &Action::onGeometrySurfaceCloseHole));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_SURFACE_COARSEN, ACTION_GROUP_GEOMETRY, "Coarsen surface", "", "", ":/icons/geometry/pixmaps/range-entity_surface_coarsen.svg", &Action::onGeometrySurfaceCoarsen));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_VOLUME_GENERATE_TETRAHEDRA, ACTION_GROUP_GEOMETRY, "Generate tetrahedral mesh", "", "Ctrl+G", ":/icons/geometry/pixmaps/range-generate_volume_mesh.svg", &Action::onGeometryGenerateTetrahedra));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_VECTOR_FIELD_CREATE, ACTION_GROUP_GEOMETRY, "Create vector field", "", "", ":/icons/geometry/pixmaps/range-entity_vector_field_create.svg", &Action::onGeometryVectorFieldCreate));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_VECTOR_FIELD_EDIT, ACTION_GROUP_GEOMETRY, "Edit vector field", "", "", ":/icons/geometry/pixmaps/range-entity_vector_field_edit.svg", &Action::onGeometryVectorFieldEdit));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_SCALAR_FIELD_CREATE, ACTION_GROUP_GEOMETRY, "Create scalar field", "", "", ":/icons/geometry/pixmaps/range-entity_scalar_field_create.svg", &Action::onGeometryScalarFieldCreate));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_SCALAR_FIELD_EDIT, ACTION_GROUP_GEOMETRY, "Edit scalar field", "", "", ":/icons/geometry/pixmaps/range-entity_scalar_field_edit.svg", &Action::onGeometryScalarFieldEdit));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_STREAM_LINE_CREATE, ACTION_GROUP_GEOMETRY, "Create stream line", "", "", ":/icons/geometry/pixmaps/range-entity_stream_line_create.svg", &Action::onGeometryStreamLineCreate));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_STREAM_LINE_EDIT, ACTION_GROUP_GEOMETRY, "Edit stream line", "", "", ":/icons/geometry/pixmaps/range-entity_stream_line_edit.svg", &Action::onGeometryStreamLineEdit));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_CUT_CREATE, ACTION_GROUP_GEOMETRY, "Create cut", "", "", ":/icons/geometry/pixmaps/range-entity_cut_create.svg", &Action::onGeometryCutCreate));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_CUT_EDIT, ACTION_GROUP_GEOMETRY, "Edit cut", "", "", ":/icons/geometry/pixmaps/range-entity_cut_edit.svg", &Action::onGeometryCutEdit));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_ISO_CREATE, ACTION_GROUP_GEOMETRY, "Create iso", "", "", ":/icons/geometry/pixmaps/range-entity_iso_create.svg", &Action::onGeometryIsoCreate));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_ISO_EDIT, ACTION_GROUP_GEOMETRY, "Edit iso", "", "", ":/icons/geometry/pixmaps/range-entity_iso_edit.svg", &Action::onGeometryIsoEdit));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_ENTITY_MERGE, ACTION_GROUP_GEOMETRY, "Merge selected entities", "", "Ctrl+M", ":/icons/geometry/pixmaps/range-entity_merge.svg", &Action::onGeometryEntityMerge));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_ENTITY_REMOVE, ACTION_GROUP_GEOMETRY, "Remove selected entities", "", "Ctrl+X", ":/icons/geometry/pixmaps/range-entity_remove.svg", &Action::onGeometryEntityRemove));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_TRANSFORM, ACTION_GROUP_GEOMETRY, "Scale, translate, rotate", "", "Ctrl+T", ":/icons/geometry/pixmaps/range-geometry_transform.svg", &Action::onGeometryTransform));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DEV_EXPORT_SLIVER_ELEMENTS, ACTION_GROUP_GEOMETRY, "Export sliver elements", "", "", "", &Action::onGeometryDevExportSliverElements));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DEV_EXPORT_INTERSECTED_ELEMENTS, ACTION_GROUP_GEOMETRY, "Export intersected elements", "", "", "", &Action::onGeometryDevExportIntersectedElements));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DEV_PURGE_UNUSED_NODES, ACTION_GROUP_GEOMETRY, "Purge unused nodes", "", "", "", &Action::onGeometryDevPurgeUnusedNodes));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DEV_PURGE_UNUSED_ELEMENTS, ACTION_GROUP_GEOMETRY, "Purge unused elements", "", "", "", &Action::onGeometryDevPurgeUnusedElements));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DEV_REMOVE_DUPLICATE_NODES, ACTION_GROUP_GEOMETRY, "Remove duplicate nodes", "", "", "", &Action::onGeometryDevRemoveDuplicateNodes));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DEV_REMOVE_DUPLICATE_ELEMENTS, ACTION_GROUP_GEOMETRY, "Remove duplicate elements", "", "", "", &Action::onGeometryDevRemoveDuplicateElements));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DEV_POINT_INSIDE_SURFACE, ACTION_GROUP_GEOMETRY, "Check if point is inside surface", "", "", "", &Action::onGeometryDevPointInsideSurface));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DEV_TETRAHEDRALIZE_SURFACE, ACTION_GROUP_GEOMETRY, "Tetrahedralize surface", "", "", "", &Action::onGeometryDevTetrahedralizeeSurface));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DEV_CONSOLIDATE, ACTION_GROUP_GEOMETRY, "Consolidate geometry", "", "", "", &Action::onGeometryDevConsolidate));
    actionDesc.push_back(ActionDefinitionItem(ACTION_PROBLEM_TASK_FLOW, ACTION_GROUP_PROBLEM, "Problem(s) task flow", "", "Ctrl+P", ":/icons/problem/pixmaps/range-problem_task_flow.svg", &Action::onProblemTaskFlow));
    actionDesc.push_back(ActionDefinitionItem(ACTION_PROBLEM_SOLVER_SETUP, ACTION_GROUP_PROBLEM, "Setup Problem(s) matrix solver", "", "", ":/icons/problem/pixmaps/range-problem_matrix_solver.svg", &Action::onProblemSolverSetup));
    actionDesc.push_back(ActionDefinitionItem(ACTION_PROBLEM_DEFINE_MONITORING_POINTS, ACTION_GROUP_PROBLEM, "Define monitoring points", "", "", ":/icons/problem/pixmaps/range-problem_monitoring_points.svg", &Action::onProblemDefineMonitoringPoints));
    actionDesc.push_back(ActionDefinitionItem(ACTION_PROBLEM_RESET, ACTION_GROUP_PROBLEM, "Reset setup", "", "", ":/icons/problem/pixmaps/range-problem_reset.svg", &Action::onProblemReset));
    actionDesc.push_back(ActionDefinitionItem(ACTION_SOLVER_START, ACTION_GROUP_SOLVER, "Start solver", "", "Ctrl+R", ":/icons/solver/pixmaps/range-solver-start.svg", &Action::onSolverStart));
    actionDesc.push_back(ActionDefinitionItem(ACTION_SOLVER_STOP, ACTION_GROUP_SOLVER, "Stop solver", "", "Ctrl+E", ":/icons/solver/pixmaps/range-solver-stop.svg", &Action::onSolverStop));
    actionDesc.push_back(ActionDefinitionItem(ACTION_SOLVER_KILL, ACTION_GROUP_SOLVER, "Kill solver", "", "Ctrl+K", ":/icons/solver/pixmaps/range-solver-kill.svg", &Action::onSolverKill));
    actionDesc.push_back(ActionDefinitionItem(ACTION_REPORT_MODEL_STATISTICS, ACTION_GROUP_REPORT, "Model statistics", "", "", ":/icons/report/pixmaps/range-report_model_statistics.svg", &Action::onReportModelStatistics));
    actionDesc.push_back(ActionDefinitionItem(ACTION_REPORT_SOLVER_LOG, ACTION_GROUP_REPORT, "Solver log file", "", "", ":/icons/report/pixmaps/range-report_solver_log.svg", &Action::onReportSolverLog));
    actionDesc.push_back(ActionDefinitionItem(ACTION_REPORT_CONVERGENCE_GRAPH, ACTION_GROUP_REPORT, "Solver convergence", "", "Ctrl+Alt+C", ":/icons/report/pixmaps/range-report-convergence.svg", &Action::onReportConvergenceGraph));
    actionDesc.push_back(ActionDefinitionItem(ACTION_REPORT_MONITORING_POINT_GRAPH, ACTION_GROUP_REPORT, "Monitoring points", "", "", ":/icons/report/pixmaps/range-report_monitoring_point.svg", &Action::onReportMonitoringPointGraph));
    actionDesc.push_back(ActionDefinitionItem(ACTION_REPORT_CREATE_REPORT, ACTION_GROUP_REPORT, "Create report", "", "Ctrl+Shift+R", ":/icons/report/pixmaps/range-report.svg", &Action::onReportCreateReport));
    actionDesc.push_back(ActionDefinitionItem(ACTION_APPLICATION_SETTINGS, ACTION_GROUP_APPLICATION, "Application settings", "", "Ctrl+A", ":/icons/application/pixmaps/range-application_settings.svg", &Action::onApplicationSettings));
    actionDesc.push_back(ActionDefinitionItem(ACTION_APPLICATION_UPDATE, ACTION_GROUP_APPLICATION, "Check for update", "", "", ":/icons/file/pixmaps/range-download.svg", &Action::onUpdate));
    actionDesc.push_back(ActionDefinitionItem(ACTION_APPLICATION_ABOUT, ACTION_GROUP_APPLICATION, "About", "", "F1", ":/icons/logos/pixmaps/range-logo-128.png", &Action::onAbout));
    actionDesc.push_back(ActionDefinitionItem(ACTION_APPLICATION_ABOUT_QT, ACTION_GROUP_APPLICATION, "About Qt", "", "", ":/icons/file/pixmaps/range-qt.svg", &Action::onAboutQt));
    actionDesc.push_back(ActionDefinitionItem(ACTION_APPLICATION_LICENSE, ACTION_GROUP_APPLICATION, "License", "", "", ":/icons/logos/pixmaps/range-logo-128.png", &Action::onLicense));
    actionDesc.push_back(ActionDefinitionItem(ACTION_APPLICATION_RELEASE_NOTES, ACTION_GROUP_APPLICATION, "Release notes", "", "", ":/icons/logos/pixmaps/range-logo-128.png", &Action::onReleaseNotes));
    actionDesc.push_back(ActionDefinitionItem(ACTION_APPLICATION_HELP, ACTION_GROUP_APPLICATION, "Help", "", "Ctrl+H", ":/icons/file/pixmaps/range-help.svg", &Action::onHelp));
    actionDesc.push_back(ActionDefinitionItem(ACTION_APPLICATION_QUIT, ACTION_GROUP_APPLICATION, "Quit", "", "Q", ":/icons/file/pixmaps/range-shutdown.svg", &Action::onQuit));
    actionDesc.push_back(ActionDefinitionItem(ACTION_SEPARATOR, ACTION_GROUP_NONE, "", "", "", "", nullptr));
}
