/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   main_window.cpp                                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   28-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Main window class definition                        *
 *********************************************************************/

#include <QtGui/QCloseEvent>
#include <QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QGridLayout>
#include <QStatusBar>
#include <QSplitter>
#include <QScrollBar>
#include <QMessageBox>

#include "logger.h"
#include "progress.h"
#include "bc_manager_tree.h"
#include "bc_tree.h"
#include "document_tree.h"
#include "draw_input_widget.h"
#include "first_run_dialog.h"
#include "ic_manager_tree.h"
#include "ic_tree.h"
#include "ec_manager_tree.h"
#include "ec_tree.h"
#include "material_manager_tree.h"
#include "material_tree.h"
#include "model_entity_display_properties_tree.h"
#include "model_entity_geometric_properties_tree.h"
#include "model_records_selector.h"
#include "problem_tree.h"
#include "problem_task_dialog.h"
#include "solver_manager.h"
#include "results_variable_selector.h"
#include "results_variable_tree.h"
#include "job_manager.h"
#include "main_window.h"
#include "quit_dialog.h"
#include "session.h"
#include "geometry_transform_widget.h"

MainWindow::MainWindow (QWidget *parent)
    : QMainWindow(parent)
    , isFirstRun(true)
{
    if (this->objectName().isEmpty())
    {
        this->setObjectName(QString::fromUtf8("MainWindow"));
    }

    this->actionList = new ActionList (this);

    this->readSettings();

    this->setWindowTitle(QApplication::translate("MainWindow", "Range :: Finite Element Analysis"));
    this->setWindowIcon(QIcon(":/icons/logos/pixmaps/range-logo-128.png"));

    this->setDockNestingEnabled(true);

    int toolbarIconSize = MainSettings::getInstance().getApplicationSettings()->getToolbarIconSize();
    this->setIconSize(QSize(toolbarIconSize,toolbarIconSize));
    this->setToolButtonStyle(Qt::ToolButtonIconOnly);

    this->createMenus();
    this->createToolBars();
    this->createStatusBar();
    this->createDownloadBar();
    this->createCentralWidget();
    this->createModelDock();
    this->createDocumentDock();
    this->createRecordsDock();
    this->createProblemDock();
    this->createBcDock();
    this->createIcDock();
    this->createEcDock();
    this->createMaterialDock();
    this->createResultsDock();

    this->setTabPosition(Qt::LeftDockWidgetArea,QTabWidget::West);
    this->setTabPosition(Qt::RightDockWidgetArea,QTabWidget::East);

    this->tabifyDockWidget(this->dockModel,this->dockRecords);
    this->tabifyDockWidget(this->dockRecords,this->dockDocuments);
    this->dockModel->raise();

    this->tabifyDockWidget(this->dockProblem,this->dockBc);
    this->tabifyDockWidget(this->dockBc,this->dockIc);
    this->tabifyDockWidget(this->dockIc,this->dockEc);
    this->tabifyDockWidget(this->dockEc,this->dockMaterial);
    this->tabifyDockWidget(this->dockMaterial,this->dockResults);
    this->dockProblem->raise();

    this->restoreGeometry(MainSettings::getInstance().value("mainWindow/geometry").toByteArray());
    this->restoreState(MainSettings::getInstance().value("mainWindow/windowState").toByteArray());

    // Actions signals
    QObject::connect(MainSettings::getInstance().getApplicationSettings(),&ApplicationSettings::shortcutChanged,this->actionList,&ActionList::changeShortcut);

    // Tooplbar icon size changed signal
    QObject::connect(MainSettings::getInstance().getApplicationSettings(),&ApplicationSettings::toolbarIconSizeChanged,this,&MainWindow::onToolbarIconSizeChanged);

    // Main progress signals
    QObject::connect(&Progress::getInstance(),&Progress::progress,this,&MainWindow::onMainProgress);
    QObject::connect(&Progress::getInstance(),&Progress::progressInitialize,this,&MainWindow::onMainProgressInitialize);
    QObject::connect(&Progress::getInstance(),&Progress::progressFinalize,this,&MainWindow::onMainProgressFinalize);

    // Download progress signals
    QObject::connect(Session::getInstance().getDownloadManager(),&DownloadManager::progress,this,&MainWindow::onDownloadProgress);
    QObject::connect(Session::getInstance().getDownloadManager(),&DownloadManager::started,this,&MainWindow::onDownloadProgressInitialize);
    QObject::connect(Session::getInstance().getDownloadManager(),&DownloadManager::finished,this,&MainWindow::onDownloadProgressFinalize);

    // Solver manager signals.
    QObject::connect(&SolverManager::getInstance(),&SolverManager::jobBlocking,this,&MainWindow::onJobBlocking);

    // Draw engine signals.
    QObject::connect(Session::getInstance().getDrawEngine(),&DrawEngine::objectAdded,this,&MainWindow::onDrawObjectAdded);
    QObject::connect(Session::getInstance().getDrawEngine(),&DrawEngine::objectRemoved,this,&MainWindow::onDrawObjectRemoved);
    QObject::connect(Session::getInstance().getDrawEngine(),&DrawEngine::objectsRemoved,this,&MainWindow::onDrawObjectRemoved);

    // Job manager signals
    QObject::connect(&JobManager::getInstance(),&JobManager::jobBlocking,this,&MainWindow::onJobBlocking);
    QObject::connect(&JobManager::getInstance(),&JobManager::jobStarted,this,&MainWindow::onJobStarted);
    QObject::connect(&JobManager::getInstance(),&JobManager::jobFinished,this,&MainWindow::onJobEnded);

    // Connect model modification signals
    QObject::connect(&Session::getInstance(),&Session::modelAdded,this,&MainWindow::onModelAdded);
    QObject::connect(&Session::getInstance(),&Session::modelRemoved,this,&MainWindow::onModelRemoved);
    QObject::connect(&Session::getInstance(),&Session::modelChanged,this,&MainWindow::onModelChanged);

    // Connect problem modification signals
    QObject::connect(&Session::getInstance(),&Session::problemChanged,this,&MainWindow::onProblemChanged);

    // Connect results modification signals
    QObject::connect(&Session::getInstance(),&Session::resultsChanged,this,&MainWindow::onResultsChanged);

    // Model selection signals
    QObject::connect(&Session::getInstance(),&Session::entityVisibilityChanged,this,&MainWindow::onEntityVisibilityChanged);

    // Number of history records has changed.
    QObject::connect(&Session::getInstance(),&Session::nHistoryRecordsChanged,this,&MainWindow::onNHistoryRecordsChanged);

    if (this->isFirstRun)
    {
        FirstRunDialog(this).exec();
    }
}

MainWindow *MainWindow::getInstance(void)
{
    static MainWindow *mainWindow = new MainWindow;
    return mainWindow;
}

void MainWindow::showTransformGeometryWidget(void)
{
    QString dockName = QString::fromUtf8("dockTransform");
    GeometryTransformWidget *transformGeometryWidget = nullptr;

    if (!findCustomDockWidget(dockName))
    {
        transformGeometryWidget = new GeometryTransformWidget;

        QObject::connect(transformGeometryWidget,
                         &GeometryTransformWidget::finalize,
                         this,
                         &MainWindow::onGeometryTransformFinalize);
    }
    this->showCustomDockWidget(transformGeometryWidget,dockName,tr("Transform geometry"));
}

