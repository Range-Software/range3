/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   geometry_scale_widget.cpp                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   6-th November 2014                                       *
 *                                                                   *
 *  DESCRIPTION: Geometry scale widget class definition              *
 *********************************************************************/

#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "geometry_scale_widget.h"
#include "session.h"

GeometryScaleWidget::GeometryScaleWidget(const RR3Vector &center, const RR3Vector &scale, QWidget *parent)
    : QWidget(parent)
    , defaultScale(scale)
{
    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    this->setLayout(vBoxLayout);

    this->sameScaleGroupBox = new QGroupBox(tr("Same scale in all directions"));
    this->sameScaleGroupBox->setCheckable(true);
    this->sameScaleGroupBox->setChecked(true);
    vBoxLayout->addWidget(this->sameScaleGroupBox);

    QGridLayout *sameLayout = new QGridLayout;
    this->sameScaleGroupBox->setLayout(sameLayout);

    QLabel *sameScaleLabel = new QLabel(tr("Scale")+":");
    sameLayout->addWidget(sameScaleLabel,0,0,1,1);

    this->sameScaleLineEdit = new ValueLineEdit;
    this->sameScaleLineEdit->setDoubleValidator();
    this->sameScaleLineEdit->setValue(this->defaultScale[0]);
    sameLayout->addWidget(this->sameScaleLineEdit,0,1,1,1);

    QPushButton *sameScaleResetButton = new QPushButton(QIcon(":/icons/file/pixmaps/range-undo.svg"),tr("Reset"));
    sameLayout->addWidget(sameScaleResetButton,0,2,1,1);

    QObject::connect(sameScaleResetButton,
                     &QPushButton::clicked,
                     this,
                     &GeometryScaleWidget::resetSameScale);

    this->differentScaleGroupBox = new QGroupBox(tr("Different scale in each directions"));
    this->differentScaleGroupBox->setCheckable(true);
    this->differentScaleGroupBox->setChecked(false);
    vBoxLayout->addWidget(this->differentScaleGroupBox);

    QGridLayout *differentLayout = new QGridLayout;
    this->differentScaleGroupBox->setLayout(differentLayout);

    QLabel *xScaleLabel = new QLabel(tr("X scale")+":");
    differentLayout->addWidget(xScaleLabel,0,0,1,1);

    this->xScaleLineEdit = new ValueLineEdit;
    this->xScaleLineEdit->setDoubleValidator();
    differentLayout->addWidget(this->xScaleLineEdit,0,1,1,1);

    QLabel *yScaleLabel = new QLabel(tr("Y scale")+":");
    differentLayout->addWidget(yScaleLabel,1,0,1,1);

    this->yScaleLineEdit = new ValueLineEdit;
    this->yScaleLineEdit->setDoubleValidator();
    differentLayout->addWidget(this->yScaleLineEdit,1,1,1,1);

    QLabel *zScaleLabel = new QLabel(tr("Z scale")+":");
    differentLayout->addWidget(zScaleLabel,2,0,1,1);

    this->zScaleLineEdit = new ValueLineEdit;
    this->zScaleLineEdit->setDoubleValidator();
    differentLayout->addWidget(this->zScaleLineEdit,2,1,1,1);

    this->scaleCenter = new PositionWidget(tr("Scale center"),center,false);
    this->scaleCenter->hideButtons();
    this->scaleCenter->hideSliders();
    vBoxLayout->addWidget(this->scaleCenter);

    QPushButton *differentScaleResetButton = new QPushButton(QIcon(":/icons/file/pixmaps/range-undo.svg"),tr("Reset"));
    differentLayout->addWidget(differentScaleResetButton,2,2,1,1);

    QObject::connect(differentScaleResetButton,
                     &QPushButton::clicked,
                     this,
                     &GeometryScaleWidget::resetDifferentScale);

    this->resetSameScale();
    this->resetDifferentScale();

    QObject::connect(this->sameScaleGroupBox,
                     &QGroupBox::toggled,
                     this,
                     &GeometryScaleWidget::onSameScaleToggled);

    QObject::connect(this->differentScaleGroupBox,
                     &QGroupBox::toggled,
                     this,
                     &GeometryScaleWidget::onDifferentScaleToggled);

    QObject::connect(this->sameScaleLineEdit,
                     &ValueLineEdit::valueChanged,
                     this,
                     &GeometryScaleWidget::onScaleValueChanged);
    QObject::connect(this->xScaleLineEdit,
                     &ValueLineEdit::valueChanged,
                     this,
                     &GeometryScaleWidget::onScaleValueChanged);
    QObject::connect(this->yScaleLineEdit,
                     &ValueLineEdit::valueChanged,
                     this,
                     &GeometryScaleWidget::onScaleValueChanged);
    QObject::connect(this->zScaleLineEdit,
                     &ValueLineEdit::valueChanged,
                     this,
                     &GeometryScaleWidget::onScaleValueChanged);

    QObject::connect(this->scaleCenter,
                     &PositionWidget::changed,
                     this,
                     &GeometryScaleWidget::onPositionChanged);

    vBoxLayout->addStretch(10);

    Session::getInstance().setBeginDrawScaleOrigin(center);
}

GeometryScaleWidget::~GeometryScaleWidget()
{
    Session::getInstance().setEndDrawScaleOrigin();
}

void GeometryScaleWidget::onSameScaleToggled(bool checked)
{
    this->differentScaleGroupBox->setChecked(!checked);
}

void GeometryScaleWidget::onDifferentScaleToggled(bool checked)
{
    this->sameScaleGroupBox->setChecked(!checked);
}

void GeometryScaleWidget::resetSameScale(void)
{
    this->sameScaleLineEdit->setValue(this->defaultScale[0]);
}

void GeometryScaleWidget::resetDifferentScale(void)
{
    this->xScaleLineEdit->setValue(this->defaultScale[0]);
    this->yScaleLineEdit->setValue(this->defaultScale[1]);
    this->zScaleLineEdit->setValue(this->defaultScale[2]);
}

void GeometryScaleWidget::onScaleValueChanged(double)
{
    if (this->sameScaleGroupBox->isChecked())
    {
        emit this->scaleChanged(this->scaleCenter->getPosition(),
                                RR3Vector(this->sameScaleLineEdit->getValue(),
                                          this->sameScaleLineEdit->getValue(),
                                          this->sameScaleLineEdit->getValue()));
    }
    if (this->differentScaleGroupBox->isChecked())
    {
        emit this->scaleChanged(this->scaleCenter->getPosition(),
                                RR3Vector(this->xScaleLineEdit->getValue(),
                                          this->yScaleLineEdit->getValue(),
                                          this->zScaleLineEdit->getValue()));
    }
}

void GeometryScaleWidget::onPositionChanged(const RR3Vector &)
{
    RR3Vector scaleVector(this->xScaleLineEdit->getValue(),
                          this->yScaleLineEdit->getValue(),
                          this->zScaleLineEdit->getValue());
    if (this->sameScaleGroupBox->isChecked())
    {
        scaleVector.fill(this->sameScaleLineEdit->getValue());
    }
    emit this->scaleChanged(this->scaleCenter->getPosition(),scaleVector);

    Session::getInstance().setBeginDrawScaleOrigin(this->scaleCenter->getPosition());
}
