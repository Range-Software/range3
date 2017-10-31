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
    for (uint i=0;i<uint(ACTION_N_TYPES);i++)
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

    for (int i=0;i<this->actionDesc.size();i++)
    {
        if (this->actionDesc[i].getGroupType() & actionGroupTypeMask)
        {
            actionTypes.push_back(this->actionDesc[i].getType());
        }
    }

    return actionTypes;
}

void ActionDefinition::generateActionDescList(QList<ActionDefinitionItem> &actionDesc)
{
    actionDesc.clear();
    actionDesc.push_back(ActionDefinitionItem(ACTION_NONE, ACTION_GROUP_NONE, "", "", "", 0 ));
    actionDesc.push_back(ActionDefinitionItem(ACTION_SESSION_NEW, ACTION_GROUP_SESSION, "New Session", "Ctrl+Alt+N", ":/icons/file/pixmaps/range-project_new.png", &Action::onSessionNew));
    actionDesc.push_back(ActionDefinitionItem(ACTION_SESSION_OPEN, ACTION_GROUP_SESSION, "Open Session", "Ctrl+Alt+O", "", &Action::onSessionOpen));
    actionDesc.push_back(ActionDefinitionItem(ACTION_SESSION_SAVE, ACTION_GROUP_SESSION, "Save Session", "Ctrl+Alt+S", "", &Action::onSessionSave));
    actionDesc.push_back(ActionDefinitionItem(ACTION_SESSION_SAVE_AS, ACTION_GROUP_SESSION, "Save Session As", "Ctrl+Alt+Shift+S", "", &Action::onSessionSaveAs));
    actionDesc.push_back(ActionDefinitionItem(ACTION_SESSION_CLOSE, ACTION_GROUP_SESSION, "Close Session", "Ctrl+Alt+W", "", &Action::onSessionClose));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_NEW, ACTION_GROUP_MODEL, "New Model", "Ctrl+N", ":/icons/file/pixmaps/range-new.svg", &Action::onModelNew));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_OPEN, ACTION_GROUP_MODEL, "Open Model", "Ctrl+O", ":/icons/file/pixmaps/range-open.svg", &Action::onModelOpen));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_SAVE, ACTION_GROUP_MODEL, "Save Model", "Ctrl+S", ":/icons/file/pixmaps/range-save.svg", &Action::onModelSave));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_SAVE_AS, ACTION_GROUP_MODEL, "Save Model As", "Ctrl+Shift+S", ":/icons/file/pixmaps/range-save-as.svg", &Action::onModelSaveAs));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_IMPORT_MSH, ACTION_GROUP_MODEL, "Import MSH model", "", ":/icons/file/pixmaps/range-import-msh.svg", &Action::onModelImportMsh));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_IMPORT_RAW, ACTION_GROUP_MODEL, "Import RAW model", "", ":/icons/file/pixmaps/range-import-raw.svg", &Action::onModelImportRaw));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_IMPORT_STL, ACTION_GROUP_MODEL, "Import STL model", "", ":/icons/file/pixmaps/range-import-stl.svg", &Action::onModelImportStl));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_EXPORT_MSH, ACTION_GROUP_MODEL, "Export MSH model", "", ":/icons/file/pixmaps/range-export-msh.svg", &Action::onModelExportMsh));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_EXPORT_RAW, ACTION_GROUP_MODEL, "Export RAW model", "", ":/icons/file/pixmaps/range-export-raw.svg", &Action::onModelExportRaw));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_EXPORT_STL_ASCII, ACTION_GROUP_MODEL, "Export STL model (ascii)", "", ":/icons/file/pixmaps/range-export-stl.svg", &Action::onModelExportStlAscii));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_EXPORT_STL_BINARY, ACTION_GROUP_MODEL, "Export STL model (binary)", "", ":/icons/file/pixmaps/range-export-stl.svg", &Action::onModelExportStlBinary));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_CLOSE, ACTION_GROUP_MODEL, "Close model", "Ctrl+W", ":/icons/file/pixmaps/range-close.svg", &Action::onModelClose));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_RELOAD_RESULTS, ACTION_GROUP_MODEL, "Reload results", "", ":/icons/file/pixmaps/range-refresh.svg", &Action::onModelReloadResults));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MODEL_DROP_RESULTS, ACTION_GROUP_MODEL, "Drop results", "", ":/icons/file/pixmaps/range-clear.svg", &Action::onModelDropResults));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MATERIAL_NEW, ACTION_GROUP_MATERIAL, "New Material", "Ctrl+Alt+M", ":/icons/file/pixmaps/range-new.svg", &Action::onMaterialNew));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MATERIAL_IMPORT, ACTION_GROUP_MATERIAL, "Import Material", "", ":/icons/file/pixmaps/range-open.svg", &Action::onMaterialImport));
    actionDesc.push_back(ActionDefinitionItem(ACTION_MATERIAL_DELETE, ACTION_GROUP_MATERIAL, "Delete Material", "Ctrl+Alt+D", ":/icons/file/pixmaps/range-close.svg", &Action::onMaterialDelete));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_UNDO, ACTION_GROUP_GEOMETRY, "Undo", "Ctrl+Z", ":/icons/geometry/pixmaps/range-undo.svg", &Action::onGeometryUndo));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_REDO, ACTION_GROUP_GEOMETRY, "Redo", "Ctrl+Shift+Z", ":/icons/geometry/pixmaps/range-redo.svg", &Action::onGeometryRedo));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_POINT, ACTION_GROUP_GEOMETRY, "Draw point", "", "", &Action::onGeometryDrawPoint));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_LINE, ACTION_GROUP_GEOMETRY, "Draw line", "", "", &Action::onGeometryDrawLine));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_TRIANGLE, ACTION_GROUP_GEOMETRY, "Draw triangle", "", "", &Action::onGeometryDrawTriangle));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_QUADRILATERAL, ACTION_GROUP_GEOMETRY, "Draw quadrilateral", "", "", &Action::onGeometryDrawQuadrilateral));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_CIRCLE, ACTION_GROUP_GEOMETRY, "Draw circle", "", "", &Action::onGeometryDrawCircle));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_ELLIPSE, ACTION_GROUP_GEOMETRY, "Draw ellipse", "", "", &Action::onGeometryDrawEllipse));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_TETRAHEDRON, ACTION_GROUP_GEOMETRY, "Draw tetrahedron", "", "", &Action::onGeometryDrawTetrahedron));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_HEXAHEDRON, ACTION_GROUP_GEOMETRY, "Draw hexahedron", "", "", &Action::onGeometryDrawHexahedron));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_CYLINDER, ACTION_GROUP_GEOMETRY, "Draw cylinder", "", "", &Action::onGeometryDrawCylinder));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DRAW_SPHERE, ACTION_GROUP_GEOMETRY, "Draw sphere", "", "", &Action::onGeometryDrawSphere));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_CREATE_ELEMENT, ACTION_GROUP_GEOMETRY, "Create element", "Ctrl+Alt+E", "", &Action::onGeometryCreateElement));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_FIX_SLIVER_ELEMENTS, ACTION_GROUP_GEOMETRY, "Fix sliver elements", "", "", &Action::onGeometryFixSliverElements));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_FIND_INTERSECTED_ELEMENTS, ACTION_GROUP_GEOMETRY, "Find intersected elements", "Ctrl+I", "", &Action::onGeometryFindIntersectedElements));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_BREAK_INTERSECTED_ELEMENTS, ACTION_GROUP_GEOMETRY, "Break intersected elements", "Ctrl+J", "", &Action::onGeometryBreakIntersectedElements));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_BOOL_UNION, ACTION_GROUP_GEOMETRY, "Union", "", "", &Action::onGeometryBoolUnion));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_BOOL_DIFFERENCE, ACTION_GROUP_GEOMETRY, "Difference", "", "", &Action::onGeometryBoolDifference));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_BOOL_INTERSECTION, ACTION_GROUP_GEOMETRY, "Intersection", "", "", &Action::onGeometryBoolIntersection));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_MERGE_NEAR_NODES, ACTION_GROUP_GEOMETRY, "Merge near nodes", "", "", &Action::onGeometryMergeNearNodes));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_MOVE_NODE, ACTION_GROUP_GEOMETRY, "Move node", "", "", &Action::onGeometryMoveNode));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_REMOVE_NODE, ACTION_GROUP_GEOMETRY, "Remove node", "Ctrl+Shift+X", "", &Action::onGeometryRemoveNode));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_REMOVE_NODE_AND_CLOSE_HOLE, ACTION_GROUP_GEOMETRY, "Remove node and close hole", "", "", &Action::onGeometryRemoveNodeAndCloseHole));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_REMOVE_ELEMENT, ACTION_GROUP_GEOMETRY, "Remove element", "Ctrl+Alt+X", "", &Action::onGeometryRemoveElement));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_LINE_GENERATE_FROM_EDGES, ACTION_GROUP_GEOMETRY, "Generate line(s) from surface edges", "", "", &Action::onGeometryLineGenerateFromEdges));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_SURFACE_MARK, ACTION_GROUP_GEOMETRY, "Mark surface", "", "", &Action::onGeometrySurfaceMark));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_SURFACE_SWAP_ELEMENT_NORMAL, ACTION_GROUP_GEOMETRY, "Swap element normal", "", "", &Action::onGeometrySurfaceSwapElementNormal));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_SURFACE_SWAP_NORMALS, ACTION_GROUP_GEOMETRY, "Swap normals", "", "", &Action::onGeometrySurfaceSwapNormals));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_SURFACE_SYNC_NORMALS, ACTION_GROUP_GEOMETRY, "Synchronize normals", "", "", &Action::onGeometrySurfaceSyncNormals));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_SURFACE_CLOSE_HOLE, ACTION_GROUP_GEOMETRY, "Close hole", "", "", &Action::onGeometrySurfaceCloseHole));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_SURFACE_COARSEN, ACTION_GROUP_GEOMETRY, "Coarsen surface", "", "", &Action::onGeometrySurfaceCoarsen));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_VOLUME_GENERATE_TETRAHEDRA, ACTION_GROUP_GEOMETRY, "Generate tetrahedral mesh", "Ctrl+G", ":/icons/geometry/pixmaps/range-generate_volume_mesh.svg", &Action::onGeometryGenerateTetrahedra));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_VECTOR_FIELD_CREATE, ACTION_GROUP_GEOMETRY, "Create vector field", "", "", &Action::onGeometryVectorFieldCreate));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_VECTOR_FIELD_EDIT, ACTION_GROUP_GEOMETRY, "Edit vector field", "", "", &Action::onGeometryVectorFieldEdit));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_SCALAR_FIELD_CREATE, ACTION_GROUP_GEOMETRY, "Create scalar field", "", "", &Action::onGeometryScalarFieldCreate));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_SCALAR_FIELD_EDIT, ACTION_GROUP_GEOMETRY, "Edit scalar field", "", "", &Action::onGeometryScalarFieldEdit));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_STREAM_LINE_CREATE, ACTION_GROUP_GEOMETRY, "Create stream line", "", "", &Action::onGeometryStreamLineCreate));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_STREAM_LINE_EDIT, ACTION_GROUP_GEOMETRY, "Edit stream line", "", "", &Action::onGeometryStreamLineEdit));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_CUT_CREATE, ACTION_GROUP_GEOMETRY, "Create cut", "", "", &Action::onGeometryCutCreate));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_CUT_EDIT, ACTION_GROUP_GEOMETRY, "Edit cut", "", "", &Action::onGeometryCutEdit));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_ISO_CREATE, ACTION_GROUP_GEOMETRY, "Create iso", "", "", &Action::onGeometryIsoCreate));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_ISO_EDIT, ACTION_GROUP_GEOMETRY, "Edit iso", "", "", &Action::onGeometryIsoEdit));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_ENTITY_MERGE, ACTION_GROUP_GEOMETRY, "Merge selected entities", "Ctrl+M", "", &Action::onGeometryEntityMerge));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_ENTITY_REMOVE, ACTION_GROUP_GEOMETRY, "Remove selected entities", "Ctrl+X", "", &Action::onGeometryEntityRemove));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_TRANSFORM, ACTION_GROUP_GEOMETRY, "Scale, translate, rotate", "Ctrl+T", ":/icons/geometry/pixmaps/range-geometry_transform.svg", &Action::onGeometryTransform));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DEV_EXPORT_INTERSECTED_ELEMENTS, ACTION_GROUP_GEOMETRY, "Export intersected elements", "", "", &Action::onGeometryDevExportIntersectedElements));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DEV_PURGE_UNUSED_NODES, ACTION_GROUP_GEOMETRY, "Purge unused nodes", "", "", &Action::onGeometryDevPurgeUnusedNodes));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DEV_PURGE_UNUSED_ELEMENTS, ACTION_GROUP_GEOMETRY, "Purge unused elements", "", "", &Action::onGeometryDevPurgeUnusedElements));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DEV_POINT_INSIDE_SURFACE, ACTION_GROUP_GEOMETRY, "Check if point is inside surface", "", "", &Action::onGeometryDevPointInsideSurface));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DEV_TETRAHEDRALIZE_SURFACE, ACTION_GROUP_GEOMETRY, "Tetrahedralize surface", "", "", &Action::onGeometryDevTetrahedralizeeSurface));
    actionDesc.push_back(ActionDefinitionItem(ACTION_GEOMETRY_DEV_CONSOLIDATE, ACTION_GROUP_GEOMETRY, "Consolidate geometry", "", "", &Action::onGeometryDevConsolidate));
    actionDesc.push_back(ActionDefinitionItem(ACTION_PROBLEM_TASK_FLOW, ACTION_GROUP_PROBLEM, "Problem(s) task flow", "Ctrl+P", ":/icons/file/pixmaps/range-list.svg", &Action::onProblemTaskFlow));
    actionDesc.push_back(ActionDefinitionItem(ACTION_PROBLEM_SOLVER_SETUP, ACTION_GROUP_PROBLEM, "Setup Problem(s) matrix solver", "", "", &Action::onProblemSolverSetup));
    actionDesc.push_back(ActionDefinitionItem(ACTION_PROBLEM_DEFINE_MONITORING_POINTS, ACTION_GROUP_PROBLEM, "Define monitoring points", "", "", &Action::onProblemDefineMonitoringPoints));
    actionDesc.push_back(ActionDefinitionItem(ACTION_PROBLEM_RESET, ACTION_GROUP_PROBLEM, "Reset setup", "", "", &Action::onProblemReset));
    actionDesc.push_back(ActionDefinitionItem(ACTION_SOLVER_START, ACTION_GROUP_SOLVER, "Start solver", "Ctrl+R", ":/icons/solver/pixmaps/range-solver-start.svg", &Action::onSolverStart));
    actionDesc.push_back(ActionDefinitionItem(ACTION_SOLVER_STOP, ACTION_GROUP_SOLVER, "Stop solver", "Ctrl+E", ":/icons/solver/pixmaps/range-solver-stop.svg", &Action::onSolverStop));
    actionDesc.push_back(ActionDefinitionItem(ACTION_SOLVER_KILL, ACTION_GROUP_SOLVER, "Kill solver", "Ctrl+K", ":/icons/solver/pixmaps/range-solver-kill.svg", &Action::onSolverKill));
    actionDesc.push_back(ActionDefinitionItem(ACTION_SOLVER_VIEW, ACTION_GROUP_SOLVER, "View solver progress", "", ":/icons/solver/pixmaps/range-solver-view.svg", &Action::onSolverView));
    actionDesc.push_back(ActionDefinitionItem(ACTION_SOLVER_QUEUE, ACTION_GROUP_SOLVER, "Show queued solvers", "", ":/icons/solver/pixmaps/range-solver-queue.svg", &Action::onSolverQueue));
    actionDesc.push_back(ActionDefinitionItem(ACTION_REPORT_MODEL_STATISTICS, ACTION_GROUP_REPORT, "Model statistics", "", "", &Action::onReportModelStatistics));
    actionDesc.push_back(ActionDefinitionItem(ACTION_REPORT_SOLVER_LOG, ACTION_GROUP_REPORT, "Solver log file", "", "", &Action::onReportSolverLog));
    actionDesc.push_back(ActionDefinitionItem(ACTION_REPORT_CONVERGENCE_GRAPH, ACTION_GROUP_REPORT, "Solver convergence", "Ctrl+Alt+C", ":/icons/report/pixmaps/range-report-convergence.svg", &Action::onReportConvergenceGraph));
    actionDesc.push_back(ActionDefinitionItem(ACTION_REPORT_MONITORING_POINT_GRAPH, ACTION_GROUP_REPORT, "Monitoring points", "", "", &Action::onReportMonitoringPointGraph));
    actionDesc.push_back(ActionDefinitionItem(ACTION_REPORT_CREATE_REPORT, ACTION_GROUP_REPORT, "Create report", "Ctrl+Shift+R", ":/icons/report/pixmaps/range-report.svg", &Action::onReportCreateReport));
    actionDesc.push_back(ActionDefinitionItem(ACTION_APPLICATION_SETTINGS, ACTION_GROUP_APPLICATION, "Application settings", "Ctrl+A", ":/icons/file/pixmaps/range-preferences.svg", &Action::onApplicationSettings));
    actionDesc.push_back(ActionDefinitionItem(ACTION_APPLICATION_MODULE_LICENSES, ACTION_GROUP_APPLICATION, "Module licenses", "Ctrl+L", ":/icons/file/pixmaps/range-keys.svg", &Action::onApplicationModuleLicenses));
    actionDesc.push_back(ActionDefinitionItem(ACTION_APPLICATION_UPDATE, ACTION_GROUP_APPLICATION, "Update", "", ":/icons/file/pixmaps/range-download.svg", &Action::onUpdate));
    actionDesc.push_back(ActionDefinitionItem(ACTION_APPLICATION_ABOUT, ACTION_GROUP_APPLICATION, "About", "F1", ":/icons/logos/pixmaps/range-logo-128.png", &Action::onAbout));
    actionDesc.push_back(ActionDefinitionItem(ACTION_APPLICATION_ABOUT_QT, ACTION_GROUP_APPLICATION, "About Qt", "", ":/icons/file/pixmaps/range-qt.svg", &Action::onAboutQt));
    actionDesc.push_back(ActionDefinitionItem(ACTION_APPLICATION_LICENSE, ACTION_GROUP_APPLICATION, "License", "", ":/icons/logos/pixmaps/range-logo-128.png", &Action::onLicense));
    actionDesc.push_back(ActionDefinitionItem(ACTION_APPLICATION_RELEASE_NOTES, ACTION_GROUP_APPLICATION, "Release notes", "", ":/icons/logos/pixmaps/range-logo-128.png", &Action::onReleaseNotes));
    actionDesc.push_back(ActionDefinitionItem(ACTION_APPLICATION_QUIT, ACTION_GROUP_APPLICATION, "Quit", "Q", ":/icons/file/pixmaps/range-quit.svg", &Action::onQuit));
    actionDesc.push_back(ActionDefinitionItem(ACTION_SEPARATOR, ACTION_GROUP_NONE, "", "", "", 0));
}
