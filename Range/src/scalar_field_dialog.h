/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   scalar_field_dialog.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   14-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Scalar field dialog class declaration               *
 *********************************************************************/

#ifndef __SCALAR_FIELD_DIALOG_H__
#define __SCALAR_FIELD_DIALOG_H__

#include <QDialog>
#include <QSpinBox>

#include <rmlib.h>

#include "model_tree_simple.h"

class ScalarFieldDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Model tree widget.
        ModelTreeSimple *modelTree;
        //! Scalar variables tree widget.
        QTreeWidget *variableTree;
        //! Point size spin box.
        QSpinBox *pointSizeSpin;
        //! OK button.
        QPushButton *okButton;
        //! Model ID.
        uint modelID;
        //! Entity ID.
        uint entityID;

    public:

        //! Constructor.
        explicit ScalarFieldDialog(uint modelID, QWidget *parent = 0);

        //! Constructor.
        explicit ScalarFieldDialog(uint modelID, uint entityID, QWidget *parent = 0);

        //! Execute dialog.
        int exec(void);

    private:

        //! Create dialog.
        void createDialog(void);

        //! Return selected variable type.
        RVariableType getVariableType(void) const;

    protected slots:

        //! Variable tree selection has changed.
        void onVariableTreeSelectionChanged(void);

        //! Model tree selection has changed.
        void onModelTreeSelectionChanged(void);

};

#endif // __SCALAR_FIELD_DIALOG_H__
