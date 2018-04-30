/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   monitoring_point_graph_dialog.cpp                        *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   28-th March 2014                                         *
 *                                                                   *
 *  DESCRIPTION: Monitoring point graph dialog class definition      *
 *********************************************************************/

#include <QGridLayout>
#include <QIcon>
#include <QPushButton>
#include <QLabel>

#include "monitoring_point_graph_dialog.h"
#include "graph_dialog.h"

MonitoringPointGraphDialog::MonitoringPointGraphDialog(const QString &monitoringFileName, QWidget *parent) :
    QDialog(parent)
{
    try
    {
        this->readDataFromFile(monitoringFileName);
    }
    catch (const RError &error)
    {
        RLogger::error("Failed to read monitoring point data from file \'%s\'. %s.\n",
                       monitoringFileName.toUtf8().constData(),
                       error.getMessage().toUtf8().constData());
    }

    QIcon closeIcon(":/icons/file/pixmaps/range-close.svg");

    this->setWindowTitle(tr("Monitoring point"));
    this->resize(600,400);

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout (mainLayout);

    uint mainLayoutRowCount = 0;

    QLabel *label = new QLabel("Source: <i>" + monitoringFileName + "</i>");
    mainLayout->addWidget(label, mainLayoutRowCount++, 0, 1, 1);

    this->monitoringPointsCombo = new QComboBox;
    QList<RMonitoringPoint> keys = this->data.keys();
    for (int i=0;i<keys.size();i++)
    {
        QString text(RVariable::getName(keys[i].getVariableType())
                     + " [" + QString::number(keys[i].getPosition()[0])
                     + "; " + QString::number(keys[i].getPosition()[1])
                     + "; " + QString::number(keys[i].getPosition()[2]) + "]");
        this->monitoringPointsCombo->addItem(text);
    }
    mainLayout->addWidget(this->monitoringPointsCombo, mainLayoutRowCount++, 0, 1, 1);

    this->connect(this->monitoringPointsCombo,
                  SIGNAL(currentIndexChanged(int)),
                  this,
                  SLOT(onCurrentIndexChanged(int)));

    this->valuesTree = new QTreeWidget;
    this->populateValuesTree();
    mainLayout->addWidget(this->valuesTree, mainLayoutRowCount++, 0, 1, 1);

    QPushButton *showGraphButton = new QPushButton(tr("Display in graph"));
    mainLayout->addWidget(showGraphButton, mainLayoutRowCount++, 0, 1, 1);

    QObject::connect(showGraphButton,&QPushButton::clicked,this,&MonitoringPointGraphDialog::onDisplayInGraphClicked);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonsLayout, mainLayoutRowCount++, 0, 1, 1);

    buttonsLayout->addStretch(1);

    QPushButton *closeButton = new QPushButton(closeIcon, tr("Close"));
    closeButton->setDefault(true);
    buttonsLayout->addWidget(closeButton);

    QObject::connect(closeButton,&QPushButton::clicked,this,&QDialog::reject);
}

int MonitoringPointGraphDialog::exec(void)
{
    int retVal = QDialog::exec();

    return retVal;
}

void MonitoringPointGraphDialog::readDataFromFile(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"Empty file name was provided");
    }

    RFile file(fileName,RFile::ASCII);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    RMonitoringPoint monitoringPoint;
    while (!file.getTextStream().atEnd())
    {
        uint iteration;
        RRVector valueVector;
        try
        {
            monitoringPoint.readRecordFromFile(file,iteration,valueVector);
        }
        catch (const RError &error)
        {
            if (file.getTextStream().atEnd())
            {
                continue;
            }
            file.close();
            throw error;
        }
        this->data[monitoringPoint].append(valueVector);
    }

    file.close();
}

void MonitoringPointGraphDialog::populateValuesTree(void)
{
    this->valuesTree->clear();

    int index = this->monitoringPointsCombo->currentIndex();
    if (index < 0)
    {
        return;
    }
    QList<RMonitoringPoint> keys = this->data.keys();
    QVector<RRVector> &values = this->data[keys[index]];

    uint nColumns = 0;
    for (int i=0;i<values.size();i++)
    {
        nColumns = std::max(nColumns,uint(values[i].size()));
    }

    this->valuesTree->setColumnCount(nColumns+1);

    QTreeWidgetItem* headerItem = new QTreeWidgetItem();
    headerItem->setText(0,"#");
    for (uint i=0;i<nColumns;i++)
    {
        headerItem->setText(i+1,QString::number(i+1));
    }
    this->valuesTree->setHeaderItem(headerItem);

    for (int i=0;i<values.size();i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(this->valuesTree);
        item->setText(0,QString::number(i+1));
        for (uint j=0;j<values[i].size();j++)
        {
            item->setText(j+1,QString::number(values[i][j]));
        }
    }

    for (uint i=0;i<nColumns+1;i++)
    {
        this->valuesTree->resizeColumnToContents(i);
    }
}

void MonitoringPointGraphDialog::displayInGraph(void)
{
    GraphObject *graphObject = new GraphObject;

    GraphData &rGraphData = graphObject->getData();

    rGraphData.setTitle(this->monitoringPointsCombo->currentText());
    rGraphData.setTypeMask(GRAPH_LINES);
    rGraphData.setXLabel(tr("Iteration"));
    rGraphData.setYLabel(tr("Value"));

    int index = this->monitoringPointsCombo->currentIndex();
    if (index >= 0)
    {
        QList<RMonitoringPoint> keys = this->data.keys();
        QVector<RRVector> &values = this->data[keys[index]];
        for (int i=0;i<values.size();i++)
        {
            RRVector &rValues = this->data[keys[index]][i];
            rGraphData[i].resize(rValues.size());
            for (uint j=0;j<rValues.size();j++)
            {
                rGraphData[i][j] = this->data[keys[index]][i][j];
            }
        }
    }

    GraphDialog graphDialog(graphObject,this);
    graphDialog.getGraphWidget()->setShowPointer(true,true,true);
    graphDialog.exec();
}

void MonitoringPointGraphDialog::onCurrentIndexChanged(int)
{
    this->populateValuesTree();
}

void MonitoringPointGraphDialog::onDisplayInGraphClicked(void)
{
    this->displayInGraph();
}
