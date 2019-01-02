/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   help_center_dialog.h                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th November 2017                                      *
 *                                                                   *
 *  DESCRIPTION: Help center dialog class declaration                *
 *********************************************************************/

#ifndef HELP_CENTER_DIALOG_H
#define HELP_CENTER_DIALOG_H

#include <QDialog>

class HelpCenterDialog : public QDialog
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit HelpCenterDialog(QWidget *parent = nullptr);

};

#endif // HELP_CENTER_DIALOG_H
