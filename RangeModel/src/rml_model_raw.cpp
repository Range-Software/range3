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
    return this->nodes.size();
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
    return this->elements.size();
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
            nId = this->nodes.size() - 1;
        }
    }
    else
    {
        this->nodes.push_back(node);
        nId = this->nodes.size() - 1;
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
            nId1 = this->nodes.size() - 1;
        }

        nId2 = this->findNearNode(node2,tolerance);
        if (nId2 == RConstants::eod)
        {
            this->nodes.push_back(node2);
            nId2 = this->nodes.size() - 1;
        }
    }
    else
    {
        this->nodes.push_back(node1);
        nId1 = this->nodes.size() - 1;
        this->nodes.push_back(node2);
        nId2 = this->nodes.size() - 1;
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
            nId1 = this->nodes.size() - 1;
        }

        nId2 = this->findNearNode(node2,tolerance);
        if (nId2 == RConstants::eod)
        {
            this->nodes.push_back(node2);
            nId2 = this->nodes.size() - 1;
        }

        nId3 = this->findNearNode(node3,tolerance);
        if (nId3 == RConstants::eod)
        {
            this->nodes.push_back(node3);
            nId3 = this->nodes.size() - 1;
        }
    }
    else
    {
        this->nodes.push_back(node1);
        nId1 = this->nodes.size() - 1;
        this->nodes.push_back(node2);
        nId2 = this->nodes.size() - 1;
        this->nodes.push_back(node3);
        nId3 = this->nodes.size() - 1;
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
            nId1 = this->nodes.size() - 1;
        }

        nId2 = this->findNearNode(nodeList[1],tolerance);
        if (nId2 == RConstants::eod)
        {
            this->nodes.push_back(nodeList[1]);
            nId2 = this->nodes.size() - 1;
        }

        nId3 = this->findNearNode(nodeList[2],tolerance);
        if (nId3 == RConstants::eod)
        {
            this->nodes.push_back(nodeList[2]);
            nId3 = this->nodes.size() - 1;
        }

        nId4 = this->findNearNode(nodeList[3],tolerance);
        if (nId4 == RConstants::eod)
        {
            this->nodes.push_back(nodeList[3]);
            nId4 = this->nodes.size() - 1;
        }
    }
    else
    {
        this->nodes.push_back(nodeList[0]);
        nId1 = this->nodes.size() - 1;
        this->nodes.push_back(nodeList[1]);
        nId2 = this->nodes.size() - 1;
        this->nodes.push_back(nodeList[2]);
        nId3 = this->nodes.size() - 1;
        this->nodes.push_back(nodeList[3]);
        nId4 = this->nodes.size() - 1;
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
    unsigned int npMax = 0, np = 0;
    for (unsigned int i=0;i<nn;i++)
    {
        nodeBook[i] = i;
        np += nn - i - 1;
    }
    npMax = np;
    np = 0;
    for (unsigned int i=0;i<nn;i++)
    {
        RProgressPrint(np+1,npMax);
        np += nn - i - 1;
        if (nodeBook[i] != i)
        {
            continue;
        }
        for (unsigned int j=i+1;j<nn;j++)
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
        RProgressPrint(i+1,nn);
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
            nodeBook[i] = nn;
        }
    }

    RProgressInitialize("Renumbering merged nodes nodes");
    // Renumber all higher node IDs in element vector.
    unsigned int ne = this->getNElements();
    for (unsigned int i=0;i<ne;i++)
    {
        RProgressPrint(i+1,ne);
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
    RNode node1;
    RNode node2;
    RNode node3;
    double x1, y1, z1;
    double x2, y2, z2;
    double x3, y3, z3;

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

    while (!in.atEnd())
    {
        in >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
        if (in.status() != QTextStream::Ok && !in.atEnd())
        {
            throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read the file \'%s\'.",fileName.toUtf8().constData());
        }
        if (in.status() == QTextStream::Ok)
        {
            node1.set(x1,y1,z1);
            node2.set(x2,y2,z2);
            node3.set(x3,y3,z3);
            this->addTriangle(node1,node2,node3,true,tolerance);
        }
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
        if (this->getElement(i).getType() != R_ELEMENT_TRI1)
        {
            continue;
        }
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

    file.close ();

    RProgressFinalize("Done");
} /* RModelRaw::write */

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
