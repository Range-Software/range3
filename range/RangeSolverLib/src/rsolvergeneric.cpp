/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolvergeneric.cpp                                       *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   12-th December 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Generic solver class definition                     *
 *********************************************************************/

#include <QFile>
#include <cstdio>

#include "rsolvergeneric.h"


const double RSolverGeneric::sigma = 5.670367e-8; // Stefan-Boltzman constant [W/(m*m*K*K*K*K)]
const double RSolverGeneric::e0 = 8.854187817e-12; // Vacuum permittivity [F/m]

void RSolverGeneric::_init(const RSolverGeneric *pGenericSolver)
{
    if (pGenericSolver)
    {
        this->meshChanged = pGenericSolver->meshChanged;
        this->problemType = pGenericSolver->problemType;
        this->pModel = pGenericSolver->pModel;
        this->M = pGenericSolver->M;
        this->A = pGenericSolver->A;
        this->x = pGenericSolver->x;
        this->b = pGenericSolver->b;
        this->nodeBook = pGenericSolver->nodeBook;
        this->localRotations = pGenericSolver->localRotations;
        this->elementTemperature = pGenericSolver->elementTemperature;
        this->pSharedData = pGenericSolver->pSharedData;
        this->firstRun = pGenericSolver->firstRun;
        this->taskIteration = pGenericSolver->taskIteration;
        this->computableElements = pGenericSolver->computableElements;
    }
}

RSolverGeneric::RSolverGeneric(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData)
    : meshChanged(true)
    , problemType(R_PROBLEM_NONE)
    , pModel(pModel)
    , modelFileName(modelFileName)
    , convergenceFileName(convergenceFileName)
    , pSharedData(&sharedData)
    , firstRun(false)
    , taskIteration(0)
    , computableElements(this->pModel->getNElements(),false)
{
    this->elementTemperature = this->pSharedData->findData("element-temperature");
    this->elementTemperature.resize(this->pModel->getNElements(),RVariable::getInitValue(R_VARIABLE_TEMPERATURE));
    this->localRotations.resize(this->pModel->getNNodes());
    this->_init();
}

RSolverGeneric::RSolverGeneric(const RSolverGeneric &genericSolver)
{
    this->_init(&genericSolver);
}

RSolverGeneric::~RSolverGeneric()
{

}

RSolverGeneric &RSolverGeneric::operator =(const RSolverGeneric &genericSolver)
{
    this->_init(&genericSolver);
    return (*this);
}

void RSolverGeneric::run(bool firstRun, uint taskIteration)
{
    this->firstRun = firstRun;
    this->taskIteration = taskIteration;

    this->elementTemperature.resize(this->pModel->getNElements(),RVariable::getInitValue(R_VARIABLE_TEMPERATURE));

    if (this->taskIteration == 0)
    {
        this->findComputableElements(this->problemType);
        this->findIncludableElements();
        this->findInwardElements();
        this->updateLocalRotations();
    }

    if (this->problemType == R_PROBLEM_STRESS_MODAL)
    {
        RModalSetup &modalSetup = this->pModel->getProblemSetup().getModalSetup();

        this->updateScales();
        this->scales.downscale(*this->pModel);

        this->recoverSharedData();
        this->recover();
        this->prepare();
        this->solve();

        this->scales.upscale(*this->pModel);

        uint nModes = modalSetup.getNModesToExtract();
        if (modalSetup.getMethod() == R_MODAL_DOMINANT_MODE)
        {
            nModes = 1;
        }

        for (uint i=0;i<nModes;i++)
        {
            uint mode = nModes - (i + 1);
            RLogger::info("Extracting mode %d\n",mode+1);
            modalSetup.setMode(mode);

            RLogger::indent();

            this->scales.downscale(*this->pModel);

            this->process();
            this->store();
            this->storeSharedData();

            this->scales.upscale(*this->pModel);

            this->writeResults();
            this->statistics();

            RLogger::unindent();

            if (RApplicationState::getInstance().getStateType() == R_APPLICATION_STATE_STOP)
            {
                break;
            }
        }
    }
    else
    {
        if (this->problemType != R_PROBLEM_STRESS && this->problemType != R_PROBLEM_STRESS_MODAL && this->problemType != R_PROBLEM_MESH)
        {
            this->applyDisplacement();
        }

        this->updateScales();
        if (this->problemType != R_PROBLEM_MESH)
        {
            this->scales.downscale(*this->pModel);
        }

        this->recoverSharedData();
        this->recover();
        this->prepare();
        this->solve();
        this->process();
        this->store();
        this->storeSharedData();

        if (this->problemType != R_PROBLEM_MESH)
        {
            this->scales.upscale(*this->pModel);
        }

        if (this->problemType != R_PROBLEM_STRESS && this->problemType != R_PROBLEM_STRESS_MODAL)
        {
            this->removeDisplacement();
        }

        this->writeResults();
        this->statistics();
    }

    this->meshChanged = (this->problemType == R_PROBLEM_MESH);
}

