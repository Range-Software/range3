/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   material_property_line_edit.h                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   14-th August 2012                                        *
 *                                                                   *
 *  DESCRIPTION: Material property line edit class declaration       *
 *********************************************************************/

#ifndef __MATERIAL_PROPERTY_LINE_EDIT_H__
#define __MATERIAL_PROPERTY_LINE_EDIT_H__

#include <rmlib.h>

#include "value_line_edit.h"

class MaterialPropertyLineEdit : public ValueLineEdit
{
    Q_OBJECT

    protected:

        //! Material property type.
        RMaterialPropertyType type;

    public:

        //! Constructor.
        explicit MaterialPropertyLineEdit(RMaterialPropertyType type,
                                          QWidget *parent = 0);

    signals:

        //! Value changed signal.
        void valueChanged(RMaterialPropertyType type, double value);

    private slots:

        //! Catch signal textChanged
        void onValueChaged(double value);
        
};

#endif /* __MATERIAL_PROPERTY_LINE_EDIT_H__ */