void MainWindow::hideTransformGeometryWidget(void)
{
    this->hideCustomDockWidget(QString::fromUtf8("dockTransform"));
}

void MainWindow::showDrawWidget(void)
{
    QString dockName = QString::fromUtf8("dockDraw");
    this->showCustomDockWidget((findCustomDockWidget(dockName) ? nullptr : new DrawInputWidget),dockName,tr("Draw object"));
}

void MainWindow::hideDrawWidget(void)
{
    this->hideCustomDockWidget(QString::fromUtf8("dockDraw"));
}

void MainWindow::progressAutoHideEnable(void)
{
    this->mainProgressBar->setAutoHide(true);
}

void MainWindow::progressAutoHideDisable(void)
{
    this->mainProgressBar->setAutoHide(false);
}

QDockWidget *MainWindow::findCustomDockWidget(const QString &name)
{
    return this->findChild<QDockWidget*>(name);
}

void MainWindow::showCustomDockWidget(QWidget *widget, const QString &name, const QString &title)
{
    QDockWidget *dockWidget = this->findCustomDockWidget(name);

    if (!dockWidget)
    {
        QWidget *layoutWidget;
        QGridLayout *gridLayout;

        layoutWidget = new QWidget();

        gridLayout = new QGridLayout(layoutWidget);

        gridLayout->addWidget(widget);

        dockWidget = new QDockWidget(this);
        dockWidget->setObjectName(name);
        dockWidget->setWindowTitle(title);
        dockWidget->setWidget(layoutWidget);

        this->addDockWidget(Qt::RightDockWidgetArea, dockWidget);

        QDockWidget *firstDockWidget = this->findFirstDockWidget(Qt::RightDockWidgetArea);
        if (firstDockWidget)
        {
            this->tabifyDockWidget(firstDockWidget,dockWidget);
        }
    }

    dockWidget->show();
    dockWidget->raise();
}

void MainWindow::hideCustomDockWidget(const QString &name)
{
    QDockWidget *dockWidget = this->findCustomDockWidget(name);
    if (dockWidget)
    {
        delete dockWidget;
    }
}

void MainWindow::createMenus(void)
{
    QMenuBar *menubar = new QMenuBar(this);
    this->setMenuBar(menubar);

    QMenu *menuFile = new QMenu(menubar);
    menuFile->setTitle(QApplication::translate("MainWindow", "File"));
    QMenu *menuFileSession = new QMenu(menuFile);
    menuFileSession->setTitle(QApplication::translate("MainWindow", "Session"));
    menuFileSession->setIcon(QIcon(":/icons/file/pixmaps/range-session.svg"));
    menuFileSession->addAction(this->actionList->getAction(ACTION_SESSION_NEW));
    menuFileSession->addAction(this->actionList->getAction(ACTION_SESSION_OPEN));
    menuFileSession->addAction(this->actionList->getAction(ACTION_SESSION_SAVE));
    menuFileSession->addAction(this->actionList->getAction(ACTION_SESSION_SAVE_AS));
    menuFileSession->addAction(this->actionList->getAction(ACTION_SESSION_CLOSE));
    menuFile->addAction(menuFileSession->menuAction());
    menuFile->addAction(this->actionList->getAction(ACTION_MODEL_NEW));
    menuFile->addAction(this->actionList->getAction(ACTION_MODEL_OPEN));
    menuFile->addAction(this->actionList->getAction(ACTION_MODEL_SAVE));
    menuFile->addAction(this->actionList->getAction(ACTION_MODEL_SAVE_AS));
    QMenu *menuFileExport = new QMenu(menuFile);
    menuFileExport->setTitle(QApplication::translate("MainWindow", "Export"));
    menuFileExport->setIcon(QIcon(":/icons/file/pixmaps/range-model_save_as.svg"));
    menuFileExport->addAction(this->actionList->getAction(ACTION_MODEL_EXPORT_MSH));
    menuFileExport->addAction(this->actionList->getAction(ACTION_MODEL_EXPORT_RAW));
    menuFileExport->addAction(this->actionList->getAction(ACTION_MODEL_EXPORT_STL_ASCII));
    menuFileExport->addAction(this->actionList->getAction(ACTION_MODEL_EXPORT_STL_BINARY));
    menuFile->addAction(menuFileExport->menuAction());
    menuFile->addAction(this->actionList->getAction(ACTION_MODEL_CLOSE));
    menuFile->addAction(this->actionList->getAction(ACTION_SEPARATOR));
    menuFile->addAction(this->actionList->getAction(ACTION_MODEL_RELOAD_RESULTS));
    menuFile->addAction(this->actionList->getAction(ACTION_MODEL_DROP_RESULTS));
    menuFile->addAction(this->actionList->getAction(ACTION_SEPARATOR));
    menuFile->addAction(this->actionList->getAction(ACTION_MODEL_RENAME));
    menuFile->addAction(this->actionList->getAction(ACTION_SEPARATOR));
    menuFile->addAction(this->actionList->getAction(ACTION_APPLICATION_QUIT));
    menubar->addAction(menuFile->menuAction());

    QMenu *menuGeometry = new QMenu(menubar);
    menuGeometry->setTitle(QApplication::translate("MainWindow", "Geometry"));
    menuGeometry->addAction(this->actionList->getAction(ACTION_GEOMETRY_UNDO));
    menuGeometry->addAction(this->actionList->getAction(ACTION_GEOMETRY_REDO));
    menuGeometry->addAction(this->actionList->getAction(ACTION_SEPARATOR));
    QMenu *menuGeometryDraw = new QMenu(menuGeometry);
    menuGeometryDraw->setTitle(QApplication::translate("MainWindow", "Draw"));
    menuGeometryDraw->setIcon(QIcon(":/icons/geometry/pixmaps/range-draw_geometry.svg"));
    menuGeometryDraw->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_POINT));
    menuGeometryDraw->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_LINE));
    menuGeometryDraw->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_TRIANGLE));
    menuGeometryDraw->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_QUADRILATERAL));
    menuGeometryDraw->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_CIRCLE));
    menuGeometryDraw->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_ELLIPSE));
    menuGeometryDraw->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_TETRAHEDRON));
    menuGeometryDraw->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_HEXAHEDRON));
    menuGeometryDraw->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_CYLINDER));
    menuGeometryDraw->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_SPHERE));
    menuGeometryDraw->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_RAW));
    menuGeometry->addAction(menuGeometryDraw->menuAction());
    menuGeometry->addAction(this->actionList->getAction(ACTION_SEPARATOR));
