/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   text_browser_dialog.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   21-st September 2017                                     *
 *                                                                   *
 *  DESCRIPTION: Text browser dialog class declaration               *
 *********************************************************************/

#ifndef __TEXT_BROWSER_DIALOG_H__
#define __TEXT_BROWSER_DIALOG_H__

#include <QDialog>

class TextBrowserDialog : public QDialog
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit TextBrowserDialog(const QString &title,
                                   const QString &desc,
                                   const QString &text,
                                   QWidget *parent = 0);

};

#endif // __TEXT_BROWSER_DIALOG_H__
