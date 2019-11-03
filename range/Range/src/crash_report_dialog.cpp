/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   crash_report_dialog.cpp                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   25-th October 2019                                       *
 *                                                                   *
 *  DESCRIPTION: Crash report dialog class definition                *
 *********************************************************************/

#include <QFile>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>

#include "crash_report_dialog.h"
#include "rra_session.h"

CrashReportDialog::CrashReportDialog(QWidget *parent, const QString &logFile)
    : QDialog(parent)
{
    bool hasLogFile = QFile::exists(logFile);

    QIcon closeIcon(":/icons/file/pixmaps/range-close.svg");
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");
    QIcon importantIcon(":/icons/file/pixmaps/range-important.svg");

    this->setWindowTitle(tr("Crash report"));
    this->resize(900,600);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    QHBoxLayout *titleLayout = new QHBoxLayout;
    mainLayout->addLayout(titleLayout);

    QLabel *titlePixmapLabel = new QLabel;
    titlePixmapLabel->setPixmap(importantIcon.pixmap(64,64));
    titleLayout->addWidget(titlePixmapLabel);

    QLabel *titleLabel = new QLabel(tr("Previous instance has terminated unexpectedly.") + "\n" + tr("Would you like to report the crash?"));
    titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    titleLayout->addWidget(titleLabel);

    if (hasLogFile)
    {
        QGroupBox *logGroupBox = new QGroupBox(tr("Log file from crashed instance"));
        logGroupBox->setCheckable(false);
        logGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        mainLayout->addWidget(logGroupBox);

        QVBoxLayout *logGroupLayout = new QVBoxLayout;
        logGroupBox->setLayout(logGroupLayout);

        this->logBrowser = new LogBrowser(logFile);
        logGroupLayout->addWidget(logBrowser);
    }

    QGroupBox *userGroupBox = new QGroupBox(tr("Additional information"));
    userGroupBox->setCheckable(false);
    userGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(userGroupBox);

    QVBoxLayout *userGroupLayout = new QVBoxLayout;
    userGroupBox->setLayout(userGroupLayout);

    this->textEdit = new QPlainTextEdit;
    userGroupLayout->addWidget(this->textEdit);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Report"));
    okButton->setDefault(true);
    buttonsLayout->addWidget(okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&QDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&QDialog::accept);
}

int CrashReportDialog::exec()
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        QString crashReport = "LOG BEGIN:\n"
                            + logBrowser->toPlainText()
                            + "\nLOG END\n"
                            + "USER BEGIN\n"
                            + textEdit->toPlainText()
                            + "\nUSER END\n";
        RRASession::getInstance().submitCrashReport(crashReport);
    }
    return retVal;
}
