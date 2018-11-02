/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_model_raw.cpp                                        *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   10-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: RAW model class definition                          *
 *********************************************************************/

#include <QFile>
#include <QTextStream>

#include <rblib.h>

#include "rml_model_raw.h"
#include "rml_polygon.h"


RModelRaw::RModelRaw ()
{
    this->_init();
} /* RModelRaw::RModelRaw */


RModelRaw::RModelRaw (const RModelRaw &modelRaw)
{
    this->_init(&modelRaw);
} /* RModelRaw::RModelRaw (copy) */


RModelRaw::~RModelRaw ()
{
} /* RModelRaw::~RModelRaw */


void RModelRaw::_init(const RModelRaw *pModelRaw)
{
    if (pModelRaw)
    {
        this->nodes = pModelRaw->nodes;
        this->elements = pModelRaw->elements;
    }
} /* RModelRaw::_init */


RModelRaw & RModelRaw::operator = (const RModelRaw &modelRaw)
{
    this->_init(&modelRaw);
    return (*this);
} /* RModelRaw::operator = */


unsigned int RModelRaw::getNNodes(void) const
{
    return (unsigned int)this->nodes.size();
} /* RModelRaw::getNNodes */


const RNode & RModelRaw::getNode(unsigned int position) const
{
    R_ERROR_ASSERT(position < this->nodes.size());
    return this->nodes[position];
} /* RModelRaw::getNode */


RNode & RModelRaw::getNode(unsigned int position)
{
    R_ERROR_ASSERT(position < this->nodes.size());
    return this->nodes[position];
} /* RModelRaw::getNode */


unsigned int RModelRaw::getNElements(void) const
{
    return (unsigned int)this->elements.size();
} /* RModelRaw::getNElements */


const RElement & RModelRaw::getElement(unsigned int position) const
{
    R_ERROR_ASSERT(position < this->elements.size());
    return this->elements[position];
} /* RModelRaw::getElement */


RElement & RModelRaw::getElement(unsigned int position)
{
    R_ERROR_ASSERT(position < this->elements.size());
    return this->elements[position];
} /* RModelRaw::getElement */


void RModelRaw::addPoint(const RNode &node, bool mergeNodes, double tolerance)
{
    RElement element;
    unsigned int nId;

    if (mergeNodes)
    {
        nId = this->findNearNode(node,tolerance);
        if (nId == RConstants::eod)
        {
            this->nodes.push_back(node);
            nId = (unsigned int)this->nodes.size() - 1;
        }
    }
    else
    {
        this->nodes.push_back(node);
        nId = (unsigned int)this->nodes.size() - 1;
    }

    element.setType(R_ELEMENT_POINT);
    element.setNodeId(0,nId);

    this->elements.push_back(element);
} /* RModelRaw::addPoint */


void RModelRaw::addSegment(const RNode &node1,
                           const RNode &node2,
                           bool         mergeNodes,
                           double       tolerance)
{
    RElement element;
    unsigned int nId1, nId2;

    if (mergeNodes)
    {
        nId1 = this->findNearNode(node1,tolerance);
        if (nId1 == RConstants::eod)
        {
            this->nodes.push_back(node1);
            nId1 = (unsigned int)this->nodes.size() - 1;
        }

        nId2 = this->findNearNode(node2,tolerance);
        if (nId2 == RConstants::eod)
        {
            this->nodes.push_back(node2);
            nId2 = (unsigned int)this->nodes.size() - 1;
        }
    }
    else
    {
        this->nodes.push_back(node1);
        nId1 = (unsigned int)this->nodes.size() - 1;
        this->nodes.push_back(node2);
        nId2 = (unsigned int)this->nodes.size() - 1;
    }

    element.setType(R_ELEMENT_TRUSS1);
    element.setNodeId(0,nId1);
    element.setNodeId(1,nId2);

    this->elements.push_back(element);
} /* RModelRaw::addSegment */


