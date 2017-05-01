/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   log_browser_dialog.cpp                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   9-th August 2013                                         *
 *                                                                   *
 *  DESCRIPTION: Log browser dialog class definition                 *
 *********************************************************************/

#include <QGridLayout>
#include <QIcon>
#include <QPushButton>
#include <QLabel>
#include <QFile>

#include <rblib.h>

#include "log_browser_dialog.h"

LogBrowserDialog::LogBrowserDialog(const QString &logFileName, QWidget *parent) :
    QDialog(parent)
{
    QIcon clearIcon(":/icons/file/pixmaps/range-clear.svg");
    QIcon closeIcon(":/icons/file/pixmaps/range-close.svg");

    this->setWindowTitle(tr("Log file viewer"));

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout (mainLayout);

    QLabel *label = new QLabel("Log: <i>" + logFileName + "</i>");
    mainLayout->addWidget(label, 0, 0, 1, 1);

    this->logBrowser = new LogBrowser(logFileName,this);
    mainLayout->addWidget(this->logBrowser, 1, 0, 1, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonsLayout, 2, 0, 1, 1);

    QPushButton *clearButton = new QPushButton(clearIcon, tr("Clear"));
    buttonsLayout->addWidget(clearButton);

    buttonsLayout->addStretch(1);

    QPushButton *closeButton = new QPushButton(closeIcon, tr("Close"));
    closeButton->setDefault(true);
    buttonsLayout->addWidget(closeButton);

    connect(clearButton,
            &QPushButton::clicked,
            this,
            &LogBrowserDialog::onClearButtonClicked);

    connect(closeButton,
            &QPushButton::clicked,
            this,
            &LogBrowserDialog::reject);
}

int LogBrowserDialog::exec(void)
{
    int retVal = QDialog::exec();

    return retVal;
}

void LogBrowserDialog::onClearButtonClicked(void)
{
    this->logBrowser->clearLog();
}