bool RSolverGeneric::getMeshChanged() const
{
    return this->meshChanged;
}

void RSolverGeneric::setMeshChanged(bool meshChanged)
{
    this->meshChanged = meshChanged;
}

void RSolverGeneric::updateOldRecords(const RTimeSolver &rTimeSolver, const QString &modelFileName)
{
    if (rTimeSolver.getEnabled())
    {
        // Update previous records.
        for (unsigned int i=0;i<=rTimeSolver.getCurrentTimeStep();i++)
        {
            QString fileName = RFileManager::getFileNameWithTimeStep(modelFileName,i);

            if (RFileManager::fileExists(fileName))
            {
                RLogger::info("Updating model file \'%s\'\n",fileName.toUtf8().constData());

                RModel model;
                model.read(fileName);
                model.getTimeSolver().setTimes(rTimeSolver.getTimes());
                model.getTimeSolver().setInputNTimeSteps(rTimeSolver.getInputNTimeSteps());
                model.getTimeSolver().setInputStartTime(rTimeSolver.getInputStartTime());
                model.getTimeSolver().setInputTimeStepSize(rTimeSolver.getInputTimeStepSize());
                model.write(fileName);
            }
        }
        // Delete records which will be computed again.
        for (unsigned int i=rTimeSolver.getCurrentTimeStep();i<rTimeSolver.getNTimeSteps();i++)
        {
            QString fileName = RFileManager::getFileNameWithTimeStep(modelFileName,i+1);

            if (RFileManager::fileExists(fileName))
            {
                RLogger::info("Removing model file \'%s\'\n",fileName.toUtf8().constData());
                try
                {
                    RFileManager::remove(fileName);
                }
                catch (RError &error)
                {
                    RLogger::warning("Failed to remove model file \'%s\'. ERROR: %s\n",fileName.toUtf8().constData(),error.getMessage().toUtf8().constData());
                }
            }
        }
    }
}

RRVector RSolverGeneric::findElementSizes(void) const
{
    uint ne = this->pModel->getNElements();
    RRVector elementSizes(ne,0.0);
    for (uint i=0;i<ne;i++)
    {
        elementSizes[i] = this->pModel->getElement(i).findSize(this->pModel->getNodes());
    }
    return elementSizes;
}

double RSolverGeneric::findElementScale(bool onlyVolumes) const
{
    RRVector elementSizes = this->findElementSizes();

    if (onlyVolumes)
    {
        uint ne = this->pModel->getNElements();

        std::vector<bool> volumeElements;
        volumeElements.resize(ne,false);

        for (uint i=0;i<this->pModel->getNLines();i++)
        {
            const RLine &rLine = this->pModel->getLine(i);
            if (rLine.getCrossArea() > RConstants::eps)
            {
                for (uint j=0;j<rLine.size();j++)
                {
                    volumeElements[rLine.get(j)] = true;
                }
            }
        }
        for (uint i=0;i<this->pModel->getNSurfaces();i++)
        {
            const RSurface &rSurface = this->pModel->getSurface(i);
            if (rSurface.getThickness() > RConstants::eps)
            {
                for (uint j=0;j<rSurface.size();j++)
                {
                    volumeElements[rSurface.get(j)] = true;
                }
            }
        }
        for (uint i=0;i<this->pModel->getNVolumes();i++)
        {
            const RVolume &rVolume = this->pModel->getVolume(i);
            for (uint j=0;j<rVolume.size();j++)
            {
                volumeElements[rVolume.get(j)] = true;
            }
        }

        uint nve = 0;
        for (uint i=0;i<ne;i++)
        {
            if (volumeElements[i])
            {
                elementSizes[nve] = elementSizes[i];
                nve ++;
            }
        }
        elementSizes.resize(nve);
    }
    double elementSize = RStatistics::findMedianValue(elementSizes);

    if (elementSize < RConstants::eps)
    {
        elementSize = 1.0;
    }
    return 1.0 / elementSize;
}

