/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   material_property_line_edit.cpp                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   14-th August 2012                                        *
 *                                                                   *
 *  DESCRIPTION: Material property line edit class definition        *
 *********************************************************************/

#include "material_property_line_edit.h"

MaterialPropertyLineEdit::MaterialPropertyLineEdit(RMaterialPropertyType type, QWidget *parent) :
    ValueLineEdit(parent),
    type(type)
{
    this->setDoubleValidator();
    QObject::connect(this,&ValueLineEdit::valueChanged,this,&MaterialPropertyLineEdit::onValueChaged);
}

void MaterialPropertyLineEdit::onValueChaged(double value)
{
    emit this->valueChanged(this->type,value);
}
