/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   sub_window.h                                             *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   29-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Sub window class declaration                        *
 *********************************************************************/

#ifndef SUB_WINDOW_H
#define SUB_WINDOW_H

#include <QMdiSubWindow>
#include <QToolBar>

#include "gl_widget.h"

class SubWindow : public QMdiSubWindow
{

    Q_OBJECT

    private:

        //! Create sub window.
        void createSubWindow(void);

    protected:

        //! Model ID.
        uint modelID;
        //! OpenGL widget.
        GLWidget *glWidget;
        //! Toolbar.
        QToolBar *toolBar;

    public:

        //! Constructor.
        explicit SubWindow(uint modelID, QWidget *parent = nullptr);

        //! Return model ID.
        uint getModelID(void) const;

        //! Set new model ID.
        void setModelID(uint modelID);

        //! Return pointer to GLWidget.
        GLWidget *getGlWidget(void) const;

    protected:

        //! Window close event.
        void closeEvent(QCloseEvent *closeEvent);

    signals:

        //! Closed signal.
        void closed(uint modelID);

    protected slots:

        //! Reset to origin.
        void onResetO(void);

        //! Reset to X plane.
        void onResetX(void);

        //! Reset to Y plane.
        void onResetY(void);

        //! Reset to Z plane.
        void onResetZ(void);

        //! Take screenshot.
        void onScreenshot(void);

        //! Take screenshot.
        void onSessionScreenshot(uint modelID, const QString &fileName);

        //! Show model edges.
        void onShowModelEdgesToggled(bool checked);

        //! Show errors.
        void onShowErrorsToggled(bool checked);

        //! Back side color check toggled.
        void onBackSideColorChanged(int state);

        //! Edit preferences.
        void onPreferences(void);

        //! Toolbar icon size has changed.
        void onToolbarIconSizeChanged(int toolbarIconSize);

};

#endif /* SUB_WINDOW_H */
