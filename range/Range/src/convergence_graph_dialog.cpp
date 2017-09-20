/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   convergence_graph_dialog.cpp                             *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   5-th February 2014                                       *
 *                                                                   *
 *  DESCRIPTION: Convergence graph dialog class definition           *
 *********************************************************************/

#include <QGridLayout>
#include <QIcon>
#include <QPushButton>
#include <QLabel>

#include <rblib.h>

#include "convergence_graph_dialog.h"
#include "convergence_graph_object.h"
#include "graph_widget.h"

ConvergenceGraphDialog::ConvergenceGraphDialog(const QStringList &convergenceFileNames, QWidget *parent) :
    QDialog(parent)
{
    QIcon clearIcon(":/icons/file/pixmaps/range-clear.svg");
    QIcon closeIcon(":/icons/file/pixmaps/range-close.svg");

    this->setWindowTitle(tr("Solver convergence"));
    this->resize(600,400);

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout (mainLayout);

    uint mainLayoutRowCount = 0;

    QLabel *label = new QLabel("Source: <i>" + convergenceFileNames.at(0) + "</i>");
    mainLayout->addWidget(label, mainLayoutRowCount++, 0, 1, 1);

    this->dataColumnCombo = new QComboBox;
    this->dataColumnCombo->addItems(ConvergenceGraphObject::findDataColumns(convergenceFileNames.at(0)));
    this->dataColumnCombo->setCurrentIndex(0);
    mainLayout->addWidget(this->dataColumnCombo, mainLayoutRowCount++, 0, 1, 1);

    this->connect(this->dataColumnCombo,SIGNAL(currentIndexChanged(int)),SLOT(onCurrentIndexChanged(int)));

    this->graphObject = new ConvergenceGraphObject;
    this->graphObject->setSourceFileName(convergenceFileNames.at(0));
    this->graphObject->setSourceReadInterval(1000);

    GraphWidget *graphWidget = new GraphWidget(graphObject,false,this);
    mainLayout->addWidget(graphWidget, mainLayoutRowCount++, 0, 1, 1);
    graphWidget->setPaintGraphLimits(false);
    graphWidget->setPaintY0(true);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonsLayout, mainLayoutRowCount++, 0, 1, 1);

    QPushButton *clearButton = new QPushButton(clearIcon, tr("Clear"));
    buttonsLayout->addWidget(clearButton);

    buttonsLayout->addStretch(1);

    QPushButton *closeButton = new QPushButton(closeIcon, tr("Close"));
    closeButton->setDefault(true);
    buttonsLayout->addWidget(closeButton);

    QObject::connect(clearButton,&QPushButton::clicked,this,&ConvergenceGraphDialog::onClearButtonClicked);
    QObject::connect(closeButton,&QPushButton::clicked,this,&ConvergenceGraphDialog::reject);
}

int ConvergenceGraphDialog::exec(void)
{
    int retVal = QDialog::exec();

    return retVal;
}

void ConvergenceGraphDialog::onCurrentIndexChanged(int index)
{
    this->graphObject->setDataColumn(index+1,true);
}

void ConvergenceGraphDialog::onClearButtonClicked(void)
{
    try
    {
        this->graphObject->clearSourceFile();
    }
    catch (RError &error)
    {
        RLogger::error("Failed to clear graph source. %s\n",error.getMessage().toUtf8().constData());
    }
}
