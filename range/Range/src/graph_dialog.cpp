/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   graph_dialog.cpp                                         *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   22-nd March 2013                                         *
 *                                                                   *
 *  DESCRIPTION: Graph dialog class definition                       *
 *********************************************************************/

#include <QGridLayout>
#include <QIcon>
#include <QPushButton>

#include "graph_dialog.h"

GraphDialog::GraphDialog(GraphObject *graphObject, QWidget *parent) :
    QDialog(parent)
{
    QIcon closeIcon(":/icons/file/pixmaps/range-close.svg");

    this->setWindowTitle(tr("Graph"));
    this->resize(600,400);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout (mainLayout);

    this->graphWidget = new GraphWidget(graphObject,true,this);
    mainLayout->addWidget(this->graphWidget);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonsLayout);

    buttonsLayout->addStretch(1);

    QPushButton *closeButton = new QPushButton(closeIcon, tr("Close"));
    closeButton->setDefault(true);
    buttonsLayout->addWidget(closeButton);

    QObject::connect(closeButton,&QPushButton::clicked,this,&QDialog::reject);
}

GraphWidget *GraphDialog::getGraphWidget(void)
{
    return this->graphWidget;
}