//    QMenu *menuGeometryPoint = new QMenu(menuGeometry);
//    menuGeometryPoint->setTitle(QApplication::translate("MainWindow", "Point"));
//    menuGeometryPoint->setIcon(QIcon(":/icons/geometry/pixmaps/range-entity_point.svg"));
//    menuGeometry->addAction(menuGeometryPoint->menuAction());
    QMenu *menuGeometryLine = new QMenu(menuGeometry);
    menuGeometryLine->setTitle(QApplication::translate("MainWindow", "Line"));
    menuGeometryLine->setIcon(QIcon(":/icons/geometry/pixmaps/range-entity_line.svg"));
    menuGeometryLine->addAction(this->actionList->getAction(ACTION_GEOMETRY_LINE_GENERATE_FROM_EDGES));
    menuGeometry->addAction(menuGeometryLine->menuAction());
    QMenu *menuGeometrySurface = new QMenu(menuGeometry);
    menuGeometrySurface->setTitle(QApplication::translate("MainWindow", "Surface"));
    menuGeometrySurface->setIcon(QIcon(":/icons/geometry/pixmaps/range-entity_surface.svg"));
    menuGeometrySurface->addAction(this->actionList->getAction(ACTION_GEOMETRY_SURFACE_MARK));
    menuGeometrySurface->addAction(this->actionList->getAction(ACTION_GEOMETRY_SURFACE_SWAP_ELEMENT_NORMAL));
    menuGeometrySurface->addAction(this->actionList->getAction(ACTION_GEOMETRY_SURFACE_SWAP_NORMALS));
    menuGeometrySurface->addAction(this->actionList->getAction(ACTION_GEOMETRY_SURFACE_SYNC_NORMALS));
    menuGeometrySurface->addAction(this->actionList->getAction(ACTION_SEPARATOR));
    menuGeometrySurface->addAction(this->actionList->getAction(ACTION_GEOMETRY_SURFACE_CLOSE_HOLE));
    menuGeometrySurface->addAction(this->actionList->getAction(ACTION_GEOMETRY_SURFACE_COARSEN));
    menuGeometrySurface->addAction(this->actionList->getAction(ACTION_GEOMETRY_FIND_INTERSECTED_ELEMENTS));
    menuGeometrySurface->addAction(this->actionList->getAction(ACTION_GEOMETRY_BREAK_INTERSECTED_ELEMENTS));
    QMenu *menuGeometryBoolOperations = new QMenu(menuGeometrySurface);
    menuGeometryBoolOperations->setTitle(QApplication::translate("MainWindow", "Boolean operations"));
    menuGeometryBoolOperations->setIcon(QIcon(":/icons/geometry/pixmaps/range-entity_surface_bool_operation.svg"));
    menuGeometryBoolOperations->addAction(this->actionList->getAction(ACTION_GEOMETRY_BOOL_UNION));
    menuGeometryBoolOperations->addAction(this->actionList->getAction(ACTION_GEOMETRY_BOOL_DIFFERENCE));
    menuGeometryBoolOperations->addAction(this->actionList->getAction(ACTION_GEOMETRY_BOOL_INTERSECTION));
    menuGeometrySurface->addAction(menuGeometryBoolOperations->menuAction());
    menuGeometry->addAction(menuGeometrySurface->menuAction());
    QMenu *menuGeometryVolume = new QMenu(menuGeometry);
    menuGeometryVolume->setTitle(QApplication::translate("MainWindow", "Volume"));
    menuGeometryVolume->setIcon(QIcon(":/icons/geometry/pixmaps/range-entity_volume.svg"));
    menuGeometryVolume->addAction(this->actionList->getAction(ACTION_GEOMETRY_VOLUME_GENERATE_TETRAHEDRA));
    menuGeometry->addAction(menuGeometryVolume->menuAction());
    menuGeometry->addAction(this->actionList->getAction(ACTION_SEPARATOR));
    QMenu *menuGeometryCut = new QMenu(menuGeometry);
    menuGeometryCut->setTitle(QApplication::translate("MainWindow", "Cut"));
    menuGeometryCut->setIcon(QIcon(":/icons/geometry/pixmaps/range-entity_cut.svg"));
    menuGeometryCut->addAction(this->actionList->getAction(ACTION_GEOMETRY_CUT_CREATE));
    menuGeometryCut->addAction(this->actionList->getAction(ACTION_GEOMETRY_CUT_EDIT));
    menuGeometry->addAction(menuGeometryCut->menuAction());
    QMenu *menuGeometryIso = new QMenu(menuGeometry);
    menuGeometryIso->setTitle(QApplication::translate("MainWindow", "Iso"));
    menuGeometryIso->setIcon(QIcon(":/icons/geometry/pixmaps/range-entity_iso.svg"));
    menuGeometryIso->addAction(this->actionList->getAction(ACTION_GEOMETRY_ISO_CREATE));
    menuGeometryIso->addAction(this->actionList->getAction(ACTION_GEOMETRY_ISO_EDIT));
    menuGeometry->addAction(menuGeometryIso->menuAction());
    QMenu *menuGeometryStreamLine = new QMenu(menuGeometry);
    menuGeometryStreamLine->setTitle(QApplication::translate("MainWindow", "Stream line"));
    menuGeometryStreamLine->setIcon(QIcon(":/icons/geometry/pixmaps/range-entity_stream_line.svg"));
    menuGeometryStreamLine->addAction(this->actionList->getAction(ACTION_GEOMETRY_STREAM_LINE_CREATE));
    menuGeometryStreamLine->addAction(this->actionList->getAction(ACTION_GEOMETRY_STREAM_LINE_EDIT));
    menuGeometry->addAction(menuGeometryStreamLine->menuAction());
    QMenu *menuGeometryScalarField = new QMenu(menuGeometry);
    menuGeometryScalarField->setTitle(QApplication::translate("MainWindow", "Scalar field"));
    menuGeometryScalarField->setIcon(QIcon(":/icons/geometry/pixmaps/range-entity_scalar_field.svg"));
    menuGeometryScalarField->addAction(this->actionList->getAction(ACTION_GEOMETRY_SCALAR_FIELD_CREATE));
    menuGeometryScalarField->addAction(this->actionList->getAction(ACTION_GEOMETRY_SCALAR_FIELD_EDIT));
    menuGeometry->addAction(menuGeometryScalarField->menuAction());
    QMenu *menuGeometryVectorField = new QMenu(menuGeometry);
    menuGeometryVectorField->setTitle(QApplication::translate("MainWindow", "Vector field"));
    menuGeometryVectorField->setIcon(QIcon(":/icons/geometry/pixmaps/range-entity_vector_field.svg"));
    menuGeometryVectorField->addAction(this->actionList->getAction(ACTION_GEOMETRY_VECTOR_FIELD_CREATE));
    menuGeometryVectorField->addAction(this->actionList->getAction(ACTION_GEOMETRY_VECTOR_FIELD_EDIT));
    menuGeometry->addAction(menuGeometryVectorField->menuAction());
    menuGeometry->addAction(this->actionList->getAction(ACTION_SEPARATOR));
    menuGeometry->addAction(this->actionList->getAction(ACTION_GEOMETRY_ENTITY_MERGE));
    menuGeometry->addAction(this->actionList->getAction(ACTION_GEOMETRY_ENTITY_REMOVE));
    menuGeometry->addAction(this->actionList->getAction(ACTION_SEPARATOR));
    menuGeometry->addAction(this->actionList->getAction(ACTION_GEOMETRY_FIND_SLIVER_ELEMENTS));
    menuGeometry->addAction(this->actionList->getAction(ACTION_GEOMETRY_FIX_SLIVER_ELEMENTS));
    menuGeometry->addAction(this->actionList->getAction(ACTION_GEOMETRY_MERGE_NEAR_NODES));
    menuGeometry->addAction(this->actionList->getAction(ACTION_GEOMETRY_MOVE_NODE));
    menuGeometry->addAction(this->actionList->getAction(ACTION_GEOMETRY_REMOVE_NODE));
    menuGeometry->addAction(this->actionList->getAction(ACTION_GEOMETRY_REMOVE_NODE_AND_CLOSE_HOLE));
    menuGeometry->addAction(this->actionList->getAction(ACTION_SEPARATOR));
    menuGeometry->addAction(this->actionList->getAction(ACTION_GEOMETRY_CREATE_ELEMENT));
    menuGeometry->addAction(this->actionList->getAction(ACTION_GEOMETRY_REMOVE_ELEMENT));
    menuGeometry->addAction(this->actionList->getAction(ACTION_SEPARATOR));
    menuGeometry->addAction(this->actionList->getAction(ACTION_GEOMETRY_TRANSFORM));
    menuGeometry->addAction(this->actionList->getAction(ACTION_SEPARATOR));
    QMenu *menuGeometryDevelopement = new QMenu(menuGeometry);
    menuGeometryDevelopement->setTitle(QApplication::translate("MainWindow", "Special tools"));
    menuGeometryDevelopement->addAction(this->actionList->getAction(ACTION_GEOMETRY_DEV_EXPORT_SLIVER_ELEMENTS));
    menuGeometryDevelopement->addAction(this->actionList->getAction(ACTION_GEOMETRY_DEV_EXPORT_INTERSECTED_ELEMENTS));
    menuGeometryDevelopement->addAction(this->actionList->getAction(ACTION_GEOMETRY_DEV_PURGE_UNUSED_NODES));
    menuGeometryDevelopement->addAction(this->actionList->getAction(ACTION_GEOMETRY_DEV_PURGE_UNUSED_ELEMENTS));
    menuGeometryDevelopement->addAction(this->actionList->getAction(ACTION_GEOMETRY_DEV_REMOVE_DUPLICATE_NODES));
    menuGeometryDevelopement->addAction(this->actionList->getAction(ACTION_GEOMETRY_DEV_REMOVE_DUPLICATE_ELEMENTS));
    menuGeometryDevelopement->addAction(this->actionList->getAction(ACTION_GEOMETRY_DEV_POINT_INSIDE_SURFACE));
    menuGeometryDevelopement->addAction(this->actionList->getAction(ACTION_GEOMETRY_DEV_TETRAHEDRALIZE_SURFACE));
    menuGeometryDevelopement->addAction(this->actionList->getAction(ACTION_GEOMETRY_DEV_CONSOLIDATE));
    menuGeometry->addAction(menuGeometryDevelopement->menuAction());
    menubar->addAction(menuGeometry->menuAction());

    QMenu *menuProblem = new QMenu(menubar);
    menuProblem->setTitle(QApplication::translate("MainWindow", "Problem"));
    menuProblem->addAction(this->actionList->getAction(ACTION_PROBLEM_TASK_FLOW));
    menuProblem->addAction(this->actionList->getAction(ACTION_PROBLEM_SOLVER_SETUP));
    menuProblem->addAction(this->actionList->getAction(ACTION_PROBLEM_DEFINE_MONITORING_POINTS));
    menuProblem->addAction(this->actionList->getAction(ACTION_SEPARATOR));
    menuProblem->addAction(this->actionList->getAction(ACTION_PROBLEM_RESET));
    menubar->addAction(menuProblem->menuAction());

    QMenu *menuSolver = new QMenu(menubar);
    menuSolver->setTitle(QApplication::translate("MainWindow", "Solution"));
    menuSolver->addAction(this->actionList->getAction(ACTION_SOLVER_START));
    menuSolver->addAction(this->actionList->getAction(ACTION_SOLVER_STOP));
    menuSolver->addAction(this->actionList->getAction(ACTION_SOLVER_KILL));
    menuSolver->addAction(this->actionList->getAction(ACTION_SEPARATOR));
    menubar->addAction(menuSolver->menuAction());

    QMenu *menuReport = new QMenu(menubar);
    menuReport->setTitle(QApplication::translate("MainWindow", "Report"));
    menuReport->addAction(this->actionList->getAction(ACTION_REPORT_MODEL_STATISTICS));
    menuReport->addAction(this->actionList->getAction(ACTION_REPORT_SOLVER_LOG));
    menuReport->addAction(this->actionList->getAction(ACTION_REPORT_CONVERGENCE_GRAPH));
    menuReport->addAction(this->actionList->getAction(ACTION_REPORT_MONITORING_POINT_GRAPH));
    menuReport->addAction(this->actionList->getAction(ACTION_REPORT_CREATE_REPORT));
    menubar->addAction(menuReport->menuAction());

    QMenu *menuTools = new QMenu(menubar);
    menuTools->setTitle(QApplication::translate("MainWindow", "Tools"));
    menuTools->addAction(this->actionList->getAction(ACTION_APPLICATION_SETTINGS));
    menubar->addAction(menuTools->menuAction());

    menubar->addSeparator();

    QMenu *menuHelp = new QMenu(menubar);
    menuHelp->setTitle(QApplication::translate("MainWindow", "Help"));
    menuHelp->addAction(this->actionList->getAction(ACTION_APPLICATION_HELP));
    menuHelp->addAction(this->actionList->getAction(ACTION_APPLICATION_UPDATE));
    menuHelp->addAction(this->actionList->getAction(ACTION_APPLICATION_ABOUT));
    menuHelp->addAction(this->actionList->getAction(ACTION_APPLICATION_ABOUT_QT));
    menuHelp->addAction(this->actionList->getAction(ACTION_APPLICATION_LICENSE));
    menuHelp->addAction(this->actionList->getAction(ACTION_APPLICATION_RELEASE_NOTES));
    menubar->addAction(menuHelp->menuAction());
}

