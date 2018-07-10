/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   mesh_generator.cpp                                       *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   7-th September 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Mesh generator class definition                     *
 *********************************************************************/

#include <rblib.h>
#include <rmlib.h>

#include "session.h"
#include "mesh_generator.h"

MeshGenerator::MeshGenerator(uint modelID, QObject *parent) :
    Job(parent),
    modelID(modelID)
{
}

void MeshGenerator::run(void)
{
    Session::getInstance().storeCurentModelVersion(this->modelID,tr("Generate 3D mesh"));

    Model model = Session::getInstance().getModel(this->modelID);
    RLogger::info("Generating 3D mesh for model \'%s\'\n",model.getName().toUtf8().constData());
    RLogger::indent();

    try
    {
        if (model.getMeshInput().getSurfaceIntegrityCheck())
        {
            uint nIntersected = model.breakIntersectedElements(10);
            if (nIntersected > 0)
            {
                RLogger::warning("Number of intersected elements found = %u.\n",nIntersected);
            }
        }
        RMeshGenerator::generate(model.getMeshInput(),model);
        model.clearEdgeNodes();
    }
    catch (const RError &error)
    {
        RLogger::error("Failed to generate 3D mesh: %s\n",error.getMessage().toUtf8().constData());
        RLogger::unindent();
        return;
    }

    RLogger::unindent();

    RLogger::info("Consolidating mesh internal structures.\n");
    RLogger::indent();

    try
    {
        model.consolidate(Model::ConsolidateEdgeElements | Model::ConsolidateHoleElements | Model::ConsolidateSliverElements);
    }
    catch (const RError &error)
    {
        RLogger::error("Failed to consolidate internal structures: %s\n",error.getMessage().toUtf8().constData());
        RLogger::unindent();
        return;
    }

    RLogger::unindent();

    Session::getInstance().setModel(this->modelID,model);
}
