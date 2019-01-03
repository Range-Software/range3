/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   component_edit_table.h                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   3-rd August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: Component edit table class declaration              *
 *********************************************************************/

#ifndef COMPONENT_EDIT_TABLE_H
#define COMPONENT_EDIT_TABLE_H

#include <rmlib.h>

#include "value_table.h"

class ComponentEditTable : public ValueTable
{
    Q_OBJECT

    protected:

    public:

        //! Constructor.
        explicit ComponentEditTable(const RConditionComponent &component, QWidget *parent = nullptr);

    protected:

        //! Populate tree.
        void populate(const RConditionComponent &component);
    
};

#endif /* COMPONENT_EDIT_TABLE_H */
