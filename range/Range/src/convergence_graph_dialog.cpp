/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   convergence_graph_dialog.cpp                             *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   5-th February 2014                                       *
 *                                                                   *
 *  DESCRIPTION: Convergence graph dialog class definition           *
 *********************************************************************/

#include <QVBoxLayout>
#include <QIcon>
#include <QPushButton>
#include <QLabel>
#include <QTabWidget>

#include <rmlib.h>

#include "convergence_graph_dialog.h"
#include "convergence_graph_widget.h"

ConvergenceGraphDialog::ConvergenceGraphDialog(const QStringList &convergenceFileNames, QWidget *parent) :
    QDialog(parent)
{
    QIcon closeIcon(":/icons/file/pixmaps/range-close.svg");

    this->setWindowTitle(tr("Solver convergence"));
    this->resize(600,400);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    QTabWidget *tabWidget = new QTabWidget;
    mainLayout->addWidget(tabWidget);

    for (int i=0;i<convergenceFileNames.size();i++)
    {
        ConvergenceGraphWidget *convergenceGraphWidget = new ConvergenceGraphWidget(convergenceFileNames.at(i),this);
        tabWidget->addTab(convergenceGraphWidget,ConvergenceGraphDialog::getNameFromID(convergenceFileNames.at(i),QString::number(i+1)));
    }

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonsLayout);
    buttonsLayout->addStretch(1);

    QPushButton *closeButton = new QPushButton(closeIcon, tr("Close"));
    closeButton->setDefault(true);
    buttonsLayout->addWidget(closeButton);

    QObject::connect(closeButton,&QPushButton::clicked,this,&ConvergenceGraphDialog::close);
}

QString ConvergenceGraphDialog::getNameFromID(const QString &fileName, const QString &fallbackName)
{
    std::vector<RProblemType> problemTypes = RProblem::getTypes(R_PROBLEM_ALL);
    for (uint i=0;i<problemTypes.size();i++)
    {
        if (fileName.contains("-" + RProblem::getId(problemTypes[i]) + "."))
        {
            return RProblem::getName(problemTypes[i]);
        }
    }

    for (RMatrixSolverType type=RMatrixSolverConf::None;type<RMatrixSolverConf::NTypes;type++)
    {
        if (fileName.contains(RMatrixSolverConf::getId(type)))
        {
            return RMatrixSolverConf::getName(type);
        }
    }

    return fallbackName;
}