void RModelRaw::addTriangle(const RNode &node1,
                            const RNode &node2,
                            const RNode &node3,
                            bool         mergeNodes,
                            double       tolerance)
{
    RElement element;
    unsigned int nId1, nId2, nId3;

    if (mergeNodes)
    {
        nId1 = this->findNearNode(node1,tolerance);
        if (nId1 == RConstants::eod)
        {
            this->nodes.push_back(node1);
            nId1 = (unsigned int)this->nodes.size() - 1;
        }

        nId2 = this->findNearNode(node2,tolerance);
        if (nId2 == RConstants::eod)
        {
            this->nodes.push_back(node2);
            nId2 = (unsigned int)this->nodes.size() - 1;
        }

        nId3 = this->findNearNode(node3,tolerance);
        if (nId3 == RConstants::eod)
        {
            this->nodes.push_back(node3);
            nId3 = (unsigned int)this->nodes.size() - 1;
        }
    }
    else
    {
        this->nodes.push_back(node1);
        nId1 = (unsigned int)this->nodes.size() - 1;
        this->nodes.push_back(node2);
        nId2 = (unsigned int)this->nodes.size() - 1;
        this->nodes.push_back(node3);
        nId3 = (unsigned int)this->nodes.size() - 1;
    }

    element.setType(R_ELEMENT_TRI1);
    element.setNodeId(0,nId1);
    element.setNodeId(1,nId2);
    element.setNodeId(2,nId3);

    this->elements.push_back(element);
} /* RModelRaw::addTriangle */


void RModelRaw::addQuadrilateral(const RNode &node1, const RNode &node2, const RNode &node3, const RNode &node4, bool mergeNodes, double tolerance)
{
    RElement element;
    unsigned int nId1, nId2, nId3, nId4;

    std::vector<RNode> nodeList;
    nodeList.push_back(node1);
    nodeList.push_back(node2);
    nodeList.push_back(node3);
    nodeList.push_back(node4);

    RPolygon::sortNodes(nodeList);

    if (mergeNodes)
    {
        nId1 = this->findNearNode(nodeList[0],tolerance);
        if (nId1 == RConstants::eod)
        {
            this->nodes.push_back(nodeList[0]);
            nId1 = (unsigned int)this->nodes.size() - 1;
        }

        nId2 = this->findNearNode(nodeList[1],tolerance);
        if (nId2 == RConstants::eod)
        {
            this->nodes.push_back(nodeList[1]);
            nId2 = (unsigned int)this->nodes.size() - 1;
        }

        nId3 = this->findNearNode(nodeList[2],tolerance);
        if (nId3 == RConstants::eod)
        {
            this->nodes.push_back(nodeList[2]);
            nId3 = (unsigned int)this->nodes.size() - 1;
        }

        nId4 = this->findNearNode(nodeList[3],tolerance);
        if (nId4 == RConstants::eod)
        {
            this->nodes.push_back(nodeList[3]);
            nId4 = (unsigned int)this->nodes.size() - 1;
        }
    }
    else
    {
        this->nodes.push_back(nodeList[0]);
        nId1 = (unsigned int)this->nodes.size() - 1;
        this->nodes.push_back(nodeList[1]);
        nId2 = (unsigned int)this->nodes.size() - 1;
        this->nodes.push_back(nodeList[2]);
        nId3 = (unsigned int)this->nodes.size() - 1;
        this->nodes.push_back(nodeList[3]);
        nId4 = (unsigned int)this->nodes.size() - 1;
    }

    element.setType(R_ELEMENT_QUAD1);
    element.setNodeId(0,nId1);
    element.setNodeId(1,nId2);
    element.setNodeId(2,nId3);
    element.setNodeId(3,nId4);

    this->elements.push_back(element);
} /* RModelRaw::addQuadrilateral */


unsigned int RModelRaw::findNearNode (const RNode &node,
                                      double       tolerance) const
{
    unsigned int nn = this->getNNodes();

    for (unsigned int i=0;i<nn;i++)
    {
        if (tolerance == 0.0)
        {
            if (this->getNode(i) == node)
            {
                return i;
            }
        }
        else
        {
            if (this->getNode(i).getDistance(node) <= tolerance)
            {
                return i;
            }
        }
    }

    return RConstants::eod;
} /* RModelRaw::findNearNode */


