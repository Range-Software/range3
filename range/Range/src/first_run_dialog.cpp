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
#include <QCheckBox>
#include <QLabel>
#include <QIcon>

#include "first_run_dialog.h"
#include "main_settings.h"

FirstRunDialog::FirstRunDialog(QWidget *parent)
    : QDialog(parent)
{
    QIcon preferencesIcon(":/icons/file/pixmaps/range-application_settings.svg");
    QIcon startIcon(":/icons/file/pixmaps/range-startup.svg");
    QIcon rangeIcon(":/icons/logos/pixmaps/range-logo-128.png");

    this->setWindowTitle(tr("First run"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    QHBoxLayout *logoLayout = new QHBoxLayout;
    mainLayout->addLayout(logoLayout);

    QLabel *logoLabel = new QLabel;
    logoLabel->setPixmap(rangeIcon.pixmap(QSize(128,128)));
    logoLayout->addWidget(logoLabel);

    QString labelMsg = "<h2>" + RVendor::name + "</h2>"
                     + "<h3>" + RVendor::description + "</h3>"
                     + "<p>" + tr("version") + ": " + RVendor::version.toString() + "</p>"
                     + "<p><strong>" + tr("Welcome") + "!</strong></p>";

    QLabel *label = new QLabel(labelMsg);
    logoLayout->addWidget(label);

    QLabel *sendUsageInfoLabel = new QLabel(tr("You can allow the sending of usage info from your computer so that Range Software can receive bug reports and statistics that help improve our software. None of the information gathered includes personal data."));
    sendUsageInfoLabel->setWordWrap(true);
    mainLayout->addWidget(sendUsageInfoLabel);

    QCheckBox *sendUsageInfoCheck = new QCheckBox(tr("Send usage info"));
    sendUsageInfoCheck->setCheckState(MainSettings::getInstance().getApplicationSettings()->getSendUsageInfo()?Qt::Checked:Qt::Unchecked);
    mainLayout->addWidget(sendUsageInfoCheck);

    QObject::connect(sendUsageInfoCheck,&QCheckBox::toggled,this,&FirstRunDialog::onSendUsageInfoToggled);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonsLayout);

    buttonsLayout->addStretch(1);

    QPushButton *closeButton = new QPushButton(startIcon, tr("Start"));
    buttonsLayout->addWidget(closeButton);
    QObject::connect(closeButton,&QPushButton::clicked,this,&FirstRunDialog::accept);
}

void FirstRunDialog::onSendUsageInfoToggled(bool checked)
{
    MainSettings::getInstance().getApplicationSettings()->setSendUsageInfo(checked);
    MainSettings::getInstance().getApplicationSettings()->setRangeApiAllowed(true);
}
