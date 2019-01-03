/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   vector_field_dialog.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   14-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Vector field dialog class declaration               *
 *********************************************************************/

#ifndef VECTOR_FIELD_DIALOG_H
#define VECTOR_FIELD_DIALOG_H

#include <QDialog>
#include <QCheckBox>

#include "model_tree_simple.h"

class VectorFieldDialog : public QDialog
{

    Q_OBJECT

    protected:

        enum TreeColumn
        {
            Name = 0,
            Type,
            NColumns
        };

        //! Model tree widget.
        ModelTreeSimple *modelTree;
        //! Vector variables tree widget.
        QTreeWidget *variableTree;
        //! 3D type check box.
        QCheckBox *type3DCheckbox;
        //! OK button.
        QPushButton *okButton;
        //! Model ID.
        uint modelID;
        //! Entity ID.
        uint entityID;

    public:

        //! Constructor.
        explicit VectorFieldDialog(uint modelID, QWidget *parent = nullptr);

        //! Constructor.
        explicit VectorFieldDialog(uint modelID, uint entityID, QWidget *parent = nullptr);

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

#endif // VECTOR_FIELD_DIALOG_H