void MainWindow::createToolBars(void)
{
    QString key, actionName;

    uint nToolBars;
    uint nActions;

    nToolBars = MainSettings::getInstance().value("nToolBars", 0).toUInt();

    //! Disable toolbar restoring untill its modification is not implemented.
    nToolBars = 0;

    if (nToolBars == 0 || nToolBars != 1)
    {
        QToolBar *mainToolBar = new QToolBar(this);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setWindowTitle(tr("Main toolbar"));
        this->addToolBar(Qt::TopToolBarArea, mainToolBar);
        this->toolBars.push_back(mainToolBar);

        mainToolBar->addAction(this->actionList->getAction(ACTION_MODEL_NEW));
        mainToolBar->addAction(this->actionList->getAction(ACTION_MODEL_OPEN));
        mainToolBar->addAction(this->actionList->getAction(ACTION_MODEL_SAVE));
        mainToolBar->addAction(this->actionList->getAction(ACTION_MODEL_CLOSE));
        mainToolBar->addAction(this->actionList->getAction(ACTION_APPLICATION_QUIT));
        mainToolBar->addAction(this->actionList->getAction(ACTION_SEPARATOR));
        mainToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_UNDO));
        mainToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_REDO));
        mainToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_TRANSFORM));
        mainToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_VOLUME_GENERATE_TETRAHEDRA));
        mainToolBar->addAction(this->actionList->getAction(ACTION_SEPARATOR));
        mainToolBar->addAction(this->actionList->getAction(ACTION_PROBLEM_TASK_FLOW));
        mainToolBar->addAction(this->actionList->getAction(ACTION_SEPARATOR));
        mainToolBar->addAction(this->actionList->getAction(ACTION_SOLVER_START));
        mainToolBar->addAction(this->actionList->getAction(ACTION_SOLVER_STOP));
        mainToolBar->addAction(this->actionList->getAction(ACTION_SOLVER_KILL));
        mainToolBar->addAction(this->actionList->getAction(ACTION_SEPARATOR));
        mainToolBar->addAction(this->actionList->getAction(ACTION_REPORT_CONVERGENCE_GRAPH));
        mainToolBar->addAction(this->actionList->getAction(ACTION_REPORT_CREATE_REPORT));
        mainToolBar->addAction(this->actionList->getAction(ACTION_SEPARATOR));
        mainToolBar->addAction(this->actionList->getAction(ACTION_MODEL_RELOAD_RESULTS));

        QWidget* spacer = new QWidget();
        spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        mainToolBar->addWidget(spacer);

        mainToolBar->addAction(this->actionList->getAction(ACTION_APPLICATION_SETTINGS));
        mainToolBar->addAction(this->actionList->getAction(ACTION_SEPARATOR));
        mainToolBar->addAction(this->actionList->getAction(ACTION_APPLICATION_HELP));
        mainToolBar->addAction(this->actionList->getAction(ACTION_APPLICATION_ABOUT));

        this->addToolBarBreak();

        QToolBar *drawToolBar = new QToolBar(this);
        drawToolBar->setObjectName(QString::fromUtf8("drawToolBar"));
        drawToolBar->setWindowTitle(tr("Draw toolbar"));
        this->addToolBar(Qt::TopToolBarArea, drawToolBar);
        this->toolBars.push_back(drawToolBar);

        drawToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_POINT));
        drawToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_LINE));
        drawToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_TRIANGLE));
        drawToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_QUADRILATERAL));
        drawToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_CIRCLE));
        drawToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_ELLIPSE));
        drawToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_TETRAHEDRON));
        drawToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_HEXAHEDRON));
        drawToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_CYLINDER));
        drawToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_SPHERE));
        drawToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_DRAW_RAW));

        QToolBar *surfaceToolBar = new QToolBar(this);
        surfaceToolBar->setObjectName(QString::fromUtf8("surfaceToolBar"));
        surfaceToolBar->setWindowTitle(tr("Surface toolbar"));
        this->addToolBar(Qt::TopToolBarArea, surfaceToolBar);
        this->toolBars.push_back(surfaceToolBar);

        surfaceToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_SURFACE_MARK));
        surfaceToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_SURFACE_SWAP_ELEMENT_NORMAL));
        surfaceToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_SURFACE_SWAP_NORMALS));
        surfaceToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_SURFACE_SYNC_NORMALS));
        surfaceToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_SURFACE_CLOSE_HOLE));
        surfaceToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_SURFACE_COARSEN));
        surfaceToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_FIND_INTERSECTED_ELEMENTS));
        surfaceToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_BREAK_INTERSECTED_ELEMENTS));
        surfaceToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_FIND_SLIVER_ELEMENTS));
        surfaceToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_FIX_SLIVER_ELEMENTS));
        surfaceToolBar->addAction(this->actionList->getAction(ACTION_SEPARATOR));
        surfaceToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_BOOL_DIFFERENCE));
        surfaceToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_BOOL_INTERSECTION));
        surfaceToolBar->addAction(this->actionList->getAction(ACTION_GEOMETRY_BOOL_UNION));
    }
    else
    {
        for (uint i=0;i<nToolBars;i++)
        {
            QToolBar *toolBar = new QToolBar(this);

            key = "toolBar_" + QString::number(i+1) + "_size";
            nActions = MainSettings::getInstance().value(key, QString()).toUInt();

            key = "toolBar_" + QString::number(i+1) + "_name";
            toolBar->setObjectName(MainSettings::getInstance().value(key, QString()).toString());

            key = "toolBar_" + QString::number(i+1) + "_title";
            toolBar->setWindowTitle(MainSettings::getInstance().value(key, QString()).toString());

            this->addToolBar(Qt::TopToolBarArea, toolBar);

            for (uint j=0;j<nActions;j++)
            {
                key = "toolBar_" + QString::number(i+1) + "_action_" + QString::number(j+1);
                actionName = MainSettings::getInstance().value(key, QString()).toString();
                toolBar->addAction(this->actionList->getAction(actionName));
            }
            this->toolBars.push_back(toolBar);
        }
    }
}

