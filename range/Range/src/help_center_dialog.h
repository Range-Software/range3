/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   help_center_dialog.h                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th November 2017                                      *
 *                                                                   *
 *  DESCRIPTION: Help center dialog class declaration                *
 *********************************************************************/

#ifndef __HELP_CENTER_DIALOG_H__
#define __HELP_CENTER_DIALOG_H__

#include <QDialog>

class HelpCenterDialog : public QDialog
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit HelpCenterDialog(QWidget *parent = 0);

};

#endif // __HELP_CENTER_DIALOG_H__
