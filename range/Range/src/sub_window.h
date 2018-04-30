/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   sub_window.h                                             *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   29-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Sub window class declaration                        *
 *********************************************************************/

#ifndef __SUB_WINDOW_H__
#define __SUB_WINDOW_H__

#include <QMdiSubWindow>

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

    public:

        //! Constructor.
        explicit SubWindow(uint modelID, QWidget *parent = 0);

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

};

#endif /* __SUB_WINDOW_H__ */
