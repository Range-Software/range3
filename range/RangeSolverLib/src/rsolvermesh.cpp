/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolvermesh.cpp                                          *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   11-th October 2018                                       *
 *                                                                   *
 *  DESCRIPTION: Mesh solver class definition                        *
 *********************************************************************/

#include <omp.h>

#include "rsolvermesh.h"

void RSolverMesh::_init(const RSolverMesh *pSolver)
{
    if (pSolver)
    {

    }
}

RSolverMesh::RSolverMesh(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData)
    : RSolverGeneric(pModel,modelFileName,convergenceFileName,sharedData)
{
    this->problemType = R_PROBLEM_MESH;
    this->_init();
}

RSolverMesh::RSolverMesh(const RSolverMesh &solver)
    : RSolverGeneric(solver)
{
    this->_init(&solver);
}

RSolverMesh::~RSolverMesh()
{

}

RSolverMesh &RSolverMesh::operator =(const RSolverMesh &solver)
{
    RSolverGeneric::operator =(solver);
    this->_init(&solver);
    return (*this);
}

bool RSolverMesh::hasConverged() const
{
    return true;
}

void RSolverMesh::updateScales()
{

}

void RSolverMesh::recover()
{

}

void RSolverMesh::prepare()
{
    RLogger::info("Calculating volume constraint.\n");
    RLogger::indent();
    double maxVolume = 0.0;

    for (uint i=0;i<this->pModel->getNVolumes();i++)
    {
        const RVolume &rVolume = this->pModel->getVolume(i);
#pragma omp parallel for default(shared)
        for (int64_t j=0;j<int64_t(rVolume.size());j++)
        {
            uint elementID = rVolume.get(uint(j));
            const RElement &rElement = this->pModel->getElement(elementID);

            double volume = 0.0;
            if (rElement.findVolume(this->pModel->getNodes(),volume))
            {
#pragma omp critical
                {
                    maxVolume = std::max(maxVolume,volume);
                }
            }
        }
    }
    RLogger::info("Maximum element volume = %g\n",maxVolume);
    RLogger::unindent();

    RLogger::info("Calculating mesh size function.\n");
    RLogger::indent();
    const RMeshSetup &rMeshSetup = this->pModel->getProblemSetup().getMeshSetup();
    RRVector meshSizeFunction = this->pModel->generateMeshSizeFunction(rMeshSetup.getVariables(),
                                                                       rMeshSetup.getMinEdgeLength(),
                                                                       rMeshSetup.getMaxEdgeLength(),
                                                                       0.5);
    RLogger::unindent();

    this->meshInput.setQualityMesh(true);
    this->meshInput.setKeepResults(true);
    this->meshInput.setReconstruct(true);
    this->meshInput.setSizeFunctionValues(meshSizeFunction);
    this->meshInput.setUseSizeFunction(true);
    this->meshInput.setUseTetGenInputParams(false);
    this->meshInput.setVolumeConstraint(maxVolume);
    this->meshInput.setVolumeMesh(true);
}

void RSolverMesh::solve()
{
    RLogger::info("Generating 3D mesh for model \'%s\'\n",this->pModel->getName().toUtf8().constData());
    RLogger::indent();
    try
    {
        RMeshGenerator::generate(this->meshInput,(*this->pModel));
    }
    catch (const RError &error)
    {
        RLogger::unindent();
        throw RError(error.getType(),R_ERROR_REF,"Failed to generate 3D mesh: %s\n",error.getMessage().toUtf8().constData());
    }

    RLogger::unindent();
}

void RSolverMesh::process()
{

}

void RSolverMesh::store()
{

}

void RSolverMesh::statistics()
{

}
