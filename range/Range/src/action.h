/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   action.h                                                 *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   8-th August 2013                                         *
 *                                                                   *
 *  DESCRIPTION: Action class declaration                            *
 *********************************************************************/

#ifndef ACTION_H
#define ACTION_H

#include <QAction>

#include "action_definition.h"

class MainWindow;

class Action : public QAction
{

    Q_OBJECT

    protected:

        //! Action type.
        ActionType type;
        //! Pointer to main window.
        MainWindow *mainWindow;

    public:

        //! Constructor.
        explicit Action(ActionType type, MainWindow *mainWindow, QObject *parent = nullptr);

    protected slots:

        //! Set action enabled.
        void enable();

        //! Set action disabled.
        void disable();

    public slots:

        //! New session.
        void onSessionNew();

        //! Open session.
        void onSessionOpen();

        //! Save session.
        void onSessionSave();

        //! Save session as.
        void onSessionSaveAs();

        //! Close session.
        void onSessionClose();

        //! New model.
        void onModelNew();

        //! Open model.
        void onModelOpen();

        //! Save model.
        void onModelSave();

        //! Save model as.
        void onModelSaveAs();

        //! Export MSH model.
        void onModelExportMsh();

        //! Export RAW model.
        void onModelExportRaw();

        //! Export STL model (ascii).
        void onModelExportStlAscii();

        //! Export STL model (binary).
        void onModelExportStlBinary();

        //! Close model.
        void onModelClose();

        //! Reload results.
        void onModelReloadResults();

        //! Rename model.
        void onModelRename();

        //! Drop results.
        void onModelDropResults();

        //! New material.
        void onMaterialNew();

        //! Import material.
        void onMaterialImport();

        //! Delete material.
        void onMaterialDelete();

        //! Geometry undo.
        void onGeometryUndo();

        //! Geometry redo.
        void onGeometryRedo();

        //! Drav point.
        void onGeometryDrawPoint();

        //! Drav line.
        void onGeometryDrawLine();

        //! Drav triangle.
        void onGeometryDrawTriangle();

        //! Drav quadrilateral.
        void onGeometryDrawQuadrilateral();

        //! Drav circle.
        void onGeometryDrawCircle();

        //! Drav ellipse.
        void onGeometryDrawEllipse();

        //! Drav tetrahedron.
        void onGeometryDrawTetrahedron();

        //! Drav hexahedron.
        void onGeometryDrawHexahedron();

        //! Drav cylinder.
        void onGeometryDrawCylinder();

        //! Drav sphere.
        void onGeometryDrawSphere();

        //! Drav RAW object.
        void onGeometryDrawRaw();

        //! Create element.
        void onGeometryCreateElement();

        //! Find sliver elements.
        void onGeometryFindSliverElements();

        //! Fix sliver elements.
        void onGeometryFixSliverElements();

        //! Find intersected elements.
        void onGeometryFindIntersectedElements();

        //! Break intersected elements.
        void onGeometryBreakIntersectedElements();

        //! Create union.
        void onGeometryBoolUnion();

        //! Create difference.
        void onGeometryBoolDifference();

        //! Create intersection.
        void onGeometryBoolIntersection();

        //! Merge near nodes.
        void onGeometryMergeNearNodes();

        //! Move node.
        void onGeometryMoveNode();

        //! Remove node.
        void onGeometryRemoveNode();

        //! Remove node and close hole.
        void onGeometryRemoveNodeAndCloseHole();

        //! Remove element.
        void onGeometryRemoveElement();

        //! Generate line from edges.
        void onGeometryLineGenerateFromEdges();

        //! Mark surfaces.
        void onGeometrySurfaceMark();

        //! Swap surface element normal.
        void onGeometrySurfaceSwapElementNormal();

        //! Swap surface normals.
        void onGeometrySurfaceSwapNormals();

        //! Synchronize surface normals.
        void onGeometrySurfaceSyncNormals();

        //! Close hole in surface.
        void onGeometrySurfaceCloseHole();

        //! Coarsen surface elements.
        void onGeometrySurfaceCoarsen();

        //! Create vector field.
        void onGeometryVectorFieldCreate();

        //! Edit vector field.
        void onGeometryVectorFieldEdit();

        //! Create scalar field.
        void onGeometryScalarFieldCreate();

        //! Edit scalar field.
        void onGeometryScalarFieldEdit();

        //! Create stream line.
        void onGeometryStreamLineCreate();

        //! Edit stream line.
        void onGeometryStreamLineEdit();

        //! Create cut.
        void onGeometryCutCreate();

        //! Edit cut.
        void onGeometryCutEdit();

        //! Create iso.
        void onGeometryIsoCreate();

        //! Edit iso.
        void onGeometryIsoEdit();

        //! Merge entities.
        void onGeometryEntityMerge();

        //! Remove entity.
        void onGeometryEntityRemove();

        //! Generate mesh.
        void onGeometryGenerateTetrahedra ();

        //! Transform geometry.
        void onGeometryTransform();

        //! Export sliver elements.
        void onGeometryDevExportSliverElements();

        //! Export intersected elements.
        void onGeometryDevExportIntersectedElements();

        //! Purge unused nodes.
        void onGeometryDevPurgeUnusedNodes();

        //! Purge unused elements.
        void onGeometryDevPurgeUnusedElements();

        //! Merge duplicate nodes.
        void onGeometryDevRemoveDuplicateNodes();

        //! Merge duplicate elements.
        void onGeometryDevRemoveDuplicateElements();

        //! Check if point is inside/behind surface.
        void onGeometryDevPointInsideSurface();

        //! Tetrahedralize surface.
        void onGeometryDevTetrahedralizeeSurface();

        //! Consolidate.
        void onGeometryDevConsolidate();

        //! Problem(s) task flow.
        void onProblemTaskFlow();

        //! Setup problem matrix solver solver.
        void onProblemSolverSetup();

        //! Define monitoring points.
        void onProblemDefineMonitoringPoints();

        //! Reset problem.
        void onProblemReset();

        //! Start solver.
        void onSolverStart();

        //! Stop solver.
        void onSolverStop();

        //! Kill solver.
        void onSolverKill();

        //! Produce model statistics.
        void onReportModelStatistics();

        //! Show solver log file.
        void onReportSolverLog();

        //! Convergence graph.
        void onReportConvergenceGraph();

        //! Monitoring point graph.
        void onReportMonitoringPointGraph();

        //! Create report.
        void onReportCreateReport();

        //! Help center.
        void onHelp();

        //! Quit program.
        void onQuit();

        //! Application settings.
        void onApplicationSettings();

        //! Check for update.
        void onUpdate();

        //! About.
        void onAbout();

        //! About Qt.
        void onAboutQt();

        //! License.
        void onLicense();

        //! Release notes.
        void onReleaseNotes();

};

#endif // ACTION_H