double RSolverGeneric::findMeshScale(void) const
{
    return this->pModel->findNodeScale();
}

void RSolverGeneric::updateLocalRotations(void)
{
    std::vector<RR3Vector> nodeNormals(this->pModel->getNNodes());
    RBVector nodeNormalsBook(this->pModel->getNNodes(),false);
    RR3Vector normal;
    RRMatrix R;

    // Calculate directions for surface elements
    for (unsigned int i=0;i<this->pModel->getNSurfaces();i++)
    {
        const RSurface &rSurface = this->pModel->getSurface(i);
        bool findLocalDirections = false;
        for (unsigned int j=0;j<rSurface.getNBoundaryConditions();j++)
        {
            const RBoundaryCondition &bc = rSurface.getBoundaryCondition(j);
            if (RBoundaryCondition::getProblemTypeMask(bc.getType()) & problemType)
            {
                if (bc.getHasLocalDirection())
                {
                    findLocalDirections = true;
                    break;
                }
            }
        }
        for (unsigned int j=0;j<rSurface.size();j++)
        {
            if (findLocalDirections)
            {
                const RElement &rElement = this->pModel->getElement(rSurface.get(j));

                if (!rElement.findNormal(this->pModel->getNodes(),normal[0],normal[1],normal[2]))
                {
                    throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Could not calculate element normal for element# = %u.",rSurface.get(j));
                }
                for (unsigned int k=0;k<rElement.size();k++)
                {
                    uint nodeId = rElement.getNodeId(k);
                    nodeNormals[nodeId][0] += normal[0];
                    nodeNormals[nodeId][1] += normal[1];
                    nodeNormals[nodeId][2] += normal[2];
                    nodeNormalsBook[nodeId] = true;
                }
            }
        }
    }

    // Apply directions for point elements
    for (unsigned int i=0;i<this->pModel->getNPoints();i++)
    {
        RR3Vector direction;
        bool hasLocalDirections = false;
        const RPoint &rPoint = this->pModel->getPoint(i);
        for (unsigned int j=0;j<rPoint.getNBoundaryConditions();j++)
        {
            const RBoundaryCondition &bc = rPoint.getBoundaryCondition(j);
            if (RBoundaryCondition::getProblemTypeMask(bc.getType()) & problemType)
            {
                if (bc.getHasLocalDirection())
                {
                    hasLocalDirections = true;
                    direction = bc.getLocalDirection();
                    break;
                }
            }
        }
        if (hasLocalDirections)
        {
            for (uint j=0;j<rPoint.size();j++)
            {
                const RElement &rElement = this->pModel->getElement(rPoint.get(j));
                for (unsigned int k=0;k<rElement.size();k++)
                {
                    uint nodeId = rElement.getNodeId(k);
                    nodeNormals[nodeId][0] = direction[0];
                    nodeNormals[nodeId][1] = direction[1];
                    nodeNormals[nodeId][2] = direction[2];
                    nodeNormalsBook[nodeId] = true;
                }
            }
        }
    }

    // Convert local directions to rotation matricies
    this->localRotations.resize(this->pModel->getNNodes());
    for (uint i=0;i<nodeNormals.size();i++)
    {
        if (nodeNormalsBook[i])
        {
            nodeNormals[i].normalize();
            nodeNormals[i].findRotationMatrix(R);
            this->localRotations[i].activate(R);
        }
        else
        {
            this->localRotations[i].deactivate();
        }
    }

    // Apply local rotation for line elements
    for (unsigned int i=0;i<this->pModel->getNLines();i++)
    {
        RR3Vector direction;
        bool findLocalDirections = false;
        const RLine &rLine = this->pModel->getLine(i);
        for (unsigned int j=0;j<rLine.getNBoundaryConditions();j++)
        {
            const RBoundaryCondition &bc = rLine.getBoundaryCondition(j);
            if (RBoundaryCondition::getProblemTypeMask(bc.getType()) & problemType)
            {
                if (bc.getHasLocalDirection())
                {
                    findLocalDirections = true;
                    break;
                }
            }
        }
        if (findLocalDirections)
        {
            for (uint j=0;j<rLine.size();j++)
            {
                const RElement &rElement = this->pModel->getElement(rLine.get(j));

                RR3Vector d1,d2,d3;
                RSegment s(this->pModel->getNode(rElement.getNodeId(0)),
                           this->pModel->getNode(rElement.getNodeId(1)));
                s.findPerpendicularVectors(d1,d2,d3);

                R.resize(3,3,0.0);
                R[0][0]=d1[0]; R[0][1]=d1[1]; R[0][2]=d1[2];
                R[1][0]=d2[0]; R[1][1]=d2[1]; R[1][2]=d2[2];
                R[2][0]=d3[0]; R[2][1]=d3[1]; R[2][2]=d3[2];
                R.invert();

                for (uint k=0;k<rElement.size();k++)
                {
                    this->localRotations[rElement.getNodeId(k)].activate(R);
                }
            }
        }
    }
}

