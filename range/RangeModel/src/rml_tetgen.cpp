/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_tetgen.cpp                                           *
 *  GROUP:  RModel                                                   *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   4-th August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: RTetGen class definition                            *
 *********************************************************************/

#include <sstream>
#include <cmath>
#include <algorithm>

#include <rmlib.h>
#include <rblib.h>

#include "rml_tetgen.h"

const int RTetGen::pointMarkerOffset = INT_MAX - 1000;
//const int RTetGen::pointMarkerOffset = 0;

RTetGen::RTetGen(const RModel &model)
{
    this->importModel(model,false);
}

void RTetGen::importModel(const RModel &model, bool reconstruct, const RRVector &nodeMeshSizeValues)
{
    this->firstnumber = 0;

    // Set nodes

    this->numberofpoints = model.getNNodes();
    this->numberofpointattributes = 0;
    this->numberofpointmtrs = nodeMeshSizeValues.size() == model.getNNodes() ? 1 : 0;

    if (this->numberofpoints)
    {
        this->pointlist = new REAL [this->numberofpoints*3];
//        if (this->numberofpointattributes)
//        {
//            this->pointattributelist = new REAL [this->numberofpoints*this->numberofpointattributes];
//        }
        if (this->numberofpointmtrs)
        {
            this->pointmtrlist = new REAL [this->numberofpoints*this->numberofpointmtrs];
        }
        this->pointmarkerlist = new int [this->numberofpoints];

        for (int i=0;i<this->numberofpoints;i++)
        {
            this->pointlist[3*i+0] = REAL(model.getNode(i).getX());
            this->pointlist[3*i+1] = REAL(model.getNode(i).getY());
            this->pointlist[3*i+2] = REAL(model.getNode(i).getZ());
//            if (this->numberofpointattributes)
//            {
//                this->pointattributelist[this->numberofpointattributes*i+0] = REAL(i);
//                for (int j=1;j<this->numberofpointattributes;j++)
//                {
//                    this->pointattributelist[this->numberofpointattributes*i+j] = 0.0;
//                }
//            }
            if (this->numberofpointmtrs)
            {
                this->pointmtrlist[this->numberofpointmtrs*i+0] = nodeMeshSizeValues[i];
                for (int j=1;j<this->numberofpointmtrs;j++)
                {
                    this->pointmtrlist[this->numberofpointmtrs*i+j] = 0.0;
                }
            }
            this->pointmarkerlist[i] = 0;
        }
    }

    for (uint i=0;i<model.getNPoints();i++)
    {
        int marker = -int(i+1) - RTetGen::pointMarkerOffset;
        const RPoint &rPoint = model.getPoint(i);
        for (uint j=0;j<rPoint.size();j++)
        {
            this->pointmarkerlist[model.getElement(rPoint.get(j)).getNodeId(0)] = marker;
        }
    }

    this->numberofedges = model.getNEntityElements(R_ENTITY_GROUP_LINE);
    if (this->numberofedges > 0)
    {
        this->edgelist = new int [2*this->numberofedges];
        this->edgemarkerlist = new int [this->numberofedges];
        uint edgeNumber = 0;
        for (uint i=0;i<model.getNLines();i++)
        {
            int marker = -int(i+1);
            const RLine &rLine = model.getLine(i);
            for (uint j=0;j<rLine.size();j++)
            {
                uint elementID = rLine.get(j);
                const RElement &rElement = model.getElement(elementID);
                this->edgelist[2*edgeNumber+0] = rElement.getNodeId(0) + this->firstnumber;
                this->edgelist[2*edgeNumber+1] = rElement.getNodeId(1) + this->firstnumber;
                this->edgemarkerlist[edgeNumber] = marker;
                edgeNumber++;
            }
        }
    }

    if (reconstruct)
    {
        // Set trifacets

        this->numberoftrifaces = int(model.getNElements(R_ELEMENT_TRI1) + model.getNElements(R_ELEMENT_TRI2));

        if (this->numberoftrifaces)
        {
            this->trifacelist = new int[this->numberoftrifaces*3];
            this->trifacemarkerlist = new int[this->numberoftrifaces];

            RUVector surfaceBook(model.getNElements());
            std::fill(surfaceBook.begin(),surfaceBook.end(),RConstants::eod);

            unsigned int ce = 0;
            for (unsigned int i=0;i<model.getNElements();i++)
            {
                if (model.getElement(i).getType() != R_ELEMENT_TRI1 &&
                    model.getElement(i).getType() != R_ELEMENT_TRI2)
                {
                    continue;
                }
                this->trifacelist[3*ce+0] = model.getElement(i).getNodeId(0) + this->firstnumber;
                this->trifacelist[3*ce+1] = model.getElement(i).getNodeId(1) + this->firstnumber;
                this->trifacelist[3*ce+2] = model.getElement(i).getNodeId(2) + this->firstnumber;

                this->trifacemarkerlist[ce] = 1;

                surfaceBook[i] = ce;

                ce++;
            }

            for (unsigned int i=0;i<model.getNSurfaces();i++)
            {
                int marker = int(i+1);
                for (unsigned int j=0;j<model.getSurface(i).size();j++)
                {
                    unsigned int elementPosition = surfaceBook[model.getSurface(i).get(j)];
                    if (elementPosition != RConstants::eod)
                    {
                        this->trifacemarkerlist[elementPosition] = marker;
                    }
                }
            }
        }

        // Set tetrahedras

        this->numberoftetrahedra = int(model.getNElements(R_ELEMENT_TETRA1));

        if (this->numberoftetrahedra)
        {
            this->numberofcorners = 4;
            this->numberoftetrahedronattributes = 1;
            this->tetrahedronlist = new int[this->numberoftetrahedra*4];
            this->tetrahedronattributelist = new REAL[this->numberoftetrahedra*this->numberoftetrahedronattributes];
            this->tetrahedronvolumelist = new REAL[this->numberoftetrahedra];

            RUVector volumeBook(model.getNElements());
            std::fill(volumeBook.begin(),volumeBook.end(),RConstants::eod);

            unsigned int ce = 0;
            for (unsigned int i=0;i<model.getNElements();i++)
            {
                if (model.getElement(i).getType() != R_ELEMENT_TETRA1)
                {
                    continue;
                }
                this->tetrahedronlist[4*ce+0] = model.getElement(i).getNodeId(0) + this->firstnumber;
                this->tetrahedronlist[4*ce+1] = model.getElement(i).getNodeId(1) + this->firstnumber;
                this->tetrahedronlist[4*ce+2] = model.getElement(i).getNodeId(2) + this->firstnumber;
                this->tetrahedronlist[4*ce+3] = model.getElement(i).getNodeId(3) + this->firstnumber;

                this->tetrahedronattributelist[this->numberoftetrahedronattributes*ce+0] = 0.0;

                this->tetrahedronvolumelist[ce] = -1.0;

                volumeBook[i] = ce;

                ce++;
            }

            // Set volume marker for each tetrahedra
            for (unsigned int i=0;i<model.getNVolumes();i++)
            {
                int marker = int(i);
                for (unsigned int j=0;j<model.getVolume(i).size();j++)
                {
                    unsigned int elementPosition = volumeBook[model.getVolume(i).get(j)];
                    if (elementPosition != RConstants::eod)
                    {
                        this->tetrahedronattributelist[this->numberoftetrahedronattributes*elementPosition+0] = marker;
                    }
                }
            }
        }
    }
    else
    {
        // Set facets
        this->numberoffacets = int(  model.getNElements(R_ELEMENT_TRI1)
                                   + model.getNElements(R_ELEMENT_TRI2)
                                   + model.getNElements(R_ELEMENT_QUAD1)
                                   + model.getNElements(R_ELEMENT_QUAD2));

        if (this->numberoffacets > 0)
        {
            this->facetlist = new tetgenio::facet[this->numberoffacets];
            this->facetmarkerlist = new int[this->numberoffacets];

            RUVector faceBook(model.getNElements());
            std::fill(faceBook.begin(),faceBook.end(),RConstants::eod);

            unsigned int ce = 0;
            for (unsigned int i=0;i<model.getNElements();i++)
            {
                uint nne = 0;
                if (model.getElement(i).getType() == R_ELEMENT_TRI1 ||
                    model.getElement(i).getType() == R_ELEMENT_TRI2)
                {
                    nne = 3;
                }
                else if (model.getElement(i).getType() == R_ELEMENT_QUAD1 ||
                         model.getElement(i).getType() == R_ELEMENT_QUAD2)
                {
                    nne = 4;
                }
                else
                {
                    continue;
                }
                this->facetlist[ce].numberofholes = 0;
                this->facetlist[ce].holelist = 0;
                this->facetlist[ce].numberofpolygons = 1;
                this->facetlist[ce].polygonlist = new tetgenio::polygon[1];
                this->facetlist[ce].polygonlist[0].numberofvertices = nne;
                this->facetlist[ce].polygonlist[0].vertexlist = new int[nne];
                for (uint j=0;j<nne;j++)
                {
                    this->facetlist[ce].polygonlist[0].vertexlist[j] = model.getElement(i).getNodeId(j) + this->firstnumber;
                }

                this->facetmarkerlist[ce] = 0;

                faceBook[i] = ce;

                ce++;
            }

            for (unsigned int i=0;i<model.getNSurfaces();i++)
            {
                int marker = -int(i+1);
                for (unsigned int j=0;j<model.getSurface(i).size();j++)
                {
                    unsigned int elementPosition = faceBook[model.getSurface(i).get(j)];
                    if (elementPosition != RConstants::eod)
                    {
                        this->facetmarkerlist[elementPosition] = marker;
                    }
                }
            }
        }
    }
}

