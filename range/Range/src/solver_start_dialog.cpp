/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   solver_start_dialog.cpp                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   19-th September 2017                                     *
 *                                                                   *
 *  DESCRIPTION: Solver start dialog class definition                *
 *********************************************************************/

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QIcon>
#include <QPushButton>
#include <QTextBrowser>
#include <QScrollBar>

#include "main_settings.h"
#include "model_io.h"
#include "session.h"
#include "solver_manager.h"
#include "solver_setup_checker.h"
#include "solver_start_dialog.h"
#include "solver_task.h"

SolverStartDialog::SolverStartDialog(uint modelID, QWidget *parent)
    : QDialog(parent)
    , modelID(modelID)
{
    SolverSetupChecker solverSetupChecker(Session::getInstance().getModel(this->modelID));
    solverSetupChecker.perform(this->warnings,this->errors);

    this->setWindowTitle(QString("Start solver"));
    this->resize(600,300);

    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout (mainLayout);

    const Model &rModel = Session::getInstance().getModel(this->modelID);

    QLabel *messageLabel = new QLabel;
    messageLabel->setText(tr("Start solver for model:") + " <b>" + rModel.getName() + "</b>");
    mainLayout->addWidget(messageLabel);

    if (this->warnings.size() || this->errors.size())
    {
//        QGroupBox *messageBox = new QGroupBox(tr("Warnings and/or Errors"));
//        mainLayout->addWidget(messageBox);

//        QVBoxLayout *messagesLayout = new QVBoxLayout;
//        messageBox->setLayout(messagesLayout);

        QTextBrowser *messagesBrowser = new QTextBrowser;
        mainLayout->addWidget(messagesBrowser);

        QString htmlMessage;

        if (this->warnings.size() > 0)
        {
            htmlMessage = "<h3>" + tr("Warnings") + "</h3>";
            htmlMessage += "<ul>";
            for (int i=0;i<this->warnings.size();i++)
            {
                htmlMessage += "<li>";
                htmlMessage +=this->warnings.at(i);
                htmlMessage += "</li>";
            }
            htmlMessage += "</ul>";
        }

        if (this->errors.size() > 0)
        {
            htmlMessage = "<h3>" + tr("Errors") + "</h3>";
            htmlMessage += "<ul>";
            for (int i=0;i<this->errors.size();i++)
            {
                htmlMessage += "<li>";
                htmlMessage +=this->errors.at(i);
                htmlMessage += "</li>";
            }
            htmlMessage += "</ul>";
        }

        messagesBrowser->insertHtml(htmlMessage);

        QTextCursor textCursor = messagesBrowser->textCursor();
        textCursor.movePosition(QTextCursor::Start);
        messagesBrowser->setTextCursor(textCursor);
    }

    this->restartSolverCheck = new QCheckBox("Restart solver / continue");
    this->restartSolverCheck->setEnabled(Session::getInstance().getModel(this->modelID).getNVariables() > 0);
    mainLayout->addWidget(this->restartSolverCheck);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    okButton->setDisabled(this->errors.size() > 0);
    okButton->setDefault(true);
    buttonsLayout->addWidget(okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&SolverStartDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&SolverStartDialog::accept);
}

int SolverStartDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        Session::getInstance().getModel(this->modelID).getProblemSetup().setRestart(this->restartSolverCheck->isChecked());

        // Start solver task
        QString modelFileName = ModelIO::getModelSaveName(MainSettings::getInstance(),
                                                          this,
                                                          Session::getInstance().getModel(this->modelID));
        Session::getInstance().getModel(this->modelID).setFileName(modelFileName);

        SolverTask *solverTask = new SolverTask(MainSettings::getInstance().getApplicationSettings(),this->modelID);
        solverTask->setBlocking(false);
        SolverManager::getInstance().submit(solverTask);
    }

    return retVal;
}
