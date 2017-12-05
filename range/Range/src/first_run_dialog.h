/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   first_run_dialog.h                                       *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   16-th November 2017                                      *
 *                                                                   *
 *  DESCRIPTION: First run dialog class declaration                  *
 *********************************************************************/

#ifndef __FIRST_RUN_DIALOG_H__
#define __FIRST_RUN_DIALOG_H__

#include <QDialog>

class FirstRunDialog : public QDialog
{

    public:

        //! Constructor.
        explicit FirstRunDialog(QWidget *parent = 0);

    private slots:

        //! On Send usage info toggled.
        void onSendUsageInfoToggled(bool checked);

        //! On Application settings button clicked.
        void onApplicationSettingsButtonClicked(bool checked);

};

#endif // __FIRST_RUN_DIALOG_H__
