/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   report_generator.cpp                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   26-th February 2014                                      *
 *                                                                   *
 *  DESCRIPTION: Report generator class definition                   *
 *********************************************************************/

#include <QTextCursor>

#include <rblib.h>

#include "report_generator.h"
#include "session.h"

ReportGenerator::ReportGenerator(uint modelID, QObject *parent)
    : DocumentGenerator(parent)
    , modelID(modelID)
{
    RLogger::trace("ReportGenerator::ReportGenerator(uint modelID, QObject *parent)\n");
    this->setEnableHeaderCounters(false);
}

void ReportGenerator::generateTitle(void)
{
    RLogger::trace("void ReportGenerator::generateTitle(void)\n");
    const Model &rModel = Session::getInstance().getModel(this->modelID);

    QTextCursor cursor(this->docTitle);

    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(Qt::AlignCenter);

    cursor.insertBlock(blockFormat);
    cursor.insertText("Analysis Report",ReportGenerator::getH1Format());
    cursor.insertBlock();
    cursor.insertText(rModel.getName(),ReportGenerator::getH2Format());
    cursor.insertBlock(blockFormat);
    std::vector<RProblemType> problemTypes = RProblem::getTypes(rModel.getProblemTaskTree().getProblemTypeMask());
    QString problemTypeList;
    for (uint i=0;i<problemTypes.size();i++)
    {
        if (i > 0)
        {
            problemTypeList += ", ";
        }
        problemTypeList += RProblem::getName(problemTypes[i]);
    }
    cursor.insertText(problemTypeList,ReportGenerator::getH3Format());
    cursor.insertBlock(blockFormat);
    cursor.insertBlock(blockFormat);
    cursor.insertText(QDateTime::currentDateTimeUtc().toLocalTime().toString("MMMM d, yyyy"),ReportGenerator::getTextFormat());
}

void ReportGenerator::generateBody(void)
{
    RLogger::trace("void ReportGenerator::generateBody(void)\n");
    this->generateModelChapter();
    this->generateProblemChapter();
    this->generateResultsChapter();
}

void ReportGenerator::generateModelChapter(void)
{
    RLogger::trace("void ReportGenerator::generateModelChapter(void)\n");

    const Model &rModel = Session::getInstance().getModel(this->modelID);

    QTextCursor cursor(this->docBody);
    cursor.movePosition(QTextCursor::End);

    this->insertH1(cursor,QObject::tr("Model"));
    this->insertH2(cursor,QObject::tr("Mesh details"));

    QTextTableFormat tableFormat;
    tableFormat.setCellPadding(1);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorder(1.0);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    tableFormat.setAlignment(Qt::AlignCenter);

    cursor.insertTable(6, 2, tableFormat);

    this->insertText(cursor,QObject::tr("Number of nodes:"),true);
    cursor.movePosition(QTextCursor::NextCell);
    this->insertText(cursor,QString::number(rModel.getNNodes()),true);

    cursor.movePosition(QTextCursor::NextCell);
    this->insertText(cursor,QObject::tr("Number of elements:"),true);
    cursor.movePosition(QTextCursor::NextCell);
    this->insertText(cursor,QString::number(rModel.getNElements()),true);

    cursor.movePosition(QTextCursor::NextCell);
    this->insertText(cursor,QObject::tr("Number of point entities:"),true);
    cursor.movePosition(QTextCursor::NextCell);
    this->insertText(cursor,QString::number(rModel.getNPoints()),true);

    cursor.movePosition(QTextCursor::NextCell);
    this->insertText(cursor,QObject::tr("Number of line entities:"),true);
    cursor.movePosition(QTextCursor::NextCell);
    this->insertText(cursor,QString::number(rModel.getNLines()),true);

    cursor.movePosition(QTextCursor::NextCell);
    this->insertText(cursor,QObject::tr("Number of surface entities:"),true);
    cursor.movePosition(QTextCursor::NextCell);
    this->insertText(cursor,QString::number(rModel.getNSurfaces()),true);

    cursor.movePosition(QTextCursor::NextCell);
    this->insertText(cursor,QObject::tr("Number of volume entities:"),true);
    cursor.movePosition(QTextCursor::NextCell);
    this->insertText(cursor,QString::number(rModel.getNVolumes()),true);
}

