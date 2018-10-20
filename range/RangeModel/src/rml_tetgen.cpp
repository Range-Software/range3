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

RTetGen::RTetGen(const RModel &model)
{
    this->importModel(model,false);
}

void RTetGen::importModel(const RModel &model, bool reconstruct, const RRVector &nodeMeshSizeValues)
{
    this->firstnumber = 0;

    // Set nodes

    this->numberofpoints = int(model.getNNodes());
    this->numberofpointattributes = 0;
    this->numberofpointmtrs = nodeMeshSizeValues.size() == model.getNNodes() ? 1 : 0;

    if (this->numberofpoints)
    {
        this->pointlist = new REAL [uint(this->numberofpoints)*3];
//        if (this->numberofpointattributes)
//        {
//            this->pointattributelist = new REAL [this->numberofpoints*this->numberofpointattributes];
//        }
        if (this->numberofpointmtrs)
        {
            this->pointmtrlist = new REAL [uint(this->numberofpoints)*uint(this->numberofpointmtrs)];
        }
        this->pointmarkerlist = new int [uint(this->numberofpoints)];

        for (uint i=0;i<uint(this->numberofpoints);i++)
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
                this->pointmtrlist[uint(this->numberofpointmtrs)*i+0] = nodeMeshSizeValues[i];
                for (uint j=1;j<uint(this->numberofpointmtrs);j++)
                {
                    this->pointmtrlist[uint(this->numberofpointmtrs)*i+j] = 0.0;
                }
            }
            this->pointmarkerlist[i] = 0;
        }
    }

    for (uint i=0;i<model.getNPoints();i++)
    {
        int marker = int(i)+1;
        const RPoint &rPoint = model.getPoint(i);
        for (uint j=0;j<rPoint.size();j++)
        {
            this->pointmarkerlist[model.getElement(rPoint.get(j)).getNodeId(0)] = -marker;
        }
    }

    this->numberofedges = int(model.getNEntityElements(R_ENTITY_GROUP_LINE));
    if (this->numberofedges > 0)
    {
        this->edgelist = new int [uint(this->numberofedges)*2];
        this->edgemarkerlist = new int [uint(this->numberofedges)];
        uint edgeNumber = 0;
        for (uint i=0;i<model.getNLines();i++)
        {
            int marker = int(i)+1;
            const RLine &rLine = model.getLine(i);
            for (uint j=0;j<rLine.size();j++)
            {
                uint elementID = rLine.get(j);
                const RElement &rElement = model.getElement(elementID);
                this->edgelist[2*edgeNumber+0] = int(rElement.getNodeId(0)) + this->firstnumber;
                this->edgelist[2*edgeNumber+1] = int(rElement.getNodeId(1)) + this->firstnumber;
                this->edgemarkerlist[edgeNumber] = -marker;
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
            this->trifacelist = new int[uint(this->numberoftrifaces)*3];
            this->trifacemarkerlist = new int[uint(this->numberoftrifaces)];

            RUVector surfaceBook(model.getNElements());
            std::fill(surfaceBook.begin(),surfaceBook.end(),RConstants::eod);

            uint ce = 0;
            for (uint i=0;i<model.getNElements();i++)
            {
                if (model.getElement(i).getType() != R_ELEMENT_TRI1 &&
                    model.getElement(i).getType() != R_ELEMENT_TRI2)
                {
                    continue;
                }
                this->trifacelist[3*ce+0] = int(model.getElement(i).getNodeId(0)) + this->firstnumber;
                this->trifacelist[3*ce+1] = int(model.getElement(i).getNodeId(1)) + this->firstnumber;
                this->trifacelist[3*ce+2] = int(model.getElement(i).getNodeId(2)) + this->firstnumber;

                this->trifacemarkerlist[ce] = -1;

                surfaceBook[i] = ce;

                ce++;
            }

            for (uint i=0;i<model.getNSurfaces();i++)
            {
                int marker = int(i+1);
                for (uint j=0;j<model.getSurface(i).size();j++)
                {
                    uint elementPosition = surfaceBook[model.getSurface(i).get(j)];
                    if (elementPosition != RConstants::eod)
                    {
                        this->trifacemarkerlist[elementPosition] = -marker;
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
            this->tetrahedronlist = new int[uint(this->numberoftetrahedra)*4];
            this->tetrahedronattributelist = new REAL[uint(this->numberoftetrahedra)*uint(this->numberoftetrahedronattributes)];
            this->tetrahedronvolumelist = new REAL[uint(this->numberoftetrahedra)];

            RUVector volumeBook(model.getNElements());
            std::fill(volumeBook.begin(),volumeBook.end(),RConstants::eod);

            uint ce = 0;
            for (uint i=0;i<model.getNElements();i++)
            {
                if (model.getElement(i).getType() != R_ELEMENT_TETRA1)
                {
                    continue;
                }
                this->tetrahedronlist[4*ce+0] = int(model.getElement(i).getNodeId(0)) + this->firstnumber;
                this->tetrahedronlist[4*ce+1] = int(model.getElement(i).getNodeId(1)) + this->firstnumber;
                this->tetrahedronlist[4*ce+2] = int(model.getElement(i).getNodeId(2)) + this->firstnumber;
                this->tetrahedronlist[4*ce+3] = int(model.getElement(i).getNodeId(3)) + this->firstnumber;

                this->tetrahedronattributelist[uint(this->numberoftetrahedronattributes)*ce+0] = 0.0;

                this->tetrahedronvolumelist[ce] = -1.0;

                volumeBook[i] = ce;

                ce++;
            }

            // Set volume marker for each tetrahedra
            for (uint i=0;i<model.getNVolumes();i++)
            {
                int marker = int(i+1);
                for (uint j=0;j<model.getVolume(i).size();j++)
                {
                    uint elementPosition = volumeBook[model.getVolume(i).get(j)];
                    if (elementPosition != RConstants::eod)
                    {
                        this->tetrahedronattributelist[uint(this->numberoftetrahedronattributes)*elementPosition+0] = -marker;
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
            this->facetlist = new tetgenio::facet[uint(this->numberoffacets)];
            this->facetmarkerlist = new int[uint(this->numberoffacets)];

            RUVector faceBook(model.getNElements());
            std::fill(faceBook.begin(),faceBook.end(),RConstants::eod);

            uint ce = 0;
            for (uint i=0;i<model.getNElements();i++)
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
                this->facetlist[ce].holelist = nullptr;
                this->facetlist[ce].numberofpolygons = 1;
                this->facetlist[ce].polygonlist = new tetgenio::polygon[1];
                this->facetlist[ce].polygonlist[0].numberofvertices = int(nne);
                this->facetlist[ce].polygonlist[0].vertexlist = new int[nne];
                for (uint j=0;j<nne;j++)
                {
                    this->facetlist[ce].polygonlist[0].vertexlist[j] = int(model.getElement(i).getNodeId(j)) + this->firstnumber;
                }

                this->facetmarkerlist[ce] = 0;

                faceBook[i] = ce;

                ce++;
            }

            for (uint i=0;i<model.getNSurfaces();i++)
            {
                int marker = int(i+1);
                for (uint j=0;j<model.getSurface(i).size();j++)
                {
                    uint elementPosition = faceBook[model.getSurface(i).get(j)];
                    if (elementPosition != RConstants::eod)
                    {
                        this->facetmarkerlist[elementPosition] = -marker;
                    }
                }
            }
        }
    }
}

void RTetGen::exportMesh(RModel &model, bool keepResults) const
{
    // Find number of point elements.
    uint numberOfPointElements = 0;
    if (this->pointmarkerlist)
    {
        for (int i=0;i<this->numberofpoints;i++)
        {
            if (this->pointmarkerlist[i] > 0)
            {
                numberOfPointElements++;
            }
        }
    }

    // Find number of line elements.
    uint numberOfLineElements = 0;
    if (this->edgemarkerlist)
    {
        for (int i=0;i<this->numberofedges;i++)
        {
            if (this->edgemarkerlist[i] > 0)
            {
                numberOfLineElements++;
            }
        }
    }

    // Interpolate element results.
    std::vector<RVariable> variables;

    if (keepResults && model.getNVariables() > 0)
    {
        RLogger::info("Interpolating results\n");
        RLogger::indent();

        RProgressInitialize("Interpolating results");
        for (uint i=0;i<model.getNVariables();i++)
        {
            RProgressPrint(i,model.getNVariables());
            RVariable variable = model.getVariable(i);
            RLogger::info("Interpolating %s\n",variable.getName().toUtf8().constData());
            RLogger::indent();
            if (model.getVariable(i).getApplyType() == R_VARIABLE_APPLY_NODE)
            {
                variable.resize(variable.getNVectors(),uint(this->numberofpoints));
#pragma omp parallel for default(shared)
                for (int64_t j=0;j<this->numberofpoints;j++)
                {
                    RNode node(this->pointlist[3*j+0],this->pointlist[3*j+1],this->pointlist[3*j+2]);
                    RRVector values = model.getInterpolatedResultsValues(variable.getType(),node);
                    for (uint k=0;k<values.size();k++)
                    {
                        variable.setValue(k,uint(j),values[k]);
                    }
                }
                variables.push_back(variable);
            }
            else if (model.getVariable(i).getApplyType() == R_VARIABLE_APPLY_ELEMENT)
            {
                variable.resize(variable.getNVectors(),  numberOfPointElements
                                                       + numberOfLineElements
                                                       + uint(this->numberoftrifaces)
                                                       + uint(this->numberoftetrahedra));
#pragma omp parallel for default(shared)
                for (int64_t j=0;j<this->numberoftetrahedra;j++)
                {
                    int n1 = this->tetrahedronlist[4*j+0] - this->firstnumber;
                    int n2 = this->tetrahedronlist[4*j+1] - this->firstnumber;
                    int n3 = this->tetrahedronlist[4*j+2] - this->firstnumber;
                    int n4 = this->tetrahedronlist[4*j+3] - this->firstnumber;

                    RNode node1(this->pointlist[3*n1+0],this->pointlist[3*n1+1],this->pointlist[3*n1+2]);
                    RNode node2(this->pointlist[3*n2+0],this->pointlist[3*n2+1],this->pointlist[3*n2+2]);
                    RNode node3(this->pointlist[3*n3+0],this->pointlist[3*n3+1],this->pointlist[3*n3+2]);
                    RNode node4(this->pointlist[3*n4+0],this->pointlist[3*n4+1],this->pointlist[3*n4+2]);

                    RNode node((node1.getX()+node2.getX()+node3.getX()+node4.getX())/4.0,
                               (node1.getY()+node2.getY()+node3.getY()+node4.getY())/4.0,
                               (node1.getZ()+node2.getZ()+node3.getZ()+node4.getZ())/4.0);

                    RRVector values = model.getInterpolatedResultsValues(variable.getType(),node);

                    uint offset = numberOfPointElements + numberOfLineElements + uint(this->numberoftrifaces);

                    for (uint k=0;k<values.size();k++)
                    {
                        variable.setValue(k,offset+uint(j),values[k]);
                    }
                }
                variables.push_back(variable);
            }
            RLogger::unindent();
        }
        RProgressFinalize();
        RLogger::unindent();
    }

    // NODES
    model.setNNodes(uint(this->numberofpoints));

    for (uint i=0;i<uint(this->numberofpoints);i++)
    {
        model.getNode(i).set(this->pointlist[3*i+0],
                             this->pointlist[3*i+1],
                             this->pointlist[3*i+2]);
    }

    model.setNVariables(uint(variables.size()));

    model.setNElements(numberOfPointElements + numberOfLineElements + uint(this->numberoftrifaces) + uint(this->numberoftetrahedra));

    std::vector<RUVector> volumeNeigs;

    model.clearSurfaceNeighbors();
    model.clearVolumeNeighbors();

    volumeNeigs.resize(numberOfPointElements + numberOfLineElements + uint(this->numberoftrifaces) + uint(this->numberoftetrahedra));

    uint nElements = 0;

    // POINTS
    for (uint i=0;i<model.getNPoints();i++)
    {
        model.getPoint(i).resize(0);
    }
    if (this->pointmarkerlist)
    {
        for (uint i=0;i<uint(this->numberofpoints);i++)
        {
            if (this->pointmarkerlist[i] > 0)
            {
                RElement element(R_ELEMENT_POINT);

                element.setNodeId(0,i);

                model.setElement(nElements,element,false);

                uint marker = uint(std::abs(this->pointmarkerlist[i])) - 1;
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
    for (uint i=0;i<model.getNLines();i++)
    {
        model.getLine(i).resize(0);
    }
    if (this->edgemarkerlist)
    {
        for (uint i=0;i<uint(this->numberofedges);i++)
        {
            if (this->edgemarkerlist[i] > 0)
            {
                RElement element(R_ELEMENT_TRUSS1);

                // Line element normals are swapped
                element.setNodeId(0,uint(this->edgelist[2*i+0] - this->firstnumber));
                element.setNodeId(1,uint(this->edgelist[2*i+1] - this->firstnumber));

                model.setElement(nElements,element,false);

                uint marker = uint(std::abs(this->edgemarkerlist[i])) - 1;
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
    for (uint i=0;i<model.getNSurfaces();i++)
    {
        model.getSurface(i).resize(0);
    }
    for (uint i=0;i<uint(this->numberoftrifaces);i++)
    {
        RElement element(R_ELEMENT_TRI1);

        // Triangle element normals are swapped
        element.setNodeId(0,uint(this->trifacelist[3*i+0] - this->firstnumber));
        element.setNodeId(1,uint(this->trifacelist[3*i+2] - this->firstnumber));
        element.setNodeId(2,uint(this->trifacelist[3*i+1] - this->firstnumber));

        model.setElement(nElements,element,false);
        uint marker = this->trifacemarkerlist ? uint(std::abs(this->trifacemarkerlist[i])) - 1 : 0;
        if (model.getNSurfaces() < marker + 1)
        {
            model.setNSurfaces(marker+1);
        }
        model.getSurface(marker).add(nElements);
        nElements++;
    }

    // VOLUMES
    RUVector tetrahedraVolumeMarker(uint(this->numberoftetrahedra),0);
    if (this->numberoftetrahedronattributes > 0)
    {
        uint minTetrahedraVolumeMarker = 0;
        uint maxTetrahedraVolumeMarker = 0;
        for (uint i=0;i<uint(this->numberoftetrahedra);i++)
        {
            tetrahedraVolumeMarker[i] = uint(std::abs(this->tetrahedronattributelist[uint(this->numberoftetrahedronattributes)*i + 0]) - 1);
            minTetrahedraVolumeMarker = std::min(minTetrahedraVolumeMarker,tetrahedraVolumeMarker[i]);
            maxTetrahedraVolumeMarker = std::max(maxTetrahedraVolumeMarker,tetrahedraVolumeMarker[i]);
        }
        RUVector tetrahedraVolumeMarkerBook(maxTetrahedraVolumeMarker+1,RConstants::eod);
        for (uint i=0;i<uint(this->numberoftetrahedra);i++)
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
        for (uint i=0;i<uint(this->numberoftetrahedra);i++)
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

    // VOLUME NEIGHBORS
    for (uint i=0;i<uint(this->numberoftetrahedra);i++)
    {
        RElement element(R_ELEMENT_TETRA1);

        element.setNodeId(0,uint(this->tetrahedronlist[4*i+0] - this->firstnumber));
        element.setNodeId(1,uint(this->tetrahedronlist[4*i+1] - this->firstnumber));
        element.setNodeId(2,uint(this->tetrahedronlist[4*i+2] - this->firstnumber));
        element.setNodeId(3,uint(this->tetrahedronlist[4*i+3] - this->firstnumber));

        model.setElement(nElements,element,false);
        model.getVolume(tetrahedraVolumeMarker[i]).add(nElements);
        for (uint j=0;j<4;j++)
        {
            if (this->neighborlist[4*i+j] >= this->firstnumber)
            {
                volumeNeigs[nElements].push_back(uint(this->neighborlist[4*i+j]) - uint(this->firstnumber) + numberOfPointElements + numberOfLineElements + uint(this->numberoftrifaces));
            }
        }
        nElements++;
    }

    model.setVolumeNeighbors(volumeNeigs);

    for (uint i=model.getNPoints();i>0;i--)
    {
        if (model.getPoint(i-1).size() == 0)
        {
            model.removePoint(i-1);
        }
    }
    for (uint i=model.getNLines();i>0;i--)
    {
        if (model.getLine(i-1).size() == 0)
        {
            model.removeLine(i-1);
        }
    }
    for (uint i=model.getNSurfaces();i>0;i--)
    {
        if (model.getSurface(i-1).size() == 0)
        {
            model.removeSurface(i-1);
        }
    }
    for (uint i=model.getNVolumes();i>0;i--)
    {
        if (model.getVolume(i-1).size() == 0)
        {
            model.removeVolume(i-1);
        }
    }
    for (uint i=0;i<model.getNVolumes();i++)
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
