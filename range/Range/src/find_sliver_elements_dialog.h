/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   find_sliver_elements_dialog.h                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th April 2018                                          *
 *                                                                   *
 *  DESCRIPTION: Find sliver elements dialog class declaration       *
 *********************************************************************/

#ifndef FIND_SLIVER_ELEMENTS_DIALOG_H
#define FIND_SLIVER_ELEMENTS_DIALOG_H

#include <QDialog>

#include "value_line_edit.h"

class FindSliverElementsDialog : public QDialog
{

        Q_OBJECT

        protected:

            //! Model ID.
            uint modelID;
            //! Tollerance edit.
            ValueLineEdit *edgeRatioEdit;

        public:

            //! Constructor.
            explicit FindSliverElementsDialog(uint modelID,QWidget *parent = nullptr);

            //! Overloaded exec function.
            int exec(void);

};

#endif // FIND_SLIVER_ELEMENTS_DIALOG_H
