/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   problem_selector_dialog.cpp                              *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   4-th February 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Problem selector dialog class definition            *
 *********************************************************************/

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include "problem_selector_dialog.h"
#include "session.h"

ProblemSelectorDialog::ProblemSelectorDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle(QString("Problem type selector"));

    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    int mainLayoutRow = 0;

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout (mainLayout);

    QLabel *messageLabel = new QLabel("Select problem types");
    mainLayout->addWidget(messageLabel, mainLayoutRow++, 0);

    this->problemTree = new ProblemSelectorTree(this);
    this->problemTree->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    this->problemTree->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);
    mainLayout->addWidget(this->problemTree, mainLayoutRow++, 0);

    QObject::connect(this->problemTree,&ProblemSelectorTree::changed,this,&ProblemSelectorDialog::onProblemTreeChanged);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout, mainLayoutRow++, 0, 1, 1);

    this->cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(this->cancelButton);

    this->okButton = new QPushButton(okIcon, tr("Ok"));
    this->okButton->setDisabled(true);
    this->okButton->setDefault(true);
    buttonsLayout->addWidget(this->okButton);

    QObject::connect(this->cancelButton,&QPushButton::clicked,this,&ProblemSelectorDialog::reject);
    QObject::connect(this->okButton,&QPushButton::clicked,this,&ProblemSelectorDialog::accept);
}

RProblemTypeMask ProblemSelectorDialog::exec(void)
{
    if (QDialog::exec() == QDialog::Accepted)
    {
        return this->problemTree->findProblemTypeMask();
    }

    return R_PROBLEM_NONE;
}

void ProblemSelectorDialog::onProblemTreeChanged(void)
{
    this->okButton->setEnabled(this->problemTree->checkProblemIsChecked());
}
