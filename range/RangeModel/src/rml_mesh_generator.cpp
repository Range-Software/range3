/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_mesh_generator.cpp                                   *
 *  GROUP:  RModel                                                   *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   5-th August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: RMeshGenerator class definition                     *
 *********************************************************************/

#include <QString>
#include <cstring>

#include <tetgen.h>

#include "rml_mesh_generator.h"
#include "rml_tetgen.h"
#include "rml_model_raw.h"

void RMeshGenerator::generate(const RMeshInput &meshInput, RModel &model)
{
    RTetGen tetgenIn;
    RTetGen tetgenOut;

    if (model.getNSurfaces() == 0 && model.getNVolumes() == 0)
    {
        throw RError(R_ERROR_INVALID_INPUT,R_ERROR_REF, "Model contains no surface nor volume elements.");
    }

    // Convert Range model to TetGen mesh.
    try
    {
        RLogger::info("Converting Range model to TetGen mesh.\n");
        RLogger::indent();
        tetgenIn.importModel(model,meshInput.getReconstruct());
        RLogger::unindent();
        RLogger::info("Successfully converted Range model to TetGen mesh.\n");
    }
    catch (const RError &error)
    {
        RLogger::unindent();
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to export mesh to TetGen format: %s", error.getMessage().toUtf8().constData());
    }

    QString parameters(meshInput.getUseTetGenInputParams() ? meshInput.getTetGenInputParams() : meshInput.generateTetGenInputParams());

    char *args = new char[parameters.size() + 1];
    snprintf(args,parameters.size() + 1,"%s",parameters.toUtf8().constData());

    // Generate 3D mesh.
    try
    {
        RLogger::info("Generating volume mesh with parameters \"%s\".\n", args);
        RLogger::indent();
        tetgen_set_print_func(RLogger::info);
        tetrahedralize(args,&tetgenIn,&tetgenOut);
        RLogger::unindent();
        RLogger::info("Mesh generation has finished successfully.\n");
    }
    catch (int errorCode)
    {
        RLogger::unindent();
        free(args);
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Mesh generation failed with error code \"%d\"", errorCode);
    }

    free(args);

    // Convert TetGen mesh to Range model.
    try
    {
        RLogger::info("Converting TetGen mesh to Range model.\n");
        RLogger::indent();
        tetgenOut.exportMesh(model,meshInput.getKeepResults());
        RLogger::unindent();
        RLogger::info("Successfully converted TetGen mesh to Range model.\n");
    }
    catch (const RError &error)
    {
        RLogger::unindent();
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to import mesh from TetGen format: %s", error.getMessage().toUtf8().constData());
    }
}

std::vector<RElement> RMeshGenerator::generate(const std::vector<RNode> &nodes, const std::vector<RElement> &elements)
{
    RModel model;

    model.setNNodes(uint(nodes.size()));
    for (uint i=0;i<nodes.size();i++)
    {
        model.setNode(i,nodes[i]);
    }
    model.setNElements(uint(elements.size()));
    for (uint i=0;i<elements.size();i++)
    {
        model.setElement(i,elements[i],true);
    }

    model.purgeUnusedElements();
    model.purgeUnusedNodes();

    RMeshInput input;
    input.setVerbose(false);
    input.setSurfaceIntegrityCheck(true);
    input.setQualityMesh(false);
    input.setKeepResults(false);
    input.setOutputEdges(false);

    try
    {
        RMeshGenerator::generate(input,model);
    }
    catch (const RError &error)
    {
        throw error;
    }

    std::vector<RElement> volumes;

    for (uint i=0;i<model.getNVolumes();i++)
    {
        const RVolume &rVolume = model.getVolume(i);
        for (uint j=0;j<rVolume.size();j++)
        {
            const RElement &rElement = model.getElement(rVolume.get(j));
            if (rElement.getType() != R_ELEMENT_TETRA1)
            {
                continue;
            }
            volumes.push_back(rElement);
        }
    }

    return volumes;
}

std::vector<RTetrahedron> RMeshGenerator::generate(const std::vector<RTriangle> &triangles)
{
    RModelRaw modelRaw;
    for (uint i=0;i<triangles.size();i++)
    {
        modelRaw.addTriangle(triangles[i].getNode1(),triangles[i].getNode2(),triangles[i].getNode3());
    }
    RModel model(modelRaw);

    RMeshInput input;
    input.setVerbose(false);
    input.setSurfaceIntegrityCheck(true);
    input.setQualityMesh(false);
    input.setKeepResults(false);
    input.setOutputEdges(false);

    try
    {
        RMeshGenerator::generate(input,model);
    }
    catch (const RError &error)
    {
        throw error;
    }

    std::vector<RTetrahedron> tetrahedrons;
    for (uint i=0;i<model.getNVolumes();i++)
    {
        const RVolume &rVolume = model.getVolume(i);
        for (uint j=0;j<rVolume.size();j++)
        {
            const RElement &rElement = model.getElement(rVolume.get(j));
            if (rElement.getType() != R_ELEMENT_TETRA1)
            {
                continue;
            }
            uint n1 = rElement.getNodeId(0);
            uint n2 = rElement.getNodeId(1);
            uint n3 = rElement.getNodeId(2);
            uint n4 = rElement.getNodeId(3);

            tetrahedrons.push_back(RTetrahedron(model.getNode(n1),model.getNode(n2),model.getNode(n3),model.getNode(n4)));
        }
    }

    return tetrahedrons;
}
