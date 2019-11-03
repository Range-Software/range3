/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   crash_report_dialog.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   25-th October 2019                                       *
 *                                                                   *
 *  DESCRIPTION: Crash report dialog class declaration               *
 *********************************************************************/

#ifndef CRASH_REPORT_DIALOG_H
#define CRASH_REPORT_DIALOG_H

#include <QDialog>

#include "log_browser.h"
#include "text_edit_widget.h"

class CrashReportDialog : public QDialog
{

    protected:

        //! Log browser.
        LogBrowser *logBrowser;

        //! Text edit;
        QPlainTextEdit *textEdit;

    public:

        //! Constructor.
        explicit CrashReportDialog(QWidget *parent, const QString &logFile);

        //! Execute dialog.
        int exec();
};

#endif // CRASH_REPORT_DIALOG_H
