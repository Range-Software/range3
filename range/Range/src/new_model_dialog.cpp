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

#include "model.h"
#include "new_model_dialog.h"
#include "model_io.h"
#include "job_manager.h"

NewModelDialog::NewModelDialog(QWidget *parent) :
    QDialog(parent)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    this->setWindowTitle(tr("Create a new model"));
    this->resize(300,-1);

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout (mainLayout);

    QLabel *labelName = new QLabel(tr("Name:"));
    mainLayout->addWidget(labelName, 0, 0, 1, 1);

    this->editName = new QLineEdit;
    this->editName->setPlaceholderText(tr("New model name"));
    mainLayout->addWidget(this->editName, 0, 1, 1, 1);

    QLabel *labelDesc = new QLabel(tr("Description:"));
    mainLayout->addWidget(labelDesc, 1, 0, 1, 1);

    this->editDesc = new QLineEdit;
    this->editDesc->setPlaceholderText(tr("New model description"));
    mainLayout->addWidget(this->editDesc, 1, 1, 1, 1);

    QSpacerItem *spacer = new QSpacerItem(-1,-1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    mainLayout->addItem(spacer, 2, 0, 1, 2);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout, 3, 0, 1, 2);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    okButton->setDefault(true);
    buttonsLayout->addWidget(okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&NewModelDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&NewModelDialog::accept);
}

int NewModelDialog::exec(void)
{
    int retVal = this->QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        Model *pNewModel = new Model;
        pNewModel->setName(this->getName());
        pNewModel->setDescription(this->getDescription());

        JobManager::getInstance().submit(new ModelIO(MODEL_IO_ADD, QString(), pNewModel));
    }
    return retVal;
}

QString NewModelDialog::getName(void) const
{
    return this->editName->text().isEmpty() ? tr("New model") : this->editName->text();
}

QString NewModelDialog::getDescription(void) const
{
    return this->editDesc->text().isEmpty() ? tr("New model") : this->editDesc->text();
}