unsigned int RModelRaw::mergeNearNodes (double tolerance)
{
    unsigned int nMerged = 0;
    std::vector<unsigned int> nodeBook;
    nodeBook.resize(this->getNNodes());

    RLogger::info("Finding near nodes\n");
    RProgressInitialize("Finfing near nodes");
    unsigned int nn = this->getNNodes();
    for (unsigned int i=0;i<nn;i++)
    {
        nodeBook[i] = i;
    }
    for (unsigned int i=0;i<nn;i++)
    {
        RProgressPrint(i,nn);
        if (nodeBook[i] != i)
        {
            continue;
        }
#pragma omp parallel for default(shared)
        for (int64_t j=int64_t(i)+1;j<int64_t(nn);j++)
        {
            if (nodeBook[j] != j)
            {
                continue;
            }
            if (tolerance == 0.0)
            {
                if (this->getNode(i) != this->getNode(j))
                {
                    continue;
                }
            }
            else
            {
                if (this->getNode(i).getDistance(this->getNode(j)) > tolerance)
                {
                    continue;
                }
            }
            nodeBook[j] = i;
        }
    }
    RProgressFinalize("Done");

    RLogger::info("Merging near nodes\n");
    RProgressInitialize("Merging near nodes");
    for (unsigned int i=0;i<nn;i++)
    {
        RProgressPrint(i,nn);
        unsigned int nID = nn-i-1;
        if (nodeBook[nID] != nID)
        {
            // Remove node from node vector.
            std::vector<RNode>::iterator iter = this->nodes.begin();
            std::advance(iter, nID);
            this->nodes.erase(iter);
            nMerged ++;
        }
    }
    RProgressFinalize("Done");

    RLogger::info("Renumbering merged nodes nodes\n");
    nn = 0;
    for (unsigned int i=0;i<nodeBook.size();i++)
    {

        if (nodeBook[i] == i)
        {
            nodeBook[i] = nn;
            nn++;
        }
        else
        {
            nodeBook[i] = nodeBook[nodeBook[i]];
        }
    }

    RProgressInitialize("Renumbering merged nodes nodes");
    // Renumber all higher node IDs in element vector.
    unsigned int ne = this->getNElements();
    for (unsigned int i=0;i<ne;i++)
    {
        RProgressPrint(i,ne);
        for (unsigned int j=0;j<this->getElement(i).size();j++)
        {
            this->getElement(i).setNodeId(j,nodeBook[this->getElement(i).getNodeId(j)]);
        }
    }
    RProgressFinalize("Done");

    return nMerged;
} /* RModelRaw::mergeNearNodes */


void RModelRaw::clear (void)
{
    this->nodes.clear();
    this->elements.clear();
} /* RModelRaw::clear */


void RModelRaw::read(const QString &fileName,
                     double         tolerance)
{
    RProgressInitialize("Reading RAW file", true);

    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    QTextStream in(&file);

    try
    {
        this->readTextStream(in,tolerance);
    }
    catch (const RError &rError)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read the file \'%s\'. %s",fileName.toUtf8().constData(),rError.getMessage().toUtf8().constData());
    }

    file.close ();

    RProgressFinalize("Done");
} /* RModelRaw::read */

void RModelRaw::write(const QString &fileName) const
{
    RProgressInitialize("Writing RAW file", true);

    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    QTextStream out(&file);

    for (unsigned int i=0;i<this->getNElements();i++)
    {
        if (this->getElement(i).getType() == R_ELEMENT_POINT ||
            this->getElement(i).getType() == R_ELEMENT_TRUSS1 ||
            this->getElement(i).getType() == R_ELEMENT_TRI1 ||
            this->getElement(i).getType() == R_ELEMENT_QUAD1)
        {
            for (unsigned int j=0;j<this->getElement(i).size();j++)
            {
                int nId = this->getElement(i).getNodeId(j);
                out << this->getNode(nId).getX() << " "
                    << this->getNode(nId).getY() << " "
                    << this->getNode(nId).getZ() << " ";
                if (out.status() != QTextStream::Ok)
                {
                    throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write to file \'%s\'.",fileName.toUtf8().constData());
                }
            }
            out << "\n";
            if (out.status() != QTextStream::Ok)
            {
                throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write to file \'%s\'.",fileName.toUtf8().constData());
            }
        }
    }

    file.close ();

    RProgressFinalize("Done");
} /* RModelRaw::write */

