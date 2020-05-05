/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_surface.cpp                                          *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   8-th December 2011                                       *
 *                                                                   *
 *  DESCRIPTION: Surface class definition                            *
 *********************************************************************/

#include <cmath>

#include "rml_element.h"
#include "rml_surface.h"
#include "rml_mesh_generator.h"

const QString RSurface::defaultName("Surface");


RSurface::RSurface () : thickness(0.0)
{
    this->name = RSurface::defaultName;
    this->_init ();
} /* RSurface::RSurface */


RSurface::RSurface (const RSurface &surface) : RElementGroup (surface)
{
    this->_init (&surface);
} /* RSurface::RSurface */


RSurface::~RSurface ()
{
} /* RSurface::~RSurface */


void RSurface::_init (const RSurface *pSurface)
{
    if (pSurface)
    {
        this->setThickness (pSurface->getThickness());
    }
} /* RSurface::_init */


double RSurface::getThickness (void) const
{
    return this->thickness;
} /* RSurface::getThickness */


void RSurface::setThickness (double thickness)
{
    this->thickness = thickness;
} /* RSurface::setThickness */


bool RSurface::pointInside(const std::vector<RNode> &nodes, const std::vector<RElement> &elements, const RR3Vector &point, bool includeSurface) const
{
    std::vector<RR3Vector> points;
    points.push_back(point);
    std::vector<bool> isInside = this->pointsInside(nodes,elements,points,includeSurface);
    if (isInside.size() != 1)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Unexpected error, size of the array = \'%u\'",isInside.size());
    }
    return isInside[0];
} /* RSurface::pointInside */


std::vector<bool> RSurface::pointsInside(const std::vector<RNode> &nodes, const std::vector<RElement> &elements, const std::vector<RR3Vector> &points, bool includeSurface) const
{
    std::vector<RNode> steinerNodes;
    std::vector<RNode> volumeNodes(nodes);
    std::vector<RElement> volumeElements;

    try
    {
        this->tetrahedralize(nodes,elements,steinerNodes,volumeElements);
    }
    catch (const RError &error)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to tetrahedralize surface \'%s\'. %s",
                     this->getName().toUtf8().constData(), error.getMessage().toUtf8().constData());
    }

    if (steinerNodes.size() > 0)
    {
        uint nVolumeNodes = uint(volumeNodes.size());
        volumeNodes.resize(nVolumeNodes + steinerNodes.size());
        for (uint i=0;i<steinerNodes.size();i++)
        {
            volumeNodes[nVolumeNodes+i] = steinerNodes[i];
        }
    }

    std::vector<bool> areInside;
    areInside.resize(points.size(),false);

    for (uint i=0;i<points.size();i++)
    {
        RNode node(points[i]);
        bool isInside = false;

#pragma omp parallel for default(shared)
        for (int64_t j=0;j<int64_t(volumeElements.size());j++)
        {
#pragma omp flush (isInside)
            if (!isInside)
            {
                if (volumeElements[j].isInside(volumeNodes,node))
                {
                    isInside = true;
#pragma omp flush (isInside)
                }
            }
        }

        if (!includeSurface)
        {
            // Check that point is not on the surface.
            for (uint j=0;j<this->size();j++)
            {
                if (elements[this->get(j)].isInside(nodes,node))
                {
                    isInside = false;
                    break;
                }
            }
        }

        areInside[i] = isInside;
    }

    return areInside;
} /* RSurface::pointsInside */


void RSurface::tetrahedralize(const std::vector<RNode> &nodes, const std::vector<RElement> &elements, std::vector<RNode> &steinerNodes, std::vector<RElement> &volumeElements) const
{
    std::vector<RNode> surfaceNodes;
    std::vector<RElement> surfaceElements;

    std::vector<uint> nodeBook;
    nodeBook.resize(nodes.size(),RConstants::eod);

    for (uint i=0;i<this->size();i++)
    {
        const RElement &rElement = elements[this->get(i)];
        surfaceElements.push_back(rElement);

        for (uint j=0;j<rElement.size();j++)
        {
            nodeBook[rElement.getNodeId(j)] = 0;
        }
    }
    uint nSurfaceNodes = 0;
    for (uint i=0;i<nodes.size();i++)
    {
        if (nodeBook[i] != RConstants::eod)
        {
            nodeBook[i] = nSurfaceNodes++;
            surfaceNodes.push_back(nodes[i]);
        }
    }
    for (uint i=0;i<surfaceElements.size();i++)
    {
        RElement &rElement = surfaceElements[i];
        for (uint j=0;j<rElement.size();j++)
        {
            rElement.setNodeId(j,nodeBook[rElement.getNodeId(j)]);
        }
    }

    std::vector<uint> sNodeBook;
    sNodeBook.resize(surfaceNodes.size(),RConstants::eod);
    for (uint i=0;i<nodeBook.size();i++)
    {
        if (nodeBook[i] != RConstants::eod)
        {
            sNodeBook[nodeBook[i]] = i;
        }
    }

    try
    {
        RMeshGenerator::generate(surfaceNodes,surfaceElements,steinerNodes,volumeElements);
    }
    catch (const RError &error)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to generate volume mesh. %s", error.getMessage().toUtf8().constData());
    }

    if (steinerNodes.size() > 0)
    {
        uint nSurfaceNodes = uint(sNodeBook.size());
        uint nSteinerNodes = uint(steinerNodes.size());

        sNodeBook.resize(nSurfaceNodes + nSteinerNodes);

        for (uint i=0;i<nSteinerNodes;i++)
        {
            sNodeBook[nSurfaceNodes+i] = uint(nodes.size()) + i;
        }
    }

    for (uint i=0;i<volumeElements.size();i++)
    {
        RElement &rElement = volumeElements[i];
        for (uint j=0;j<rElement.size();j++)
        {
            uint nodeId = rElement.getNodeId(j);
            rElement.setNodeId(j,sNodeBook[nodeId]);
        }
    }
} /* RSurface::tetrahedralize */


double RSurface::findArea(const std::vector<RNode> &nodes, const std::vector<RElement> &elements) const
{
    double surfaceArea = 0.0;

    for (uint i=0;i<this->size();i++)
    {
        const RElement &rElement = elements[this->get(i)];

        double elementArea = 0.0;
        if (rElement.findArea(nodes,elementArea))
        {
            surfaceArea += elementArea;
        }
    }

    return surfaceArea;
} /* RSurface::findArea */


void RSurface::findAverageNormal(const std::vector<RNode> &nodes, const std::vector<RElement> &elements, RR3Vector &normal) const
{
    normal[0] = normal[1] = normal[2] = 0.0;
    for (uint i=0;i<this->size();i++)
    {
        const RElement &rElement = elements[this->get(i)];

        RR3Vector elementNormal;
        if (rElement.findNormal(nodes,elementNormal[0],elementNormal[1],elementNormal[2]))
        {
            normal[0] += elementNormal[0];
            normal[1] += elementNormal[1];
            normal[2] += elementNormal[2];
        }
    }
    normal.normalize();
} /* RSurface::findAverageNormal */


RSurface & RSurface::operator = (const RSurface &surface)
{
    this->RElementGroup::operator = (surface);
    this->_init (&surface);
    return (*this);
} /* RSurface::operator = */