void ReportGenerator::generateProblemChapter(void)
{
    RLogger::trace("void ReportGenerator::generateProblemChapter(void)\n");

    const Model &rModel = Session::getInstance().getModel(this->modelID);

    QTextCursor cursor(this->docBody);
    cursor.movePosition(QTextCursor::End);

    this->insertH1(cursor,QObject::tr("Problem"));

//    RProblemTypeMask problemTypeMask = rModel.getProblemTaskTree().getProblemTypeMask();
//    if (problemTypeMask & R_PROBLEM_RADIATIVE_HEAT)
//    {
//        this->insertH2(cursor,QObject::tr("Radiation setup"));
//        const RRadiationSetup &radiationSetup = rModel.getProblemSetup().getRadiationSetup();
//        this->insertBold(cursor,QObject::tr("Resolution:"));
//        this->insertText(cursor,RRadiationSetup::getResolutionText(radiationSetup.getResolution()),true);
//    }

//    this->insertH2(cursor,QObject::tr("Time solver setup"));
//    const RTimeSolver &timeSolver = rModel.getTimeSolver();

    this->insertH2(cursor,QObject::tr("Boundary conditions"));

    for (uint i=0;i<rModel.getNElementGroups();i++)
    {
        const RElementGroup *pElementGroup = rModel.getElementGroupPtr(i);
        if (!pElementGroup)
        {
            continue;
        }

        uint nbc = pElementGroup->getNBoundaryConditions();
        if (nbc == 0)
        {
            continue;
        }

        this->insertH3(cursor,pElementGroup->getName());

        for (uint j=0;j<nbc;j++)
        {
            const RBoundaryCondition &bc = pElementGroup->getBoundaryCondition(j);

            this->insertH4(cursor,RBoundaryCondition::getName(bc.getType()));

            for (uint k=0;k<bc.size();k++)
            {
                const RConditionComponent &cc = bc.getComponent(k);

                cursor.insertTable(cc.size()+1,2,ReportGenerator::getTableFormat());

                this->insertBold(cursor,QObject::tr(cc.getKeyName().toUtf8().constData()) + " [" + cc.getKeyUnits() + "]",true);
                cursor.movePosition(QTextCursor::NextCell);

                this->insertBold(cursor,QObject::tr(cc.getValueName().toUtf8().constData()) + " [" + cc.getUnits() + "]",true);
                cursor.movePosition(QTextCursor::NextCell);

                for (uint l=0;l<cc.size();l++)
                {
                    this->insertText(cursor,QString::number(cc.getKey(l)),true);
                    cursor.movePosition(QTextCursor::NextCell);

                    this->insertText(cursor,QString::number(cc.getValue(l)),true);
                    cursor.movePosition(QTextCursor::NextCell);
                }

                cursor.movePosition(QTextCursor::NextBlock);
            }

        }
    }

    this->insertH2(cursor,QObject::tr("Initial conditions"));

    for (uint i=0;i<rModel.getNElementGroups();i++)
    {
        const RElementGroup *pElementGroup = rModel.getElementGroupPtr(i);
        if (!pElementGroup)
        {
            continue;
        }

        uint nic = pElementGroup->getNInitialConditions();
        if (nic == 0)
        {
            continue;
        }

        this->insertH3(cursor,pElementGroup->getName());

        for (uint j=0;j<nic;j++)
        {
            const RInitialCondition &ic = pElementGroup->getInitialCondition(j);

            this->insertH4(cursor,RInitialCondition::getName(ic.getType()));

            for (uint k=0;k<ic.size();k++)
            {
                const RConditionComponent &cc = ic.getComponent(k);

                cursor.insertTable(cc.size()+1,2,ReportGenerator::getTableFormat());

                this->insertBold(cursor,QObject::tr(cc.getKeyName().toUtf8().constData()) + " [" + cc.getKeyUnits() + "]",true);
                cursor.movePosition(QTextCursor::NextCell);

                this->insertBold(cursor,QObject::tr(cc.getValueName().toUtf8().constData()) + " [" + cc.getUnits() + "]",true);
                cursor.movePosition(QTextCursor::NextCell);

                for (uint l=0;l<cc.size();l++)
                {
                    this->insertText(cursor,QString::number(cc.getKey(l)),true);
                    cursor.movePosition(QTextCursor::NextCell);

                    this->insertText(cursor,QString::number(cc.getValue(l)),true);
                    cursor.movePosition(QTextCursor::NextCell);
                }

                cursor.movePosition(QTextCursor::NextBlock);
            }

        }
    }

    this->insertH2(cursor,QObject::tr("Environment conditions"));

    for (uint i=0;i<rModel.getNElementGroups();i++)
    {
        const RElementGroup *pElementGroup = rModel.getElementGroupPtr(i);
        if (!pElementGroup)
        {
            continue;
        }

        uint nec = pElementGroup->getNEnvironmentConditions();
        if (nec == 0)
        {
            continue;
        }

        this->insertH3(cursor,pElementGroup->getName());

        for (uint j=0;j<nec;j++)
        {
            const REnvironmentCondition &ec = pElementGroup->getEnvironmentCondition(j);

            this->insertH4(cursor,REnvironmentCondition::getName(ec.getType()));

            for (uint k=0;k<ec.size();k++)
            {
                const RConditionComponent &cc = ec.getComponent(k);

                cursor.insertTable(cc.size()+1,2,ReportGenerator::getTableFormat());

                this->insertBold(cursor,QObject::tr(cc.getKeyName().toUtf8().constData()) + " [" + cc.getKeyUnits() + "]",true);
                cursor.movePosition(QTextCursor::NextCell);

                this->insertBold(cursor,QObject::tr(cc.getValueName().toUtf8().constData()) + " [" + cc.getUnits() + "]",true);
                cursor.movePosition(QTextCursor::NextCell);

                for (uint l=0;l<cc.size();l++)
                {
                    this->insertText(cursor,QString::number(cc.getKey(l)),true);
                    cursor.movePosition(QTextCursor::NextCell);

                    this->insertText(cursor,QString::number(cc.getValue(l)),true);
                    cursor.movePosition(QTextCursor::NextCell);
                }

                cursor.movePosition(QTextCursor::NextBlock);
            }
        }
    }

    this->insertH2(cursor,QObject::tr("Materials"));

    for (uint i=0;i<rModel.getNElementGroups();i++)
    {
        const RElementGroup *pElementGroup = rModel.getElementGroupPtr(i);
        if (!pElementGroup)
        {
            continue;
        }

        this->insertH3(cursor,pElementGroup->getName());

        const RMaterial &material = pElementGroup->getMaterial();

        this->insertH4(cursor,material.getName());

        for (uint k=0;k<material.size();k++)
        {
            const RMaterialProperty &materialProperty = material.get(k);

            this->insertH5(cursor,QObject::tr(materialProperty.getValueName().toUtf8().constData()));

            cursor.insertTable(materialProperty.size()+1,2,ReportGenerator::getTableFormat());

            this->insertBold(cursor,QObject::tr(materialProperty.getKeyName().toUtf8().constData()) + " [" + materialProperty.getKeyUnits() + "]",true);
            cursor.movePosition(QTextCursor::NextCell);

            this->insertBold(cursor,QObject::tr(materialProperty.getValueName().toUtf8().constData()) + " [" + materialProperty.getUnits() + "]",true);
            cursor.movePosition(QTextCursor::NextCell);

            for (uint l=0;l<materialProperty.size();l++)
            {
                this->insertText(cursor,QString::number(materialProperty.getKey(l)),true);
                cursor.movePosition(QTextCursor::NextCell);

                this->insertText(cursor,QString::number(materialProperty.getValue(l)),true);
                cursor.movePosition(QTextCursor::NextCell);
            }

            cursor.movePosition(QTextCursor::NextBlock);
        }
    }
}