void RModelRaw::readTextStream(QTextStream &textSTream, double tolerance)
{
    RNode node1;
    RNode node2;
    RNode node3;
    RNode node4;

    uint lineNumber = 0;

    while (!textSTream.atEnd())
    {
        QString line = textSTream.readLine();
        lineNumber++;

        int hashPos = line.indexOf('#');
        if (hashPos >= 0)
        {
            line.remove(hashPos,line.size());
        }

        if (!line.isEmpty())
        {
            QRegExp rx("[,; ]");// match a comma or semicolon or a space
            QStringList list = line.split(rx, QString::SkipEmptyParts);

            RRVector coordinates(list.size(),0.0);

            QString invalidRecordMessage;

            if (list.size() % 3 != 0)
            {
                invalidRecordMessage = "Invalid number of coordinates = " + QString::number(list.size());
            }
            else
            {
                for (int i=0;i<list.size();i++)
                {
                    bool isDouble = true;
                    coordinates[i] = list[i].toDouble(&isDouble);
                    if (!isDouble)
                    {
                        invalidRecordMessage = "Record can contain only numbers";
                        break;
                    }
                }
            }

            if (invalidRecordMessage.isEmpty())
            {
                if (coordinates.size() == 3)
                {
                    node1.set(coordinates[0],coordinates[1],coordinates[2]);
                    this->addPoint(node1,true,tolerance);
                }
                else if (coordinates.size() == 6)
                {
                    node1.set(coordinates[0],coordinates[1],coordinates[2]);
                    node2.set(coordinates[3],coordinates[4],coordinates[5]);
                    this->addSegment(node1,node2,true,tolerance);
                }
                else if (coordinates.size() == 9)
                {
                    node1.set(coordinates[0],coordinates[1],coordinates[2]);
                    node2.set(coordinates[3],coordinates[4],coordinates[5]);
                    node3.set(coordinates[6],coordinates[7],coordinates[8]);
                    this->addTriangle(node1,node2,node3,true,tolerance);
                }
                else if (coordinates.size() == 12)
                {
                    node1.set(coordinates[0],coordinates[1],coordinates[2]);
                    node2.set(coordinates[3],coordinates[4],coordinates[5]);
                    node3.set(coordinates[6],coordinates[7],coordinates[8]);
                    node4.set(coordinates[9],coordinates[10],coordinates[11]);
                    this->addQuadrilateral(node1,node2,node3,node4,true,tolerance);
                }
                else
                {
                    invalidRecordMessage = "Invalid number of nodes = " + QString::number(coordinates.size() / 3);
                }
            }
            if (!invalidRecordMessage.isEmpty())
            {
                RLogger::warning("Invalid RAW element record @ line %u (%s). %s.\n", lineNumber, line.toUtf8().constData(), invalidRecordMessage.toUtf8().constData());
            }
        }

        if (textSTream.status() != QTextStream::Ok && !textSTream.atEnd())
        {
            throw RError(R_ERROR_INVALID_INPUT,R_ERROR_REF,"Failed to read the text stream.");
        }
    }
} /* RModelRaw::readTextStream */

bool RModelRaw::getNormal(unsigned int elementID, double &nx, double &ny, double &nz) const
{
    const RElement &element = this->getElement(elementID);
    if (element.getType() == R_ELEMENT_TRI1)
    {
        nx = ny = nz = 0.0;
        return true;
    }
    return false;
} /* RModelRaw::getNormal */
