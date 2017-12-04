/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   action.h                                                 *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   8-th August 2013                                         *
 *                                                                   *
 *  DESCRIPTION: Action class declaration                            *
 *********************************************************************/

#ifndef __ACTION_H__
#define __ACTION_H__

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
        explicit Action(ActionType type, MainWindow *mainWindow, QObject *parent = 0);

    protected slots:

        //! Set action enabled.
        void enable(void);

        //! Set action disabled.
        void disable(void);

    public slots:

        //! New session.
        void onSessionNew(void);

        //! Open session.
        void onSessionOpen(void);

        //! Save session.
        void onSessionSave(void);

        //! Save session as.
        void onSessionSaveAs(void);

        //! Close session.
        void onSessionClose(void);

        //! New model.
        void onModelNew(void);

        //! Open model.
        void onModelOpen(void);

        //! Save model.
        void onModelSave(void);

        //! Save model as.
        void onModelSaveAs(void);

        //! Import MSH model.
        void onModelImportMsh(void);

        //! Import RAW model.
        void onModelImportRaw(void);

        //! Import STL model.
        void onModelImportStl(void);

        //! Export MSH model.
        void onModelExportMsh(void);

        //! Export RAW model.
        void onModelExportRaw(void);

        //! Export STL model (ascii).
        void onModelExportStlAscii(void);

        //! Export STL model (binary).
        void onModelExportStlBinary(void);

        //! Close model.
        void onModelClose(void);

        //! Reload results.
        void onModelReloadResults(void);

        //! Drop results.
        void onModelDropResults(void);

        //! New material.
        void onMaterialNew(void);

        //! Import material.
        void onMaterialImport(void);

        //! Delete material.
        void onMaterialDelete(void);

        //! Geometry undo.
        void onGeometryUndo(void);

        //! Geometry redo.
        void onGeometryRedo(void);

        //! Drav point.
        void onGeometryDrawPoint(void);

        //! Drav line.
        void onGeometryDrawLine(void);

        //! Drav triangle.
        void onGeometryDrawTriangle(void);

        //! Drav quadrilateral.
        void onGeometryDrawQuadrilateral(void);

        //! Drav circle.
        void onGeometryDrawCircle(void);

        //! Drav ellipse.
        void onGeometryDrawEllipse(void);

        //! Drav tetrahedron.
        void onGeometryDrawTetrahedron(void);

        //! Drav hexahedron.
        void onGeometryDrawHexahedron(void);

        //! Drav cylinder.
        void onGeometryDrawCylinder(void);

        //! Drav sphere.
        void onGeometryDrawSphere(void);

        //! Create element.
        void onGeometryCreateElement(void);

        //! Fix sliver elements.
        void onGeometryFixSliverElements(void);

        //! Find intersected elements.
        void onGeometryFindIntersectedElements(void);

        //! Break intersected elements.
        void onGeometryBreakIntersectedElements(void);

        //! Create union.
        void onGeometryBoolUnion(void);

        //! Create difference.
        void onGeometryBoolDifference(void);

        //! Create intersection.
        void onGeometryBoolIntersection(void);

        //! Merge near nodes.
        void onGeometryMergeNearNodes(void);

        //! Move node.
        void onGeometryMoveNode(void);

        //! Remove node.
        void onGeometryRemoveNode(void);

        //! Remove node and close hole.
        void onGeometryRemoveNodeAndCloseHole(void);

        //! Remove element.
        void onGeometryRemoveElement(void);

        //! Generate line from edges.
        void onGeometryLineGenerateFromEdges(void);

        //! Mark surfaces.
        void onGeometrySurfaceMark(void);

        //! Swap surface element normal.
        void onGeometrySurfaceSwapElementNormal(void);

        //! Swap surface normals.
        void onGeometrySurfaceSwapNormals(void);

        //! Synchronize surface normals.
        void onGeometrySurfaceSyncNormals(void);

        //! Close hole in surface.
        void onGeometrySurfaceCloseHole(void);

        //! Coarsen surface elements.
        void onGeometrySurfaceCoarsen(void);

        //! Create vector field.
        void onGeometryVectorFieldCreate(void);

        //! Edit vector field.
        void onGeometryVectorFieldEdit(void);

        //! Create scalar field.
        void onGeometryScalarFieldCreate(void);

        //! Edit scalar field.
        void onGeometryScalarFieldEdit(void);

        //! Create stream line.
        void onGeometryStreamLineCreate(void);

        //! Edit stream line.
        void onGeometryStreamLineEdit(void);

        //! Create cut.
        void onGeometryCutCreate(void);

        //! Edit cut.
        void onGeometryCutEdit(void);

        //! Create iso.
        void onGeometryIsoCreate(void);

        //! Edit iso.
        void onGeometryIsoEdit(void);

        //! Merge entities.
        void onGeometryEntityMerge(void);

        //! Remove entity.
        void onGeometryEntityRemove(void);

        //! Generate mesh.
        void onGeometryGenerateTetrahedra (void);

        //! Transform geometry.
        void onGeometryTransform(void);

        //! Export intersected elements.
        void onGeometryDevExportIntersectedElements(void);

        //! Purge unused nodes.
        void onGeometryDevPurgeUnusedNodes(void);

        //! Purge unused elements.
        void onGeometryDevPurgeUnusedElements(void);

        //! Check if point is inside/behind surface.
        void onGeometryDevPointInsideSurface(void);

        //! Tetrahedralize surface.
        void onGeometryDevTetrahedralizeeSurface(void);

        //! Consolidate.
        void onGeometryDevConsolidate(void);

        //! Problem(s) task flow.
        void onProblemTaskFlow(void);

        //! Setup problem matrix solver solver.
        void onProblemSolverSetup(void);

        //! Define monitoring points.
        void onProblemDefineMonitoringPoints(void);

        //! Reset problem.
        void onProblemReset(void);

        //! Start solver.
        void onSolverStart(void);

        //! Stop solver.
        void onSolverStop(void);

        //! Kill solver.
        void onSolverKill(void);

        //! View solver.
        void onSolverView(void);

        //! Produce model statistics.
        void onReportModelStatistics(void);

        //! Show solver log file.
        void onReportSolverLog(void);

        //! Convergence graph.
        void onReportConvergenceGraph(void);

        //! Monitoring point graph.
        void onReportMonitoringPointGraph(void);

        //! Create report.
        void onReportCreateReport(void);

        //! Help center.
        void onHelp(void);

        //! Quit program.
        void onQuit(void);

        //! Application settings.
        void onApplicationSettings(void);

        //! Module licenses.
        void onApplicationModuleLicenses(void);

        //! Check for update.
        void onUpdate(void);

        //! About.
        void onAbout(void);

        //! About Qt.
        void onAboutQt(void);

        //! License.
        void onLicense(void);

        //! Release notes.
        void onReleaseNotes(void);

};

#endif // __ACTION_H__