void MainWindow::createStatusBar(void)
{
    QStatusBar *statusbar;

    statusbar = new QStatusBar(this);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    this->setStatusBar(statusbar);

    QWidget *progressBarWidget = new QWidget;
    statusbar->addWidget(progressBarWidget,1);

    QVBoxLayout *progressBarLayout = new QVBoxLayout;
    progressBarLayout->setSpacing(0);
    progressBarLayout->setMargin(0);
    progressBarWidget->setLayout(progressBarLayout);

    this->mainProgressBar = new ProgressBar(this);
    this->mainProgressBar->hide();
    progressBarLayout->addWidget(this->mainProgressBar);

    this->downloadProgressBar = new ProgressBar(this);
    this->downloadProgressBar->hide();
    progressBarLayout->addWidget(this->downloadProgressBar);
}

void MainWindow::createDownloadBar(void)
{

}

void MainWindow::createCentralWidget(void)
{
    this->centralTabWidget = new CentralTabWidget(this);
    this->setCentralWidget(this->centralTabWidget);
    this->modelSubWindows = new SubWindowManager(this->centralTabWidget->getMdiArea(), this);
}

void MainWindow::createModelDock(void)
{
    this->dockModel = new QDockWidget(this);
    this->dockModel->setObjectName(QString::fromUtf8("dockModel"));
    this->dockModel->setWindowTitle(QApplication::translate("MainWindow", "Model"));
    this->addDockWidget(Qt::LeftDockWidgetArea, this->dockModel);

    QWidget *layoutModels = new QWidget();
    this->dockModel->setWidget(layoutModels);

    QGridLayout *gridLayoutModels = new QGridLayout(layoutModels);

    QSplitter *splitter = new QSplitter;
    splitter->setOpaqueResize(false);
    splitter->setOrientation(Qt::Vertical);
    splitter->setChildrenCollapsible(false);
    gridLayoutModels->addWidget(splitter);

    //! Model tree.
    this->treeModelManager = new ModelTree(this->actionList,splitter);

    //! Model display properties tree.
    ModelEntityDisplayPropertiesTree *treeModelDisplayProperties = new ModelEntityDisplayPropertiesTree;
    splitter->addWidget(treeModelDisplayProperties);

    //! Model geometric properties tree.
    ModelEntityGeometricPropertiesTree *treeModelGeometricProperties = new ModelEntityGeometricPropertiesTree;
    splitter->addWidget(treeModelGeometricProperties);

    splitter->setStretchFactor(0,10);
    splitter->setStretchFactor(1,4);
    splitter->setStretchFactor(2,1);
}

