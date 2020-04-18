/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   main_window.h                                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   28-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Main window class declaration                       *
 *********************************************************************/

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QDockWidget>
#include <QToolBar>
#include <QMainWindow>
#include <QLabel>

#include "action_list.h"
#include "central_tab_widget.h"
#include "logger.h"
#include "main_settings.h"
#include "model_tree.h"
#include "sub_window_manager.h"
#include "progress_bar.h"

class MainWindow : public QMainWindow
{

    Q_OBJECT

    private:

        //! List of actions.
        ActionList *actionList;
        //! Subwindow manager.
        SubWindowManager *modelSubWindows;
        //! List of toolbars containing action types.
        QList<QToolBar*> toolBars;
        //! Model manager dock.
        QDockWidget *dockModel;
        //! Problem setup manager dock.
        QDockWidget *dockProblem;
        //! Boundary conditions manager dock.
        QDockWidget *dockBc;
        //! Initial conditions manager dock.
        QDockWidget *dockIc;
        //! Environment conditions manager dock.
        QDockWidget *dockEc;
        //! Material manager dock.
        QDockWidget *dockMaterial;
        //! Results manager dock.
        QDockWidget *dockResults;
        //! Document manager dock.
        QDockWidget *dockDocuments;
        //! Record manager dock.
        QDockWidget *dockRecords;
        //! Central tab widget.
        CentralTabWidget *centralTabWidget;
        //! Model manager.
        ModelTree *treeModelManager;
        //! Progress bar.
        ProgressBar* mainProgressBar;
        //! Download bar.
        ProgressBar* downloadProgressBar;
        //! First run flag.
        bool isFirstRun;

    private:

        //! Constructor.
        explicit MainWindow (QWidget *parent = nullptr);

    public:

        //! Get instance of singleton.
        static MainWindow *getInstance(void);

        //! Show transform geometry dock widget.
        void showTransformGeometryWidget(void);

        //! Hide transform geometry dock widget.
        void hideTransformGeometryWidget(void);

        //! Show draw dock widget.
        void showDrawWidget(void);

        //! Hide draw dock widget.
        void hideDrawWidget(void);

        //! Enable progress-bar autohide.
        void progressAutoHideEnable(void);

        //! Disable progress-bar autohide.
        void progressAutoHideDisable(void);

    protected:

        //! Find custom dock widget.
        QDockWidget *findCustomDockWidget(const QString &name);

        //! Show custom dock widget.
        void showCustomDockWidget(QWidget *widget, const QString &name, const QString &title);

        //! Hide custom dock widget.
        void hideCustomDockWidget(const QString &name);

    protected:

        //! Return number of toolbars.
        uint getNToolBars(void) const;

        //! Return list of selected model IDs.
        QList<uint> getSelectedModelIDs(void) const;

        //! Create all menu items.
        void createMenus(void);

        //! Create all tool bars.
        void createToolBars(void);

        //! Create status bar.
        void createStatusBar(void);

        //! Create download bar.
        void createDownloadBar(void);

        //! Create central widget.
        void createCentralWidget(void);

        //! Create model dock.
        void createModelDock(void);

        //! Create problem setup dock.
        void createProblemDock(void);

        //! Create boundary condition dock.
        void createBcDock(void);

        //! Create initial condition dock.
        void createIcDock(void);

        //! Create environmant condition dock.
        void createEcDock(void);

        //! Create material dock.
        void createMaterialDock(void);

        //! Create results dock.
        void createResultsDock(void);

        //! Create document dock.
        void createDocumentDock(void);

        //! Create records dock.
        void createRecordsDock(void);

        //! Create custom popup menu.
        QMenu * createPopupMenu (void);

        //! Write application settings.
        void writeSettings(void) const;

        //! Read application settings.
        void readSettings(void);

        //! Overloaded closeEvent function to catch close event.
        void closeEvent(QCloseEvent *event);

        //! Find first dock widget.
        QDockWidget *findFirstDockWidget(Qt::DockWidgetArea area);

        //! Set progress fraction bar.
        void progressBar(ProgressBar *progressBar, double fraction);

        //! Initialize progress bar.
        void progressBarInitialize(ProgressBar *progressBar, const QString &message, bool pulseType);

        //! Finalize progress bar.
        void progressBarFinalize(ProgressBar *progressBar, const QString &message);

    protected slots:

        //! Enable whole window.
        void setEnabled(bool enabled);

        //! Enable whole window.
        void enable(void);

        //! Disable whole window.
        void disable(void);

        //! Set main progress fraction.
        void onMainProgress(double fraction);

        //! Initialize main progress.
        void onMainProgressInitialize(const QString &message, bool pulseType);

        //! Finalize main progress.
        void onMainProgressFinalize(const QString &message);

        //! Set download progress fraction.
        void onDownloadProgress(uint downloadID, qint64 bytesReceived, qint64 bytesTotal, double speed);

        //! Initialize download progress.
        void onDownloadProgressInitialize(uint);

        //! Finalize download progress.
        void onDownloadProgressFinalize(uint);

        //! Draw object was added.
        void onDrawObjectAdded(void);

        //! Draw object was removed.
        void onDrawObjectRemoved(void);

        //! Job is blocking.
        void onJobBlocking(bool blocking);

        //! New job has started.
        void onJobStarted(void);

        //! Job has ended.
        void onJobEnded(void);

        //! New model has been added.
        void onModelAdded(uint position);

        //! Model has been removed.
        void onModelRemoved(uint position);

        //! Model has been changed.
        void onModelChanged(uint position);

        //! Problem has been changed.
        void onProblemChanged(uint);

        //! Results has been changed.
        void onResultsChanged(uint);

        //! Model visibility has changed.
        void onEntityVisibilityChanged(uint modelID, REntityGroupType, uint, bool visible);

        //! Finalize geometry transform widget.
        void onGeometryTransformFinalize(void);

        //! Called when number of history records has changed.
        void onNHistoryRecordsChanged(uint);

        //! Toolbar icon size has changed.
        void onToolbarIconSizeChanged(int toolbarIconSize);

};

#endif /* MAIN_WINDOW_H */
