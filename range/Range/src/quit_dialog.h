/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   quit_dialog.h                                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   28-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Quit dialog class declaration                       *
 *********************************************************************/

#ifndef QUIT_DIALOG_H
#define QUIT_DIALOG_H

#include <QDialog>

class QuitDialog : public QDialog
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit QuitDialog (QWidget *parent = nullptr);

};

#endif /* QUIT_DIALOG_H */
