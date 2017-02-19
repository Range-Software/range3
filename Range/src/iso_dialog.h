/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   iso_dialog.h                                             *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   22-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Iso dialog class declaration                        *
 *********************************************************************/

#ifndef __ISO_DIALOG_H__
#define __ISO_DIALOG_H__

#include <QDialog>

#include "model_tree_simple.h"
#include "value_line_edit.h"

class IsoDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Model tree widget.
        ModelTreeSimple *modelTree;
        //! Vector variables tree widget.
        QTreeWidget *variableTree;
        //! Variable value.
        ValueLineEdit *variableValue;
        //! OK button.
        QPushButton *okButton;
        //! Model ID.
        uint modelID;
        //! Entity ID.
        uint entityID;

    public:

        //! Constructor.
        explicit IsoDialog(uint modelID, QWidget *parent = 0);

        //! Constructor.
        explicit IsoDialog(uint modelID, uint entityID, QWidget *parent = 0);

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

#endif // __ISO_DIALOG_H__
