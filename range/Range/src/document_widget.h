/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   document_widget.h                                        *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th November 2017                                      *
 *                                                                   *
 *  DESCRIPTION: Document widget class declaration                   *
 *********************************************************************/

#ifndef DOCUMENT_WIDGET_H
#define DOCUMENT_WIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QTextBrowser>

class DocumentWidget : public QWidget
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
        explicit DocumentWidget(const QString &defaultFileName, QWidget *parent = nullptr);

    protected:

        //! Add list item.
        void addListItem(QIcon icon, const QString &text, const QString &fileName);

        //! Add list item.
        void addListItem(const QString &text, const QString &fileName);

        //! Load file.
        void loadFile(const QString &fileName);

    private slots:

        //! List selection changed
        void onListSelectionChanged(void);

};

#endif // DOCUMENT_WIDGET_H
