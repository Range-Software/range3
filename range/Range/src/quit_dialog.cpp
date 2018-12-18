/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   quit_dialog.cpp                                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   28-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Quit dialog class definition                        *
 *********************************************************************/

#include <QIcon>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>

#include "quit_dialog.h"
#include "solver_manager.h"

QuitDialog::QuitDialog (QWidget *parent) : QDialog(parent)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon quitIcon(":/icons/file/pixmaps/range-shutdown.svg");

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    connect (cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QPushButton *quitButton = new QPushButton(quitIcon, tr("Quit"));
    quitButton->setDefault(true);
    connect (quitButton, SIGNAL(clicked()), this, SLOT(accept()));

    QString quitMessage(tr("Are you sure you want to quit?"));
    if (SolverManager::getInstance().getNRunning() > 0)
    {
        quitMessage += "<br/>" + tr("If you quit now solver will continue executing in the background.");
    }

    QLabel *quitLabel = new QLabel(quitMessage);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addWidget(quitButton);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(quitLabel, 0, 0);
    mainLayout->addLayout(buttonsLayout, 1, 0, 1, 1);

    this->setLayout (mainLayout);

    this->setWindowTitle(tr("Quit?"));

} /* QuitDialog::QuitDialog */

