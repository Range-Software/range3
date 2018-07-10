/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   material_property_edit_dialog.cpp                        *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   2-nd August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: Material property edit dialog class definition      *
 *********************************************************************/

#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>

#include "material_property_edit_dialog.h"

MaterialPropertyEditDialog::MaterialPropertyEditDialog(RMaterialProperty &property, QWidget *parent) :
    QDialog(parent),
    pProperty(&property)
{
    this->setWindowTitle(QString("Material property editor"));
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

    this->spinNValues = new QSpinBox;
    this->spinNValues->setMinimum(1);
    this->spinNValues->setValue(property.size());
    nValuesLayout->addWidget(this->spinNValues);

    this->connect(this->spinNValues,SIGNAL(valueChanged(int)),SLOT(onSpinNValuesChanged(int)));

    nValuesLayout->addStretch(1);

    this->editTable = new MaterialPropertyEditTable(property, parent);
    mainLayout->addWidget(this->editTable, mainLayoutRow++, 0, 1, 1);

    QObject::connect(this->editTable,&ValueTable::sizeChanged,this,&MaterialPropertyEditDialog::onTableSizeChanged);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout, mainLayoutRow++, 0, 1, 1);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    buttonsLayout->addWidget(okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&MaterialPropertyEditDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&MaterialPropertyEditDialog::accept);
}

int MaterialPropertyEditDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted && this->pProperty)
    {
        this->pProperty->clear();
        for (int i=0;i<this->editTable->rowCount();i++)
        {
            this->pProperty->add(this->editTable->getKey(i),
                                 this->editTable->getValue(i));
        }
    }
    return retVal;
}

void MaterialPropertyEditDialog::onTableSizeChanged(int nRows)
{
    this->spinNValues->setValue(nRows);
}

void MaterialPropertyEditDialog::onSpinNValuesChanged(int nValues)
{
     int oldRowCount = this->editTable->rowCount();

     if (oldRowCount > nValues)
     {
         this->editTable->setRowCount(nValues);
         return;
     }

     double keyIncrement = 1.0;

     double key = this->editTable->getKey(oldRowCount-1);
     double value = this->editTable->getValue(oldRowCount-1);

     for (int i=oldRowCount;i<nValues;i++)
     {
         key += keyIncrement;
         this->editTable->addValue(key,value);
     }
}