void MainWindow::createProblemDock(void)
{
    QWidget *layoutProblem;
    QGridLayout *gridLayoutProblem;

    layoutProblem = new QWidget();

    gridLayoutProblem = new QGridLayout(layoutProblem);

    ProblemTree*treeProblem = new ProblemTree;
    gridLayoutProblem->addWidget(treeProblem);

    this->dockProblem = new QDockWidget(this);
    this->dockProblem->setObjectName(QString::fromUtf8("dockProblem"));
    this->dockProblem->setWindowTitle(QApplication::translate("MainWindow", "Problem setup"));
    this->dockProblem->setWidget(layoutProblem);
    this->addDockWidget(Qt::RightDockWidgetArea, this->dockProblem);
}

void MainWindow::createDocumentDock(void)
{
    QWidget *layoutDocuments;
    QGridLayout *gridLayoutDocuments;

    layoutDocuments = new QWidget();
    gridLayoutDocuments = new QGridLayout(layoutDocuments);

    DocumentTree *treeDocuments = new DocumentTree;
    gridLayoutDocuments->addWidget(treeDocuments);

    this->dockDocuments = new QDockWidget(this);
    this->dockDocuments->setObjectName(QString::fromUtf8("dockDocuments"));
    this->dockDocuments->setWindowTitle(QApplication::translate("MainWindow", "Documents"));
    this->dockDocuments->setWidget(layoutDocuments);

    this->addDockWidget(Qt::LeftDockWidgetArea, this->dockDocuments);
}

void MainWindow::createRecordsDock(void)
{
    QWidget *layoutRecords;
    QGridLayout *gridLayoutRecords;

    layoutRecords = new QWidget();
    gridLayoutRecords = new QGridLayout(layoutRecords);

    ModelRecordsSelector *treeRecords = new ModelRecordsSelector;
    gridLayoutRecords->addWidget(treeRecords);

    this->dockRecords = new QDockWidget(this);
    this->dockRecords->setObjectName(QString::fromUtf8("dockRecords"));
    this->dockRecords->setWindowTitle(QApplication::translate("MainWindow", "Model records"));
    this->dockRecords->setWidget(layoutRecords);

    this->addDockWidget(Qt::LeftDockWidgetArea, this->dockRecords);
}

void MainWindow::createBcDock(void)
{
    this->dockBc = new QDockWidget(this);
    this->dockBc->setObjectName(QString::fromUtf8("dockBcList"));
    this->dockBc->setWindowTitle(QApplication::translate("MainWindow", "Boundary conditions"));
    this->addDockWidget(Qt::RightDockWidgetArea, this->dockBc);

    QSplitter *splitter = new QSplitter(this->dockBc);
    splitter->setOrientation(Qt::Vertical);
    splitter->setChildrenCollapsible(false);
    this->dockBc->setWidget(splitter);

    BCManagerTree *treeBcList = new BCManagerTree(splitter);

    BCTree *treeBc = new BCTree(splitter);

    QObject::connect(treeBcList,
                     &BCManagerTree::bcSelected,
                     treeBc,
                     &BCTree::onBcSelected);
}

void MainWindow::createIcDock(void)
{
    this->dockIc = new QDockWidget(this);
    this->dockIc->setObjectName(QString::fromUtf8("dockIcList"));
    this->dockIc->setWindowTitle(QApplication::translate("MainWindow", "Initial conditions"));
    this->addDockWidget(Qt::RightDockWidgetArea, this->dockIc);

    QSplitter *splitter = new QSplitter(this->dockIc);
    splitter->setOrientation(Qt::Vertical);
    splitter->setChildrenCollapsible(false);
    this->dockIc->setWidget(splitter);

    ICManagerTree *treeIcList = new ICManagerTree(splitter);

    ICTree *treeIc = new ICTree(splitter);

    QObject::connect(treeIcList,
                     &ICManagerTree::icSelected,
                     treeIc,
                     &ICTree::onIcSelected);
}

void MainWindow::createEcDock(void)
{
    this->dockEc = new QDockWidget(this);
    this->dockEc->setObjectName(QString::fromUtf8("dockEcList"));
    this->dockEc->setWindowTitle(QApplication::translate("MainWindow", "Environment conditions"));
    this->addDockWidget(Qt::RightDockWidgetArea, this->dockEc);

    QSplitter *splitter = new QSplitter(this->dockEc);
    splitter->setOrientation(Qt::Vertical);
    splitter->setChildrenCollapsible(false);
    this->dockEc->setWidget(splitter);

    ECManagerTree *treeEcList = new ECManagerTree(splitter);

    ECTree *treeEc = new ECTree(splitter);

    QObject::connect(treeEcList,
                     &ECManagerTree::ecSelected,
                     treeEc,
                     &ECTree::onEcSelected);
}

void MainWindow::createMaterialDock(void)
{
    this->dockMaterial = new QDockWidget(this);
    this->dockMaterial->setObjectName(QString::fromUtf8("dockMaterialList"));
    this->dockMaterial->setWindowTitle(QApplication::translate("MainWindow", "Materials"));
    this->addDockWidget(Qt::RightDockWidgetArea, this->dockMaterial);

    QSplitter *splitter = new QSplitter(this->dockMaterial);
    splitter->setOrientation(Qt::Vertical);
    splitter->setChildrenCollapsible(false);
    this->dockMaterial->setWidget(splitter);

    MaterialManagerTree *treeMaterialList = new MaterialManagerTree(this->actionList,splitter);

    MaterialTree *treeMaterial = new MaterialTree(splitter);

    QObject::connect(treeMaterialList,
                     &MaterialManagerTree::materialSelected,
                     treeMaterial,
                     &MaterialTree::onMaterialSelected);
}

void MainWindow::createResultsDock(void)
{
    this->dockResults = new QDockWidget(this);
    this->dockResults->setObjectName(QString::fromUtf8("dockResultsList"));
    this->dockResults->setWindowTitle(QApplication::translate("MainWindow", "Results"));
    this->addDockWidget(Qt::RightDockWidgetArea, this->dockResults);

    QWidget *layoutResults = new QWidget();
    QGridLayout *gridLayout = new QGridLayout(layoutResults);

    this->dockResults->setWidget(layoutResults);

    ResultsVariableSelector *resultsComboBox = new ResultsVariableSelector;

    gridLayout->addWidget(resultsComboBox,0,0,1,1);

    ResultsVariableTree *treeResultsVariable = new ResultsVariableTree;
    gridLayout->addWidget(treeResultsVariable,1,0,1,1);

    QObject::connect(resultsComboBox,
                     &ResultsVariableSelector::resultsVariableSelected,
                     treeResultsVariable,
                     &ResultsVariableTree::onResultsVariableSelected);
}