void RSolverGeneric::clearSharedData(void)
{
    this->pSharedData->clearData();
}

void RSolverGeneric::storeSharedData(void)
{
    this->pSharedData->addData("element-temperature",this->elementTemperature);
}

void RSolverGeneric::recoverSharedData(void)
{
    if (this->pSharedData->hasData("element-temperature",this->pModel->getNElements()))
    {
        this->elementTemperature = this->pSharedData->findData("element-temperature");
    }
}

void RSolverGeneric::writeResults(void)
{
    if (this->modelFileName.isEmpty())
    {
        return;
    }

    bool canWrite = true;
    if (this->pModel->getTimeSolver().getEnabled())
    {
        canWrite = false;
        if (/*(!this->pModel->getTimeSolver().getRestartSolver() && this->pModel->getTimeSolver().getCurrentTimeStep() == 0) ||*/
            (this->pModel->getTimeSolver().getCurrentTimeStep()+1) == this->pModel->getTimeSolver().getNTimeSteps() ||
            ((this->pModel->getTimeSolver().getCurrentTimeStep()+1) % this->pModel->getTimeSolver().getOutputFrequency() == 0))
        {
            canWrite = true;
        }
    }

    if (canWrite)
    {
        this->modelFileName = this->pModel->write(this->modelFileName);
    }
}

void RSolverGeneric::applyDisplacement(void)
{
    uint variablePosition = this->pModel->findVariable(R_VARIABLE_DISPLACEMENT);
    if (variablePosition == RConstants::eod)
    {
        return;
    }

    const RVariable &rVariable = this->pModel->getVariable(variablePosition);

    R_ERROR_ASSERT(rVariable.getNValues() == this->pModel->getNNodes());
    R_ERROR_ASSERT(rVariable.getNVectors() == 3);

    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        RRVector u = rVariable.getValueVector(i);
        this->pModel->getNode(i).move(RR3Vector(u[0],u[1],u[2]));
    }
}

void RSolverGeneric::removeDisplacement(void)
{
    uint variablePosition = this->pModel->findVariable(R_VARIABLE_DISPLACEMENT);
    if (variablePosition == RConstants::eod)
    {
        return;
    }

    const RVariable &rVariable = this->pModel->getVariable(variablePosition);

    R_ERROR_ASSERT(rVariable.getNValues() == this->pModel->getNNodes());
    R_ERROR_ASSERT(rVariable.getNVectors() == 3);

    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        RRVector u = rVariable.getValueVector(i);
        u *= -1.0;
        this->pModel->getNode(i).move(RR3Vector(u[0],u[1],u[2]));
    }
}

