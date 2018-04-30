/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   log_browser_dialog.h                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   9-th August 2013                                         *
 *                                                                   *
 *  DESCRIPTION: Log browser dialog class declaration                *
 *********************************************************************/

#ifndef __LOG_BROWSER_DIALOG_H__
#define __LOG_BROWSER_DIALOG_H__

#include <QDialog>

#include "log_browser.h"

class LogBrowserDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Log browser.
        LogBrowser *logBrowser;

    public:

        //! Constructor.
        explicit LogBrowserDialog(const QString &logFileName, QWidget *parent = 0);

        //! Execute dialog.
        int exec(void);

    signals:

    protected slots:

        //! Clear button was clicked.
        void onClearButtonClicked(void);

};

#endif // __LOG_BROWSER_DIALOG_H__
