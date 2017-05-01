/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   model_statistics_dialog.cpp                              *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   24-th November 2016                                      *
 *                                                                   *
 *  DESCRIPTION: Model statistics dialog class definition            *
 *********************************************************************/

#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "model_statistics_dialog.h"
#include "session.h"

ModelStatisticsDialog::ModelStatisticsDialog(uint modelID, QWidget *parent)
    : QDialog(parent)
{
    QIcon closeIcon(":/icons/file/pixmaps/range-close.svg");

    this->setWindowTitle(tr("Model statistics"));
    this->resize(860,600);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout (mainLayout);

    QTextBrowser *textBrowser = new QTextBrowser;
    this->produceStatistics(modelID,textBrowser);
    textBrowser->setLineWrapMode(QTextEdit::NoWrap);
    QTextCursor textCursor = textBrowser->textCursor();
    textCursor.setPosition(0);
    textBrowser->setTextCursor(textCursor);
    mainLayout->addWidget(textBrowser);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout);

    QPushButton *closeButton = new QPushButton(closeIcon, tr("Close"));
    closeButton->setDefault(true);
    buttonsLayout->addWidget(closeButton);

    QObject::connect(closeButton,&QPushButton::clicked,this,&ModelStatisticsDialog::reject);
}

int ModelStatisticsDialog::exec(void)
{
    int retVal = QDialog::exec();
    return retVal;
}

void ModelStatisticsDialog::produceStatistics(uint modelID, QTextBrowser *textBrowser)
{
    const Model &rModel = Session::getInstance().getModel(modelID);

    QString content;

    content += "<h1>" + rModel.getName() + "</h1>";
    content += "<table  border=\"0\">";
    content += ModelStatisticsDialog::generateTableRow(tr("Number of nodes"),QString::number(rModel.getNNodes()));
    content += ModelStatisticsDialog::generateTableRow(tr("Number&nbsp;of&nbsp;elements"),QString::number(rModel.getNElements()));

    RStatistics nodeDistanceStatistics = rModel.findNodeDistanceStatistics();

    content += "<tr><th align=\"center\" colspan=\"2\">"+ tr("Node distance statistics") +"</th></tr>";

    content += ModelStatisticsDialog::generateTableRow(tr("Minimum"),QString::number(nodeDistanceStatistics.getMin()));
    content += ModelStatisticsDialog::generateTableRow(tr("Maximum"),QString::number(nodeDistanceStatistics.getMax()));
    content += ModelStatisticsDialog::generateTableRow(tr("Average"),QString::number(nodeDistanceStatistics.getAvg()));
    content += ModelStatisticsDialog::generateTableRow(tr("Median"),QString::number(nodeDistanceStatistics.getMed()));
    content += ModelStatisticsDialog::generateTableRow(tr("Percentile") +" 5",QString::number(nodeDistanceStatistics.getP05()));
    content += ModelStatisticsDialog::generateTableRow(tr("Percentile") +" 95",QString::number(nodeDistanceStatistics.getP95()));

    content += "<tr><th align=\"center\" colspan=\"2\">"+ tr("Geometry statistics") +"</th></tr>";

    double lineLength(0.0);
    for (uint i=0;i<rModel.getNLines();i++)
    {
        lineLength += rModel.getLine(i).findLength(rModel.getNodes(),rModel.getElements());
    }
    double surfaceArea(0.0);
    for (uint i=0;i<rModel.getNSurfaces();i++)
    {
        surfaceArea += rModel.getSurface(i).findArea(rModel.getNodes(),rModel.getElements());
    }
    double volume(0.0);
    for (uint i=0;i<rModel.getNVolumes();i++)
    {
        volume += rModel.getVolume(i).findVolume(rModel.getNodes(),rModel.getElements());
    }

    content += ModelStatisticsDialog::generateTableRow(tr("Line length"),QString::number(lineLength));
    content += ModelStatisticsDialog::generateTableRow(tr("Surface area"),QString::number(surfaceArea));
    content += ModelStatisticsDialog::generateTableRow(tr("Total volume"),QString::number(volume));

    double xc(0.0), yc(0.0), zc(0.0);
    rModel.findNodeCenter(xc,yc,zc);
    content += ModelStatisticsDialog::generateTableRow(tr("Model center"),"["+ QString::number(xc,'e') + " " + QString::number(yc,'e') + " " + QString::number(zc,'e') +"]");

    double xr[2], yr[2], zr[2];
    rModel.findNodeLimits(xr[0],xr[1],yr[0],yr[1],zr[0],zr[1]);
    content += ModelStatisticsDialog::generateTableRow(tr("Model center"),"["+ QString::number(xr[0],'e') + " " + QString::number(yr[0],'e') + " " + QString::number(zr[0],'e') +"] : ["+ QString::number(xr[1],'e') + " " + QString::number(yr[1],'e') + " " + QString::number(zr[1],'e') +"]");
    content += ModelStatisticsDialog::generateTableRow(tr("Model size"),"["+ QString::number(xr[1]-xr[0],'e') + " " + QString::number(yr[1]-yr[0],'e') + " " + QString::number(zr[1]-zr[0],'e') +"]");

    content += "</table>";

    textBrowser->insertHtml(content);
}

QString ModelStatisticsDialog::generateTableRow(const QString &label, const QString &value)
{
    return QString("<tr><th align=\"left\" nowrap>" + label + "</th><td align=\"right\"><pre>" + value + "</pre></td></tr>");
}