void RSolverGeneric::generateNodeBook(RProblemType problemType)
{
    if (problemType == R_PROBLEM_FLUID)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,
                     "Unsupported problem type \'%s\' for node book generation.",
                     RProblem::getName(problemType).toUtf8().constData());
    }

    this->nodeBook.resize(this->pModel->getNNodes());

    for (unsigned int i=0;i<this->pModel->getNElementGroups();i++)
    {
        const RElementGroup *pElementGroup = this->pModel->getElementGroupPtr(i);
        if (!pElementGroup)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Element group could not be found (%u of %u).",i,this->pModel->getNElementGroups());
        }
        bool hasExplicitBc = false;
        for (unsigned int j=0;j<pElementGroup->getNBoundaryConditions();j++)
        {
            const RBoundaryCondition &bc = pElementGroup->getBoundaryCondition(j);
            if (RBoundaryCondition::getProblemTypeMask(bc.getType()) & problemType)
            {
                if (bc.getExplicit())
                {
                    hasExplicitBc = true;
                    break;
                }
            }
        }
        for (unsigned int j=0;j<pElementGroup->size();j++)
        {
            if (hasExplicitBc)
            {
                const RElement &element = this->pModel->getElement(pElementGroup->get(j));
                for (unsigned int k=0;k<element.size();k++)
                {
                    this->nodeBook.disable(element.getNodeId(k),true);
                }
            }
        }
    }
    RBVector computableNodes(this->pModel->getNNodes(),false);
    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        if (this->computableElements[i])
        {
            const RElement &rElement = this->pModel->getElement(i);
            for (uint j=0;j<rElement.size();j++)
            {
                computableNodes[rElement.getNodeId(j)] = true;
            }
        }
    }
    for (uint i=0;i<computableNodes.size();i++)
    {
        if (!computableNodes[i])
        {
            this->nodeBook.disable(i,true);
        }
    }
}

void RSolverGeneric::generateMaterialVecor(RMaterialPropertyType materialPropertyType, RRVector &materialPropertyValues) const
{
    unsigned int ne = this->pModel->getNElements();

    materialPropertyValues.resize(ne,0.0);

    for (unsigned int i=0;i<this->pModel->getNElementGroups();i++)
    {
        const RElementGroup *pElementGroup = this->pModel->getElementGroupPtr(i);
        if (!pElementGroup)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Element group could not be found (%u of %u).",i,this->pModel->getNElementGroups());
        }

        const RMaterial &material = pElementGroup->getMaterial();

        // Get material property
        unsigned int materialPropertyPosition = material.findPosition(materialPropertyType);
        if (materialPropertyPosition == material.size())
        {
//            RLogger::warning("Could not find material property \'%s\' in element group \'%s\'\n",
//                             RMaterialProperty::getName(materialPropertyType).toUtf8().constData(),
//                             pElementGroup->getName().toUtf8().constData());
            continue;
        }
        const RMaterialProperty &materialProperty = material.get(materialPropertyPosition);

        // Apply values to element vectors.
        for (unsigned int j=0;j<pElementGroup->size();j++)
        {
            unsigned int elementID = pElementGroup->get(j);
            double temperature = this->elementTemperature[elementID];
            materialPropertyValues[elementID] = materialProperty.get(temperature);
        }
    }
}

