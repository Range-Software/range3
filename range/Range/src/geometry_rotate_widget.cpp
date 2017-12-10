/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   geometry_rotate_widget.cpp                               *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   6-th November 2014                                       *
 *                                                                   *
 *  DESCRIPTION: Geometry rotate widget class definition             *
 *********************************************************************/

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include "geometry_rotate_widget.h"

const RR3Vector GeometryRotateWidget::defaultAngles = RR3Vector(0.0,0.0,0.0);

GeometryRotateWidget::GeometryRotateWidget(const RR3Vector &center, const RR3Vector &rotation, QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    this->setLayout(vBoxLayout);

    QGridLayout *layout = new QGridLayout;
    vBoxLayout->addLayout(layout);

    QLabel *xAngleLabel = new QLabel(tr("X rotation")+":");
    layout->addWidget(xAngleLabel,0,0,1,1);

    double maxAngle = R_RAD_TO_DEG(2.0*RConstants::pi);

    this->xAngleLineEdit = new ValueLineEdit;
    this->xAngleLineEdit->setDoubleValidator();
    this->xAngleLineEdit->setRange(-maxAngle,maxAngle);
    this->xAngleLineEdit->setValue(rotation[0]);
    layout->addWidget(this->xAngleLineEdit,0,1,1,1);

    QLabel *yAngleLabel = new QLabel(tr("Y rotation")+":");
    layout->addWidget(yAngleLabel,1,0,1,1);

    this->yAngleLineEdit = new ValueLineEdit;
    this->yAngleLineEdit->setDoubleValidator();
    this->yAngleLineEdit->setRange(-maxAngle,maxAngle);
    this->yAngleLineEdit->setValue(rotation[1]);
    layout->addWidget(this->yAngleLineEdit,1,1,1,1);

    QLabel *zAngleLabel = new QLabel(tr("Z rotation")+":");
    layout->addWidget(zAngleLabel,2,0,1,1);

    this->zAngleLineEdit = new ValueLineEdit;
    this->zAngleLineEdit->setDoubleValidator();
    this->zAngleLineEdit->setRange(-maxAngle,maxAngle);
    this->zAngleLineEdit->setValue(rotation[2]);
    layout->addWidget(this->zAngleLineEdit,2,1,1,1);

    this->rotationCenter = new PositionWidget(tr("Rotation center"),center,false);
    this->rotationCenter->hideButtons();
    this->rotationCenter->hideSliders();
    layout->addWidget(this->rotationCenter,3,0,1,2);

    QPushButton *anglesResetButton = new QPushButton(QIcon(":/icons/file/pixmaps/range-undo.svg"),tr("Reset"));
    layout->addWidget(anglesResetButton,2,2,1,1);

    QObject::connect(anglesResetButton,
                     &QPushButton::clicked,
                     this,
                     &GeometryRotateWidget::resetAngles);

    QObject::connect(this->xAngleLineEdit,
                     &ValueLineEdit::valueChanged,
                     this,
                     &GeometryRotateWidget::onAngleValueChanged);

    QObject::connect(this->yAngleLineEdit,
                     &ValueLineEdit::valueChanged,
                     this,
                     &GeometryRotateWidget::onAngleValueChanged);

    QObject::connect(this->zAngleLineEdit,
                     &ValueLineEdit::valueChanged,
                     this,
                     &GeometryRotateWidget::onAngleValueChanged);

    QObject::connect(this->rotationCenter,
                     &PositionWidget::changed,
                     this,
                     &GeometryRotateWidget::onPositionChanged);

    this->resetAngles();

    vBoxLayout->addStretch(10);
}

void GeometryRotateWidget::resetAngles(void)
{
    this->xAngleLineEdit->setValue(this->defaultAngles[0]);
    this->yAngleLineEdit->setValue(this->defaultAngles[1]);
    this->zAngleLineEdit->setValue(this->defaultAngles[2]);
}

void GeometryRotateWidget::onAngleValueChanged(double)
{
    emit this->rotationChanged(this->rotationCenter->getPosition(),
                               RR3Vector(this->xAngleLineEdit->getValue(),
                                         this->yAngleLineEdit->getValue(),
                                         this->zAngleLineEdit->getValue()));
}

void GeometryRotateWidget::onPositionChanged(const RR3Vector &)
{
    emit this->rotationChanged(this->rotationCenter->getPosition(),
                               RR3Vector(this->xAngleLineEdit->getValue(),
                                         this->yAngleLineEdit->getValue(),
                                         this->zAngleLineEdit->getValue()));
}
