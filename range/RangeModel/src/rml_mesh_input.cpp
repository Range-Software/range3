/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_mesh_input.cpp                                       *
 *  GROUP:  RModel                                                   *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   5-th August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: RMeshInput class definition                         *
 *********************************************************************/

#include "rml_mesh_input.h"

void RMeshInput::_init(const RMeshInput *pMeshInput)
{
    if (pMeshInput)
    {
        this->verbose = pMeshInput->verbose;
        this->volumeMesh = pMeshInput->volumeMesh;
        this->qualityMesh = pMeshInput->qualityMesh;
        this->outputEdges = pMeshInput->outputEdges;
        this->reconstruct = pMeshInput->reconstruct;
        this->useSizeFunction = pMeshInput->useSizeFunction;
        this->sizeFunctionValues = pMeshInput->sizeFunctionValues;
        this->radiusEdgeRatio = pMeshInput->radiusEdgeRatio;
        this->volumeConstraint = pMeshInput->volumeConstraint;
        this->tolerance = pMeshInput->tolerance;
        this->surfaceIntegrityCheck = pMeshInput->surfaceIntegrityCheck;
        this->keepResults = pMeshInput->keepResults;
        this->useTetGenInputParams = pMeshInput->useTetGenInputParams;
        this->tetGenInputParams = pMeshInput->tetGenInputParams;
    }
}

RMeshInput::RMeshInput() :
    verbose(true),
    volumeMesh(true),
    qualityMesh(true),
    outputEdges(false),
    reconstruct(true),
    useSizeFunction(false),
    radiusEdgeRatio(1.6),
    volumeConstraint(0.1),
    tolerance(1.0e-10),
    surfaceIntegrityCheck(false),
    keepResults(true),
    useTetGenInputParams(false),
    tetGenInputParams(QString())
{
    this->_init();
}

RMeshInput::RMeshInput(const RMeshInput &meshInput)
{
    this->_init(&meshInput);
}

RMeshInput::~RMeshInput()
{
}

RMeshInput &RMeshInput::operator =(const RMeshInput &meshInput)
{
    this->_init(&meshInput);
    return (*this);
}

bool RMeshInput::getVerbose(void) const
{
    return this->verbose;
}

void RMeshInput::setVerbose(bool verbose)
{
    this->verbose = verbose;
}

bool RMeshInput::getVolumeMesh(void) const
{
    return this->volumeMesh;
}

void RMeshInput::setVolumeMesh(bool volumeMesh)
{
    this->volumeMesh = volumeMesh;
}

bool RMeshInput::getQualityMesh(void) const
{
    return this->qualityMesh;
}

void RMeshInput::setQualityMesh(bool qualityMesh)
{
    this->qualityMesh = qualityMesh;
}

bool RMeshInput::getOutputEdges(void) const
{
    return this->outputEdges;
}

void RMeshInput::setOutputEdges(bool outputEdges)
{
    this->outputEdges = outputEdges;
}

bool RMeshInput::getReconstruct(void) const
{
    return this->reconstruct;
}

void RMeshInput::setReconstruct(bool reconstruct)
{
    this->reconstruct = reconstruct;
}

bool RMeshInput::getUseSizeFunction(void) const
{
    return this->useSizeFunction;
}

void RMeshInput::setUseSizeFunction(bool useSizeFunction)
{
    this->useSizeFunction = useSizeFunction;
}

const RRVector &RMeshInput::getSizeFunctionValues(void) const
{
    return this->sizeFunctionValues;
}

void RMeshInput::setSizeFunctionValues(const RRVector &sizeFunctionValues)
{
    this->sizeFunctionValues = sizeFunctionValues;
}

double RMeshInput::getRadiusEdgeRatio(void) const
{
    return this->radiusEdgeRatio;
}

void RMeshInput::setRadiusEdgeRatio(double radiusEdgeRatio)
{
    this->radiusEdgeRatio = radiusEdgeRatio;
}

double RMeshInput::getVolumeConstraint(void) const
{
    return this->volumeConstraint;
}

void RMeshInput::setVolumeConstraint(double volumeConstrain)
{
    this->volumeConstraint = volumeConstrain;
}

double RMeshInput::getTolerance(void) const
{
    return this->tolerance;
}

void RMeshInput::setTolerance(double tolerance)
{
    this->tolerance = tolerance;
}

bool RMeshInput::getSurfaceIntegrityCheck(void) const
{
    return this->surfaceIntegrityCheck;
}

void RMeshInput::setSurfaceIntegrityCheck(bool surfaceIntegrityCheck)
{
    this->surfaceIntegrityCheck = surfaceIntegrityCheck;
}

bool RMeshInput::getKeepResults(void) const
{
    return this->keepResults;
}

void RMeshInput::setKeepResults(bool keepResults)
{
    this->keepResults = keepResults;
}

bool RMeshInput::getUseTetGenInputParams(void) const
{
    return this->useTetGenInputParams;
}

void RMeshInput::setUseTetGenInputParams(bool useTetGenInputParams)
{
    this->useTetGenInputParams = useTetGenInputParams;
}

const QString &RMeshInput::getTetGenInputParams(void) const
{
    return this->tetGenInputParams;
}

void RMeshInput::setTetGenInputParams(const QString &tetGenInputParams)
{
    this->tetGenInputParams = tetGenInputParams;
}
