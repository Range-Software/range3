/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   document_widget.h                                        *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th November 2017                                      *
 *                                                                   *
 *  DESCRIPTION: Document widget class declaration                   *
 *********************************************************************/

#ifndef __DOCUMENT_WIDGET_H__
#define __DOCUMENT_WIDGET_H__

#include <QWidget>
#include <QListWidget>
#include <QTextBrowser>

class DocumentWidget : public QWidget
{

    Q_OBJECT

    protected:

        //! List widget
        QListWidget *listWidget;
        //! Text browser
        QTextBrowser *textBrowser;

    public:

        //! Constructor
        explicit DocumentWidget(QWidget *parent = nullptr);

    protected:

        //! Add list item.
        void addListItem(QIcon icon, const QString &text, const QString &fileName);

        //! Add list item.
        void addListItem(const QString &text, const QString &fileName);

    private slots:

        //! List selection changed
        void onListSelectionChanged(void);

};

#endif // __DOCUMENT_WIDGET_H__
