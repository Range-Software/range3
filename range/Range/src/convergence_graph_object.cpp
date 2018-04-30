/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   convergence_graph_object.cpp                             *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   7-th March 2014                                          *
 *                                                                   *
 *  DESCRIPTION: Convergence graph object class definition           *
 *********************************************************************/

#include <QFile>
#include <QTextStream>

#include <rblib.h>

#include "convergence_graph_object.h"

ConvergenceGraphObject::ConvergenceGraphObject(QObject *parent) :
    GraphObject(parent),
    dataColumn(1)
{
}

const QString &ConvergenceGraphObject::getSourceFileName(void) const
{
    return this->sourceFileName;
}

void ConvergenceGraphObject::setSourceFileName(const QString &sourceFileName, bool loadData)
{
    this->sourceFileName = sourceFileName;
    if (loadData)
    {
        this->loadData();
    }
}

void ConvergenceGraphObject::setDataColumn(uint dataColumn, bool loadData)
{
    this->dataColumn = dataColumn;
    if (loadData)
    {
        this->loadData();
    }
}

bool ConvergenceGraphObject::isSourceSet(void) const
{
    return (!this->sourceFileName.isEmpty());
}

void ConvergenceGraphObject::clearSourceFile()
{
    QFile file(this->sourceFileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",sourceFileName.toUtf8().constData());
    }

    QTextStream in(&file);

    QString line = in.readLine();
    file.close();

    if (!file.open(QFile::WriteOnly|QFile::Truncate))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to truncate the file \'%s\'.",this->sourceFileName.toUtf8().constData());
    }
    file.write(line.toUtf8().constData());
    file.write("\n");
    file.close();
    this->loadData();
}

QStringList ConvergenceGraphObject::findDataColumns(const QString &sourceFileName)
{
    QFile file(sourceFileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",sourceFileName.toUtf8().constData());
    }

    QTextStream in(&file);

    QString line = in.readLine();

    QStringList dataColumns = line.split(QRegExp("(\\||\\;)"));
    dataColumns.erase(dataColumns.begin());

    file.close();

    return dataColumns;
}

void ConvergenceGraphObject::readSource(void)
{
    if (!this->isSourceSet())
    {
        return;
    }

    QFile file(this->sourceFileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",this->sourceFileName.toUtf8().constData());
    }

    QTextStream in(&file);

    double iteration;
    RRVector error(1);

    this->graphData.setTitle(tr("Convergence"));
    this->graphData.setTypeMask(GRAPH_LINES);

    this->graphData.clear();

    bool isFirst = true;
    while (!in.atEnd())
    {
        QString line = in.readLine();

        QStringList query = line.split(QRegExp("(\\||\\;)"));

        if (uint(query.size()) < this->dataColumn + 1)
        {
            file.close();
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Invalid file format. Expected %u columns but found only %d.",this->dataColumn + 1,query.size());
        }

        // First line is header
        if (isFirst)
        {
            isFirst = false;
            this->graphData.setXLabel(query.at(0));
            this->graphData.setYLabel(query.at(this->dataColumn));
        }
        else
        {
            iteration = query.at(0).toDouble();
            error[0] = query.at(this->dataColumn).toDouble();

            this->graphData[iteration] = error;
        }
    }

    file.close();
}
