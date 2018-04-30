/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rename_model_dialog.cpp                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   3-rd February 2018                                       *
 *                                                                   *
 *  DESCRIPTION: Rename model dialog class definition                *
 *********************************************************************/

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include "rename_model_dialog.h"
#include "session.h"

RenameModelDialog::RenameModelDialog(uint modelId, QWidget *parent)
	: QDialog(parent)
    , modelId(modelId)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    this->setWindowTitle(tr("Rename model"));

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout (mainLayout);

    QLabel *labelName = new QLabel(tr("Name:"));
    mainLayout->addWidget(labelName, 0, 0, 1, 1);

    this->editName = new QLineEdit(Session::getInstance().getModel(this->modelId).getName());
    this->editName->setPlaceholderText(tr("Model name"));
    mainLayout->addWidget(this->editName, 0, 1, 1, 1);

    QLabel *labelDesc = new QLabel(tr("Description:"));
    mainLayout->addWidget(labelDesc, 1, 0, 1, 1);

    this->editDesc = new QLineEdit(Session::getInstance().getModel(this->modelId).getDescription());
    this->editDesc->setPlaceholderText(tr("Model description"));
    mainLayout->addWidget(this->editDesc, 1, 1, 1, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout, 2, 0, 1, 2);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    okButton->setDefault(true);
    buttonsLayout->addWidget(okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&RenameModelDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&RenameModelDialog::accept);
}

int RenameModelDialog::exec(void)
{
    int retVal = this->QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        Session::getInstance().getModel(this->modelId).setName(this->getName());
        Session::getInstance().getModel(this->modelId).setDescription(this->getDescription());
        Session::getInstance().setModelChanged(this->modelId);
    }
    return retVal;
}

QString RenameModelDialog::getName(void) const
{
    return this->editName->text().isEmpty() ? tr("New model") : this->editName->text();
}

QString RenameModelDialog::getDescription(void) const
{
    return this->editDesc->text().isEmpty() ? tr("New model") : this->editDesc->text();
}
