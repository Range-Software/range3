/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   new_model_dialog.cpp                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   31-st August 2012                                        *
 *                                                                   *
 *  DESCRIPTION: New model dialog class definition                   *
 *********************************************************************/

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include "new_model_dialog.h"

NewModelDialog::NewModelDialog(QWidget *parent) :
    QDialog(parent)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    this->setWindowTitle(tr("Create a new model"));

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout (mainLayout);

    QLabel *labelName = new QLabel(tr("Name:"));
    mainLayout->addWidget(labelName, 0, 0, 1, 1);

    this->editName = new QLineEdit(tr("New model"));
    mainLayout->addWidget(this->editName, 0, 1, 1, 1);

    QLabel *labelDesc = new QLabel(tr("Description:"));
    mainLayout->addWidget(labelDesc, 1, 0, 1, 1);

    this->editDesc = new QLineEdit(tr("New model"));
    mainLayout->addWidget(this->editDesc, 1, 1, 1, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout, 2, 0, 1, 2);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    okButton->setDefault(true);
    buttonsLayout->addWidget(okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&NewModelDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&NewModelDialog::accept);
}

QString NewModelDialog::getName() const
{
    return this->editName->text();
}

QString NewModelDialog::getDescription() const
{
    return this->editDesc->text();
}
