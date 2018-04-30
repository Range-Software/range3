/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   clipping_plane_widget.cpp                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   18-th March 2015                                         *
 *                                                                   *
 *  DESCRIPTION: Clipping plane widget class definition              *
 *********************************************************************/

#include <QHBoxLayout>

#include "clipping_plane_widget.h"

ClippingPlaneWidget::ClippingPlaneWidget(QWidget *parent) :
    QWidget(parent)
{
    bool isEnabled = false;

    QHBoxLayout *layout = new QHBoxLayout;
    this->setLayout(layout);

    this->enabledCheck = new QCheckBox(tr("Clipping plane"));
    this->enabledCheck->setChecked(isEnabled);
    layout->addWidget(this->enabledCheck);

    this->distanceLabel = new QLabel(tr("Distance"));
    this->distanceLabel->setEnabled(isEnabled);
    layout->addWidget(this->distanceLabel);

    this->distanceSlider = new QSlider(Qt::Horizontal);
    this->distanceSlider->setMinimum(0);
    this->distanceSlider->setMaximum(100);
    this->distanceSlider->setValue(50);
    this->distanceSlider->setEnabled(isEnabled);
    layout->addWidget(this->distanceSlider);

    QObject::connect(this->enabledCheck,&QCheckBox::stateChanged,this,&ClippingPlaneWidget::onStateChanged);
    QObject::connect(this->distanceSlider,&QSlider::valueChanged,this,&ClippingPlaneWidget::onValueChanged);
}

void ClippingPlaneWidget::onStateChanged(int state)
{
    this->distanceLabel->setDisabled(state == Qt::Unchecked);
    this->distanceSlider->setDisabled(state == Qt::Unchecked);

    emit this->changed(state != Qt::Unchecked,double(this->distanceSlider->value())/100.0);
}

void ClippingPlaneWidget::onValueChanged(int value)
{
    emit this->changed(this->enabledCheck->isChecked(),double(value)/100.0);
}
