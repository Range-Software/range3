/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   material_property_edit_dialog.h                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   2-nd August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: Material property edit dialog class declaration     *
 *********************************************************************/

#ifndef __MATERIAL_PROPERTY_EDIT_DIALOG_H__
#define __MATERIAL_PROPERTY_EDIT_DIALOG_H__

#include <QDialog>
#include <QSpinBox>

#include <rmlib.h>

#include "material_property_edit_table.h"

class MaterialPropertyEditDialog : public QDialog
{

    Q_OBJECT

    private:

        //! Pointer to condition component.
        RMaterialProperty *pProperty;

        //! Number of values spin box.
        QSpinBox *spinNValues;
        //! Component table.
        MaterialPropertyEditTable *editTable;

    public:

        //! Constructor.
        explicit MaterialPropertyEditDialog(RMaterialProperty &property,
                                            QWidget *parent = 0);

        //! Execute dialog.
        int exec(void);

    protected slots:

        //! Catch table size changed signal.
        void onTableSizeChanged(int nRows);

        //! Catch value changed signal and set new number of values.
        void onSpinNValuesChanged(int nValues);
        
};

#endif /* __MATERIAL_PROPERTY_EDIT_DIALOG_H__ */
