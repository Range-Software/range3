/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   text_browser_dialog.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   21-st September 2017                                     *
 *                                                                   *
 *  DESCRIPTION: Text browser dialog class declaration               *
 *********************************************************************/

#ifndef TEXT_BROWSER_DIALOG_H
#define TEXT_BROWSER_DIALOG_H

#include <QDialog>

class TextBrowserDialog : public QDialog
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit TextBrowserDialog(const QString &title,
                                   const QString &desc,
                                   const QString &text,
                                   QWidget *parent = nullptr);

};

#endif // TEXT_BROWSER_DIALOG_H
