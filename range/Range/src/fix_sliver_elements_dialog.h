/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   fix_sliver_elements_dialog.h                             *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   1-st December 2016                                       *
 *                                                                   *
 *  DESCRIPTION: Fix sliver elements dialog class declaration        *
 *********************************************************************/

#ifndef FIXSLIVERELEMENTSDIALOG_H
#define FIXSLIVERELEMENTSDIALOG_H

#include <QDialog>

#include "value_line_edit.h"

class FixSliverElementsDialog : public QDialog
{

        Q_OBJECT

        protected:

            //! Model ID.
            uint modelID;
            //! Tollerance edit.
            ValueLineEdit *edgeRatioEdit;

        public:

            //! Constructor.
            explicit FixSliverElementsDialog(uint modelID,QWidget *parent = nullptr);

            //! Overloaded exec function.
            int exec(void);

};

#endif // FIXSLIVERELEMENTSDIALOG_H
