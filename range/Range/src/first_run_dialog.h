/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   first_run_dialog.h                                       *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   16-th November 2017                                      *
 *                                                                   *
 *  DESCRIPTION: First run dialog class declaration                  *
 *********************************************************************/

#ifndef FIRST_RUN_DIALOG_H
#define FIRST_RUN_DIALOG_H

#include <QDialog>

class FirstRunDialog : public QDialog
{

    public:

        //! Constructor.
        explicit FirstRunDialog(QWidget *parent = nullptr);

    private slots:

        //! On Send usage info toggled.
        void onSendUsageInfoToggled(bool checked);

};

#endif // FIRST_RUN_DIALOG_H
