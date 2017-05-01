/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   vector_field_dialog.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   14-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Vector field dialog class declaration               *
 *********************************************************************/

#ifndef __VECTOR_FIELD_DIALOG_H__
#define __VECTOR_FIELD_DIALOG_H__

#include <QDialog>

#include "model_tree_simple.h"

class VectorFieldDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Model tree widget.
        ModelTreeSimple *modelTree;
        //! Vector variables tree widget.
        QTreeWidget *variableTree;
        //! OK button.
        QPushButton *okButton;
        //! Model ID.
        uint modelID;
        //! Entity ID.
        uint entityID;

    public:

        //! Constructor.
        explicit VectorFieldDialog(uint modelID, QWidget *parent = 0);

        //! Constructor.
        explicit VectorFieldDialog(uint modelID, uint entityID, QWidget *parent = 0);

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

#endif // __VECTOR_FIELD_DIALOG_H__
