/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   component_edit_dialog.h                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   2-nd August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: Component edit dialog class declaration             *
 *********************************************************************/

#ifndef __COMPONENT_EDIT_DIALOG_H__
#define __COMPONENT_EDIT_DIALOG_H__

#include <QDialog>

#include <rmlib.h>

#include "component_edit_table.h"

class ComponentEditDialog : public QDialog
{
    Q_OBJECT

    private:

        //! Pointer to condition component.
        RConditionComponent *pComponent;

        //! Component table.
        ComponentEditTable *editTable;

    public:

        //! Constructor.
        explicit ComponentEditDialog(RConditionComponent &component, QWidget *parent = 0);

        //! Execute dialog.
        int exec(void);
        
    protected slots:

        //! Catch value changed signal and set new number of values.
        void onSpinNValuesChanged(int nValues);
        
};

#endif /* __COMPONENT_VALUE_EDITOR_DIALOG_H__ */
