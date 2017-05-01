/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   problem_task_dialog.cpp                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   5-th November 2015                                       *
 *                                                                   *
 *  DESCRIPTION: Problem task dialog class definition                *
 *********************************************************************/

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "session.h"
#include "problem_task_dialog.h"

ProblemTaskDialog::ProblemTaskDialog(uint modelID, QWidget *parent)
    : QDialog(parent)
    , modelID(modelID)
{
    Model &rModel = Session::getInstance().getModel(this->modelID);

    this->setWindowTitle(QString("Problem task flow"));
    this->resize(600,300);

    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout (mainLayout);

    QLabel *messageLabel = new QLabel;
    messageLabel->setText(tr("Problem task flow for model:") + " <b>" + rModel.getName() + "</b>");
    mainLayout->addWidget(messageLabel);

    this->taskTree = new ProblemTaskTree(rModel.getProblemTaskTree(),this);
    mainLayout->addWidget(this->taskTree);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout);

    this->cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    this->cancelButton->setDisabled(rModel.getProblemTaskTree().getProblemTypeMask() == R_PROBLEM_NONE);
    buttonsLayout->addWidget(this->cancelButton);

    this->okButton = new QPushButton(okIcon, tr("Ok"));
    this->okButton->setDisabled(true);
    this->okButton->setDefault(true);
    buttonsLayout->addWidget(this->okButton);

    QObject::connect(this->taskTree,&ProblemTaskTree::changed,this,&ProblemTaskDialog::onProblemTaskTreeChanged);
    QObject::connect(this->cancelButton,&QPushButton::clicked,this,&ProblemTaskDialog::reject);
    QObject::connect(this->okButton,&QPushButton::clicked,this,&ProblemTaskDialog::accept);
}

int ProblemTaskDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        Session::getInstance().getModel(this->modelID).setProblemTaskTree(this->taskTree->getProblemTaskTree());

        RLogger::info("Problem task flow have changed for model \'%s\'\n", Session::getInstance().getModel(this->modelID).getName().toUtf8().constData());

        Session::getInstance().setProblemSelectionChanged(this->modelID);
    }
    return retVal;
}

void ProblemTaskDialog::closeEvent(QCloseEvent *e)
{
    if (Session::getInstance().getModel(this->modelID).getProblemTaskTree().getProblemTypeMask() == R_PROBLEM_NONE)
    {
        e->ignore();
    }
    else
    {
        QDialog::closeEvent(e);
    }
}

void ProblemTaskDialog::keyPressEvent(QKeyEvent *e)
{
    // Ignore ESC key.
    if (e->key() == Qt::Key_Escape && (Session::getInstance().getModel(this->modelID).getProblemTaskTree().getProblemTypeMask() == R_PROBLEM_NONE))
    {
        e->ignore();
    }
    else
    {
        QDialog::keyPressEvent(e);
    }
}

void ProblemTaskDialog::onProblemTaskTreeChanged(void)
{
    RProblemTaskItem problemTaskTree = this->taskTree->getProblemTaskTree();
    this->okButton->setEnabled(problemTaskTree.getNChildren() > 0);
}

