/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   material_property_edit_table.h                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   14-th August 2012                                        *
 *                                                                   *
 *  DESCRIPTION: Material property edit table class declaration      *
 *********************************************************************/

#ifndef MATERIAL_PROPERTY_EDIT_TABLE_H
#define MATERIAL_PROPERTY_EDIT_TABLE_H

#include <QTableWidget>

#include <rmlib.h>

#include "value_table.h"

class MaterialPropertyEditTable : public ValueTable
{
    Q_OBJECT

    public:

        //! Constructor.
        explicit MaterialPropertyEditTable(RMaterialProperty &property,
                                           QWidget *parent = nullptr);

    protected:

        //! Populate tree.
        void populate(RMaterialProperty &property);
        
};

#endif /* MATERIAL_PROPERTY_EDIT_TABLE_H */
