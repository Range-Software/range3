/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   component_edit_dialog.cpp                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   2-nd August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: Component edit dialog class definition              *
 *********************************************************************/

#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>

#include "component_edit_dialog.h"
#include "session.h"

ComponentEditDialog::ComponentEditDialog(RConditionComponent &component, QWidget *parent) :
    QDialog(parent),
    pComponent(&component)
{
    this->setWindowTitle(QString("Component editor"));
    this->resize(400,500);

    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    int mainLayoutRow = 0;

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout (mainLayout);

    QHBoxLayout *nValuesLayout = new QHBoxLayout;
    mainLayout->addLayout(nValuesLayout, mainLayoutRow++, 0, 1, 1);

    QLabel *labelNValues = new QLabel(tr("Number of values:"));
    nValuesLayout->addWidget(labelNValues);

    QSpinBox *spinNValues = new QSpinBox;
    spinNValues->setRange(R_TIME_STEP_MIN_NUMBER,R_TIME_STEP_MAX_NUMBER);
    spinNValues->setValue(component.size());
    nValuesLayout->addWidget(spinNValues);

    nValuesLayout->addStretch(1);

    this->editTable = new ComponentEditTable(component, parent);
    mainLayout->addWidget(this->editTable, mainLayoutRow++, 0, 1, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout, mainLayoutRow++, 0, 1, 1);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    okButton->setDefault(true);
    buttonsLayout->addWidget(okButton);

    this->connect(spinNValues,SIGNAL(valueChanged(int)),SLOT(onSpinNValuesChanged(int)));

    QObject::connect(cancelButton,&QPushButton::clicked,this,&ComponentEditDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&ComponentEditDialog::accept);
}

int ComponentEditDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted && this->pComponent)
    {
        this->pComponent->clear();
        for (int i=0;i<this->editTable->rowCount();i++)
        {
            this->pComponent->add(this->editTable->getKey(i),
                                  this->editTable->getValue(i));
        }
    }
    return retVal;
}

void ComponentEditDialog::onSpinNValuesChanged(int nValues)
{
    int oldRowCount = this->editTable->rowCount();

    if (oldRowCount > nValues)
    {
        this->editTable->setRowCount(nValues);
        return;
    }

    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();
    double timeStepSize = R_TIME_STEP_DEFAULT_SIZE;
    if (modelIDs.size() > 0)
    {
        timeStepSize = Session::getInstance().getModel(modelIDs[0]).getTimeSolver().getInputTimeStepSize();
    }

    double currTime = this->editTable->getKey(oldRowCount-1);
    double value = this->editTable->getValue(oldRowCount-1);

    for (int i=oldRowCount;i<nValues;i++)
    {
        currTime += timeStepSize;
        this->editTable->addValue(currTime,value);
    }
}