void RTetGen::exportMesh(RModel &model, bool keepResults) const
{
    // Interpolate element results.
    std::vector<RVariable> variables;

    if (keepResults)
    {
        RLogger::info("Interpolating results\n");
        RLogger::indent();

        RProgressInitialize("Interpolating results");
        for (uint i=0;i<model.getNVariables();i++)
        {
            RProgressPrint(i,model.getNVariables());
            // Only node results can be safely interpolated.
            if (model.getVariable(i).getApplyType() == R_VARIABLE_APPLY_NODE)
            {
                RVariable variable = model.getVariable(i);
                RLogger::info("Interpolating %s\n",variable.getName().toUtf8().constData());
                RLogger::indent();
                variable.resize(variable.getNVectors(),this->numberofpoints);
#pragma omp parallel for default(shared)
                for (int j=0;j<this->numberofpoints;j++)
                {

                    RNode node(this->pointlist[3*j+0],
                               this->pointlist[3*j+1],
                               this->pointlist[3*j+2]);
                    RRVector values = model.getInterpolatedResultsValues(variable.getType(),node);
                    for (uint k=0;k<values.size();k++)
                    {
                        variable.setValue(k,j,values[k]);
                    }
                }
                variables.push_back(variable);
                RLogger::unindent();
            }
        }
        RProgressFinalize();
        RLogger::unindent();
    }


    // Find number of point elements.
    uint numberOfPointElements = 0;
    if (this->pointmarkerlist)
    {
        for (int i=0;i<this->numberofpoints;i++)
        {
            if (this->pointmarkerlist[i] < -RTetGen::pointMarkerOffset)
            {
                numberOfPointElements ++;
            }
        }
    }

    // Find number of line elements.
    uint numberOfLineElements = 0;
    if (this->edgemarkerlist)
    {
        for (int i=0;i<this->numberofedges;i++)
        {
            if (this->edgemarkerlist[i] < 0)
            {
                numberOfLineElements ++;
            }
        }
    }

    // NODES
    model.setNNodes(this->numberofpoints);

    for (int i=0;i<this->numberofpoints;i++)
    {
        model.getNode(i).set(this->pointlist[3*i+0],
                             this->pointlist[3*i+1],
                             this->pointlist[3*i+2]);
    }

    model.setNVariables(uint(variables.size()));

    model.setNElements(numberOfPointElements + numberOfLineElements + this->numberoftrifaces + this->numberoftetrahedra);

    std::vector<RUVector> volumeNeigs;

    model.clearSurfaceNeighbors();
    model.clearVolumeNeighbors();

    volumeNeigs.resize(numberOfPointElements + numberOfLineElements + this->numberoftrifaces + this->numberoftetrahedra);

    unsigned int nElements = 0;

    // POINTS
    for (unsigned int i=0;i<model.getNPoints();i++)
    {
        model.getPoint(i).resize(0);
    }
    if (this->pointmarkerlist)
    {
        for (int i=0;i<this->numberofpoints;i++)
        {
            if (this->pointmarkerlist[i] < -RTetGen::pointMarkerOffset)
            {
                RElement element(R_ELEMENT_POINT);

                element.setNodeId(0,i);

                model.setElement(nElements,element,false);

                uint marker = std::abs(this->pointmarkerlist[i]) - RTetGen::pointMarkerOffset - 1;
                if (model.getNPoints() < marker + 1)
                {
                    model.setNPoints(marker+1);
                }
                model.getPoint(marker).add(nElements);
                nElements++;
            }
        }
    }

    //LINES
    for (unsigned int i=0;i<model.getNLines();i++)
    {
        model.getLine(i).resize(0);
    }
    if (this->edgemarkerlist)
    {
        for (int i=0;i<this->numberofedges;i++)
        {
            if (this->edgemarkerlist[i] < 0)
            {
                RElement element(R_ELEMENT_TRUSS1);

                // Line element normals are swapped
                element.setNodeId(0,this->edgelist[2*i+0] - this->firstnumber);
                element.setNodeId(1,this->edgelist[2*i+1] - this->firstnumber);

                model.setElement(nElements,element,false);

                uint marker = std::abs(this->edgemarkerlist[i]) - 1;
                if (model.getNLines() < marker + 1)
                {
                    model.setNLines(marker+1);
                }
                model.getLine(marker).add(nElements);
                nElements++;
            }
        }
    }

    // SURFACES
    for (unsigned int i=0;i<model.getNSurfaces();i++)
    {
        model.getSurface(i).resize(0);
    }
    for (int i=0;i<this->numberoftrifaces;i++)
    {
        RElement element(R_ELEMENT_TRI1);

        // Triangle element normals are swapped
        element.setNodeId(0,this->trifacelist[3*i+0] - this->firstnumber);
        element.setNodeId(1,this->trifacelist[3*i+2] - this->firstnumber);
        element.setNodeId(2,this->trifacelist[3*i+1] - this->firstnumber);

        model.setElement(nElements,element,false);
        uint marker = this->trifacemarkerlist ? std::abs(this->trifacemarkerlist[i]) - 1 : 0;
        if (model.getNSurfaces() < marker + 1)
        {
            model.setNSurfaces(marker+1);
        }
        model.getSurface(marker).add(nElements);
        nElements++;
    }

    // VOLUMES
    RUVector tetrahedraVolumeMarker(this->numberoftetrahedra,0);
    if (this->numberoftetrahedronattributes > 0)
    {
        uint minTetrahedraVolumeMarker = 0;
        uint maxTetrahedraVolumeMarker = 0;
        for (int i=0;i<this->numberoftetrahedra;i++)
        {
            tetrahedraVolumeMarker[i] = uint(this->tetrahedronattributelist[this->numberoftetrahedronattributes*i + 0]);
            minTetrahedraVolumeMarker = std::min(minTetrahedraVolumeMarker,tetrahedraVolumeMarker[i]);
            maxTetrahedraVolumeMarker = std::max(maxTetrahedraVolumeMarker,tetrahedraVolumeMarker[i]);
        }
        RUVector tetrahedraVolumeMarkerBook(maxTetrahedraVolumeMarker+1,RConstants::eod);
        for (int i=0;i<this->numberoftetrahedra;i++)
        {
            tetrahedraVolumeMarkerBook[tetrahedraVolumeMarker[i]] = 0;
        }
        uint tetrahedraVolumeMarkerBookCnt = 0;
        for (uint i=0;i<tetrahedraVolumeMarkerBook.size();i++)
        {
            if (tetrahedraVolumeMarkerBook[i] == 0)
            {
                tetrahedraVolumeMarkerBook[i] = tetrahedraVolumeMarkerBookCnt++;
            }
        }
        for (uint i=0;i<tetrahedraVolumeMarkerBook.size();i++)
        {
            if (tetrahedraVolumeMarkerBook[i] == RConstants::eod)
            {
                tetrahedraVolumeMarkerBook[i] = tetrahedraVolumeMarkerBookCnt++;
            }
        }
        for (int i=0;i<this->numberoftetrahedra;i++)
        {
            tetrahedraVolumeMarker[i] = tetrahedraVolumeMarkerBook[tetrahedraVolumeMarker[i]];
        }

        // Resize number of volumes
        model.setNVolumes(tetrahedraVolumeMarkerBookCnt);
    }
    else
    {
        model.setNVolumes(1);
    }

    for (int i=0;i<this->numberoftetrahedra;i++)
    {
        RElement element(R_ELEMENT_TETRA1);

        element.setNodeId(0,this->tetrahedronlist[4*i+0] - this->firstnumber);
        element.setNodeId(1,this->tetrahedronlist[4*i+1] - this->firstnumber);
        element.setNodeId(2,this->tetrahedronlist[4*i+2] - this->firstnumber);
        element.setNodeId(3,this->tetrahedronlist[4*i+3] - this->firstnumber);

        model.setElement(nElements,element,false);
        model.getVolume(tetrahedraVolumeMarker[i]).add(nElements);
        for (unsigned int j=0;j<4;j++)
        {
            if (this->neighborlist[4*i+j] >= this->firstnumber)
            {
                volumeNeigs[nElements].push_back(this->neighborlist[4*i+j] - this->firstnumber + numberOfPointElements + numberOfLineElements + this->numberoftrifaces);
            }
        }
        nElements++;
    }

    model.setVolumeNeighbors(volumeNeigs);

    for (unsigned int i=model.getNPoints();i>0;i--)
    {
        if (model.getPoint(i-1).size() == 0)
        {
            model.removePoint(i-1);
        }
    }
    for (unsigned int i=model.getNLines();i>0;i--)
    {
        if (model.getLine(i-1).size() == 0)
        {
            model.removeLine(i-1);
        }
    }
    for (unsigned int i=model.getNSurfaces();i>0;i--)
    {
        if (model.getSurface(i-1).size() == 0)
        {
            model.removeSurface(i-1);
        }
    }
    for (unsigned int i=model.getNVolumes();i>0;i--)
    {
        if (model.getVolume(i-1).size() == 0)
        {
            model.removeVolume(i-1);
        }
    }
    for (unsigned int i=0;i<model.getNVolumes();i++)
    {
        if (model.getVolume(i).getName().isEmpty() || model.getVolume(i).getName() == RVolume::defaultName)
        {
            model.getVolume(i).setName(RVolume::defaultName + " " + QString::number(i+1));
        }
    }

    if (keepResults)
    {
        for (uint i=0;i<model.getNVariables();i++)
        {
            model.setVariable(i,variables[i]);
        }
    }

    if (model.checkMesh() != R_MODEL_PROBLEM_NONE)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Invalid mesh was generated.");
    }

    // Synchronize normals.
    model.syncSurfaceNormals();
}