void RSolverGeneric::generateVariableVector(RVariableType variableType,
                                            RRVector &variableValues,
                                            RBVector &setValues,
                                            bool boundaryConditions,
                                            bool initialConditions,
                                            bool environmentConditions,
                                            bool onlyExplicitBcs) const
{
    unsigned int ne = this->pModel->getNElements();

    variableValues.resize(ne,0.0);
    setValues.resize(ne,false);

    for (unsigned int i=0;i<this->pModel->getNElementGroups();i++)
    {
        const RElementGroup *pElementGroup = this->pModel->getElementGroupPtr(i);
        if (!pElementGroup)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Element group could not be found (%u of %u).",i,this->pModel->getNElementGroups());
        }

        if (environmentConditions)
        {
            // Apply environment conditions.
            for (unsigned int j=0;j<pElementGroup->getNEnvironmentConditions();j++)
            {
                const REnvironmentCondition &ec = pElementGroup->getEnvironmentCondition(j);
                unsigned int ecComponentPosition = ec.findComponentPosition(variableType);
                if (ecComponentPosition == RConstants::eod)
                {
                    continue;
                }
                const RConditionComponent &conditionComponent = ec.getComponent(ecComponentPosition);
                double value = conditionComponent.get(this->pModel->getTimeSolver().getCurrentTime());

                for (unsigned int k=0;k<pElementGroup->size();k++)
                {
                    variableValues[pElementGroup->get(k)] = value;
                    setValues[pElementGroup->get(k)] = true;
                }
            }
        }

        if (initialConditions)
        {
            // Apply initial conditions.
            for (unsigned int j=0;j<pElementGroup->getNInitialConditions();j++)
            {
                const RInitialCondition &ic = pElementGroup->getInitialCondition(j);
                unsigned int icComponentPosition = ic.findComponentPosition(variableType);
                if (icComponentPosition == RConstants::eod)
                {
                    continue;
                }
                const RConditionComponent &conditionComponent = ic.getComponent(icComponentPosition);
                double value = conditionComponent.get(this->pModel->getTimeSolver().getCurrentTime());

                for (unsigned int k=0;k<pElementGroup->size();k++)
                {
                    variableValues[pElementGroup->get(k)] = value;
                    setValues[pElementGroup->get(k)] = true;
                }
            }
        }

        if (boundaryConditions)
        {
            // Apply boundary conditions.
            for (unsigned int j=0;j<pElementGroup->getNBoundaryConditions();j++)
            {
                const RBoundaryCondition &bc = pElementGroup->getBoundaryCondition(j);
                if (onlyExplicitBcs && !bc.getExplicit())
                {
                    continue;
                }
                unsigned int bcComponentPosition = bc.findComponentPosition(variableType);
                if (bcComponentPosition == RConstants::eod)
                {
                    continue;
                }
                const RConditionComponent &conditionComponent = bc.getComponent(bcComponentPosition);
                double value = conditionComponent.get(this->pModel->getTimeSolver().getCurrentTime());

                for (unsigned int k=0;k<pElementGroup->size();k++)
                {
                    variableValues[pElementGroup->get(k)] = value;
                    setValues[pElementGroup->get(k)] = true;
                }
            }
        }
    }
}

void RSolverGeneric::recoverVariable(RVariableType variableType,
                                     RVariableApplyType applyType,
                                     uint expectedSize,
                                     uint vectorPosition,
                                     RRVector &variableValues,
                                     double defaultValue) const
{
    variableValues.resize(expectedSize);
    variableValues.fill(defaultValue);

    uint variablePosition = this->pModel->findVariable(variableType);

    if (variablePosition != RConstants::eod)
    {
        const RVariable &rVariable = this->pModel->getVariable(variablePosition);
        if (rVariable.getApplyType() == applyType)
        {
            variableValues = rVariable.getValues(vectorPosition);
        }
    }

    R_ERROR_ASSERT(variableValues.size() == expectedSize);
}

void RSolverGeneric::syncShared(const QString &keyName, RRVector &values)
{
    if (this->taskIteration == 0)
    {
        this->pSharedData->addData(keyName, values);
    }
    else
    {
        if (this->pSharedData->hasData(keyName,uint(values.size())))
        {
            values = this->pSharedData->findData(keyName);
        }
    }
}