void ReportGenerator::generateResultsChapter(void)
{
    RLogger::trace("void ReportGenerator::generateResultsChapter(void)\n");

    const Model &rModel = Session::getInstance().getModel(this->modelID);

    QTextCursor cursor(this->docBody);
    cursor.movePosition(QTextCursor::End);

    this->insertH1(cursor,QObject::tr("Results"));

    this->insertBold(cursor,QObject::tr("Time-solver:"));
    this->insertText(cursor," " + QObject::tr(rModel.getTimeSolver().getEnabled() ? "enabled" : "disabled"),true);

    if (rModel.getTimeSolver().getEnabled())
    {
        this->insertBold(cursor,QObject::tr("Current time step:"));
        this->insertText(cursor," " + QString::number(rModel.getTimeSolver().getCurrentTimeStep()+1),true);
        this->insertBold(cursor,QObject::tr("Current computational time:"));
        this->insertText(cursor," " + QString::number(rModel.getTimeSolver().getCurrentTime()) + " [" + RVariable::getUnits(R_VARIABLE_TIME) + "]",true);
    }
    else
    {
        if (rModel.getProblemTaskTree().getProblemTypeMask() & R_PROBLEM_STRESS_MODAL)
        {
            this->insertBold(cursor,QObject::tr("Current mode:"));
            this->insertText(cursor," " + QString::number(rModel.getProblemSetup().getModalSetup().getMode()+1),true);
            this->insertBold(cursor,QObject::tr("Current frequency:"));
            this->insertText(cursor," " + QString::number(rModel.getProblemSetup().getModalSetup().getFrequency()) + " [" + RVariable::getUnits(R_VARIABLE_FREQUENCY) + "]",true);
        }
    }

//    const RMonitoringPointManager &monitoringPointManager = rModel.getMonitoringPointManager();

//    if (monitoringPointManager.size() > 0)
//    {
//        this->insertH2(cursor,QObject::tr("Monitoring points"));

//        for (uint i=0;i<monitoringPointManager.size();i++)
//        {
//            const RMonitoringPoint &monitoringPoint = monitoringPointManager.at(i);
////            monitoringPoint.g
//        }
//    }

    this->insertH2(cursor,QObject::tr("Variables"));

    for (uint i=0;i<rModel.getNVariables();i++)
    {
        const RVariable &rVariable = rModel.getVariable(i);
        this->insertH3(cursor,rVariable.getName());

        QTextBlockFormat blockFormat;
        blockFormat.setIndent(2);

        QTextBlockFormat rightAlignment;
        rightAlignment.setAlignment(Qt::AlignRight);

        QTextTableFormat tableFormat;
        tableFormat.setCellPadding(1);
        tableFormat.setCellSpacing(0);
        tableFormat.setBorder(0.0);
        tableFormat.setHeaderRowCount(0);
        tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_None);
        tableFormat.setAlignment(Qt::AlignCenter);

        RStatistics stat(rVariable.getValues(),100,true);

        cursor.insertTable(8,2,tableFormat);

        this->insertBold(cursor,QObject::tr("Variable type:"),true);
        cursor.movePosition(QTextCursor::NextCell);
        cursor.setBlockFormat(rightAlignment);
        this->insertText(cursor,QObject::tr(rVariable.getNVectors() == 1 ? "Scalar" : "Vector"),true);
        cursor.movePosition(QTextCursor::NextCell);

        this->insertBold(cursor,QObject::tr("Units:"),true);
        cursor.movePosition(QTextCursor::NextCell);
        cursor.setBlockFormat(rightAlignment);
        this->insertText(cursor,rVariable.getUnits(),true);
        cursor.movePosition(QTextCursor::NextCell);

        this->insertBold(cursor,QObject::tr("Minimum:"),true);
        cursor.movePosition(QTextCursor::NextCell);
        cursor.setBlockFormat(rightAlignment);
        this->insertText(cursor,QString::number(stat.getMin()),true);
        cursor.movePosition(QTextCursor::NextCell);

        this->insertBold(cursor,QObject::tr("Maximum:"),true);
        cursor.movePosition(QTextCursor::NextCell);
        cursor.setBlockFormat(rightAlignment);
        this->insertText(cursor,QString::number(stat.getMax()),true);
        cursor.movePosition(QTextCursor::NextCell);

        this->insertBold(cursor,QObject::tr("Average:"),true);
        cursor.movePosition(QTextCursor::NextCell);
        cursor.setBlockFormat(rightAlignment);
        this->insertText(cursor,QString::number(stat.getAvg()),true);
        cursor.movePosition(QTextCursor::NextCell);

        this->insertBold(cursor,QObject::tr("Median:"),true);
        cursor.movePosition(QTextCursor::NextCell);
        cursor.setBlockFormat(rightAlignment);
        this->insertText(cursor,QString::number(stat.getMed()),true);
        cursor.movePosition(QTextCursor::NextCell);

        this->insertBold(cursor,QObject::tr("Percentile 5:"),true);
        cursor.movePosition(QTextCursor::NextCell);
        cursor.setBlockFormat(rightAlignment);
        this->insertText(cursor,QString::number(stat.getP05()),true);
        cursor.movePosition(QTextCursor::NextCell);

        this->insertBold(cursor,QObject::tr("Percentile 95:"),true);
        cursor.movePosition(QTextCursor::NextCell);
        cursor.setBlockFormat(rightAlignment);
        this->insertText(cursor,QString::number(stat.getP95()),true);
        cursor.movePosition(QTextCursor::NextCell);

        cursor.movePosition(QTextCursor::NextBlock);
    }

    this->insertH2(cursor,QObject::tr("Screenshot"));

    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(Qt::AlignCenter);
    cursor.insertBlock(blockFormat);
    QImage modelScreenShotImage(this->takeModelScreenshot());
    cursor.insertImage(modelScreenShotImage.scaledToWidth(qCeil(double(this->printer->pageRect().width())*0.9),Qt::SmoothTransformation));
}

QImage ReportGenerator::takeModelScreenshot(void) const
{
    RLogger::trace("QImage ReportGenerator::takeModelScreenshot(void)\n");

    const Model &rModel = Session::getInstance().getModel(this->modelID);
    QString screenShotFile(rModel.buildScreenShotFileName());

    Session::getInstance().setTakeScreenShot(this->modelID,screenShotFile,true);

    return QImage(screenShotFile);
}
