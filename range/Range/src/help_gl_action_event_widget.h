/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   help_gl_action_event_widget.h                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   31-st October 2020                                       *
 *                                                                   *
 *  DESCRIPTION: Help GL action event widget class declaration       *
 *********************************************************************/

#ifndef HELP_GL_ACTION_EVENT_WIDGET_H
#define HELP_GL_ACTION_EVENT_WIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QTextBrowser>

class HelpGLActionEventWidget : public QWidget
{

    Q_OBJECT

    protected:

        //! List widget.
        QListWidget *listWidget;
        //! Text browser.
        QTextBrowser *textBrowser;
        //! Default document.
        QString defaultFileName;

    public:

        //! Constructor
        explicit HelpGLActionEventWidget(QWidget *parent = nullptr);

    private slots:

        //! List selection changed
        void onListSelectionChanged(void);

};

#endif // HELP_GL_ACTION_EVENT_WIDGET_H
