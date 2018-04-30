/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   help_center_dialog.cpp                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   26-th November 2017                                      *
 *                                                                   *
 *  DESCRIPTION: Help center dialog class definition                 *
 *********************************************************************/

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QPushButton>

#include "help_center_dialog.h"
#include "help_general_widget.h"
#include "help_actions_widget.h"
#include "help_tutorial_widget.h"
#include "help_tips_widget.h"
#include "main_settings.h"

HelpCenterDialog::HelpCenterDialog(QWidget *parent)
    : QDialog(parent)
{
    QIcon closeIcon(":/icons/file/pixmaps/range-close.svg");

    this->setWindowTitle(tr("Help"));
    this->resize(1300,700);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    QTabWidget *tabWidget = new QTabWidget;
    mainLayout->addWidget(tabWidget);

    HelpGeneralWidget *helpGeneralWidget = new HelpGeneralWidget;
    tabWidget->addTab(helpGeneralWidget,tr("General"));

    HelpActionsWidget *helpActionsWidget = new HelpActionsWidget(MainSettings::getInstance().getApplicationSettings()->getActionDefinition());
    tabWidget->addTab(helpActionsWidget,tr("Actions"));

    HelpTutorialWidget *helpTutorialWidget = new HelpTutorialWidget;
    tabWidget->addTab(helpTutorialWidget,tr("Tutorials"));

    HelpTipsWidget *helpTipsWidget = new HelpTipsWidget;
    tabWidget->addTab(helpTipsWidget,tr("Tips and tricks"));

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonsLayout);
    buttonsLayout->addStretch(1);

    QPushButton *closeButton = new QPushButton(closeIcon, tr("Close"));
    closeButton->setDefault(true);
    buttonsLayout->addWidget(closeButton);

    QObject::connect(closeButton,&QPushButton::clicked,this,&HelpCenterDialog::close);
}
