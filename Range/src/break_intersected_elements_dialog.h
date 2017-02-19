/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   break_intersected_elements_dialog.h                      *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   24-th March 2015                                         *
 *                                                                   *
 *  DESCRIPTION: Break intersected elements dialog class declaration *
 *********************************************************************/

#ifndef __BREAK_INTERSECTED_ELEMENTS_DIALOG_H__
#define __BREAK_INTERSECTED_ELEMENTS_DIALOG_H__

#include <QDialog>
#include <QSpinBox>

class BreakIntersectedElementsDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Model ID.
        uint modelID;
        //! Number of iterations spinbox.
        QSpinBox *nIterationsSpin;

    public:

        //! Constructor.
        explicit BreakIntersectedElementsDialog(uint modelID, QWidget *parent = 0);

        //! Execute dialog.
        int exec(void);

};

#endif // __BREAK_INTERSECTED_ELEMENTS_DIALOG_H__
