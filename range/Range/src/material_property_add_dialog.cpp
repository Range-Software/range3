/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   material_property_add_dialog.cpp                         *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   30-th August 2012                                        *
 *                                                                   *
 *  DESCRIPTION: Material property add dialog class definition       *
 *********************************************************************/

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include "material_property_add_dialog.h"

MaterialPropertyAddDialog::MaterialPropertyAddDialog(RMaterial &material, QWidget *parent) :
    QDialog(parent),
    pMaterial(&material)
{
    this->setWindowTitle(QString("Material property editor"));
    this->resize(300,300);

    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    int mainLayoutRow = 0;

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout (mainLayout);

    QLabel *label = new QLabel(tr("Select one or more material properties from the list below."));
    mainLayout->addWidget(label, mainLayoutRow++, 0, 1, 1);

    this->propertyList = new QListWidget;
    this->propertyList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mainLayout->addWidget(this->propertyList, mainLayoutRow++, 0, 1, 1);

    QList<RMaterialPropertyType> properties = this->findAvailableProperties(material);
    for (int i=0;i<properties.size();i++)
    {
        QListWidgetItem *item = new QListWidgetItem(this->propertyList);
        item->setText(RMaterialProperty::getName(properties[i]));
        item->setData(Qt::UserRole,QVariant(int(properties[i])));
    }

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout, mainLayoutRow++, 0, 1, 1);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    buttonsLayout->addWidget(okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&MaterialPropertyAddDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&MaterialPropertyAddDialog::accept);
}

int MaterialPropertyAddDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted && this->pMaterial)
    {
        QList<QListWidgetItem*> items = this->propertyList->selectedItems();
        for (int i=0;i<items.size();i++)
        {
            RMaterialPropertyType type = RMaterialPropertyType(items[i]->data(Qt::UserRole).toInt());
            this->pMaterial->add(RMaterialProperty(type));
        }
    }
    return retVal;
}

QList<RMaterialPropertyType> MaterialPropertyAddDialog::findAvailableProperties(const RMaterial &material) const
{
    QList<RMaterialPropertyType> propertyTypes;

    for (int i=int(R_MATERIAL_PROPERTY_NONE);
         i<int(R_MATERIAL_PROPERTY_N_TYPES);
         i++)
    {
        if (RMaterialPropertyType(i) == R_MATERIAL_PROPERTY_NONE ||
            RMaterialPropertyType(i) == R_MATERIAL_PROPERTY_CUSTOM)
        {
            continue;
        }
        if (material.findPosition(RMaterialPropertyType(i)) >= material.size())
        {
            propertyTypes.push_back(RMaterialPropertyType(i));
        }
    }

    return propertyTypes;
}