void RSolverGeneric::findComputableElements(RProblemType problemType)
{
    this->computableElements.resize(this->pModel->getNElements());
    this->computableElements.fill(false);

    std::vector<RMaterialPropertyType> propList;

    bool needsMaterial = (RMaterialProperty::getTypes(problemType).size() > 0);

    if (needsMaterial)
    {
        for (uint i=0;i<R_MATERIAL_PROPERTY_N_TYPES;i++)
        {
            if (problemType & RMaterialProperty::getProblemTypeMask(RMaterialPropertyType(i)))
            {
                propList.push_back(RMaterialPropertyType(i));
            }
        }
    }
    for (uint i=0;i<this->pModel->getNElementGroups();i++)
    {
        const RElementGroup *pElementGroup = this->pModel->getElementGroupPtr(i);
        const RMaterial &rMaterial = pElementGroup->getMaterial();
        if (!needsMaterial || rMaterial.hasProperties(propList))
        {
            for (uint j=0;j<pElementGroup->size();j++)
            {
                this->computableElements[pElementGroup->get(j)] = true;
            }
            continue;
        }
        for (uint j=0;j<pElementGroup->getNBoundaryConditions();j++)
        {
            const RBoundaryCondition &rBoundaryCondition = pElementGroup->getBoundaryCondition(j);
            if (problemType & RBoundaryCondition::getProblemTypeMask(rBoundaryCondition.getType()))
            {
                for (uint k=0;k<pElementGroup->size();k++)
                {
                    this->computableElements[pElementGroup->get(k)] = true;
                }
            }
        }
    }
}

void RSolverGeneric::findIncludableElements(void)
{
    this->includableElements = this->computableElements;

    RBVector computableNodes(this->pModel->getNNodes(),false);

    for (uint i=0;i<this->computableElements.size();i++)
    {
        if (this->computableElements[i])
        {
            const RElement &rElement = this->pModel->getElement(i);
            for (uint j=0;j<rElement.size();j++)
            {
                computableNodes[rElement.getNodeId(j)] = true;
            }
        }
    }

    for (uint i=0;i<this->includableElements.size();i++)
    {
        if (!this->includableElements[i])
        {
            const RElement &rElement = this->pModel->getElement(i);
            uint nComputable = 0;
            for (uint j=0;j<rElement.size();j++)
            {
                if (computableNodes[rElement.getNodeId(j)])
                {
                    nComputable++;
                }
            }
            if (nComputable == rElement.size())
            {
                this->includableElements[i] = true;
            }
        }
    }

}

void RSolverGeneric::findInwardElements(void)
{
    this->inwardElements.resize(this->pModel->getNElements());
    this->inwardElements.fill(false);

    // Assuming that surface normals are synchronized.

    for (uint i=0;i<this->pModel->getNSurfaces();i++)
    {
        const RSurface &rSurface = this->pModel->getSurface(i);

        bool isInward = false;

        for (uint j=0;j<rSurface.size();j++)
        {
            uint elementID = rSurface.get(j);
            uint volumeElementID = RConstants::eod;
            if (!this->includableElements[elementID])
            {
                continue;
            }
            const RElement &rElement = this->pModel->getElement(elementID);
            // Find related volume element.
            for (uint l=0;l<this->pModel->getNElements();l++)
            {
                if (!this->computableElements[l])
                {
                    continue;
                }
                const RElement &rVolumeElement = this->pModel->getElement(l);
                if (R_ELEMENT_TYPE_IS_VOLUME(rVolumeElement.getType()))
                {
                    uint nNodesFound = 0;
                    for (uint m=0;m<rElement.size();m++)
                    {
                        if (rVolumeElement.hasNodeId(rElement.getNodeId(m)))
                        {
                            nNodesFound++;
                        }
                    }
                    if (nNodesFound == rElement.size())
                    {
                        volumeElementID = l;
                        break;
                    }
                }
            }
            if (volumeElementID != RConstants::eod)
            {
                const RElement &rVolumeElement = this->pModel->getElement(volumeElementID);

                std::vector<RElement> edgeElements = rVolumeElement.generateEdgeElements();

                RR3Vector elementNormal;
                rElement.findNormal(this->pModel->getNodes(),elementNormal[0],elementNormal[1],elementNormal[2]);

                for (uint l=0;l<edgeElements.size();l++)
                {
                    uint nNodesFound = 0;
                    for (uint m=0;m<rElement.size();m++)
                    {
                        if (edgeElements[l].hasNodeId(rElement.getNodeId(m)))
                        {
                            nNodesFound++;
                        }
                    }
                    if (nNodesFound == rElement.size())
                    {
                        RR3Vector edgeElementNormal;
                        edgeElements[l].findNormal(this->pModel->getNodes(),edgeElementNormal[0],edgeElementNormal[1],edgeElementNormal[2]);

                        if (std::fabs(RR3Vector::angle(elementNormal,edgeElementNormal)) < RConstants::pi/2.0)
                        {
                            isInward = true;
                        }
                    }
                }

                break;
            }
        }
        for (uint j=0;j<rSurface.size();j++)
        {
            this->inwardElements[rSurface.get(j)] = isInward;
        }
    }
}

