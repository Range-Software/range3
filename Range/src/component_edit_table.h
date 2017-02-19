/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   component_edit_table.h                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   3-rd August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: Component edit table class declaration              *
 *********************************************************************/

#ifndef __COMPONENT_EDIT_TABLE_H__
#define __COMPONENT_EDIT_TABLE_H__

#include <rmlib.h>

#include "value_table.h"

class ComponentEditTable : public ValueTable
{
    Q_OBJECT

    protected:

    public:

        //! Constructor.
        explicit ComponentEditTable(const RConditionComponent &component, QWidget *parent = 0);

    protected:

        //! Populate tree.
        void populate(const RConditionComponent &component);
    
};

#endif /* __COMPONENT_EDIT_TABLE_H__ */
