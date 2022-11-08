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
        //! Model manager tab.
        QWidget *modelsTab;
        //! Document manager tab.
        QWidget *documentsTab;
        //! Record manager tab.
        QWidget *recordsTab;
        //! Problem setup manager tab.
        QWidget *problemTab;
        //! Boundary conditions manager tab.
        QWidget *bcTab;
        //! Initial conditions manager tab.
        QWidget *icTab;
        //! Environment conditions manager tab.
        QWidget *ecTab;
        //! Material manager tab.
        QWidget *materialTab;
        //! Results manager tab.
        QWidget *resultsTab;
        //! Central tab widget.
        CentralTabWidget *centralTabWidget;
        //! Model manager.
        ModelTree *modelManagerTree;
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
        static MainWindow *getInstance();

        //! Show transform geometry dock widget.
        void showTransformGeometryWidget();

        //! Hide transform geometry dock widget.
        void hideTransformGeometryWidget();

        //! Show draw dock widget.
        void showDrawWidget();

        //! Hide draw dock widget.
        void hideDrawWidget();

        //! Enable progress-bar autohide.
        void progressAutoHideEnable();

        //! Disable progress-bar autohide.
        void progressAutoHideDisable();

    protected:

        //! Find custom dock widget.
        QDockWidget *findCustomDockWidget(const QString &name);

        //! Show custom dock widget.
        void showCustomDockWidget(QWidget *widget, const QString &name, const QString &title);

        //! Hide custom dock widget.
        void hideCustomDockWidget(const QString &name);

    protected:

        //! Return number of toolbars.
        uint getNToolBars() const;

        //! Return list of selected model IDs.
        QList<uint> getSelectedModelIDs() const;

        //! Create all menu items.
        void createMenus();

        //! Create all tool bars.
        void createToolBars();

        //! Create status bar.
        void createStatusBar();

        //! Create download bar.
        void createDownloadBar();

        //! Create central widget.
        void createCentralWidget();

        //! Create model tab.
        QWidget *createModelTab();

        //! Create document tab.
        QWidget *createDocumentTab();

        //! Create records tab.
        QWidget *createRecordsTab();

        //! Create problem setup tab.
        QWidget *createProblemTab();

        //! Create boundary condition tab.
        QWidget *createBcTab();

        //! Create initial condition tab.
        QWidget *createIcTab();

        //! Create environmant condition tab.
        QWidget *createEcTab();

        //! Create material tab.
        QWidget *createMaterialTab();

        //! Create results tab.
        QWidget *createResultsTab();

        //! Create custom popup menu.
        QMenu * createPopupMenu ();

        //! Write application settings.
        void writeSettings() const;

        //! Read application settings.
        void readSettings();

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
        void enable();

        //! Disable whole window.
        void disable();

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
        void onDrawObjectAdded();

        //! Draw object was removed.
        void onDrawObjectRemoved();

        //! Job is blocking.
        void onJobBlocking(bool blocking);

        //! New job has started.
        void onJobStarted();

        //! Job has ended.
        void onJobEnded();

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
        void onGeometryTransformFinalize();

        //! Called when number of history records has changed.
        void onNHistoryRecordsChanged(uint);

        //! Toolbar icon size has changed.
        void onToolbarIconSizeChanged(int toolbarIconSize);

};

#endif /* MAIN_WINDOW_H */