void RSolverGeneric::processMonitoringPoints(void) const
{
    for (uint i=0;i<this->pModel->getMonitoringPointManager().size();i++)
    {
        RMonitoringPoint &rMonitorinPoint = this->pModel->getMonitoringPointManager()[i];

        unsigned int variableID = this->pModel->findVariable(rMonitorinPoint.getVariableType());
        if (variableID == RConstants::eod)
        {
            RLogger::warning("Monitoring point variable (%s) was not found among the results\n",
                             RVariable::getName(rMonitorinPoint.getVariableType()).toUtf8().constData());
            continue;
        }
        RVariable &rVariable = this->pModel->getVariable(variableID);

        RNode iNode(rMonitorinPoint.getPosition());

        unsigned int elementID = RConstants::eod;
        for (unsigned int j=0;j<this->pModel->getNElements();j++)
        {
            if (this->pModel->getElement(j).isInside(this->pModel->getNodes(),iNode))
            {
                elementID = j;
            }
        }
        if (elementID == RConstants::eod)
        {
            RLogger::warning("Monitoring point [%g %g %g] is outside of the model\n",
                             rMonitorinPoint.getPosition()[0],
                             rMonitorinPoint.getPosition()[1],
                             rMonitorinPoint.getPosition()[2]);
            continue;
        }

        RElement &rElement = this->pModel->getElement(elementID);

        RValueVector valueVector;
        valueVector.resize(rVariable.getNVectors());

        if (rVariable.getApplyType() == R_VARIABLE_APPLY_ELEMENT)
        {
            RRVector values(rVariable.getValueVector(elementID));
            for (unsigned int j=0;j<values.size();j++)
            {
                valueVector[j] = values[j];
            }
        }
        else if (rVariable.getApplyType() == R_VARIABLE_APPLY_NODE)
        {
            std::vector<RRVector> values(rElement.size());
            for (unsigned int j=0;j<rElement.size();j++)
            {
                values[j] = rVariable.getValueVector(rElement.getNodeId(j));
            }

            for (unsigned int j=0;j<rVariable.getNVectors();j++)
            {
                RRVector nodeValues(rElement.size());
                for (unsigned int k=0;k<rElement.size();k++)
                {
                    nodeValues[k] = values[k][j];
                }
                valueVector[j] = rElement.interpolate(this->pModel->getNodes(),iNode,nodeValues);
            }
        }

        try
        {
            rMonitorinPoint.writeValueVectorToFile(this->pModel->getTimeSolver().getCurrentTimeStep()+1,valueVector);
        }
        catch (const RError &error)
        {
            RLogger::error("Failed to write monitoring point. %s\n",error.getMessage().toUtf8().constData());
        }
    }
}

void RSolverGeneric::printStats(RVariableType variableType) const
{
    unsigned int variablePosition = this->pModel->findVariable(variableType);
    if (variablePosition == RConstants::eod)
    {
        return;
    }
    const RVariable &rVariable = this->pModel->getVariable(variablePosition);

    RStatistics stat(rVariable.getValues(),100,true);

    RLogger::info("%s [%s]\n",rVariable.getName().toUtf8().constData(),rVariable.getUnits().toUtf8().constData());
    RLogger::indent();
    stat.print();
    RLogger::unindent();
}