QMenu * MainWindow::createPopupMenu (void)
{
    QMenu *popupMenu = this->QMainWindow::createPopupMenu();
//    Disabled untill toolbar modification is not implemented.
//    popupMenu->addAction(this->actionList->getAction(ACTION_SEPARATOR));
//    popupMenu->addAction(this->actionList->getAction(ACTION_SETTINGS_RESTORE));
    return popupMenu;
}

void MainWindow::readSettings(void)
{
    ApplicationSettings *pApplicationSettings = MainSettings::getInstance().getApplicationSettings();
    if (!pApplicationSettings)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Nonexisting application object.");
    }

    RVersion version(MainSettings::getInstance().value("application/version").toString());

    this->isFirstRun = (version != RVendor::version);

    if (version <= RVendor::version)
    {
        pApplicationSettings->setSolverPath(MainSettings::getInstance().value("application/SolverPath").toString());
        if (pApplicationSettings->getSolverPath().isEmpty())
        {
            pApplicationSettings->setSolverPath(ApplicationSettings::getDefaultRangeSolverExecutable());
        }

        pApplicationSettings->setHelpDir(MainSettings::getInstance().value("application/HelpDir").toString());
        if (pApplicationSettings->getHelpDir().isEmpty())
        {
            pApplicationSettings->setHelpDir(ApplicationSettings::getDefaultHelpDir());
        }

        pApplicationSettings->setNThreads(MainSettings::getInstance().value("application/nThreads",pApplicationSettings->getNThreads()).toUInt());
        pApplicationSettings->setNHistoryRecords(MainSettings::getInstance().value("application/nHistoryRecords",pApplicationSettings->getNHistoryRecords()).toUInt());
        pApplicationSettings->setStyle(MainSettings::getInstance().value("application/style",pApplicationSettings->getStyle()).toString());
        pApplicationSettings->setToolbarIconSize(MainSettings::getInstance().value("application/toolbarIconSize",pApplicationSettings->getToolbarIconSize()).toInt());
        pApplicationSettings->setSendUsageInfo(MainSettings::getInstance().value("application/sendUsageInfo",pApplicationSettings->getSendUsageInfo()).toBool());
        pApplicationSettings->setRangeApiAllowed(MainSettings::getInstance().value("application/rangeApiAllowed",pApplicationSettings->getRangeApiAllowed()).toBool());
        pApplicationSettings->setRangeApiServer(MainSettings::getInstance().value("application/rangeApiServer",pApplicationSettings->getRangeApiServer()).toString());
        pApplicationSettings->setRangeAccount(MainSettings::getInstance().value("application/rangeAccount",pApplicationSettings->getRangeAccount()).toString());
        pApplicationSettings->setRangePassword(MainSettings::getInstance().value("application/rangePassword",pApplicationSettings->getRangePassword()).toString());

        for (int i=ACTION_NONE;i<ACTION_N_TYPES;i++)
        {
            if (!ACTION_TYPE_IS_ACTION(ActionType(i)))
            {
                continue;
            }
            QString key = "application/shortcut_" + this->actionList->getAction(ActionType(i))->text();
            if (MainSettings::getInstance().contains(key))
            {
                const QString &shortcut = MainSettings::getInstance().value(key).toString();

                pApplicationSettings->getActionDefinition()->setShortcut(ActionType(i),shortcut);
                this->actionList->getAction(ActionType(i))->setShortcut(shortcut);
            }
        }
    }

    MainSettings::getInstance().sync();
}

void MainWindow::writeSettings(void) const
{
    QList<QAction*> actionList;
    QAction *action;
    QString key;

    // Main window
    MainSettings::getInstance().setValue("mainWindow/geometry", this->saveGeometry());
    MainSettings::getInstance().setValue("mainWindow/windowState", this->saveState());
    // Tool bars
    MainSettings::getInstance().setValue("nToolBars", uint(this->toolBars.size()));
    for (QList<QToolBar*>::size_type i = 0; i != this->toolBars.size(); i++)
    {
        actionList = this->toolBars[i]->actions();

        key = "toolBar_" + QString::number(i+1) + "_size";
        MainSettings::getInstance().setValue(key, actionList.size());
        key = "toolBar_" + QString::number(i+1) + "_name";
        MainSettings::getInstance().setValue(key, this->toolBars[i]->objectName());
        key = "toolBar_" + QString::number(i+1) + "_title";
        MainSettings::getInstance().setValue(key, this->toolBars[i]->windowTitle());

        for (int j=0;j<actionList.size();j++)
        {
            action = actionList[j];
            key = "toolBar_" + QString::number(i+1) + "_action_" + QString::number(j+1);
            MainSettings::getInstance().setValue(key,action->objectName());
        }
    }
    // Session
    QString sessionFileName = Session::getInstance().getFileName();
    if (sessionFileName.isEmpty())
    {
        sessionFileName = Session::getInstance().getDefaultFileName();
    }
    MainSettings::getInstance().setValue("session/fileName", sessionFileName.toUtf8().constData());
    // Application settings
    MainSettings::getInstance().setValue("application/SolverPath", MainSettings::getInstance().getApplicationSettings()->getSolverPath());
    MainSettings::getInstance().setValue("application/HelpDir", MainSettings::getInstance().getApplicationSettings()->getHelpDir());
    MainSettings::getInstance().setValue("application/nThreads", MainSettings::getInstance().getApplicationSettings()->getNThreads());
    MainSettings::getInstance().setValue("application/nHistoryRecords", MainSettings::getInstance().getApplicationSettings()->getNHistoryRecords());
    MainSettings::getInstance().setValue("application/style", MainSettings::getInstance().getApplicationSettings()->getStyle());
    MainSettings::getInstance().setValue("application/toolbarIconSize", MainSettings::getInstance().getApplicationSettings()->getToolbarIconSize());
    MainSettings::getInstance().setValue("application/sendUsageInfo", MainSettings::getInstance().getApplicationSettings()->getSendUsageInfo());
    MainSettings::getInstance().setValue("application/rangeApiAllowed", MainSettings::getInstance().getApplicationSettings()->getRangeApiAllowed());
    MainSettings::getInstance().setValue("application/rangeApiServer", MainSettings::getInstance().getApplicationSettings()->getRangeApiServer());
    MainSettings::getInstance().setValue("application/rangeAccount", MainSettings::getInstance().getApplicationSettings()->getRangeAccount());
    MainSettings::getInstance().setValue("application/rangePassword", MainSettings::getInstance().getApplicationSettings()->getRangePassword());

    for (int i=ACTION_NONE;i<ACTION_N_TYPES;i++)
    {
        if (!ACTION_TYPE_IS_ACTION(ActionType(i)))
        {
            continue;
        }
        QString key = "application/shortcut_" + this->actionList->getAction(ActionType(i))->text();
        MainSettings::getInstance().setValue(key, this->actionList->getAction(ActionType(i))->shortcut().toString());
    }

    MainSettings::getInstance().setValue("application/version",RVendor::version.toString());

    MainSettings::getInstance().sync();
}

