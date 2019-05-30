/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   push_button.cpp                                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   2-nd August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: Custom push button class definition                 *
 *********************************************************************/

#include "push_button.h"

PushButton::PushButton(uint id, const QString &text, QWidget *parent) :
    QPushButton(text, parent),
    id(id)
{
    QObject::connect(this,&QPushButton::clicked,this,&PushButton::onClicked);
}

void PushButton::onClicked(void)
{
    emit this->clicked(int(this->id));
}
