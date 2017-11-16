/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   first_run_dialog.cpp                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   16-th November 2017                                      *
 *                                                                   *
 *  DESCRIPTION: First run dialog class definition                   *
 *********************************************************************/

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QIcon>

#include "first_run_dialog.h"
#include "application_settings_dialog.h"
#include "main_settings.h"

FirstRunDialog::FirstRunDialog(QWidget *parent)
    : QDialog(parent)
{
    QIcon closeIcon(":/icons/file/pixmaps/range-close.svg");

    this->setWindowTitle(tr("First run"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    QPushButton *applicationSettingsButton = new QPushButton(tr("application settings"));
    mainLayout->addWidget(applicationSettingsButton);

    QObject::connect(applicationSettingsButton,&QPushButton::clicked,this,&FirstRunDialog::onApplicationSettingsButtonClicked);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout);

    QPushButton *closeButton = new QPushButton(closeIcon, tr("Close"));
    buttonsLayout->addWidget(closeButton);

    QObject::connect(closeButton,&QPushButton::clicked,this,&FirstRunDialog::accept);
}

void FirstRunDialog::onApplicationSettingsButtonClicked(bool checked)
{
    ApplicationSettingsDialog(MainSettings::getInstance().getApplicationSettings(),this).exec();
}