uint MainWindow::getNToolBars(void) const
{
    return uint(this->toolBars.size());
}

QList<uint> MainWindow::getSelectedModelIDs(void) const
{
    return this->treeModelManager->getSelectedModelIDs();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QuitDialog quitDialog(this);

    if (quitDialog.exec() == QDialog::Accepted) {
        this->writeSettings();
        // Accept event -> close window.
        event->accept();
    }
    else {
        // Ignore event -> leave window open.
        event->ignore();
    }
}

QDockWidget *MainWindow::findFirstDockWidget(Qt::DockWidgetArea area)
{
    QList<QDockWidget *> dockWidgets = this->findChildren<QDockWidget *>();

    for (int i=0;i<dockWidgets.size();i++)
    {
        if (this->dockWidgetArea(dockWidgets[i]) == area)
        {
            return dockWidgets[i];
        }
    }
    return nullptr;
}

void MainWindow::progressBar(ProgressBar *progressBar, double fraction)
{
    progressBar->setValue(qRound(fraction*100));
//    progressBar->show();
}

void MainWindow::progressBarInitialize(ProgressBar *progressBar, const QString &message, bool pulseType)
{
    if (pulseType)
    {
        progressBar->startPulse();
    }
    else
    {
        progressBar->setRange(0,100);
    }
    progressBar->setMessage(message);
    progressBar->setValue(0);
    progressBar->show();
    this->statusBar()->clearMask();
}

void MainWindow::progressBarFinalize(ProgressBar *progressBar, const QString &message)
{
    progressBar->stopPulse();
    progressBar->setRange(0,100);
    progressBar->setValue(100);
    progressBar->setMessage("");
    progressBar->hide();
    this->statusBar()->showMessage(message,5000);
}

void MainWindow::setEnabled(bool enabled)
{
    this->dockModel->setEnabled(enabled);
    this->dockProblem->setEnabled(enabled);
    this->dockBc->setEnabled(enabled);
    this->dockIc->setEnabled(enabled);
    this->dockEc->setEnabled(enabled);
    this->dockMaterial->setEnabled(enabled);
    this->dockResults->setEnabled(enabled);
    this->dockDocuments->setEnabled(enabled);
//    this->dockRecords->setEnabled(enabled);
    if (enabled)
    {
        this->actionList->enable();
    }
    else
    {
        this->actionList->disable();
    }
}

void MainWindow::enable(void)
{
    this->setEnabled(true);
}

void MainWindow::disable(void)
{
    this->setEnabled(false);
}

void MainWindow::onMainProgress(double fraction)
{
    this->progressBar(this->mainProgressBar,fraction);
}

void MainWindow::onMainProgressInitialize(const QString &message, bool pulseType)
{
    this->progressBarInitialize(this->mainProgressBar,message,pulseType);
}

void MainWindow::onMainProgressFinalize(const QString &message)
{
    this->progressBarFinalize(this->mainProgressBar,message);
}

void MainWindow::onDownloadProgress(uint downloadID, qint64 bytesReceived, qint64 bytesTotal, double speed)
{
    double fraction = bytesTotal == 0 ?  0.0 : double(bytesReceived) / double(bytesTotal);

    QString unit;
    double displaySpeed = speed;
    if (displaySpeed < 1024)
    {
        unit = "bytes/sec";
    }
    else if (displaySpeed < 1024*1024)
    {
        displaySpeed /= 1024;
        unit = "kB/s";
    }
    else
    {
        displaySpeed /= 1024*1024;
        unit = "MB/s";
    }
    this->downloadProgressBar->setMessage(tr("Downloading file")
                                          + " # "
                                          + QString::number(downloadID)
                                          + " "
                                          + QString::number(bytesReceived)
                                          + " / "
                                          + QString::number(bytesTotal)
                                          + " Bytes ("
                                          + QString::number(displaySpeed,'g',2)
                                          + " "
                                          + unit
                                          + ")");
    this->progressBar(this->downloadProgressBar,fraction);
}

void MainWindow::onDownloadProgressInitialize(uint)
{
    this->progressBarInitialize(this->downloadProgressBar,tr("Downloading"),false);
}

void MainWindow::onDownloadProgressFinalize(uint)
{
    this->progressBarFinalize(this->downloadProgressBar,tr("Download complete"));
}

void MainWindow::onDrawObjectAdded(void)
{
    this->showDrawWidget();
}

void MainWindow::onDrawObjectRemoved(void)
{
    if (Session::getInstance().getDrawEngine()->getNObjects() == 0)
    {
        this->hideDrawWidget();
    }
}

void MainWindow::onJobBlocking(bool blocking)
{
    if (blocking)
    {
        this->disable();
    }
    else
    {
        this->enable();
    }
}

void MainWindow::onJobStarted(void)
{
}

void MainWindow::onJobEnded(void)
{
}

void MainWindow::onModelAdded(uint position)
{
    this->modelSubWindows->onModelAdded(position);
    this->treeModelManager->onModelAdded(position);
    if (!Session::getInstance().isModelSelected(position))
    {
        Session::getInstance().setModelSelected(position,true);
    }
    if (Session::getInstance().getModel(position).getProblemTaskTree().getProblemTypeMask() == R_PROBLEM_NONE)
    {
        ProblemTaskDialog problemTaskDialog(position,this);
        problemTaskDialog.exec();
    }
}

void MainWindow::onModelRemoved(uint position)
{
    this->modelSubWindows->onModelRemoved(position);
    this->treeModelManager->onModelRemoved(position);
    this->actionList->processAvailability();
}

void MainWindow::onModelChanged(uint position)
{
    this->modelSubWindows->onModelChanged(position);
    this->treeModelManager->onModelChanged(position);
    this->actionList->processAvailability();
}

void MainWindow::onProblemChanged(uint)
{
    this->actionList->processAvailability();
}

void MainWindow::onResultsChanged(uint)
{
    this->actionList->processAvailability();
}

void MainWindow::onEntityVisibilityChanged(uint modelID, REntityGroupType, uint, bool visible)
{
    bool modelIsVisible = false;
    if (visible)
    {
        modelIsVisible = true;
    }
    else
    {
        if (!Session::getInstance().getModel(modelID).isVisible(false))
        {
            modelIsVisible = true;
        }
    }
    if (modelIsVisible)
    {
        if (!this->modelSubWindows->windowExists(modelID))
        {
            this->modelSubWindows->windowCreate(modelID);
        }
    }
}

void MainWindow::onGeometryTransformFinalize(void)
{
    this->hideTransformGeometryWidget();
}

void MainWindow::onNHistoryRecordsChanged(uint)
{
    this->actionList->processAvailability();
}

void MainWindow::onToolbarIconSizeChanged(int toolbarIconSize)
{
    this->setIconSize(QSize(toolbarIconSize,toolbarIconSize));
}
