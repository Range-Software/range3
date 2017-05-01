/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   quit_dialog.h                                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   28-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Quit dialog class declaration                       *
 *********************************************************************/

#ifndef __QUIT_DIALOG_H__
#define __QUIT_DIALOG_H__

#include <QDialog>

class QuitDialog : public QDialog
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit QuitDialog (QWidget *parent = 0);

};

#endif /* __QUIT_DIALOG_H__ */
