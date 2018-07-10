/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   material_property_edit_table.h                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   14-th August 2012                                        *
 *                                                                   *
 *  DESCRIPTION: Material property edit table class declaration      *
 *********************************************************************/

#ifndef __MATERIAL_PROPERTY_EDIT_TABLE_H__
#define __MATERIAL_PROPERTY_EDIT_TABLE_H__

#include <QTableWidget>

#include <rmlib.h>

#include "value_table.h"

class MaterialPropertyEditTable : public ValueTable
{
    Q_OBJECT

    public:

        //! Constructor.
        explicit MaterialPropertyEditTable(RMaterialProperty &property,
                                           QWidget *parent = 0);

    protected:

        //! Populate tree.
        void populate(RMaterialProperty &property);
        
};

#endif /* __MATERIAL_PROPERTY_EDIT_TABLE_H__ */
