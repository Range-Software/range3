/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_model.cpp                                            *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   8-th December 2011                                       *
 *                                                                   *
 *  DESCRIPTION: Model class definition                              *
 *********************************************************************/

#include <QDir>
#include <QSaveFile>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QSetIterator>

#include <vector>
#include <stack>
#include <cmath>
#include <float.h>

#include <rblib.h>

#include "rml_model.h"
#include "rml_file_io.h"
#include "rml_file_manager.h"
#include "rml_view_factor_matrix.h"
#include "rml_polygon.h"
#include "rml_segment.h"


static const RVersion _version = RVersion(FILE_MAJOR_VERSION,FILE_MINOR_VERSION,FILE_RELEASE_VERSION);


void RModel::_init (const RModel *pModel)
{
    if (pModel)
    {
        this->name = pModel->name;
        this->description = pModel->description;
        this->nodes = pModel->nodes;
        this->elements = pModel->elements;
        this->points = pModel->points;
        this->lines = pModel->lines;
        this->surfaces = pModel->surfaces;
        this->volumes = pModel->volumes;
        this->vectorFields = pModel->vectorFields;
        this->scalarFields = pModel->scalarFields;
        this->streamLines = pModel->streamLines;
        this->cuts = pModel->cuts;
        this->isos = pModel->isos;
        this->modelData = pModel->modelData;
    }
} /* RModel::_init */


RModel::RModel ()
{
    this->_init ();
} /* RModel::RModel */


RModel::RModel (const RModel &model) : RProblem (model)
                                     , RResults (model)
{
    this->_init (&model);
} /* RModel::RModel copy */


RModel::RModel (const RModelMsh &modelMsh)
{
    RNode node;
    RElement element;
    RIVector pointBook(modelMsh.pointsAll.getNRows());
    RIVector lineBook(modelMsh.linesAll.getNRows());
    RIVector faceBook(modelMsh.facesAll.getNRows());
    RIVector bodyBook(modelMsh.bodiesAll.getNRows());

    this->_init ();

    // Set name and description

    this->setName(modelMsh.title);
    this->setDescription(modelMsh.comment);

    // Convert nodes

    this->setNNodes(modelMsh.nodes.getNRows());
    for (uint i=0;i<modelMsh.nodes.getNRows();i++)
    {
        node.set(modelMsh.nodes[i][0],modelMsh.nodes[i][1],modelMsh.nodes[i][2]);
        this->setNode(i,node);
    }

    // Convert elements

    this->setNElements(  modelMsh.pointsAll.getNRows()
                       + modelMsh.linesAll.getNRows()
                       + modelMsh.facesAll.getNRows()
                       + modelMsh.bodiesAll.getNRows());

    uint elNum = 0;
    element.setType(R_ELEMENT_POINT);
    for (uint i=0;i<modelMsh.pointsAll.getNRows();i++)
    {
        pointBook[i] = elNum;
        element.setNodeId(0,modelMsh.pointsAll[i][0]);
        this->setElement(elNum++,element);
    }

    element.setType(R_ELEMENT_TRUSS1);
    for (uint i=0;i<modelMsh.linesAll.getNRows();i++)
    {
        lineBook[i] = elNum;
        element.setNodeId(0,modelMsh.linesAll[i][0]);
        element.setNodeId(1,modelMsh.linesAll[i][1]);
        this->setElement(elNum++,element);
    }

    element.setType(R_ELEMENT_TRI1);
    for (uint i=0;i<modelMsh.facesAll.getNRows();i++)
    {
        faceBook[i] = elNum;
        element.setNodeId(0,modelMsh.facesAll[i][0]);
        element.setNodeId(1,modelMsh.facesAll[i][1]);
        element.setNodeId(2,modelMsh.facesAll[i][2]);
        this->setElement(elNum++,element);
    }

    element.setType(R_ELEMENT_TETRA1);
    for (uint i=0;i<modelMsh.bodiesAll.getNRows();i++)
    {
        bodyBook[i] = elNum;
        element.setNodeId(0,modelMsh.bodiesAll[i][0]);
        element.setNodeId(1,modelMsh.bodiesAll[i][1]);
        element.setNodeId(2,modelMsh.bodiesAll[i][2]);
        element.setNodeId(3,modelMsh.bodiesAll[i][3]);
        this->setElement(elNum++,element);
    }

    // Convert element groups

    this->setNPoints(modelMsh.points.getNRows());
    for (uint i=0;i<modelMsh.points.getNRows();i++)
    {
        RPoint point;
        point.setName(modelMsh.pointsNames[i]);
        point.setVolume(modelMsh.pointsVolume[i]);
        point.resize(modelMsh.points[i][1]);
        for (uint j=0;j<(uint)modelMsh.points[i][1];j++)
        {
            point.set(j,pointBook[modelMsh.points[i][j+2]]);
        }
        this->setPoint(i,point);
    }

    this->setNLines(modelMsh.lines.getNRows());
    for (uint i=0;i<modelMsh.lines.getNRows();i++)
    {
        RLine line;
        line.setName(modelMsh.linesNames[i]);
        line.setCrossArea(modelMsh.linesCarea[i]);
        line.resize(modelMsh.lines[i][1]);
        for (uint j=0;j<(uint)modelMsh.lines[i][1];j++)
        {
            line.set(j,lineBook[modelMsh.lines[i][j+2]]);
        }
        this->setLine(i,line);
    }

    this->setNSurfaces(modelMsh.faces.getNRows());
    for (uint i=0;i<modelMsh.faces.getNRows();i++)
    {
        RSurface surface;
        surface.setName(modelMsh.facesNames[i]);
        surface.setThickness(modelMsh.facesThickness[i]);
        surface.resize(modelMsh.faces[i][1]);
        for (uint j=0;j<(uint)modelMsh.faces[i][1];j++)
        {
            surface.set(j,faceBook[modelMsh.faces[i][j+2]]);
        }
        this->setSurface(i,surface);
    }

    this->setNVolumes(modelMsh.bodies.getNRows());
    for (uint i=0;i<modelMsh.bodies.getNRows();i++)
    {
        RVolume volume;
        volume.setName(modelMsh.bodiesNames[i]);
        volume.resize(modelMsh.bodies[i][1]);
        for (uint j=0;j<(uint)modelMsh.bodies[i][1];j++)
        {
            volume.set(j,bodyBook[modelMsh.bodies[i][j+2]]);
        }
        this->setVolume(i,volume);
    }
} /* RModel::RModel copy - MSH */


RModel::RModel (const RModelStl &modelStl)
{
    this->_init ();
    this->setName(modelStl.getName());
    this->setDescription(modelStl.getDescription());
    this->setNNodes(modelStl.getNNodes());
    for (uint i=0;i<this->getNNodes();i++)
    {
        this->setNode(i,modelStl.getNode(i));
    }
    this->setNElements(modelStl.getNElements());
    for (uint i=0;i<this->getNElements();i++)
    {
        this->setElement(i,modelStl.getElement(i));
    }
    for (uint i=0;i<this->getNSurfaces();i++)
    {
        if (this->getSurface(i).getName().isEmpty())
        {
            this->getSurface(i).setName(QString("Surface ") + QString::number(i+1));
        }
    }
} /* RModel::RModel copy - STL */


RModel::RModel (const RModelRaw &modelRaw,
                const QString &name,
                const QString &description)
{
    this->_init ();
    this->setName(name);
    this->setDescription(description);
    this->setNNodes(modelRaw.getNNodes());
    for (uint i=0;i<this->getNNodes();i++)
    {
        this->setNode(i,modelRaw.getNode(i));
    }
    this->setNElements(modelRaw.getNElements());
    for (uint i=0;i<this->getNElements();i++)
    {
        this->setElement(i,modelRaw.getElement(i));
    }
    for (uint i=0;i<this->getNSurfaces();i++)
    {
        if (this->getSurface(i).getName().isEmpty())
        {
            this->getSurface(i).setName(QString("Surface ") + QString::number(i+1));
        }
    }
} /* RModel::RModel copy - RAW */


RModel::~RModel ()
{
} /* RModel::~RModel */


RModel & RModel::operator = (const RModel &model)
{
    this->RProblem::operator = (model);
    this->RResults::operator = (model);
    this->_init (&model);
    return (*this);
} /* RModel::operator = */

void RModel::update(const RModel &rModel)
{
    uint nUpdatePoints = std::min(this->getNPoints(),rModel.getNPoints());
    std::vector<REntityGroupData> updatePointGroupData;
    for (uint i=0;i<nUpdatePoints;i++)
    {
        updatePointGroupData.push_back(this->getPoint(i).getData());
    }

    uint nUpdateLines = std::min(this->getNLines(),rModel.getNLines());
    std::vector<REntityGroupData> updateLineGroupData;
    for (uint i=0;i<nUpdateLines;i++)
    {
        updateLineGroupData.push_back(this->getLine(i).getData());
    }

    uint nUpdateSurfaces = std::min(this->getNSurfaces(),rModel.getNSurfaces());
    std::vector<REntityGroupData> updateSurfaceGroupData;
    for (uint i=0;i<nUpdateSurfaces;i++)
    {
        updateSurfaceGroupData.push_back(this->getSurface(i).getData());
    }

    uint nUpdateVolumes = std::min(this->getNVolumes(),rModel.getNVolumes());
    std::vector<REntityGroupData> updateVolumeGroupData;
    for (uint i=0;i<nUpdateVolumes;i++)
    {
        updateVolumeGroupData.push_back(this->getVolume(i).getData());
    }

    std::vector<RScalarField> scalarFields;
    for (uint i=0;i<this->getNScalarFields();i++)
    {
        scalarFields.push_back(this->getScalarField(i));
    }

    std::vector<RVectorField> vectorFields;
    for (uint i=0;i<this->getNVectorFields();i++)
    {
        vectorFields.push_back(this->getVectorField(i));
    }

    std::vector<RStreamLine> streamLines;
    for (uint i=0;i<this->getNStreamLines();i++)
    {
        streamLines.push_back(this->getStreamLine(i));
    }

    std::vector<RCut> cuts;
    for (uint i=0;i<this->getNCuts();i++)
    {
        cuts.push_back(this->getCut(i));
    }

    std::vector<RIso> isos;
    for (uint i=0;i<this->getNIsos();i++)
    {
        isos.push_back(this->getIso(i));
    }

    uint nUpdateVariables = std::min(this->getNVariables(),rModel.getNVariables());
    std::vector<RVariableData> updateVariableData;
    std::vector<RVariableType> updateVariableType;
    for (uint i=0;i<nUpdateVariables;i++)
    {
        updateVariableData.push_back(this->getVariable(i).getVariableData());
        updateVariableType.push_back(this->getVariable(i).getType());
    }

    RTimeSolver timeSolver(this->getTimeSolver());

    this->operator =(rModel);

    for (uint i=0;i<nUpdatePoints;i++)
    {
        this->getPoint(i).setData(updatePointGroupData[i]);
    }

    for (uint i=0;i<nUpdateLines;i++)
    {
        this->getLine(i).setData(updateLineGroupData[i]);
    }

    for (uint i=0;i<nUpdateSurfaces;i++)
    {
        this->getSurface(i).setData(updateSurfaceGroupData[i]);
    }

    for (uint i=0;i<nUpdateVolumes;i++)
    {
        this->getVolume(i).setData(updateVolumeGroupData[i]);
    }

    this->setNScalarFields(uint(scalarFields.size()));
    for (uint i=0;i<this->getNScalarFields();i++)
    {
        this->setScalarField(i,scalarFields[i]);
    }

    this->setNVectorFields(uint(vectorFields.size()));
    for (uint i=0;i<this->getNVectorFields();i++)
    {
        this->setVectorField(i,vectorFields[i]);
    }

    this->setNStreamLines(uint(streamLines.size()));
    for (uint i=0;i<this->getNStreamLines();i++)
    {
        this->setStreamLine(i,streamLines[i]);
    }

    this->setNCuts(uint(cuts.size()));
    for (uint i=0;i<this->getNCuts();i++)
    {
        this->setCut(i,cuts[i]);
    }

    this->setNIsos(uint(isos.size()));
    for (uint i=0;i<this->getNIsos();i++)
    {
        this->setIso(i,isos[i]);
    }

    for (uint i=0;i<nUpdateVariables;i++)
    {
        if (this->getVariable(i).getType() == updateVariableType[i])
        {
            this->getVariable(i).setVariableData(updateVariableData[i]);
        }
    }

    this->getTimeSolver().setInputNTimeSteps(timeSolver.getInputNTimeSteps());
    this->getTimeSolver().setInputStartTime(timeSolver.getInputStartTime());
    this->getTimeSolver().setInputTimeStepSize(timeSolver.getInputTimeStepSize());
    this->getTimeSolver().setTimes(timeSolver.getTimes());

} /* RModel::update */


void RModel::read(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    QString targetFileName(fileName);

    while (!targetFileName.isEmpty())
    {
        QString ext = RFileManager::getExtension(targetFileName);

        try
        {
            if (ext == RModel::getDefaultFileExtension(false))
            {
                targetFileName = this->readAscii(targetFileName);
            }
            else if (ext == RModel::getDefaultFileExtension(true))
            {
                targetFileName = this->readBinary(targetFileName);
            }
            else
            {
                throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF, "Unknown extension \"" + ext + "\".");
            }
        }
        catch (RError &error)
        {
            throw error;
        }
        catch (std::bad_alloc&)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "Memory allocation failed.");
        }
        catch (const std::exception& x)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "%s.", typeid(x).name());
        }
        catch (...)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "Unknown exception.");
        }
    }

    RModelProblemTypeMask modelProblemType = this->checkMesh();
    if (modelProblemType != R_MODEL_PROBLEM_NONE)
    {
        RLogger::warning("Inconsistent mesh was loaded which needs to be repaired.\n");
        if (modelProblemType == R_MODEL_PROBLEM_INVALID_NEIGHBORS)
        {
            RLogger::info("Recalculating element neighbors\n");
            RLogger::indent();
            this->setSurfaceNeighbors(this->findSurfaceNeighbors());
            this->setVolumeNeighbors(this->findVolumeNeighbors());
            RLogger::unindent();
        }
    }
} /* RModel::read */


QString RModel::write(const QString &fileName, bool writeLinkFile) const
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    QString ext = RFileManager::getExtension(fileName);
    QString targetFileName(fileName);
    QString linkFileName = RFileManager::getFileNameWithOutTimeStep(fileName);

    uint recordNumber = 0;

    if (this->getTimeSolver().getEnabled())
    {
        recordNumber = this->getTimeSolver().getCurrentTimeStep() + 1;
    }
    else
    {
        if (this->getProblemTaskTree().getProblemTypeMask() & R_PROBLEM_STRESS_MODAL)
        {
            recordNumber = this->getProblemSetup().getModalSetup().getMode() + 1;
        }
    }

    targetFileName = RFileManager::getFileNameWithTimeStep(linkFileName,recordNumber);

    try
    {
        if (writeLinkFile)
        {
            RModel::writeLink(linkFileName,targetFileName);
        }

        if (ext == RModel::getDefaultFileExtension(false))
        {
            this->writeAscii(writeLinkFile?targetFileName:linkFileName);
        }
        else if (ext == RModel::getDefaultFileExtension(true))
        {
            this->writeBinary(writeLinkFile?targetFileName:linkFileName);
        }
        else
        {
            throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF, "Unknown extension \"" + ext + "\".");
        }
    }
    catch (RError &error)
    {
        throw error;
    }
    catch (std::bad_alloc&)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "Memory allocation failed.");
    }
    catch (const std::exception& x)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "%s.", typeid(x).name());
    }
    catch (...)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "Unknown exception.");
    }

    return linkFileName;
} /* RModel::write */


void RModel::exportTo (RModelMsh &modelMsh) const
{
    modelMsh.clear();
    modelMsh.title = this->getName();
    modelMsh.comment = this->getDescription();
    modelMsh.facesNeighborsComputed = false;
    modelMsh.bodiesNeighborsComputed = false;

    uint nn = this->getNNodes();
    uint ne = this->getNElements();
    uint np = 0;
    uint nl = 0;
    uint nf = 0;
    uint nb = 0;
    uint npg = this->getNPoints();
    uint nlg = this->getNLines();
    uint nfg = this->getNSurfaces();
    uint nbg = this->getNVolumes();

    std::vector<uint> elementIDs(ne);
    const RElement *pElement = 0;
    for (uint i=0;i<ne;i++)
    {
        pElement = this->getElementPtr(i);

        if (pElement->getType() == R_ELEMENT_POINT)
        {
            elementIDs[i] = np;
            np++;
        }
        else if (pElement->getType() == R_ELEMENT_TRUSS1 ||
                 pElement->getType() == R_ELEMENT_TRUSS2)
        {
            elementIDs[i] = nl;
            nl++;
        }
        else if (pElement->getType() == R_ELEMENT_TRI1 ||
                 pElement->getType() == R_ELEMENT_TRI2)
        {
            elementIDs[i] = nf;
            nf++;
        }
        else if (pElement->getType() == R_ELEMENT_TETRA1)
        {
            elementIDs[i] = nb;
            nb++;
        }
        else
        {
            RLogger::warning("Unsupported element (%u) type \'%s\'\n",i+1,RElement::getName(pElement->getType()).toUtf8().constData());
        }
    }

    uint nppg = 0;
    for (uint i=0;i<npg;i++)
    {
        if (this->getPoint(i).size() > nppg)
        {
            nppg = this->getPoint(i).size();
        }
    }
    uint nlpg = 0;
    for (uint i=0;i<nlg;i++)
    {
        if (this->getLine(i).size() > nlpg)
        {
            nlpg = this->getLine(i).size();
        }
    }
    uint nfpg = 0;
    for (uint i=0;i<nfg;i++)
    {
        if (this->getSurface(i).size() > nfpg)
        {
            nfpg = this->getSurface(i).size();
        }
    }
    uint nbpg = 0;
    for (uint i=0;i<nbg;i++)
    {
        if (this->getVolume(i).size() > nbpg)
        {
            nbpg = this->getVolume(i).size();
        }
    }

    modelMsh.nodes.resize(nn,3);

    modelMsh.pointsAll.resize(np,1);
    modelMsh.points.resize(npg,nlpg+2,-9);
    modelMsh.pointsNames.resize(npg);
    modelMsh.pointsVolume.resize(npg);

    modelMsh.linesAll.resize(nl,2);
    modelMsh.lines.resize(nlg,nlpg+2,-9);
    modelMsh.linesNames.resize(nlg);
    modelMsh.linesCarea.resize(nlg);

    modelMsh.facesAll.resize(nf,3);
    modelMsh.faces.resize(nfg,nfpg+2,-9);
    modelMsh.facesNames.resize(nfg);
    modelMsh.facesThickness.resize(nfg);
    modelMsh.facesNeighbors.resize(nf,3,-9);

    modelMsh.bodiesAll.resize(nb,4);
    modelMsh.bodies.resize(nbg,nbpg+2,-9);
    modelMsh.bodiesNames.resize(nbg);
    modelMsh.bodiesNeighbors.resize(nb,4,-9);

    for (uint i=0;i<nn;i++)
    {
        modelMsh.nodes[i][0] = this->getNode(i).getX();
        modelMsh.nodes[i][1] = this->getNode(i).getY();
        modelMsh.nodes[i][2] = this->getNode(i).getZ();
    }

    uint elementID = 0;
    for (uint i=0;i<this->getNPoints();i++)
    {
        nlpg = 0;
        const RPoint *pPoint = this->getPointPtr(i);
        for (uint j=0;j<pPoint->size();j++)
        {
            elementID = pPoint->get(j);
            pElement = this->getElementPtr(elementID);
            if (pElement->getType() == R_ELEMENT_POINT)
            {
                modelMsh.pointsAll[elementIDs[elementID]][0] = pElement->getNodeId(0);
                modelMsh.points[i][nlpg+2] = elementIDs[elementID];
                nlpg++;
            }
        }
        modelMsh.points[i][0] = i+1;
        modelMsh.points[i][1] = nlpg;
        modelMsh.pointsNames[i] = pPoint->getName();
        modelMsh.pointsVolume[i] = pPoint->getVolume();
    }

    for (uint i=0;i<this->getNLines();i++)
    {
        nlpg = 0;
        const RLine *pLine = this->getLinePtr(i);
        for (uint j=0;j<pLine->size();j++)
        {
            elementID = pLine->get(j);
            pElement = this->getElementPtr(elementID);
            if (pElement->getType() == R_ELEMENT_TRUSS1 ||
                pElement->getType() == R_ELEMENT_TRUSS2)
            {
                modelMsh.linesAll[elementIDs[elementID]][0] = pElement->getNodeId(0);
                modelMsh.linesAll[elementIDs[elementID]][1] = pElement->getNodeId(1);
                modelMsh.lines[i][nlpg+2] = elementIDs[elementID];
                nlpg++;
            }
        }
        modelMsh.lines[i][0] = i+1;
        modelMsh.lines[i][1] = nlpg;
        modelMsh.linesNames[i] = pLine->getName();
        modelMsh.linesCarea[i] = pLine->getCrossArea();
    }

    for (uint i=0;i<this->getNSurfaces();i++)
    {
        nfpg = 0;
        const RSurface *pSurface = this->getSurfacePtr(i);
        for (uint j=0;j<pSurface->size();j++)
        {
            elementID = pSurface->get(j);
            pElement = this->getElementPtr(elementID);
            if (pElement->getType() == R_ELEMENT_TRI1 ||
                pElement->getType() == R_ELEMENT_TRI2)
            {
                modelMsh.facesAll[elementIDs[elementID]][0] = pElement->getNodeId(0);
                modelMsh.facesAll[elementIDs[elementID]][1] = pElement->getNodeId(1);
                modelMsh.facesAll[elementIDs[elementID]][2] = pElement->getNodeId(2);
                modelMsh.faces[i][nfpg+2] = elementIDs[elementID];
                nfpg++;
            }
        }
        modelMsh.faces[i][0] = i+1;
        modelMsh.faces[i][1] = nfpg;
        modelMsh.facesNames[i] = pSurface->getName();
        modelMsh.facesThickness[i] = pSurface->getThickness();
    }

    for (uint i=0;i<this->getNVolumes();i++)
    {
        nbpg = 0;
        const RVolume *pVolume = this->getVolumePtr(i);
        for (uint j=0;j<pVolume->size();j++)
        {
            elementID = pVolume->get(j);
            pElement = this->getElementPtr(elementID);
            if (pElement->getType() == R_ELEMENT_TETRA1)
            {
                modelMsh.bodiesAll[elementIDs[elementID]][0] = pElement->getNodeId(0);
                modelMsh.bodiesAll[elementIDs[elementID]][1] = pElement->getNodeId(1);
                modelMsh.bodiesAll[elementIDs[elementID]][2] = pElement->getNodeId(2);
                modelMsh.bodiesAll[elementIDs[elementID]][3] = pElement->getNodeId(3);
                modelMsh.bodies[i][nbpg+2] = elementIDs[elementID];
                nbpg++;
            }
        }
        modelMsh.bodies[i][0] = i+1;
        modelMsh.bodies[i][1] = nbpg;
        modelMsh.bodiesNames[i] = pVolume->getName();
    }
} /* RModel::exportTo */


void RModel::exportTo (RModelRaw &modelRaw) const
{
    modelRaw.clear();

    for (uint i=0;i<this->getNElements();i++)
    {
        const RElement &rElement = this->getElement(i);
        if (rElement.getType() == R_ELEMENT_TRI1 ||
            rElement.getType() == R_ELEMENT_TRI2)
        {
            modelRaw.addTriangle(this->getNode(rElement.getNodeId(0)),
                                 this->getNode(rElement.getNodeId(1)),
                                 this->getNode(rElement.getNodeId(2)),
                                 false);
        }
        if (rElement.getType() == R_ELEMENT_QUAD1 ||
            rElement.getType() == R_ELEMENT_QUAD2)
        {
            modelRaw.addTriangle(this->getNode(rElement.getNodeId(0)),
                                 this->getNode(rElement.getNodeId(1)),
                                 this->getNode(rElement.getNodeId(2)),
                                 false);
            modelRaw.addTriangle(this->getNode(rElement.getNodeId(0)),
                                 this->getNode(rElement.getNodeId(2)),
                                 this->getNode(rElement.getNodeId(3)),
                                 false);
        }
    }
} /* RModel::exportTo */


void RModel::exportTo (RModelStl &modelStl) const
{
    modelStl.clear();
    modelStl.setName(this->getName());
    modelStl.setDescription(this->getDescription());

    for (uint i=0;i<this->getNElements();i++)
    {
        const RElement &rElement = this->getElement(i);
        if (rElement.getType() == R_ELEMENT_TRI1 ||
            rElement.getType() == R_ELEMENT_TRI2)
        {
            modelStl.addTriangle(this->getNode(rElement.getNodeId(0)),
                                 this->getNode(rElement.getNodeId(1)),
                                 this->getNode(rElement.getNodeId(2)),
                                 false);
        }
        if (rElement.getType() == R_ELEMENT_QUAD1 ||
            rElement.getType() == R_ELEMENT_QUAD2)
        {
            modelStl.addTriangle(this->getNode(rElement.getNodeId(0)),
                                 this->getNode(rElement.getNodeId(1)),
                                 this->getNode(rElement.getNodeId(2)),
                                 false);
            modelStl.addTriangle(this->getNode(rElement.getNodeId(0)),
                                 this->getNode(rElement.getNodeId(2)),
                                 this->getNode(rElement.getNodeId(3)),
                                 false);
        }
    }
} /* RModel::exportTo */


QString RModel::getName(void) const
{
    return this->name;
} /* RModel::getName */


void RModel::setName (const QString &name)
{
    this->name = name;
} /* RModel::setName */


QString RModel::getDescription(void) const
{
    return this->description;
} /* RModel::getDescription */


void RModel::setDescription (const QString &description)
{
    this->description = description;
} /* RModel::setDescription */


void RModel::addElementToGroup (uint elementID,
                                uint groupID)
{
    R_ERROR_ASSERT (elementID < this->getNElements());

    RElementType elementType = this->getElement(elementID).getType();

    if (R_ELEMENT_TYPE_IS_POINT (elementType))
    {
        if (this->getNPoints() <= groupID)
        {
            this->setNPoints (groupID+1);
        }
        this->getPoint(groupID).add (elementID);
    }
    else if (R_ELEMENT_TYPE_IS_LINE (elementType))
    {
        if (this->getNLines() <= groupID)
        {
            this->setNLines (groupID+1);
        }
        this->getLine(groupID).add (elementID);
    }
    else if (R_ELEMENT_TYPE_IS_SURFACE (elementType))
    {
        if (this->getNSurfaces() <= groupID)
        {
            this->setNSurfaces (groupID+1);
        }
        this->getSurface(groupID).add (elementID);
    }
    else if (R_ELEMENT_TYPE_IS_VOLUME (elementType))
    {
        if (this->getNVolumes() <= groupID)
        {
            this->setNVolumes (groupID+1);
        }
        this->getVolume(groupID).add (elementID);
    }
} /* RModel::addElementToGroup */


void RModel::setProblemTaskTree (const RProblemTaskItem &taskTree)
{
    RProblem::setProblemTaskTree(taskTree);

    RProblemTypeMask typeMask = taskTree.getProblemTypeMask();
    RProblemTypeMask conditionTypeMask;
    RElementGroup *pElementGroup = 0;

    uint ng = 0;
    uint pos = 0;

    std::vector<REntityGroupType> grpTypes = REntityGroup::getAllTypes();

    for (uint i=0;i<grpTypes.size();i++)
    {
        switch (grpTypes[i])
        {
            case R_ENTITY_GROUP_POINT:
                ng = this->getNPoints();
                break;
            case R_ENTITY_GROUP_LINE:
                ng = this->getNLines();
                break;
            case R_ENTITY_GROUP_SURFACE:
                ng = this->getNSurfaces();
                break;
            case R_ENTITY_GROUP_VOLUME:
                ng = this->getNVolumes();
                break;
            default:
                ng = 0;
                break;
        }
        for (uint j=0;j<ng;j++)
        {
            if (grpTypes[i] == R_ENTITY_GROUP_POINT)
            {
                pElementGroup = this->getPointPtr(j);
            }
            else if (grpTypes[i] == R_ENTITY_GROUP_LINE)
            {
                pElementGroup = this->getLinePtr(j);
            }
            else if (grpTypes[i] == R_ENTITY_GROUP_SURFACE)
            {
                pElementGroup = this->getSurfacePtr(j);
            }
            else if (grpTypes[i] == R_ENTITY_GROUP_VOLUME)
            {
                pElementGroup = this->getVolumePtr(j);
            }
            else
            {
                pElementGroup = 0;
            }

            // Check all boundary conditions
            pos = 0;
            while (pos < pElementGroup->getNBoundaryConditions())
            {
                conditionTypeMask = RBoundaryCondition::getProblemTypeMask(pElementGroup->getBoundaryCondition(pos).getType());
                if (!(conditionTypeMask & typeMask))
                {
                    pElementGroup->removeBoundaryCondition (pos);
                }
                else
                {
                    pos++;
                }
            }

            // Check all environment conditions
            pos = 0;
            while (pos < pElementGroup->getNEnvironmentConditions())
            {
                conditionTypeMask = REnvironmentCondition::getProblemTypeMask(pElementGroup->getEnvironmentCondition(pos).getType());
                if (!(conditionTypeMask & typeMask))
                {
                    pElementGroup->removeEnvironmentCondition (pos);
                }
                else
                {
                    pos++;
                }
            }

            // Check all initial conditions
            pos = 0;
            while (pos < pElementGroup->getNInitialConditions())
            {
                conditionTypeMask = RInitialCondition::getProblemTypeMask(pElementGroup->getInitialCondition(pos).getType());
                if (!(conditionTypeMask & typeMask))
                {
                    pElementGroup->removeInitialCondition (pos);
                }
                else
                {
                    pos++;
                }
            }
        }
    }
}/* RModel::setProblemTypeMask */


/*********************************************************************
 * Node interface                                                    *
 *********************************************************************/


void RModel::removeVariable(uint position)
{
    RVariableType variableType = this->getVariable(position).getType();
    this->RResults::removeVariable(position);
    this->clearEntityVariableData(variableType);
}/* RModel::removeVariable */


void RModel::removeAllVariables(void)
{
    this->RResults::removeAllVariables();
    this->clearEntityVariableData();
}/* RModel::removeAllVariables */


/*********************************************************************
 * Node interface                                                    *
 *********************************************************************/


uint RModel::getNNodes (void) const
{
    return uint(this->nodes.size());
} /* RModel::getNNodes */


void RModel::setNNodes (uint nnodes)
{
    this->nodes.resize(nnodes);
    this->RResults::setNNodes (nnodes);
} /* RModel::setNNodes */


const RNode * RModel::getNodePtr (uint position) const
{
    R_ERROR_ASSERT (position < this->nodes.size());
    return &this->nodes[position];
} /* RModel::get_node_ptr */


RNode * RModel::getNodePtr (uint position)
{
    R_ERROR_ASSERT (position < this->nodes.size());
    return &this->nodes[position];
} /* RModel::get_node_ptr */


const RNode & RModel::getNode (uint position) const
{
    R_ERROR_ASSERT (position < this->nodes.size());
    return this->nodes[position];
} /* RModel::getNode */


RNode & RModel::getNode (uint position)
{
    R_ERROR_ASSERT (position < this->nodes.size());
    return this->nodes[position];
} /* RModel::getNode */


const std::vector<RNode> &RModel::getNodes(void) const
{
    return this->nodes;
} /* RModel::getNodes */


void RModel::addNode (const RNode &node,
                      double  value)
{
    this->nodes.push_back (node);
    this->RResults::addNode (value);
} /* RModel::addNode */


void RModel::addNode (double x, 
                      double y,
                      double z,
                      double value)
{
    RNode node;

    node.set (x, y, z);

    this->addNode (node);
    this->RResults::addNode (value);
} /* RModel::addNode */


void RModel::setNode (uint  position,
                       const RNode  &node)
{
    R_ERROR_ASSERT (position < this->nodes.size());
    this->nodes[position] = node;
} /* RModel::set_node */


void RModel::removeNode (uint                position,
                         std::vector<uint> * removedElements,
                         std::vector<uint> * removedPoints,
                         std::vector<uint> * removedLines,
                         std::vector<uint> * removedSurfaces,
                         std::vector<uint> * removedVolumes)
{
    R_ERROR_ASSERT (position < this->nodes.size());

    // Loop over all elements and remove each containing the node
    std::vector<uint> eps = this->findElementPositionsByNodeId(position);
    if (removedElements)
    {
        (*removedElements) = eps;
    }

    for (std::vector<uint>::reverse_iterator rIterEps = eps.rbegin(); rIterEps != eps.rend(); ++rIterEps)
    {
        this->removeElement (*rIterEps,
                              true,
                              removedPoints,
                              removedLines,
                              removedSurfaces,
                              removedVolumes);
    }

    // Remove node from nodes vector
    std::vector<RNode>::iterator iterNode = this->nodes.begin();
    std::advance (iterNode, position);

    this->nodes.erase (iterNode);
    this->RResults::removeNode (position);

    // Decrease each node ID which is greater then possitin s by one
    for (std::vector<RElement>::iterator iterElement = this->elements.begin();
         iterElement != this->elements.end();
         ++iterElement)
    {
        for (uint i=0;i<iterElement->size();i++)
        {
            uint nodeID = iterElement->getNodeId (i);
            if (nodeID > position)
            {
                iterElement->setNodeId (i, nodeID-1);
            }
        }
    }
} /* RModel::removeNode */


void RModel::removeNodes(const QList<uint> &nodeIDs, bool closeHole)
{
    QList<uint> sortedNodeIDs(nodeIDs);

    qSort(sortedNodeIDs.begin(),sortedNodeIDs.end());

    for (int i=sortedNodeIDs.size()-1;i>=0;i--)
    {
        RLogger::info("Removing node #%u\n",sortedNodeIDs[i]);

        QList<uint> edgeNodeIds;
        if (closeHole)
        {
            edgeNodeIds = this->findNodeEdgeRing(sortedNodeIDs[i]);
        }

        for (int j=0;j<edgeNodeIds.size();j++)
        {
            if (edgeNodeIds[j] > sortedNodeIDs[i])
            {
                edgeNodeIds[j]--;
            }
        }

        this->removeNode(sortedNodeIDs[i]);

        if (closeHole)
        {
            std::vector<RNode> edgeNodes;
            edgeNodes.resize(edgeNodeIds.size());
            for (uint i=0;i<edgeNodes.size();i++)
            {
                edgeNodes[i] = this->getNode(edgeNodeIds[i]);
            }

            if (edgeNodes.size() > 0)
            {
                std::vector<RElement> patchEelements = RPolygon::triangulate(edgeNodes,true);
                for (uint j=0;j<patchEelements.size();j++)
                {
                    for (uint k=0;k<patchEelements[j].size();k++)
                    {
                        patchEelements[j].setNodeId(k,edgeNodeIds[patchEelements[j].getNodeId(k)]);
                    }
                    this->addElement(patchEelements[j]);
                }
            }
        }
    }
} /* RModel::removeNodes */


void RModel::mergeNodes(uint position1, uint position2, bool average, bool allowDowngrade)
{
    R_ERROR_ASSERT (position1 != position2);

    uint n1 = (position1 < position2) ? position1 : position2;
    uint n2 = (position1 < position2) ? position2 : position1;

    if (average)
    {
        double x = (this->getNode(n1).getX() + this->getNode(n2).getX())/2.0;
        double y = (this->getNode(n1).getY() + this->getNode(n2).getY())/2.0;
        double z = (this->getNode(n1).getZ() + this->getNode(n2).getZ())/2.0;

        this->getNode(n1).set(x,y,z);
    }

    for (uint i=0;i<this->getNElements();i++)
    {
        this->getElement(i).mergeNodes(n1,n2,allowDowngrade);
    }
    this->removeNode(n2);
} /* RModel::mergeNodes */


std::vector<uint> RModel::findNearNodePositions(const RNode &node, double tolerance) const
{
    double dl;
    const RNode *pNode = 0;
    std::vector<uint> nearNodes;

    for (uint i=0;i<this->nodes.size();i++)
    {
        pNode = this->getNodePtr(i);

        dl = pNode->getDistance(node);

        if (dl < tolerance)
        {
            nearNodes.push_back(i);
        }
    }
    return nearNodes;
} /* RModel::findNearNodePositions */


uint RModel::mergeNearNodes(double tolerance)
{
    uint nMerged = 0;
    for (uint i=0;i<this->getNNodes();i++)
    {
        uint nodeID = i+1;
        while (nodeID < this->getNNodes())
        {
            if (this->getNode(i).getDistance(this->getNode(nodeID)) <= tolerance)
            {
                this->mergeNodes(i,nodeID,false,true);
                nMerged++;
            }
            nodeID++;
        }
    }
    return nMerged;
} /* RModel::mergeNearNodes */


void RModel::findNodeLimits(double &xmin, double &xmax, double &ymin, double &ymax, double &zmin, double &zmax) const
{
    xmin = xmax = ymin = ymax = zmin = zmax = 0.0;
    for (uint i=0;i<this->nodes.size();i++)
    {
        if (i == 0)
        {
            xmin = xmax = this->getNode(i).getX();
            ymin = ymax = this->getNode(i).getY();
            zmin = zmax = this->getNode(i).getZ();
        }
        xmin = std::min(xmin,this->getNode(i).getX());
        xmax = std::max(xmax,this->getNode(i).getX());
        ymin = std::min(ymin,this->getNode(i).getY());
        ymax = std::max(ymax,this->getNode(i).getY());
        zmin = std::min(zmin,this->getNode(i).getZ());
        zmax = std::max(zmax,this->getNode(i).getZ());
    }
} /* RModel::getNodeLimits */


double RModel::findNodeScale(void) const
{
    double xmin,xmax,ymin,ymax,zmin,zmax;
    this->findNodeLimits(xmin,xmax,ymin,ymax,zmin,zmax);
    double dx = xmax - xmin;
    double dy = ymax - ymin;
    double dz = zmax - zmin;

    if (dx >= dy && dx >= dz && dx != 0.0)
    {
        return 1.0/dx;
    }
    if (dy >= dx && dy >= dz && dy != 0.0)
    {
        return 1.0/dy;
    }
    if (dz >= dx && dz >= dy && dz != 0.0)
    {
        return 1.0/dz;
    }

    return 1.0;
} /* RModel::getNodeScale */


void RModel::findNodeCenter(double &xc, double &yc, double &zc) const
{
    double xmin,xmax,ymin,ymax,zmin,zmax;

    this->findNodeLimits(xmin,xmax,ymin,ymax,zmin,zmax);

    xc = (xmin + xmax)/2.0;
    yc = (ymin + ymax)/2.0;
    zc = (zmin + zmax)/2.0;
} /* RModel::getNodeCenter */


RStatistics RModel::findNodeDistanceStatistics(void) const
{
    QMap< QPair<uint,uint> , double > distMap;

    for (uint i=0;i<this->elements.size();i++)
    {
        for (uint j=0;j<this->elements[i].size();j++)
        {
            for (uint k=j+1;k<this->elements[i].size();k++)
            {
                uint n1 = this->elements[i].getNodeId(j);
                uint n2 = this->elements[i].getNodeId(k);
                if (n1 > n2)
                {
                    std::swap(n1,n2);
                }
                QPair<uint,uint> key(n1,n2);
                if (!distMap.contains(key))
                {
                    distMap.insert(key,this->nodes[n1].getDistance(this->nodes[n2]));
                }
            }
        }
    }

    RRVector nodeDist;
    nodeDist.reserve((distMap.size()));

    foreach (double distance, distMap)
    {
        nodeDist.push_back(distance);
    }

    return RStatistics(nodeDist,100,true);
} /* RModel::findNodeDistanceStatistics */


bool RModel::isNodeUsed(uint nodeID) const
{
    for (uint i=0;i<this->getNElements();i++)
    {
        if (this->getElement(i).hasNodeId(nodeID))
        {
            return true;
        }
    }
    return false;
} /* RModel::isNodeUsed */


uint RModel::purgeUnusedNodes(void)
{
    RLogger::info("Purging unused nodes\n");
    RLogger::indent();

    // Find unused nodes.
    std::vector<uint> nodeBook(this->getNNodes(),RConstants::eod);

//#pragma omp parallel for default(shared)
    for (uint i=0;i<this->getNElements();i++)
    {
        const RElement &rElement = this->getElement(i);
        for (uint j=0;j<rElement.size();j++)
        {
            nodeBook[rElement.getNodeId(j)] = 0;
        }
    }

    // Remove unused nodes.
    uint nn = this->getNNodes();
    for (uint i=0;i<nn;i++)
    {
        uint pos = nn - i - 1;
        if (nodeBook[pos] == RConstants::eod)
        {
            this->nodes.erase(this->nodes.begin()+pos);
            this->RResults::removeNode(pos);
        }
    }

    // Fix node IDs
    uint nNodes = 0;
    for (uint i=0;i<nodeBook.size();i++)
    {
        if (nodeBook[i] != RConstants::eod)
        {
            nodeBook[i] = nNodes++;
        }
    }

//#pragma omp parallel for default(shared)
    for (uint i=0;i<this->getNElements();i++)
    {
        RElement &rElement = this->getElement(i);
        for (uint j=0;j<rElement.size();j++)
        {
            rElement.setNodeId(j,nodeBook[rElement.getNodeId(j)]);
        }
    }
    RLogger::unindent();

    return uint(nodeBook.size()) - nNodes;
} /* RModel::purgeUnusedNodes */


/*********************************************************************
 * Element interface                                                 *
 *********************************************************************/


uint RModel::getNElements (RElementType elementType) const
{
    if (elementType == R_ELEMENT_NONE)
    {
        return uint(this->elements.size());
    }
    uint nElements = 0;
    for (uint i=0;i<this->elements.size();i++)
    {
        if (this->getElement(i).getType() == elementType)
        {
            nElements++;
        }
    }
    return nElements;
} /* RModel::getNElements */


uint RModel::getNElements(REntityGroupType elementGroupType) const
{
    if (elementGroupType == R_ENTITY_GROUP_NONE)
    {
        return uint(this->elements.size());
    }
    uint nElements = 0;
    for (uint i=0;i<this->elements.size();i++)
    {
        if (RElementGroup::getGroupType(this->getElement(i).getType()) == elementGroupType)
        {
            nElements++;
        }
    }
    return nElements;
} /* RModel::getNElements */


void RModel::setNElements (uint nelements)
{
    this->elements.resize(nelements);
    this->RResults::setNElements(nelements);

    for (uint i=0;i<this->elements.size();i++)
    {
        this->elements[i].setType(R_ELEMENT_NONE);
    }
    for (uint i=0;i<this->getNPoints();i++)
    {
        this->getPoint(i).resize(0);
    }
    for (uint i=0;i<this->getNLines();i++)
    {
        this->getLine(i).resize(0);
    }
    for (uint i=0;i<this->getNSurfaces();i++)
    {
        this->getSurface(i).resize(0);
    }
    for (uint i=0;i<this->getNVolumes();i++)
    {
        this->getVolume(i).resize(0);
    }
} /* RModel::getNElements */


const RElement * RModel::getElementPtr (uint position) const
{
    R_ERROR_ASSERT (position < this->elements.size());
    return &this->elements[position];
} /* RModel::getElementPtr */


RElement * RModel::getElementPtr (uint position)
{
    R_ERROR_ASSERT (position < this->elements.size());
    return &this->elements[position];
} /* RModel::getElementPtr */


const RElement & RModel::getElement (uint position) const
{
    R_ERROR_ASSERT (position < this->elements.size());
    return this->elements[position];
} /* RModel::getElement */


RElement & RModel::getElement (uint position)
{
    R_ERROR_ASSERT (position < this->elements.size());
    return this->elements[position];
} /* RModel::getElement */


const std::vector<RElement> &RModel::getElements(void) const
{
    return this->elements;
} /* RModel::getElements */


std::vector<RElement> &RModel::getElements(void)
{
    return this->elements;
} /* RModel::getElements */


void RModel::addElement (const RElement &element,
                         bool            addToGroup,
                         uint            groupID)
{
    this->elements.push_back(element);
    this->RResults::addElement(0.0);

    if (addToGroup)
    {
        this->addElementToGroup(this->getNElements()-1,groupID);
    }
    else
    {
        uint oldGroupID = 0;

        if (R_ELEMENT_TYPE_IS_POINT(element.getType()))
        {
            oldGroupID = this->getNPoints();
//            this->volumeNeigs.p
        }
        else if (R_ELEMENT_TYPE_IS_LINE(element.getType()))
        {
            oldGroupID = this->getNLines();
        }
        else if (R_ELEMENT_TYPE_IS_SURFACE(element.getType()))
        {
            oldGroupID = this->getNSurfaces();
            // Following works only for surface elements.
            double minNormalAngle = RConstants::pi;
            const RElement &rElement = this->getElement(this->getNElements()-1);
            RR3Vector en;
            rElement.findNormal(this->getNodes(),en[0],en[1],en[2]);
            for (uint i=0;i<this->getNSurfaces();i++)
            {
                const RSurface &rSurface = this->getSurface(i);
                for (uint j=0;j<rSurface.size();j++)
                {
                    uint elementID = rSurface.get(j);
                    RR3Vector nn;
                    this->getElement(elementID).findNormal(this->getNodes(),nn[0],nn[1],nn[2]);

                    double angle = RRVector::angle(en,nn);
                    if (angle < minNormalAngle)
                    {
                        minNormalAngle = angle;
                        oldGroupID = i;
                    }
                }
            }
        }
        else if (R_ELEMENT_TYPE_IS_VOLUME(element.getType()))
        {
            oldGroupID = this->getNVolumes();
        }
        this->addElementToGroup(this->getNElements()-1,oldGroupID);
    }

    // HACK: In case of point and line elements enlarge negbor arrays to avoid costly recalculations.
    if (R_ELEMENT_TYPE_IS_POINT(element.getType()) ||
        R_ELEMENT_TYPE_IS_LINE(element.getType()))
    {
        this->surfaceNeigs.push_back(RUVector());
        this->volumeNeigs.push_back(RUVector());
    }
} /* RModel::addElement */


void RModel::setElement (uint    position,
                         const RElement &element,
                         bool            addToGroup)
{
    R_ERROR_ASSERT (position < this->elements.size());

    REntityGroupType oldType = RElementGroup::getGroupType (this->elements[position].getType());
    REntityGroupType newType = RElementGroup::getGroupType (element.getType());

    this->elements[position] = element;

    if (oldType != newType)
    {
        uint grpPosition;

        // Remove element ID from its current group
        if (oldType == R_ENTITY_GROUP_POINT)
        {
            for (uint i=0;i<this->getNPoints();i++)
            {
                if (this->getPoint(i).findPosition(position,&grpPosition))
                {
                    this->getPoint(i).remove(grpPosition);
                }
            }
        }
        else if (oldType == R_ENTITY_GROUP_LINE)
        {
            for (uint i=0;i<this->getNLines();i++)
            {
                if (this->getLine(i).findPosition(position,&grpPosition))
                {
                    this->getLine(i).remove(grpPosition);
                }
            }
        }
        else if (oldType == R_ENTITY_GROUP_SURFACE)
        {
            for (uint i=0;i<this->getNSurfaces();i++)
            {
                if (this->getSurface(i).findPosition(position,&grpPosition))
                {
                    this->getSurface(i).remove(grpPosition);
                }
            }
        }
        else if (oldType == R_ENTITY_GROUP_VOLUME)
        {
            for (uint i=0;i<this->getNVolumes();i++)
            {
                if (this->getVolume(i).findPosition(position,&grpPosition))
                {
                    this->getVolume(i).remove(grpPosition);
                }
            }
        }

        // Add element ID to its appropriate group
        if (addToGroup)
        {
            this->addElementToGroup (position);
        }
    }
} /* RModel::setElement */


void RModel::removeElement (uint                position,
                            bool                removeGroups,
                            std::vector<uint> * removedPoints,
                            std::vector<uint> * removedLines,
                            std::vector<uint> * removedSurfaces,
                            std::vector<uint> * removedVolumes)
{
    R_ERROR_ASSERT (position < this->elements.size());

    if (removeGroups)
    {
        // Remove element from element groups
        RElementType elementType = this->getElement(position).getType();
        uint ePosition;

        if (R_ELEMENT_TYPE_IS_POINT(elementType))
        {
            std::vector<RPoint>::reverse_iterator rIter;
            for (rIter = this->points.rbegin();
                 rIter != this->points.rend();
                 ++rIter)
            {
                if (rIter->findPosition (position, &ePosition))
                {
                    rIter->remove(ePosition);
                }
                // Check if element group is empty
                if (rIter->empty())
                {
                    // Remove empty element group
                    if (removedPoints)
                    {
                        uint gPosition = std::distance (this->points.begin(),
                                                   (rIter+1).base());
                        removedPoints->push_back (gPosition);
                    }
                    this->points.erase ((rIter+1).base());
                }
                else
                {
                    // Decrease all element IDs by one which element ID
                    // is greater then position of remved element.
                    for (uint i=0;i<rIter->size();i++)
                    {
                        uint elementID = rIter->get(i);
                        if (elementID > position)
                        {
                            rIter->set(i,elementID-1);
                        }
                    }
                }
            }
        }
        else if (R_ELEMENT_TYPE_IS_LINE(elementType))
        {
            std::vector<RLine>::reverse_iterator rIter;
            for (rIter = this->lines.rbegin();
                 rIter != this->lines.rend();
                 ++rIter)
            {
                if (rIter->findPosition (position, &ePosition))
                {
                    rIter->remove(ePosition);
                }
                // Check if element group is empty
                if (rIter->empty())
                {
                    // Remove empty element group
                    if (removedLines)
                    {
                        uint gPosition = std::distance (this->lines.begin(),
                                                   (rIter+1).base());
                        removedLines->push_back (gPosition);
                    }
                    this->lines.erase ((rIter+1).base());
                }
                else
                {
                    // Decrease all element IDs by one which element ID
                    // is greater then position of remved element.
                    for (uint i=0;i<rIter->size();i++)
                    {
                        uint elementID = rIter->get(i);
                        if (elementID > position)
                        {
                            rIter->set(i,elementID-1);
                        }
                    }
                }
            }
        }
        else if (R_ELEMENT_TYPE_IS_SURFACE(elementType))
        {
            std::vector<RSurface>::reverse_iterator rIter;
            for (rIter = this->surfaces.rbegin();
                 rIter != this->surfaces.rend();
                 ++rIter)
            {
                if (rIter->findPosition (position, &ePosition))
                {
                    rIter->remove(ePosition);
                }
                // Check if element group is empty
                if (rIter->empty())
                {
                    // Remove empty element group
                    if (removedSurfaces)
                    {
                        uint gPosition = std::distance (this->surfaces.begin(),(rIter+1).base());
                        removedSurfaces->push_back (gPosition);
                    }
                    this->surfaces.erase ((rIter+1).base());
                }
                else
                {
                    // Decrease all element IDs by one which element ID
                    // is greater then position of remved element.
                    for (uint i=0;i<rIter->size();i++)
                    {
                        uint elementID = rIter->get(i);
                        if (elementID > position)
                        {
                            rIter->set(i,elementID-1);
                        }
                    }
                }
            }
        }
        else if (R_ELEMENT_TYPE_IS_VOLUME(elementType))
        {
            std::vector<RVolume>::reverse_iterator rIter;
            for (rIter = this->volumes.rbegin();
                 rIter != this->volumes.rend();
                 ++rIter)
            {
                if (rIter->findPosition (position, &ePosition))
                {
                    rIter->remove(ePosition);
                }
                // Check if element group is empty
                if (rIter->empty())
                {
                    // Remove empty element group
                    if (removedVolumes)
                    {
                        uint gPosition = std::distance (this->volumes.begin(),
                                                   (rIter+1).base());
                        removedVolumes->push_back (gPosition);
                    }
                    this->volumes.erase ((rIter+1).base());
                }
                else
                {
                    // Decrease all element IDs by one which element ID
                    // is greater then position of remved element.
                    for (uint i=0;i<rIter->size();i++)
                    {
                        uint elementID = rIter->get(i);
                        if (elementID > position)
                        {
                            rIter->set(i,elementID-1);
                        }
                    }
                }
            }
        }
    }

    // Find which nodes should be removed.
    std::vector<uint> nodesToRemove;
    for (uint i=0;i<this->getElement(position).size();i++)
    {
        uint nodeID = this->getElement(position).getNodeId(i);
        bool nodeIsUsed = false;
        for (uint j=0;j<this->getNElements() && !nodeIsUsed;j++)
        {
            if (j == position)
            {
                continue;
            }
            if (this->getElement(j).hasNodeId(nodeID))
            {
                nodeIsUsed = true;
            }
        }
        if (!nodeIsUsed)
        {
            nodesToRemove.push_back(nodeID);
        }
    }
    std::sort(nodesToRemove.begin(),nodesToRemove.end());

    // Remove element from elements vector
    std::vector<RElement>::iterator iter = this->elements.begin();
    std::advance (iter, position);

    this->elements.erase(iter);
    this->RResults::removeElement(position);

    // Remove node
    for (uint i=0;i<nodesToRemove.size();i++)
    {
        uint pos = uint(nodesToRemove.size()) - i - 1;
        this->removeNode(nodesToRemove[pos]);
    }
} /* RModel::removeElement */


void RModel::removeElements(const QList<uint> &elementIDs, bool closeHole)
{
    QList<uint> sortedElementIDs(elementIDs);

    qSort(sortedElementIDs.begin(),sortedElementIDs.end());

    for (int i=sortedElementIDs.size()-1;i>=0;i--)
    {
//        RLogger::info("Removing element #%u\n",sortedElementIDs[i]);

//        QList<uint> edgeNodeIds;
        if (closeHole)
        {
//            edgeNodeIds = this->findElementEdgeRing(sortedElementIDs[i]);
        }

//        for (int j=0;j<edgeNodeIds.size();j++)
//        {
//            if (edgeNodeIds[j] > sortedElementIDs[i])
//            {
//                edgeNodeIds[j]--;
//            }
//        }

        this->removeElement(sortedElementIDs[i],true);

//        if (closeHole)
//        {
//            std::vector<RNode> edgeNodes;
//            edgeNodes.resize(edgeNodeIds.size());
//            for (uint i=0;i<edgeNodes.size();i++)
//            {
//                edgeNodes[i] = this->getNode(edgeNodeIds[i]);
//            }

//            if (edgeNodes.size() > 0)
//            {
//                std::vector<RElement> patchEelements = RPolygon::triangulate(edgeNodes,true);
//                for (uint j=0;j<patchEelements.size();j++)
//                {
//                    for (uint k=0;k<patchEelements[j].size();k++)
//                    {
//                        patchEelements[j].setNodeId(k,edgeNodeIds[patchEelements[j].getNodeId(k)]);
//                    }
//                    this->addElement(patchEelements[j]);
//                }
//            }
//        }
    }
} /* RModel::removeElements */


std::vector<uint> RModel::findElementPositionsByNodeId
                                         ( uint nodeID ) const
{
    std::vector<uint> elementPositions;
    std::vector<RElement>::iterator iter;
    const RElement *pElement = 0;

    R_ERROR_ASSERT (nodeID < this->getNNodes());

    for (uint i=0;i<this->getNElements();i++)
    {
        pElement = this->getElementPtr (i);
        if (pElement->hasNodeId(nodeID))
        {
            elementPositions.push_back(i);
        }
    }
    return elementPositions;
} /* RModel::findElementPositionsByNodeId */


RStatistics RModel::findLineElementSizeStatistics(void) const
{
    RRVector elementSizes;
    return RStatistics(elementSizes,100,true);
} /* RModel::findLineElementSizeStatistics */


RStatistics RModel::findSurfaceElementSizeStatistics() const
{
    RRVector elementSizes;
    return RStatistics(elementSizes,100,true);
} /* RModel::findSurfaceElementSizeStatistics */


RStatistics RModel::findVolumeElementSizeStatistics() const
{
    RRVector elementSizes;
    return RStatistics(elementSizes,100,true);
} /* RModel::findVolumeElementSizeStatistics */


uint RModel::purgeUnusedElements(void)
{
    RLogger::info("Purging unused elements\n");
    RLogger::indent();

    uint nElementGroups = this->getNElementGroups();

    // Find unused elements
    std::vector<uint> elementBook(this->getNElements(),RConstants::eod);

    uint ne = this->getNElements();

#pragma omp parallel for default(shared)
    for (int i=0;i<int(nElementGroups);i++)
    {
        const RElementGroup *pElementGroup = this->getElementGroupPtr(i);
        for (uint j=0;j<pElementGroup->size();j++)
        {
            elementBook[pElementGroup->get(j)] = 0;
        }
    }

    // Remove unused elements
    for (uint i=uint(this->elements.size());i>0;i--)
    {
        if (elementBook[i-1] == RConstants::eod)
        {
            this->elements.erase(this->elements.begin()+i-1);
            this->RResults::removeElement(i-1);
        }
    }

    // Fix element ID references in element groups
    uint nElements = 0;
    for (uint i=0;i<elementBook.size();i++)
    {
        if (elementBook[i] != RConstants::eod)
        {
            elementBook[i] = nElements++;
        }
    }

#pragma omp parallel for default(shared)
    for (int64_t i=0;i<int64_t(nElementGroups);i++)
    {
        RElementGroup *pElementGroup = this->getElementGroupPtr(i);
        for (uint j=0;j<pElementGroup->size();j++)
        {
            pElementGroup->set(j,elementBook[pElementGroup->get(j)]);
        }
    }

    // Update surface neighbors
    if (this->surfaceNeigs.size() == elementBook.size())
    {
        for (uint i=0;i<this->surfaceNeigs.size();i++)
        {
            RUVector::reverse_iterator riter;
            for (riter=this->surfaceNeigs[i].rbegin();riter!=this->surfaceNeigs[i].rend();++riter)
            {
                uint neigborID = (*riter);
                if (elementBook[neigborID] == RConstants::eod)
                {
                    this->surfaceNeigs[i].erase(--riter.base());
                }
                else
                {
                    (*riter) = elementBook[neigborID];
                }
            }
        }
        for (uint i=uint(elementBook.size());i>0;i--)
        {
            uint elementID = i-1;
            if (elementBook[elementID] == RConstants::eod)
            {
                this->surfaceNeigs.erase(this->surfaceNeigs.begin() + elementID);
            }
        }
    }

    // Update volume neighbors
    if (this->volumeNeigs.size() == elementBook.size())
    {
        for (uint i=0;i<this->volumeNeigs.size();i++)
        {
            RUVector::reverse_iterator riter;
            for (riter=this->volumeNeigs[i].rbegin();riter!=this->volumeNeigs[i].rend();++riter)
            {
                uint neigborID = (*riter);
                if (elementBook[neigborID] == RConstants::eod)
                {
                    this->volumeNeigs[i].erase(--riter.base());
                }
                else
                {
                    (*riter) = elementBook[neigborID];
                }
            }
        }
        for (uint i=uint(elementBook.size());i>0;i--)
        {
            uint elementID = i-1;
            if (elementBook[elementID] == RConstants::eod)
            {
                this->volumeNeigs.erase(this->volumeNeigs.begin() + elementID);
            }
        }
    }
    RLogger::unindent();

    return ne - this->getNElements();
} /* RModel::purgeUnusedElements */


/*********************************************************************
 * Interpolated element interface                                    *
 *********************************************************************/


const RInterpolatedElement *RModel::getInterpolatedElement(REntityGroupType type, uint entityID, uint elementID) const
{
    switch (type)
    {
        case R_ENTITY_GROUP_CUT:
        {
            return &this->cuts[entityID].at(elementID);
        }
        case R_ENTITY_GROUP_ISO:
        {
            return &this->isos[entityID].at(elementID);
        }
        case R_ENTITY_GROUP_STREAM_LINE:
        {
            return &this->streamLines[entityID].at(elementID);
        }
        default:
        {
            return 0;
        }
    }
} /* RModel::purgeUnusedElements */


RInterpolatedElement *RModel::getInterpolatedElement(REntityGroupType type, uint entityID, uint elementID)
{
    switch (type)
    {
        case R_ENTITY_GROUP_CUT:
        {
            return &this->cuts[entityID].at(elementID);
        }
        case R_ENTITY_GROUP_ISO:
        {
            return &this->isos[entityID].at(elementID);
        }
        case R_ENTITY_GROUP_STREAM_LINE:
        {
            return &this->streamLines[entityID].at(elementID);
        }
        default:
        {
            return 0;
        }
    }
} /* RModel::purgeUnusedElements */


/*********************************************************************
 * Element group interface                                           *
 *********************************************************************/


uint RModel::getNEntityElements(REntityGroupType entityType) const
{
    uint nElements = 0;

    if (R_ENTITY_GROUP_POINT & entityType)
    {
        for (uint i=0;i<this->getNPoints();i++)
        {
            nElements += this->getPoint(i).size();
        }
    }
    if (R_ENTITY_GROUP_LINE & entityType)
    {
        for (uint i=0;i<this->getNLines();i++)
        {
            nElements += this->getLine(i).size();
        }
    }
    if (R_ENTITY_GROUP_SURFACE & entityType)
    {
        for (uint i=0;i<this->getNSurfaces();i++)
        {
            nElements += this->getSurface(i).size();
        }
    }
    if (R_ENTITY_GROUP_VOLUME & entityType)
    {
        for (uint i=0;i<this->getNVolumes();i++)
        {
            nElements += this->getVolume(i).size();
        }
    }

    return nElements;
} /* RModel::getNEntityElements */


uint RModel::getNElementGroups(void) const
{
    return this->getNEntityGroups(true);
} /* RModel::getNElementGroups */


uint RModel::getNEntityGroups(bool onlyElements) const
{
    uint nEntityGroups = this->getNPoints() + this->getNLines() + this->getNSurfaces() + this->getNVolumes();;
    if (!onlyElements)
    {
        nEntityGroups += this->getNVectorFields() + this->getNScalarFields() + this->getNStreamLines() + this->getNCuts() + this->getNIsos();
    }
    return nEntityGroups;
} /* RModel::getNEntityGroups */


const RElementGroup *RModel::getElementGroupPtr(uint groupID) const
{
    R_ERROR_ASSERT(groupID < this->getNElementGroups());

    return static_cast<const RElementGroup*>(this->getEntityGroupPtr(groupID,true));
} /* RModel::getElementGroupPtr */


RElementGroup *RModel::getElementGroupPtr(uint groupID)
{
    R_ERROR_ASSERT(groupID < this->getNElementGroups());

    return static_cast<RElementGroup*>(this->getEntityGroupPtr(groupID,true));
} /* RModel::getElementGroupPtr */


const REntityGroup *RModel::getEntityGroupPtr(uint groupID, bool onlyElements) const
{
    R_ERROR_ASSERT(groupID < this->getNEntityGroups());

    uint groupPossition = groupID;
    if (groupPossition < this->getNPoints())
    {
        return this->getPointPtr(groupPossition);
    }

    groupPossition -= this->getNPoints();
    if (groupPossition < this->getNLines())
    {
        return this->getLinePtr(groupPossition);
    }

    groupPossition -= this->getNLines();
    if (groupPossition < this->getNSurfaces())
    {
        return this->getSurfacePtr(groupPossition);
    }

    groupPossition -= this->getNSurfaces();
    if (groupPossition < this->getNVolumes())
    {
        return this->getVolumePtr(groupPossition);
    }

    if (!onlyElements)
    {
        groupPossition -= this->getNVolumes();
        if (groupPossition < this->getNVectorFields())
        {
            return this->getVectorFieldPtr(groupPossition);
        }

        groupPossition -= this->getNVectorFields();
        if (groupPossition < this->getNScalarFields())
        {
            return this->getScalarFieldPtr(groupPossition);
        }

        groupPossition -= this->getNScalarFields();
        if (groupPossition < this->getNStreamLines())
        {
            return this->getStreamLinePtr(groupPossition);
        }

        groupPossition -= this->getNStreamLines();
        if (groupPossition < this->getNCuts())
        {
            return this->getCutPtr(groupPossition);
        }

        groupPossition -= this->getNCuts();
        if (groupPossition < this->getNIsos())
        {
            return this->getIsoPtr(groupPossition);
        }
    }

    return 0;
} /* RModel::getEntityGroupPtr */


REntityGroup *RModel::getEntityGroupPtr(uint groupID, bool onlyElements)
{
    R_ERROR_ASSERT(groupID < this->getNEntityGroups());

    uint groupPossition = groupID;
    if (groupPossition < this->getNPoints())
    {
        return this->getPointPtr(groupPossition);
    }

    groupPossition -= this->getNPoints();
    if (groupPossition < this->getNLines())
    {
        return this->getLinePtr(groupPossition);
    }

    groupPossition -= this->getNLines();
    if (groupPossition < this->getNSurfaces())
    {
        return this->getSurfacePtr(groupPossition);
    }

    groupPossition -= this->getNSurfaces();
    if (groupPossition < this->getNVolumes())
    {
        return this->getVolumePtr(groupPossition);
    }

    if (!onlyElements)
    {
        groupPossition -= this->getNVolumes();
        if (groupPossition < this->getNVectorFields())
        {
            return this->getVectorFieldPtr(groupPossition);
        }

        groupPossition -= this->getNVectorFields();
        if (groupPossition < this->getNScalarFields())
        {
            return this->getScalarFieldPtr(groupPossition);
        }

        groupPossition -= this->getNScalarFields();
        if (groupPossition < this->getNStreamLines())
        {
            return this->getStreamLinePtr(groupPossition);
        }

        groupPossition -= this->getNStreamLines();
        if (groupPossition < this->getNCuts())
        {
            return this->getCutPtr(groupPossition);
        }

        groupPossition -= this->getNCuts();
        if (groupPossition < this->getNIsos())
        {
            return this->getIsoPtr(groupPossition);
        }
    }

    return 0;
} /* RModel::getEntityGroupPtr */


REntityGroupType RModel::getEntityGroupType(uint groupID, bool onlyElements) const
{
    R_ERROR_ASSERT(groupID < this->getNEntityGroups());

    uint groupPossition = groupID;
    if (groupPossition < this->getNPoints())
    {
        return R_ENTITY_GROUP_POINT;
    }

    groupPossition -= this->getNPoints();
    if (groupPossition < this->getNLines())
    {
        return R_ENTITY_GROUP_LINE;
    }

    groupPossition -= this->getNLines();
    if (groupPossition < this->getNSurfaces())
    {
        return R_ENTITY_GROUP_SURFACE;
    }

    groupPossition -= this->getNSurfaces();
    if (groupPossition < this->getNVolumes())
    {
        return R_ENTITY_GROUP_VOLUME;
    }

    if (!onlyElements)
    {
        groupPossition -= this->getNVolumes();
        if (groupPossition < this->getNVectorFields())
        {
            return R_ENTITY_GROUP_VECTOR_FIELD;
        }

        groupPossition -= this->getNVectorFields();
        if (groupPossition < this->getNScalarFields())
        {
            return R_ENTITY_GROUP_SCALAR_FIELD;
        }

        groupPossition -= this->getNScalarFields();
        if (groupPossition < this->getNStreamLines())
        {
            return R_ENTITY_GROUP_STREAM_LINE;
        }

        groupPossition -= this->getNStreamLines();
        if (groupPossition < this->getNCuts())
        {
            return R_ENTITY_GROUP_CUT;
        }

        groupPossition -= this->getNCuts();
        if (groupPossition < this->getNIsos())
        {
            return R_ENTITY_GROUP_ISO;
        }
    }

    return R_ENTITY_GROUP_NONE;
} /* RModel::getEntityGroupType */


uint RModel::getEntityGroupPosition(uint groupID, bool onlyElements) const
{
    R_ERROR_ASSERT(groupID < this->getNEntityGroups());

    uint groupPossition = groupID;
    if (groupPossition < this->getNPoints())
    {
        return groupPossition;
    }

    groupPossition -= this->getNPoints();
    if (groupPossition < this->getNLines())
    {
        return groupPossition;
    }

    groupPossition -= this->getNLines();
    if (groupPossition < this->getNSurfaces())
    {
        return groupPossition;
    }

    groupPossition -= this->getNSurfaces();
    if (groupPossition < this->getNVolumes())
    {
        return groupPossition;
    }

    if (!onlyElements)
    {
        groupPossition -= this->getNVolumes();
        if (groupPossition < this->getNVectorFields())
        {
            return groupPossition;
        }

        groupPossition -= this->getNVectorFields();
        if (groupPossition < this->getNScalarFields())
        {
            return groupPossition;
        }

        groupPossition -= this->getNScalarFields();
        if (groupPossition < this->getNStreamLines())
        {
            return groupPossition;
        }

        groupPossition -= this->getNStreamLines();
        if (groupPossition < this->getNCuts())
        {
            return groupPossition;
        }

        groupPossition -= this->getNCuts();
        if (groupPossition < this->getNIsos())
        {
            return groupPossition;
        }
    }

    return RConstants::eod;
} /* RModel::getEntityGroupPosition */


uint RModel::getElementGroupID(REntityGroupType entityType, uint entityID) const
{
    return this->getEntityGroupID(entityType,entityID,true);
} /* RModel::getElementGroupID */


uint RModel::getEntityGroupID(REntityGroupType entityType, uint entityID, bool onlyElements) const
{
    uint elementGroupID = 0;
    switch (entityType)
    {
        case R_ENTITY_GROUP_POINT:
            if (entityID > this->getNPoints())
            {
                return RConstants::eod;
            }
            break;
        case R_ENTITY_GROUP_LINE:
            if (entityID > this->getNLines())
            {
                return RConstants::eod;
            }
            elementGroupID += this->getNPoints();
            break;
        case R_ENTITY_GROUP_SURFACE:
            if (entityID > this->getNSurfaces())
            {
                return RConstants::eod;
            }
            elementGroupID += this->getNPoints();
            elementGroupID += this->getNLines();
            break;
        case R_ENTITY_GROUP_VOLUME:
            if (entityID > this->getNVolumes())
            {
                return RConstants::eod;
            }
            elementGroupID += this->getNPoints();
            elementGroupID += this->getNLines();
            elementGroupID += this->getNSurfaces();
            break;
        case R_ENTITY_GROUP_VECTOR_FIELD:
            if (entityID > this->getNVectorFields() || onlyElements)
            {
                return RConstants::eod;
            }
            elementGroupID += this->getNPoints();
            elementGroupID += this->getNLines();
            elementGroupID += this->getNSurfaces();
            elementGroupID += this->getNVolumes();
            break;
        case R_ENTITY_GROUP_SCALAR_FIELD:
            if (entityID > this->getNScalarFields() || onlyElements)
            {
                return RConstants::eod;
            }
            elementGroupID += this->getNPoints();
            elementGroupID += this->getNLines();
            elementGroupID += this->getNSurfaces();
            elementGroupID += this->getNVolumes();
            elementGroupID += this->getNVectorFields();
            break;
        case R_ENTITY_GROUP_STREAM_LINE:
            if (entityID > this->getNStreamLines() || onlyElements)
            {
                return RConstants::eod;
            }
            elementGroupID += this->getNPoints();
            elementGroupID += this->getNLines();
            elementGroupID += this->getNSurfaces();
            elementGroupID += this->getNVolumes();
            elementGroupID += this->getNVectorFields();
            elementGroupID += this->getNScalarFields();
            break;
        case R_ENTITY_GROUP_CUT:
            if (entityID > this->getNCuts() || onlyElements)
            {
                return RConstants::eod;
            }
            elementGroupID += this->getNPoints();
            elementGroupID += this->getNLines();
            elementGroupID += this->getNSurfaces();
            elementGroupID += this->getNVolumes();
            elementGroupID += this->getNVectorFields();
            elementGroupID += this->getNScalarFields();
            elementGroupID += this->getNStreamLines();
            break;
        case R_ENTITY_GROUP_ISO:
            if (entityID > this->getNIsos() || onlyElements)
            {
                return RConstants::eod;
            }
            elementGroupID += this->getNPoints();
            elementGroupID += this->getNLines();
            elementGroupID += this->getNSurfaces();
            elementGroupID += this->getNVolumes();
            elementGroupID += this->getNVectorFields();
            elementGroupID += this->getNScalarFields();
            elementGroupID += this->getNStreamLines();
            elementGroupID += this->getNCuts();
            break;
        default:
            return RConstants::eod;
    }

    elementGroupID += entityID;

    return elementGroupID;
} /* RModel::getEntityGroupID */


std::vector<uint> RModel::getEntityGroupIDs(REntityGroupType entityType) const
{
    uint nEntities = 0;

    switch (entityType)
    {
        case R_ENTITY_GROUP_POINT:
            nEntities = this->getNPoints();
            break;
        case R_ENTITY_GROUP_LINE:
            nEntities = this->getNLines();
            break;
        case R_ENTITY_GROUP_SURFACE:
            nEntities = this->getNSurfaces();
            break;
        case R_ENTITY_GROUP_VOLUME:
            nEntities = this->getNVolumes();
            break;
        case R_ENTITY_GROUP_VECTOR_FIELD:
            nEntities = this->getNVectorFields();
            break;
        case R_ENTITY_GROUP_SCALAR_FIELD:
            nEntities = this->getNScalarFields();
            break;
        case R_ENTITY_GROUP_STREAM_LINE:
            nEntities = this->getNStreamLines();
            break;
        case R_ENTITY_GROUP_CUT:
            nEntities = this->getNCuts();
            break;
        case R_ENTITY_GROUP_ISO:
            nEntities = this->getNIsos();
            break;
        default:
            nEntities = 0;
            break;
    }

    std::vector<uint> groupIDs;

    for (uint i=0;i<nEntities;i++)
    {
        groupIDs.push_back(this->getEntityGroupID(entityType,i));
    }

    return groupIDs;
} /* RModel::getEntityGroupIDs */


bool RModel::getEntityID(uint groupID, REntityGroupType &entityType, uint &entityID) const
{
    uint ceid = 0;
    uint cgid = this->getNPoints();
    if (groupID < cgid)
    {
        entityType = R_ENTITY_GROUP_POINT;
        entityID = groupID - ceid;
        return true;
    }
    ceid = cgid;
    cgid += this->getNLines();
    if (groupID < cgid)
    {
        entityType = R_ENTITY_GROUP_LINE;
        entityID = groupID - ceid;
        return true;
    }
    ceid = cgid;
    cgid += this->getNSurfaces();
    if (groupID < cgid)
    {
        entityType = R_ENTITY_GROUP_SURFACE;
        entityID = groupID - ceid;
        return true;
    }
    ceid = cgid;
    cgid += this->getNVolumes();
    if (groupID < cgid)
    {
        entityType = R_ENTITY_GROUP_VOLUME;
        entityID = groupID - ceid;
        return true;
    }
    ceid = cgid;
    cgid += this->getNVectorFields();
    if (groupID < cgid)
    {
        entityType = R_ENTITY_GROUP_VECTOR_FIELD;
        entityID = groupID - ceid;
        return true;
    }
    ceid = cgid;
    cgid += this->getNScalarFields();
    if (groupID < cgid)
    {
        entityType = R_ENTITY_GROUP_SCALAR_FIELD;
        entityID = groupID - ceid;
        return true;
    }
    ceid = cgid;
    cgid += this->getNStreamLines();
    if (groupID < cgid)
    {
        entityType = R_ENTITY_GROUP_STREAM_LINE;
        entityID = groupID - ceid;
        return true;
    }
    ceid = cgid;
    cgid += this->getNCuts();
    if (groupID < cgid)
    {
        entityType = R_ENTITY_GROUP_CUT;
        entityID = groupID - ceid;
        return true;
    }
    ceid = cgid;
    cgid += this->getNIsos();
    if (groupID < cgid)
    {
        entityType = R_ENTITY_GROUP_ISO;
        entityID = groupID - ceid;
        return true;
    }
    return false;
} /* RModel::getEntityID */


uint RModel::findElementID(REntityGroupType entityType, uint entityID, uint elementPosition) const
{
    uint groupID = this->getElementGroupID(entityType,entityID);
    if (groupID == RConstants::eod)
    {
        return RConstants::eod;
    }
    const RElementGroup *pElementGroup = this->getElementGroupPtr(groupID);
    if (!pElementGroup)
    {
        return RConstants::eod;
    }
    return pElementGroup->get(elementPosition);
} /* RModel::findElementID */


uint RModel::findNodeID(REntityGroupType entityType, uint entityID, uint elementPosition, uint nodePosition) const
{
    uint elementID = this->findElementID(entityType,entityID,elementPosition);
    if (elementID == RConstants::eod)
    {
        return RConstants::eod;
    }
    const RElement &rElement = this->getElement(elementID);
    if (nodePosition >= rElement.size())
    {
        return RConstants::eod;
    }
    return rElement.getNodeId(nodePosition);
} /* RModel::findNodeID */


std::vector<uint> RModel::findElementIDs(REntityGroupType entityType, const std::vector<uint> entityIDs) const
{
    std::vector<uint> elementIDsSet;
    elementIDsSet.resize(this->getNElements(),RConstants::eod);
    uint nElementIDs = 0;

    for (uint i=0;i<entityIDs.size();i++)
    {
        uint groupID = this->getElementGroupID(entityType,entityIDs[i]);
        if (groupID == RConstants::eod)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Invalid entity ID.");
        }
        const RElementGroup *pElementGroup = this->getElementGroupPtr(groupID);
        if (!pElementGroup)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Invalid entity ID. Element group was not found.");
        }
        for (uint j=0;j<pElementGroup->size();j++)
        {
            if (elementIDsSet[pElementGroup->get(j)] == RConstants::eod)
            {
                elementIDsSet[pElementGroup->get(j)] = nElementIDs++;
            }
        }
    }

    std::vector<uint> elementIDs(nElementIDs);
    for (uint i=0;i<elementIDsSet.size();i++)
    {
        if (elementIDsSet[i] != RConstants::eod)
        {
            elementIDs[elementIDsSet[i]] = i;
        }
    }

    return elementIDs;
} /* RModel::findElementIDs */


void RModel::mergeEntities(REntityGroupType entityType, const QList<uint> &entityIDs)
{
    if (entityIDs.size() < 2)
    {
        return;
    }

    switch (entityType)
    {
        case R_ENTITY_GROUP_POINT:
        {
            RLogger::info("Merging point entities\n");
            RLogger::indent();
            for (uint i=entityIDs.size()-1;i>0;i--)
            {
                for (uint j=0;j<this->getPoint(entityIDs[i]).size();j++)
                {
                    this->getPoint(entityIDs[0]).add(this->getPoint(entityIDs[i]).get(j));
                }
                this->removePoint(entityIDs[i]);
            }
            RLogger::unindent();
            break;
        }
        case R_ENTITY_GROUP_LINE:
        {
            RLogger::info("Merging line entities\n");
            RLogger::indent();
            for (uint i=entityIDs.size()-1;i>0;i--)
            {
                for (uint j=0;j<this->getLine(entityIDs[i]).size();j++)
                {
                    this->getLine(entityIDs[0]).add(this->getLine(entityIDs[i]).get(j));
                }
                this->removeLine(entityIDs[i]);
            }
            RLogger::unindent();
            break;
        }
        case R_ENTITY_GROUP_SURFACE:
        {
            RLogger::info("Merging surface entities\n");
            RLogger::indent();
            for (uint i=entityIDs.size()-1;i>0;i--)
            {
                for (uint j=0;j<this->getSurface(entityIDs[i]).size();j++)
                {
                    this->getSurface(entityIDs[0]).add(this->getSurface(entityIDs[i]).get(j));
                }
                this->removeSurface(entityIDs[i]);
            }
            RLogger::unindent();
            break;
        }
        case R_ENTITY_GROUP_VOLUME:
        {
            RLogger::info("Merging volume entities\n");
            RLogger::indent();
            for (uint i=entityIDs.size()-1;i>0;i--)
            {
                for (uint j=0;j<this->getVolume(entityIDs[i]).size();j++)
                {
                    this->getVolume(entityIDs[0]).add(this->getVolume(entityIDs[i]).get(j));
                }
                this->removeVolume(entityIDs[i]);
            }
            RLogger::unindent();
            break;
        }
        default:
        {
            break;
        }
    }
} /* RModel::mergeEntities */


void RModel::removeEntities(REntityGroupType entityType, const QList<uint> &entityIDs)
{
    if (entityIDs.size() == 0)
    {
        return;
    }

    switch (entityType)
    {
        case R_ENTITY_GROUP_POINT:
        {
            RLogger::info("Removing point entities\n");
            RLogger::indent();
            for (int i=entityIDs.size()-1;i>=0;i--)
            {
                RLogger::info("Removing point \'%s\'\n",this->getPoint(entityIDs[i]).getName().toUtf8().constData());
                this->removePoint(entityIDs[i]);
            }
            RLogger::unindent();
            break;
        }
        case R_ENTITY_GROUP_LINE:
        {
            RLogger::info("Removing line entities\n");
            RLogger::indent();
            for (int i=entityIDs.size()-1;i>=0;i--)
            {
                RLogger::info("Removing line \'%s\'\n",this->getLine(entityIDs[i]).getName().toUtf8().constData());
                this->removeLine(entityIDs[i]);
            }
            RLogger::unindent();
            break;
        }
        case R_ENTITY_GROUP_SURFACE:
        {
            RLogger::info("Removing surface entities\n");
            RLogger::indent();
            for (int i=entityIDs.size()-1;i>=0;i--)
            {
                RLogger::info("Removing surface \'%s\'\n",this->getSurface(entityIDs[i]).getName().toUtf8().constData());
                this->removeSurface(entityIDs[i]);
            }
            RLogger::unindent();
            break;
        }
        case R_ENTITY_GROUP_VOLUME:
        {
            RLogger::info("Removing volume entities\n");
            RLogger::indent();
            for (int i=entityIDs.size()-1;i>=0;i--)
            {
                RLogger::info("Removing volume \'%s\'\n",this->getVolume(entityIDs[i]).getName().toUtf8().constData());
                this->removeVolume(entityIDs[i]);
            }
            RLogger::unindent();
            break;
        }
        case R_ENTITY_GROUP_VECTOR_FIELD:
        {
            RLogger::info("Removing vector field entities\n");
            RLogger::indent();
            for (int i=entityIDs.size()-1;i>=0;i--)
            {
                RLogger::info("Removing vector field \'%s\'\n",this->getVectorField(entityIDs[i]).getName().toUtf8().constData());
                this->removeVectorField(entityIDs[i]);
            }
            RLogger::unindent();
            break;
        }
        case R_ENTITY_GROUP_SCALAR_FIELD:
        {
            RLogger::info("Removing scalar field entities\n");
            RLogger::indent();
            for (int i=entityIDs.size()-1;i>=0;i--)
            {
                RLogger::info("Removing scalar field \'%s\'\n",this->getScalarField(entityIDs[i]).getName().toUtf8().constData());
                this->removeScalarField(entityIDs[i]);
            }
            RLogger::unindent();
            break;
        }
        case R_ENTITY_GROUP_STREAM_LINE:
        {
            RLogger::info("Removing stream line entities\n");
            RLogger::indent();
            for (int i=entityIDs.size()-1;i>=0;i--)
            {
                RLogger::info("Removing stream line \'%s\'\n",this->getStreamLine(entityIDs[i]).getName().toUtf8().constData());
                this->removeStreamLine(entityIDs[i]);
            }
            RLogger::unindent();
            break;
        }
        case R_ENTITY_GROUP_CUT:
        {
            RLogger::info("Removing cut entities\n");
            RLogger::indent();
            for (int i=entityIDs.size()-1;i>=0;i--)
            {
                RLogger::info("Removing cut \'%s\'\n",this->getCut(entityIDs[i]).getName().toUtf8().constData());
                this->removeCut(entityIDs[i]);
            }
            RLogger::unindent();
            break;
        }
        case R_ENTITY_GROUP_ISO:
        {
            RLogger::info("Removing iso entities\n");
            RLogger::indent();
            for (int i=entityIDs.size()-1;i>=0;i--)
            {
                RLogger::info("Removing iso \'%s\'\n",this->getIso(entityIDs[i]).getName().toUtf8().constData());
                this->removeIso(entityIDs[i]);
            }
            RLogger::unindent();
            break;
        }
        default:
        {
            break;
        }
    }
} /* RModel::removeEntities */


/*********************************************************************
 * Point interface                                                   *
 *********************************************************************/


uint RModel::getNPoints (void) const
{
    return uint(this->points.size());
} /* RModel::getNPoints */


void RModel::setNPoints (uint npoints)
{
    this->points.resize(npoints);
} /* RModel::setNPoints */


const RPoint * RModel::getPointPtr (uint position) const
{
    R_ERROR_ASSERT (position < this->points.size());
    return &this->points[position];
} /* RModel::getPointPtr */


RPoint * RModel::getPointPtr (uint position)
{
    R_ERROR_ASSERT (position < this->points.size());
    return &this->points[position];
} /* RModel::getPointPtr */


const RPoint & RModel::getPoint (uint position) const
{
    R_ERROR_ASSERT (position < this->points.size());
    return this->points[position];
} /* RModel::getPoint */


RPoint & RModel::getPoint (uint position)
{
    R_ERROR_ASSERT (position < this->points.size());
    return this->points[position];
} /* RModel::getPoint */


void RModel::addPoint (const RPoint &point)
{
    this->points.push_back (point);
} /* RModel::addPoint */


void RModel::setPoint (uint  position,
                        const RPoint &point)
{
    R_ERROR_ASSERT (position < this->points.size());
    this->points[position] = point;
} /* RModel::setPoint */


void RModel::removePoint (uint position)
{
    this->removeEntityGroupIdReferences(this->getEntityGroupID(R_ENTITY_GROUP_POINT,position));

    std::vector<RPoint>::iterator iter;

    R_ERROR_ASSERT (position < this->points.size());

    iter = this->points.begin();
    std::advance (iter, position);

    this->points.erase (iter);
} /* RModel::removePoint */


/*********************************************************************
 * Line interface                                                    *
 *********************************************************************/


uint RModel::getNLines (void) const
{
    return uint(this->lines.size());
} /* RModel::getNLines */


void RModel::setNLines (uint nlines)
{
    this->lines.resize(nlines);
} /* RModel::setNLines */


const RLine * RModel::getLinePtr (uint position) const
{
    R_ERROR_ASSERT (position < this->lines.size());
    return &this->lines[position];
} /* RModel::getLinePtr */


RLine * RModel::getLinePtr (uint position)
{
    R_ERROR_ASSERT (position < this->lines.size());
    return &this->lines[position];
} /* RModel::getLinePtr */


const RLine & RModel::getLine (uint position) const
{
    R_ERROR_ASSERT (position < this->lines.size());
    return this->lines[position];
} /* RModel::getLine */


RLine & RModel::getLine (uint position)
{
    R_ERROR_ASSERT (position < this->lines.size());
    return this->lines[position];
} /* RModel::getLine */


void RModel::addLine (const RLine &line)
{
    this->lines.push_back (line);
} /* RModel::addLine */


void RModel::setLine (uint  position,
                       const RLine  &line)
{
    R_ERROR_ASSERT (position < this->lines.size());
    this->lines[position] = line;
} /* RModel::setLine */


void RModel::removeLine (uint position)
{
    this->removeEntityGroupIdReferences(this->getEntityGroupID(R_ENTITY_GROUP_LINE,position));

    std::vector<RLine>::iterator iter;

    R_ERROR_ASSERT (position < this->lines.size());

    iter = this->lines.begin();
    std::advance (iter, position);

    this->lines.erase (iter);
} /* RModel::removeLine */


/*********************************************************************
 * Surface interface                                                    *
 *********************************************************************/


uint RModel::getNSurfaces (void) const
{
    return uint(this->surfaces.size());
} /* RModel::getNSurfaces */


void RModel::setNSurfaces (uint nsurfaces)
{
    this->surfaces.resize(nsurfaces);
} /* RModel::setNSurfaces */


const RSurface * RModel::getSurfacePtr (uint position) const
{
    R_ERROR_ASSERT (position < this->surfaces.size());
    return &this->surfaces[position];
} /* RModel::getSurfacePtr */


RSurface * RModel::getSurfacePtr (uint position)
{
    R_ERROR_ASSERT (position < this->surfaces.size());
    return &this->surfaces[position];
} /* RModel::getSurfacePtr */


const RSurface & RModel::getSurface (uint position) const
{
    R_ERROR_ASSERT (position < this->surfaces.size());
    return this->surfaces[position];
} /* RModel::getSurface */


RSurface & RModel::getSurface (uint position)
{
    R_ERROR_ASSERT (position < this->surfaces.size());
    return this->surfaces[position];
} /* RModel::getSurface */


void RModel::addSurface (const RSurface &surface)
{
    this->surfaces.push_back (surface);
} /* RModel::add_surface */


void RModel::setSurface (uint    position,
                          const RSurface &surface)
{
    R_ERROR_ASSERT (position < this->surfaces.size());
    this->surfaces[position] = surface;
} /* RModel::addSurface */


void RModel::removeSurface (uint position)
{
    this->removeEntityGroupIdReferences(this->getEntityGroupID(R_ENTITY_GROUP_SURFACE,position));

    std::vector<RSurface>::iterator iter;

    R_ERROR_ASSERT (position < this->surfaces.size());

    iter = this->surfaces.begin();
    std::advance (iter, position);

    this->surfaces.erase (iter);
} /* RModel::removeSurface */


bool RModel::checkIfSurfaceIsClosed(uint surfaceID) const
{
    QList<uint> surfaceIDList;
    surfaceIDList.append(surfaceID);
    return this->checkIfSurfacesAreClosed(surfaceIDList);
} /* RModel::checkIfSurfaceIsClosed */


bool RModel::checkIfSurfacesAreClosed(const QList<uint> &surfaceIDs) const
{
    // Build book of elements, which are part of the surface.
    std::vector<bool> elementBook;
    elementBook.resize(this->getNElements(),false);

    for (int i=0;i<surfaceIDs.size();i++)
    {
        const RSurface &rSurface = this->getSurface(surfaceIDs[i]);
        for (uint j=0;j<rSurface.size();j++)
        {
            elementBook[rSurface.get(j)] = true;
        }
    }

    for (uint i=0;i<elementBook.size();i++)
    {
        if (elementBook[i])
        {
            const std::vector<uint> *neighborIDs = this->getNeighborIDs(i);
            if (!neighborIDs)
            {
                return false;
            }
            if (neighborIDs->size() < RElement::getNNeighbors(this->getElement(i).getType()))
            {
                return false;
            }
            for (uint j=0;j<neighborIDs->size();j++)
            {
                if (!elementBook[neighborIDs->at(j)])
                {
                    return false;
                }
            }
        }
    }

    return true;
} /* RModel::checkIfSurfacesAreClosed */


void RModel::syncSurfaceNormals(void)
{
    if (this->getNElements() != this->surfaceNeigs.size())
    {
        this->setSurfaceNeighbors(this->findSurfaceNeighbors());
    }

    uint nSwapped = 0;
    for (uint i=0;i<this->getNSurfaces();i++)
    {
        RBVector elementSurface(this->getNElements(),false);
        RBVector elementChecked(this->getNElements(),false);

        const RSurface &rSurface = this->getSurface(i);

        for (uint j=0;j<rSurface.size();j++)
        {
            elementSurface[rSurface.get(j)] = true;
        }

        for (uint j=0;j<rSurface.size();j++)
        {
            uint elementID = rSurface.get(j);
            if (!elementChecked[elementID])
            {
                elementChecked[elementID] = true;

                std::stack<uint> elementStack;
                elementStack.push(elementID);

                while (elementStack.size() > 0)
                {
                    elementID = elementStack.top();
                    for (uint k=0;k<this->surfaceNeigs[elementID].size();k++)
                    {
                        uint neighbourID = surfaceNeigs[elementID][k];
                        if (elementSurface[neighbourID] && !elementChecked[neighbourID])
                        {
                            // Check neighbor orientation.
                            if (!this->getElement(elementID).isNeighborNormalSync(this->getElement(neighbourID)))
                            {
                                this->getElement(neighbourID).swapNormal();
                                nSwapped++;
                            }
                            elementChecked[neighbourID] = true;
                            elementStack.push(neighbourID);
                            break;
                        }

                    }
                    if (elementID == elementStack.top())
                    {
                        elementStack.pop();
                    }
                }
            }
        }
    }
    RLogger::info("Number of swapped elements = %u\n",nSwapped);
} /* RModel::syncSurfaceNormals */


/*********************************************************************
 * Volume interface                                                  *
 *********************************************************************/


uint RModel::getNVolumes (void) const
{
    return uint(this->volumes.size());
} /* RModel::getNVolumes */


void RModel::setNVolumes (uint nvolumes)
{
    this->volumes.resize(nvolumes);
} /* RModel::setNVolumes */


const RVolume * RModel::getVolumePtr (uint position) const
{
    R_ERROR_ASSERT (position < this->volumes.size());
    return &this->volumes[position];
} /* RModel::getVolumePtr */


RVolume * RModel::getVolumePtr (uint position)
{
    R_ERROR_ASSERT (position < this->volumes.size());
    return &this->volumes[position];
} /* RModel::getVolumePtr */


const RVolume & RModel::getVolume (uint position) const
{
    R_ERROR_ASSERT (position < this->volumes.size());
    return this->volumes[position];
} /* RModel::getVolume */


RVolume & RModel::getVolume (uint position)
{
    R_ERROR_ASSERT (position < this->volumes.size());
    return this->volumes[position];
} /* RModel::getVolume */


void RModel::addVolume (const RVolume &volume)
{
    this->volumes.push_back (volume);
} /* RModel::addVolume */


void RModel::setVolume (uint   position,
                         const RVolume &volume)
{
    R_ERROR_ASSERT (position < this->volumes.size());
    this->volumes[position] = volume;
} /* RModel::setVolume */


void RModel::removeVolume (uint position)
{
    this->removeEntityGroupIdReferences(this->getEntityGroupID(R_ENTITY_GROUP_VOLUME,position));

    std::vector<RVolume>::iterator iter;

    R_ERROR_ASSERT (position < this->volumes.size());

    iter = this->volumes.begin();
    std::advance (iter, position);

    this->volumes.erase (iter);
} /* RModel::removeVolume */


/*********************************************************************
 * Vector field interface                                            *
 *********************************************************************/


uint RModel::getNVectorFields(void) const
{
    return uint(this->vectorFields.size());
} /* RModel::getNVectorFields */


void RModel::setNVectorFields(uint nVectorFields)
{
    this->vectorFields.resize(nVectorFields);
} /* RModel::setNVectorFields */


const RVectorField *RModel::getVectorFieldPtr(uint position) const
{
    return &this->vectorFields[position];
} /* RModel::getVectorFieldPtr */


RVectorField *RModel::getVectorFieldPtr(uint position)
{
    R_ERROR_ASSERT (position < this->vectorFields.size());

    return &this->vectorFields[position];
} /* RModel::getVectorFieldPtr */


const RVectorField &RModel::getVectorField(uint position) const
{
    R_ERROR_ASSERT (position < this->vectorFields.size());

    return this->vectorFields[position];
} /* RModel::getVectorField */


RVectorField &RModel::getVectorField(uint position)
{
    R_ERROR_ASSERT (position < this->vectorFields.size());

    return this->vectorFields[position];
} /* RModel::getVectorField */


void RModel::addVectorField(const RVectorField &vectorField)
{
    this->vectorFields.push_back(vectorField);
} /* RModel::addVectorField */


void RModel::setVectorField(uint position, const RVectorField &vectorField)
{
    R_ERROR_ASSERT (position < this->vectorFields.size());

    this->vectorFields[position] = vectorField;
} /* RModel::setVectorField */


void RModel::removeVectorField(uint position)
{
    std::vector<RVectorField>::iterator iter;

    R_ERROR_ASSERT (position < this->vectorFields.size());

    iter = this->vectorFields.begin();
    std::advance (iter, position);

    this->vectorFields.erase (iter);
} /* RModel::removeVectorField */


/*********************************************************************
 * Scalar field interface                                          *
 *********************************************************************/


uint RModel::getNScalarFields(void) const
{
    return uint(this->scalarFields.size());
} /* RModel::getNScalarFields */


void RModel::setNScalarFields(uint nScalarFields)
{
    this->scalarFields.resize(nScalarFields);
} /* RModel::setNScalarFields */


const RScalarField *RModel::getScalarFieldPtr(uint position) const
{
    return &this->scalarFields[position];
} /* RModel::getScalarFieldPtr */


RScalarField *RModel::getScalarFieldPtr(uint position)
{
    R_ERROR_ASSERT (position < this->scalarFields.size());

    return &this->scalarFields[position];
} /* RModel::getScalarFieldPtr */


const RScalarField &RModel::getScalarField(uint position) const
{
    R_ERROR_ASSERT (position < this->scalarFields.size());

    return this->scalarFields[position];
} /* RModel::getScalarField */


RScalarField &RModel::getScalarField(uint position)
{
    R_ERROR_ASSERT (position < this->scalarFields.size());

    return this->scalarFields[position];
} /* RModel::getScalarField */


void RModel::addScalarField(const RScalarField &scalarField)
{
    this->scalarFields.push_back(scalarField);
} /* RModel::addScalarField */


void RModel::setScalarField(uint position, const RScalarField &scalarField)
{
    R_ERROR_ASSERT (position < this->scalarFields.size());

    this->scalarFields[position] = scalarField;
} /* RModel::setScalarField */


void RModel::removeScalarField(uint position)
{
    std::vector<RScalarField>::iterator iter;

    R_ERROR_ASSERT (position < this->scalarFields.size());

    iter = this->scalarFields.begin();
    std::advance (iter, position);

    this->scalarFields.erase (iter);
} /* RModel::removeScalarField */


/*********************************************************************
 * Stream line interface                                             *
 *********************************************************************/


uint RModel::getNStreamLines(void) const
{
    return uint(this->streamLines.size());
} /* RModel::getNStreamLines */


void RModel::setNStreamLines(uint nStreamLines)
{
    this->streamLines.resize(nStreamLines);
} /* RModel::setNStreamLines */


const RStreamLine *RModel::getStreamLinePtr(uint position) const
{
    R_ERROR_ASSERT (position < this->streamLines.size());
    return &this->streamLines[position];
} /* RModel::getStreamLinePtr */


RStreamLine *RModel::getStreamLinePtr(uint position)
{
    R_ERROR_ASSERT (position < this->streamLines.size());
    return &this->streamLines[position];
} /* RModel::getStreamLinePtr */


const RStreamLine &RModel::getStreamLine(uint position) const
{
    R_ERROR_ASSERT (position < this->streamLines.size());
    return this->streamLines[position];
} /* RModel::getStreamLine */


RStreamLine &RModel::getStreamLine(uint position)
{
    R_ERROR_ASSERT (position < this->streamLines.size());
    return this->streamLines[position];
} /* RModel::getStreamLine */


void RModel::addStreamLine(const RStreamLine &streamLine)
{
    this->streamLines.push_back(streamLine);
} /* RModel::addStreamLine */


void RModel::setStreamLine(uint position, const RStreamLine &streamLine)
{
    R_ERROR_ASSERT (position < this->streamLines.size());
    this->streamLines[position] = streamLine;
} /* RModel::setStreamLine */


void RModel::removeStreamLine(uint position)
{
    std::vector<RStreamLine>::iterator iter;

    R_ERROR_ASSERT (position < this->streamLines.size());

    iter = this->streamLines.begin();
    std::advance (iter, position);

    this->streamLines.erase (iter);
} /* RModel::removeStreamLine */


/*********************************************************************
 * Cut interface                                                     *
 *********************************************************************/


uint RModel::getNCuts(void) const
{
    return uint(this->cuts.size());
} /* RModel::getNCuts */


void RModel::setNCuts(uint nCuts)
{
    this->cuts.resize(nCuts);
} /* RModel::setNCuts */


const RCut *RModel::getCutPtr(uint position) const
{
    R_ERROR_ASSERT (position < this->cuts.size());
    return &this->cuts[position];
} /* RModel::getCutPtr */


RCut *RModel::getCutPtr(uint position)
{
    R_ERROR_ASSERT (position < this->cuts.size());
    return &this->cuts[position];
} /* RModel::getCutPtr */


const RCut &RModel::getCut(uint position) const
{
    R_ERROR_ASSERT (position < this->cuts.size());
    return this->cuts[position];
} /* RModel::getCut */


RCut &RModel::getCut(uint position)
{
    R_ERROR_ASSERT (position < this->cuts.size());
    return this->cuts[position];
} /* RModel::getCut */


void RModel::addCut(const RCut &cut)
{
    this->cuts.push_back(cut);
} /* RModel::addCut */


void RModel::setCut(uint position, const RCut &cut)
{
    R_ERROR_ASSERT (position < this->cuts.size());

    this->cuts[position] = cut;
} /* RModel::setCut */


void RModel::removeCut(uint position)
{
    std::vector<RCut>::iterator iter;

    R_ERROR_ASSERT (position < this->cuts.size());

    iter = this->cuts.begin();
    std::advance (iter, position);

    this->cuts.erase (iter);
} /* RModel::removeCut */


/*********************************************************************
 * ISO interface                                                     *
 *********************************************************************/


uint RModel::getNIsos(void) const
{
    return uint(this->isos.size());
} /* RModel::getNIsos */


void RModel::setNIsos(uint nIsos)
{
    this->isos.resize(nIsos);
} /* RModel::setNIsos */


const RIso *RModel::getIsoPtr(uint position) const
{
    R_ERROR_ASSERT (position < this->isos.size());
    return &this->isos[position];
} /* RModel::getIsoPtr */


RIso *RModel::getIsoPtr(uint position)
{
    R_ERROR_ASSERT (position < this->isos.size());
    return &this->isos[position];
} /* RModel::getIsoPtr */


const RIso &RModel::getIso(uint position) const
{
    R_ERROR_ASSERT (position < this->isos.size());
    return this->isos[position];
} /* RModel::getIso */


RIso &RModel::getIso(uint position)
{
    R_ERROR_ASSERT (position < this->isos.size());
    return this->isos[position];
} /* RModel::getIso */


void RModel::addIso(const RIso &iso)
{
    this->isos.push_back(iso);
} /* RModel::addIso */


void RModel::setIso(uint position, const RIso &iso)
{
    R_ERROR_ASSERT (position < this->isos.size());
    this->isos[position] = iso;
} /* RModel::setIso */


void RModel::removeIso(uint position)
{
    std::vector<RIso>::iterator iter;

    R_ERROR_ASSERT (position < this->isos.size());

    iter = this->isos.begin();
    std::advance (iter, position);

    this->isos.erase (iter);
} /* RModel::removeIso */


const RVariable *RModel::findVariableByDisplayType(REntityGroupVariableDisplayTypeMask displayTypeMask, REntityGroupType entityType, uint entityID) const
{
    uint groupID = this->getEntityGroupID(entityType,entityID);
    if (groupID == RConstants::eod)
    {
        return 0;
    }

    const REntityGroup *pEntity = this->getEntityGroupPtr(groupID);
    if (!pEntity)
    {
        return 0;
    }

    RVariableType variableType = pEntity->getData().findVariableByDisplayType(displayTypeMask);

    uint variablePosition = this->findVariable(variableType);
    if (variablePosition == RConstants::eod)
    {
        return 0;
    }

    return &this->getVariable(variablePosition);
} /* RModel::findVariableByDisplayType */


std::vector<RRVector> RModel::getElementResultsValues(RVariableType variableType,
                                                      uint elementID) const
{
    uint variablePosition = this->findVariable(variableType);
    if (variablePosition == RConstants::eod)
    {
        return std::vector<RRVector>();
    }

    std::vector<RRVector> variableVectors;

    const RVariable &rVariable = this->getVariable(variablePosition);
    if (rVariable.getApplyType() == R_VARIABLE_APPLY_ELEMENT)
    {
        variableVectors.push_back(rVariable.getValueVector(elementID));
    }
    else if (rVariable.getApplyType() == R_VARIABLE_APPLY_NODE)
    {
        const RElement &rElement = this->getElement(elementID);
        for (uint i=0;i<rElement.size();i++)
        {
            variableVectors.push_back(rVariable.getValueVector(rElement.getNodeId(i)));
        }
    }

    return variableVectors;
} /* RModel::getElementResultsValues */


RRVector RModel::getNodeResultsValues(RVariableType variableType,
                                      uint nodeID) const
{
    uint variablePosition = this->findVariable(variableType);
    if (variablePosition == RConstants::eod)
    {
        return RRVector();
    }
    const RVariable &rVariable = this->getVariable(variablePosition);
    if (rVariable.getApplyType() != R_VARIABLE_APPLY_NODE)
    {
        return RRVector();
    }

    return rVariable.getValueVector(nodeID);
} /* RModel::getNodeResultsValues */


std::vector<RRVector> RModel::getInterpolatedElementResultsValues(RVariableType variableType,
                                                                  REntityGroupType entityGroupType,
                                                                  uint entityID,
                                                                  uint elementID) const
{
    uint variablePosition = this->findVariable(variableType);
    if (variablePosition == RConstants::eod)
    {
        return std::vector<RRVector>();
    }

    const RInterpolatedElement *pIElement = 0;
    switch (entityGroupType)
    {
        case R_ENTITY_GROUP_CUT:
        {
            pIElement = &this->getCut(entityID).at(elementID);
            break;
        }
        case R_ENTITY_GROUP_ISO:
        {
            pIElement = &this->getIso(entityID).at(elementID);
            break;
        }
        default:
        {
            return std::vector<RRVector>();
        }
    }
    if (!pIElement)
    {
        return std::vector<RRVector>();
    }
    if (pIElement->size() == 0)
    {
        return std::vector<RRVector>();
    }

    std::vector<RRVector> resultsValues;

    const RVariable &rVariable = this->getVariable(variablePosition);

    if (rVariable.getApplyType() == R_VARIABLE_APPLY_ELEMENT)
    {
        resultsValues.push_back(rVariable.getValueVector(pIElement->at(0).getElementID()));
    }
    else if (rVariable.getApplyType() == R_VARIABLE_APPLY_NODE)
    {
        for (uint i=0;i<pIElement->size();i++)
        {
            resultsValues.push_back(this->getInterpolatedNodeResultsValues(variableType,entityGroupType,entityID,elementID,i));
        }
    }

    return resultsValues;
} /* RModel::getInterpolatedElementResultsValues */


RRVector RModel::getInterpolatedNodeResultsValues(RVariableType variableType,
                                                  REntityGroupType entityGroupType,
                                                  uint entityID,
                                                  uint elementID,
                                                  uint nodeID) const
{
    uint variablePosition = this->findVariable(variableType);
    if (variablePosition == RConstants::eod)
    {
        return RRVector();
    }

    const RInterpolatedElement *pIElement = 0;
    switch (entityGroupType)
    {
        case R_ENTITY_GROUP_CUT:
        {
            pIElement = &this->getCut(entityID).at(elementID);
            break;
        }
        case R_ENTITY_GROUP_ISO:
        {
            pIElement = &this->getIso(entityID).at(elementID);
            break;
        }
        default:
        {
            return RRVector();
        }
    }
    if (!pIElement)
    {
        return RRVector();
    }

    const RInterpolatedNode &rINode = pIElement->at(nodeID);

    const RVariable &rVariable = this->getVariable(variablePosition);

    if (rVariable.getApplyType() == R_VARIABLE_APPLY_ELEMENT)
    {
        return rVariable.getValueVector(rINode.getElementID());
    }
    else if (rVariable.getApplyType() == R_VARIABLE_APPLY_NODE)
    {
        const RElement &rElement = this->getElement(rINode.getElementID());

        RRVector nodeValues;
        nodeValues.resize(rElement.size());

        RRVector resultsValues(rVariable.getNVectors());

        for (uint i=0;i<rVariable.getNVectors();i++)
        {
            for (uint j=0;j<rElement.size();j++)
            {
                nodeValues[j] = rVariable.getValue(i,rElement.getNodeId(j));
            }
            resultsValues[i] = rElement.interpolate(this->getNodes(),rINode,nodeValues);
        }
        return resultsValues;
    }

    return RRVector();
} /* RModel::getInterpolatedNodeResultsValues */


RRVector RModel::getInterpolatedResultsValues(RVariableType variableType, const RNode &rNode, REntityGroupTypeMask entityGroup) const
{
    uint variablePosition = this->findVariable(variableType);
    if (variablePosition == RConstants::eod)
    {
        return RRVector();
    }

    // Find element containing given position.
    uint elementPos = RConstants::eod;
    for (uint i=0;i<this->getNElements();i++)
    {
        const RElement &rElement = this->getElement(i);
        if (RElementGroup::getGroupType(rElement.getType()) & entityGroup)
        {
            if (rElement.isInside(this->getNodes(),rNode))
            {
                elementPos = i;
                break;
            }
        }
    }
    if (elementPos == RConstants::eod)
    {
        return RRVector();
    }

    const RVariable &rVariable = this->getVariable(variablePosition);

    if (rVariable.getApplyType() == R_VARIABLE_APPLY_ELEMENT)
    {
        return rVariable.getValueVector(elementPos);

    }
    else if (rVariable.getApplyType() == R_VARIABLE_APPLY_NODE)
    {
        const RElement &rElement = this->getElement(elementPos);
        RRVector nodeValues(rElement.size());

        RRVector resultsValues(rVariable.getNVectors());

        for (uint i=0;i<rVariable.getNVectors();i++)
        {
            for (uint j=0;j<rElement.size();j++)
            {
                nodeValues[j] = rVariable.getValue(i,rElement.getNodeId(j));
            }
            resultsValues[i] = rElement.interpolate(this->getNodes(),rNode,nodeValues);
        }
        return resultsValues;
    }

    return RRVector();
} /* RModel::getInterpolatedNodeResultsValues */


/*************************************************************
 * Geometry transformation                                   *
 *************************************************************/


void RModel::rotateGeometry(const QSet<uint> &nodeIDs, const RR3Vector &rotationVector, const RR3Vector &rotationCenter)
{
    RLogger::info("Rotate\n");
    RLogger::info("  Vector: %s\n",rotationVector.toString(true).toUtf8().constData());
    RLogger::info("  Center: %s\n",rotationCenter.toString(true).toUtf8().constData());

    RR3Vector t(-rotationCenter[0],-rotationCenter[1],-rotationCenter[2]);

    double rx = R_DEG_TO_RAD(rotationVector[0]);
    double ry = R_DEG_TO_RAD(rotationVector[1]);
    double rz = R_DEG_TO_RAD(rotationVector[2]);

    RRMatrix R = RRMatrix::generateRotationMatrix(rx,ry,rz);

    foreach (uint i, nodeIDs)
    {
        this->getNode(i).transform(R,t,true);
        this->getNode(i).translate(rotationCenter);
    }
} /* RModel::rotateGeometry */


void RModel::scaleGeometry(const QSet<uint> &nodeIDs, const RR3Vector &scaleVector, const RR3Vector &scaleCenter)
{
    RLogger::info("Scale\n");
    RLogger::info("  Vector: %s\n",scaleVector.toString(true).toUtf8().constData());
    RLogger::info("  Center: %s\n",scaleCenter.toString(true).toUtf8().constData());

    foreach (uint i, nodeIDs)
    {
        this->getNode(i).translate(scaleCenter.getOpposite());
        this->getNode(i).scale(scaleVector);
        this->getNode(i).translate(scaleCenter);
    }
} /* RModel::scaleGeometry */


void RModel::scaleGeometry(double scaleFactor)
{
    RLogger::info("Scale\n");
    RLogger::info("  Factor: %g\n",scaleFactor);

    for (uint i=0;i<this->nodes.size();i++)
    {
        this->nodes[i].scale(scaleFactor);
    }
} /* RModel::scaleGeometry */


void RModel::translateGeometry(const QSet<uint> &nodeIDs, const RR3Vector &translateVector)
{
    RLogger::info("Translate\n");
    RLogger::info("  Vector: %s\n",translateVector.toString(true).toUtf8().constData());
    foreach (uint i, nodeIDs)
    {
        this->getNode(i).translate(translateVector);
    }
} /* RModel::translateGeometry */


/*************************************************************
 * Other methods                                             *
 *************************************************************/


RModelProblemTypeMask RModel::checkMesh(bool printOutput) const
{
    RModelProblemTypeMask problemType = R_MODEL_PROBLEM_NONE;

    // Points
    for (uint i=0;i<this->getNPoints();i++)
    {
        const RPoint &rPoint = this->getPoint(i);
        for (uint j=0;j<rPoint.size();j++)
        {
            uint elementID = rPoint.get(j);
            RElementType elementType = this->getElement(elementID).getType();
            if (!R_ELEMENT_TYPE_IS_POINT(elementType))
            {
                problemType |= R_MODEL_PROBLEM_INVALID_ELEMENT_TYPE;
                if (printOutput)
                {
                    RLogger::warning("Point element #%u has an invalid type %d (%s)\n",elementID,int(elementType),RElement::getName(elementType).toUtf8().constData());
                }
            }
        }
    }

    // Lines
    for (uint i=0;i<this->getNLines();i++)
    {
        const RLine &rLine = this->getLine(i);
        for (uint j=0;j<rLine.size();j++)
        {
            uint elementID = rLine.get(j);
            RElementType elementType = this->getElement(elementID).getType();
            if (!R_ELEMENT_TYPE_IS_LINE(elementType))
            {
                problemType |= R_MODEL_PROBLEM_INVALID_ELEMENT_TYPE;
                if (printOutput)
                {
                    RLogger::warning("Line element #%u has an invalid type %d (%s)\n",elementID,int(elementType),RElement::getName(elementType).toUtf8().constData());
                }
            }
        }
    }

    // Surfaces
    for (uint i=0;i<this->getNSurfaces();i++)
    {
        const RSurface &rSurface = this->getSurface(i);
        for (uint j=0;j<rSurface.size();j++)
        {
            uint elementID = rSurface.get(j);
            RElementType elementType = this->getElement(elementID).getType();
            if (!R_ELEMENT_TYPE_IS_SURFACE(elementType))
            {
                problemType |= R_MODEL_PROBLEM_INVALID_ELEMENT_TYPE;
                if (printOutput)
                {
                    RLogger::warning("Surface element #%u has an invalid type %d (%s)\n",elementID,int(elementType),RElement::getName(elementType).toUtf8().constData());
                }
            }
        }
    }

    // Volumes
    for (uint i=0;i<this->getNVolumes();i++)
    {
        const RVolume &rVolume = this->getVolume(i);
        for (uint j=0;j<rVolume.size();j++)
        {
            uint elementID = rVolume.get(j);
            RElementType elementType = this->getElement(elementID).getType();
            if (!R_ELEMENT_TYPE_IS_VOLUME(elementType))
            {
                problemType |= R_MODEL_PROBLEM_INVALID_ELEMENT_TYPE;
                if (printOutput)
                {
                    RLogger::warning("Volume element #%u has an invalid type %d (%s)\n",elementID,int(elementType),RElement::getName(elementType).toUtf8().constData());
                }
            }
        }
    }

    // Check element neighbors.
#pragma omp parallel for default(shared)
    for (int64_t i=0;i<int64_t(this->getNElements());i++)
    {
        const std::vector<uint> *pNeighbors = this->getNeighborIDs(i);
        if (!pNeighbors)
        {
            continue;
        }
        for (uint j=0;j<pNeighbors->size();j++)
        {
            if (RElementGroup::getGroupType(this->getElement(i).getType())
                ==
                RElementGroup::getGroupType(this->getElement(pNeighbors->at(j)).getType()))
            {
                // TODO: Cross check for element neighbors.
            }
            else
            {
                problemType |= R_MODEL_PROBLEM_INVALID_NEIGHBORS;
                if (printOutput)
                {
                    RLogger::warning("Two neighboring elements %u and %u have incompatible types %d (%s) / %d (%s)\n",
                                     i,
                                     pNeighbors->at(j),
                                     int(this->getElement(i).getType()),
                                     RElement::getName(this->getElement(i).getType()).toUtf8().constData(),
                                     int(this->getElement(pNeighbors->at(j)).getType()),
                                     RElement::getName(this->getElement(pNeighbors->at(j)).getType()).toUtf8().constData());
                }
            }
        }
    }

    return problemType;
} /* RModel::checkMesh */


QVector<bool> RModel::findEdgeNodes(void) const
{
    RLogger::info("Finding edge nodes\n");
    RLogger::indent();
    RProgressInitialize("Finding edge nodes");

    QVector<bool> edgeNodes;

    QVector<uint> nodeCount;
    QVector<uint> nodeGroup;

    edgeNodes.resize(this->getNNodes());
    edgeNodes.fill(true);
    nodeCount.resize(this->getNNodes());
    nodeCount.fill(0);
    nodeGroup.resize(this->getNNodes());
    nodeGroup.fill(0);

    uint nodeGroupCounter = 1;

    uint nNodeGroupCounter = this->getNPoints()
                                   + this->getNLines()
                                   + this->getNSurfaces()
                                   + this->getNVolumes();

    for (uint i=0;i<this->getNPoints();i++)
    {
        RProgressPrint(nodeGroupCounter,nNodeGroupCounter);
        const RPoint &point = this->getPoint(i);
        for (uint j=0;j<point.size();j++)
        {
            const RElement &element = this->getElement(point.get(j));
            for (uint k=0;k<element.size();k++)
            {
                uint nodeID = element.getNodeId(k);
                if (nodeGroup[nodeID] != nodeGroupCounter)
                {
                    nodeGroup[nodeID] = nodeGroupCounter;
                    nodeCount[nodeID]++;
                }
            }
        }
        nodeGroupCounter++;
    }

    for (uint i=0;i<this->getNLines();i++)
    {
        RProgressPrint(nodeGroupCounter,nNodeGroupCounter);
        const RLine &line = this->getLine(i);
        for (uint j=0;j<line.size();j++)
        {
            const RElement &element = this->getElement(line.get(j));
            for (uint k=0;k<element.size();k++)
            {
                uint nodeID = element.getNodeId(k);
                if (nodeGroup[nodeID] != nodeGroupCounter)
                {
                    nodeGroup[nodeID] = nodeGroupCounter;
                    nodeCount[nodeID]++;
                }
            }
        }
        nodeGroupCounter++;
    }

    for (uint i=0;i<this->getNSurfaces();i++)
    {
        RProgressPrint(nodeGroupCounter,nNodeGroupCounter);
        const RSurface &surface = this->getSurface(i);
        for (uint j=0;j<surface.size();j++)
        {
            const RElement &element = this->getElement(surface.get(j));
            for (uint k=0;k<element.size();k++)
            {
                uint nodeID = element.getNodeId(k);
                if (nodeGroup[nodeID] != nodeGroupCounter)
                {
                    nodeGroup[nodeID] = nodeGroupCounter;
                    nodeCount[nodeID]++;
                }
            }
        }
        nodeGroupCounter++;
    }

    for (uint i=0;i<this->getNVolumes();i++)
    {
        RProgressPrint(nodeGroupCounter,nNodeGroupCounter);
        const RVolume &volume = this->getVolume(i);
        for (uint j=0;j<volume.size();j++)
        {
            const RElement &element = this->getElement(volume.get(j));
            for (uint k=0;k<element.size();k++)
            {
                uint nodeID = element.getNodeId(k);
                if (nodeGroup[nodeID] != nodeGroupCounter)
                {
                    nodeGroup[nodeID] = nodeGroupCounter;
                    nodeCount[nodeID]++;
                }
            }
        }
        nodeGroupCounter++;
    }

    for (uint i=0;i<this->getNNodes();i++)
    {
        edgeNodes[i] = (nodeCount[i] > 1);
    }

    // Add to edge nodes element nodes which neighbor count is less than expected.
    if (this->surfaceNeigs.size() == this->getNElements())
    {
        for (uint i=0;i<this->getNSurfaces();i++)
        {
            const RSurface &rSurface = this->getSurface(i);
            for (uint j=0;j<rSurface.size();j++)
            {
                uint elementID = rSurface.get(j);
                const RElement &rElement = this->getElement(elementID);
                if (rElement.getNNeighbors(rElement.getType()) != this->surfaceNeigs[elementID].size())
                {
                    for (uint k=0;k<rElement.size();k++)
                    {
                        edgeNodes[rElement.getNodeId(k)] = true;
                    }
                }
            }
        }
    }
    if (this->volumeNeigs.size() == this->getNElements())
    {
        for (uint i=0;i<this->getNVolumes();i++)
        {
            const RVolume &rVolume = this->getVolume(i);
            for (uint j=0;j<rVolume.size();j++)
            {
                uint elementID = rVolume.get(j);
                const RElement &rElement = this->getElement(elementID);
                uint nEdges = rElement.getNEdgeElements();
                RBVector edgeBook(nEdges,false);
                for (uint k=0;k<this->volumeNeigs[elementID].size();k++)
                {
                    uint edgePosition = rElement.findEdgePositionForNeighborElement(this->getElement(this->volumeNeigs[elementID][k]));
                    if (edgePosition != RConstants::eod)
                    {
                        edgeBook[edgePosition] = true;
                    }
                }
                for (uint k=0;k<edgeBook.size();k++)
                {
                    if (edgeBook[k])
                    {
                        continue;
                    }
                    for (uint l=0;l<rElement.size();l++)
                    {
                        if (rElement.nodeIsOnEdge(l,k))
                        {
                            edgeNodes[rElement.getNodeId(l)] = true;
                        }
                    }
                }
            }
        }
    }

    RProgressFinalize();
    RLogger::unindent();

    return edgeNodes;
} /* RModel::findEdgeNodes */


struct LineElementItem
{
    uint id;
    uint n1;
    uint n2;
};


QList<uint> RModel::sortLineElements(const QList<RElement> &edges, uint firstID) const
{
    if (edges.size() == 0)
    {
        return QList<uint>();
    }

    QList<LineElementItem> items;

    QList<RElement> sortedEdges(edges);
    for (int i=0;i<sortedEdges.size();i++)
    {
        uint n1 = sortedEdges[i].getNodeId(0);
        uint n2 = sortedEdges[i].getNodeId(1);
        LineElementItem item;
        item.id = i;
        item.n1 = RConstants::eod;
        item.n2 = RConstants::eod;
        for (int j=0;j<sortedEdges.size();j++)
        {
            if (i == j)
            {
                continue;
            }
            if (n1 == sortedEdges[j].getNodeId(0) || n2 == sortedEdges[j].getNodeId(1))
            {
                sortedEdges[j].swapNodeIds(0,1);
            }
            if (n1 == sortedEdges[j].getNodeId(1))
            {
                item.n1 = j;
            }
            if (n2 == sortedEdges[j].getNodeId(0))
            {
                item.n2 = j;
            }
            if (item.n1 != RConstants::eod && item.n2 != RConstants::eod)
            {
                break;
            }
        }
        items.append(item);
    }

    // Find first node.
    uint firstItemID = 0;
    if (firstID != RConstants::eod)
    {
        firstItemID = firstID;
    }
    else
    {
        for (int i=0;i<items.size();i++)
        {
            if (items[i].n1 == RConstants::eod)
            {
                firstItemID = i;
                break;
            }
        }
    }

    QList<uint> elementIDs;

    uint n = firstItemID;

    while (n != RConstants::eod)
    {
        elementIDs.append(items[n].id);
        uint next = items[n].n2;
        items[n].n2 = RConstants::eod;
        n = next;
    }
    if (elementIDs.last() == elementIDs.first())
    {
        elementIDs.erase(elementIDs.end()-1);
    }

    return elementIDs;
} /* RModel::sortLineElements */


QList<uint> RModel::findNodeEdgeRing(uint nodeID) const
{
    std::vector<uint> elementPositions = this->findElementPositionsByNodeId(nodeID);

    QList<RElement> edges;
    for (std::vector<uint>::reverse_iterator rIterEps = elementPositions.rbegin(); rIterEps != elementPositions.rend(); ++rIterEps)
    {
        std::vector<RElement> edgeElements = this->getElement(*rIterEps).generateEdgeElements();
        for (uint i=0;i<edgeElements.size();i++)
        {
            if (edgeElements[i].hasNodeId(nodeID) || !R_ELEMENT_TYPE_IS_LINE(edgeElements[i].getType()))
            {
                continue;
            }
            edges.append(edgeElements[i]);
        }
    }

    QList<uint> elementIDs = this->sortLineElements(edges);

    QList<uint> nodeIDs;
    nodeIDs.reserve(elementIDs.size());

    for (int i=0;i<elementIDs.size();i++)
    {
        nodeIDs.append(edges[elementIDs[i]].getNodeId(0));
    }

    return nodeIDs;
} /* RModel::findNodeEdgeRing */


void RModel::createCut(RCut &rCut) const
{
    std::vector<uint> elementIDs;

    const std::vector<uint> &elementGroupIDs = rCut.getElementGroupIDs();
    for (uint i=0;i<elementGroupIDs.size();i++)
    {
        const RElementGroup *pGrp = this->getElementGroupPtr(elementGroupIDs[i]);
        if (pGrp)
        {
            for (uint j=0;j<pGrp->size();j++)
            {
                elementIDs.push_back(pGrp->get(j));
            }
        }
    }

    rCut.clear();

#pragma omp parallel for default(shared)
    for (int64_t i=0;i<int64_t(elementIDs.size());i++)
    {
        const RElement &element = this->getElement(elementIDs[i]);

        RInterpolatedElement iElement = element.createInterpolatedElement(rCut.getPlane(),this->getNodes(),elementIDs[i]);
        if (iElement.size() > 0)
        {
#pragma omp critical
            {
                rCut.push_back(iElement);
            }
        }
    }
} /* RModel::createCut */


void RModel::createIso(RIso &rIso) const
{
    std::vector<uint> elementIDs;

    const std::vector<uint> &elementGroupIDs = rIso.getElementGroupIDs();
    for (uint i=0;i<elementGroupIDs.size();i++)
    {
        const RElementGroup *pGrp = this->getElementGroupPtr(elementGroupIDs[i]);
        if (pGrp)
        {
            for (uint j=0;j<pGrp->size();j++)
            {
                elementIDs.push_back(pGrp->get(j));
            }
        }
    }

    rIso.clear();

    uint variablePosition = this->findVariable(rIso.getVariableType());
    if (variablePosition == RConstants::eod)
    {
        return;
    }

    const RVariable &rVariable = this->getVariable(variablePosition);

#pragma omp parallel for default(shared)
    for (int64_t i=0;i<int64_t(elementIDs.size());i++)
    {
        const RElement &rElement = this->getElement(elementIDs[i]);
        std::vector<double> nodeValues;
        nodeValues.resize(rElement.size(),0.0);
        for (uint j=0;j<rElement.size();j++)
        {
            if (rVariable.getApplyType() == R_VARIABLE_APPLY_NODE)
            {
                nodeValues[j] = rVariable.getValue(rElement.getNodeId(j));
            }
            else if (rVariable.getApplyType() == R_VARIABLE_APPLY_ELEMENT)
            {
                nodeValues[j] = rVariable.getValue(elementIDs[i]);
            }
        }

        RInterpolatedElement iElement = rElement.createInterpolatedElement(rIso.getVariableValue(),nodeValues,this->getNodes(),elementIDs[i]);
        if (iElement.size() > 0)
        {
#pragma omp critical
            {
                rIso.push_back(iElement);
            }
        }
    }
} /* RModel::createIso */


void RModel::createStreamLine(RStreamLine &rStreamLine) const
{
    rStreamLine.clear();

    uint variablePosition = this->findVariable(rStreamLine.getVariableType());
    if (variablePosition == RConstants::eod)
    {
        return;
    }
    const RVariable &rVariable = this->getVariable(variablePosition);

    RNode startNode(rStreamLine.getPosition());

    // Find starting element.
    uint pointElementID = RConstants::eod;
    uint lineElementID = RConstants::eod;
    uint surfaceElementID = RConstants::eod;
    uint volumeElementID = RConstants::eod;

#pragma omp parallel for default(shared)
    for (int64_t i=0;i<int64_t(this->getNElements());i++)
    {
        const RElement &rElement = this->getElement(i);

        if (R_ELEMENT_TYPE_IS_POINT(rElement.getType()) && pointElementID == RConstants::eod)
        {
            if (rElement.isInside(this->getNodes(),startNode))
            {
#pragma omp critical
                {
                    pointElementID = i;
                }
            }
        }
        if (R_ELEMENT_TYPE_IS_LINE(rElement.getType()) && lineElementID == RConstants::eod)
        {
            if (rElement.isInside(this->getNodes(),startNode))
            {
#pragma omp critical
                {
                    lineElementID = i;
                }
            }
        }
        if (R_ELEMENT_TYPE_IS_SURFACE(rElement.getType()) && surfaceElementID == RConstants::eod)
        {
            if (rElement.isInside(this->getNodes(),startNode))
            {
#pragma omp critical
                {
                    surfaceElementID = i;
                }
            }
        }
        if (R_ELEMENT_TYPE_IS_VOLUME(rElement.getType()) && volumeElementID == RConstants::eod)
        {
            if (rElement.isInside(this->getNodes(),startNode))
            {
#pragma omp critical
                {
                    volumeElementID = i;
                }
            }
        }
    }

    uint elementID = RConstants::eod;

    elementID = (pointElementID != RConstants::eod) ? pointElementID : elementID;
    elementID = (lineElementID != RConstants::eod) ? lineElementID : elementID;
    elementID = (surfaceElementID != RConstants::eod) ? surfaceElementID : elementID;
    elementID = (volumeElementID != RConstants::eod) ? volumeElementID : elementID;

    if (elementID == RConstants::eod)
    {
        RLogger::warning("Could not find start of the stream line \'%s\'\n",this->getName().toUtf8().constData());
        return;
    }

    RR3Vector vectorStart(startNode.toVector());
    RR3Vector vectorStartNext;

    // Find variable vector
    RR3Vector oldVariableVector(0.0,0.0,0.0);
    bool firstTime = true;

    while (elementID != RConstants::eod)
    {
        const RElement &rElement = this->getElement(elementID);

        // Find variable vector
        RR3Vector variableVector(0.0,0.0,0.0);
        if (rVariable.getApplyType() == R_VARIABLE_APPLY_ELEMENT)
        {
            if (rVariable.getNVectors() > 0)
            {
                variableVector[0] = rVariable.getValue(0,elementID);
            }
            if (rVariable.getNVectors() > 1)
            {
                variableVector[1] = rVariable.getValue(1,elementID);
            }
            if (rVariable.getNVectors() > 2)
            {
                variableVector[2] = rVariable.getValue(2,elementID);
            }
        }
        else if (rVariable.getApplyType() == R_VARIABLE_APPLY_NODE)
        {
            std::vector<double> nodeValues;
            nodeValues.resize(rElement.size(),0);

            for (uint i=0;i<rVariable.getNVectors() && i<3;i++)
            {
                for (uint j=0;j<rElement.size();j++)
                {
                    nodeValues[j] = rVariable.getValue(i,rElement.getNodeId(j));
                }
                variableVector[i] = rElement.interpolate(this->getNodes(),RNode(vectorStart),nodeValues);
            }
        }
        variableVector.normalize();

        // Find intersected element side/face
        uint intersectedSide = rElement.findIntersectedSide(this->getNodes(),
                                                            vectorStart,
                                                            variableVector,
                                                            vectorStartNext);

        if (intersectedSide == RConstants::eod)
        {
            if (!firstTime)
            {
                // It could be that the vector is pointing back into the current element.
                // Therefore this vector is replaced with the previous one to push it forward.
                variableVector = oldVariableVector;
                intersectedSide = rElement.findIntersectedSide(this->getNodes(),
                                                               vectorStart,
                                                               variableVector,
                                                               vectorStartNext);
            }
            if (intersectedSide == RConstants::eod)
            {
                break;
            }
        }

        RInterpolatedElement iElement;
        iElement.push_back(RInterpolatedNode(elementID,vectorStart));
        iElement.push_back(RInterpolatedNode(elementID,vectorStartNext));
        if (iElement.size() > 0)
        {
            rStreamLine.push_back(iElement);
        }

        elementID = this->getNeighbor(elementID,intersectedSide);

        vectorStart = vectorStartNext;
        oldVariableVector = variableVector;

        firstTime = false;
    }
} /* RModel::createStreamLine */


void RModel::createDependentEntities(void)
{
    for (uint i=0;i<this->getNCuts();i++)
    {
        this->createCut(this->getCut(i));
    }
    for (uint i=0;i<this->getNIsos();i++)
    {
        this->createIso(this->getIso(i));
    }
    for (uint i=0;i<this->getNStreamLines();i++)
    {
        this->createStreamLine(this->getStreamLine(i));
    }
} /* RModel::createDependentEntities */


uint RModel::getNeighbor(uint elementID, uint neighborPosition) const
{
    const std::vector<uint> *pNeighbors = this->getNeighborIDs(elementID);

    if (!pNeighbors)
    {
        return RConstants::eod;
    }

    for (uint i=0;i<pNeighbors->size();i++)
    {
        uint neighborID = pNeighbors->at(i);

        if (this->getElement(elementID).findEdgePositionForNeighborElement(this->getElement(neighborID)) == neighborPosition)
        {
            return neighborID;
        }
    }

    return RConstants::eod;
} /* RModel::getNeighbor */


const std::vector<uint> *RModel::getNeighborIDs(uint elementID) const
{
    switch (this->getElement(elementID).getType())
    {
        case R_ELEMENT_TRI1:
        case R_ELEMENT_TRI2:
        case R_ELEMENT_QUAD1:
        case R_ELEMENT_QUAD2:
        {
            if (this->surfaceNeigs.size() != this->getNElements())
            {
                return 0;
            }
            return &this->surfaceNeigs[elementID];
        }
        case R_ELEMENT_TETRA1:
        {
            if (this->volumeNeigs.size() != this->getNElements())
            {
                return 0;
            }
            return &this->volumeNeigs[elementID];
        }
        default:
        {
            return 0;
        }
    }
    return 0;
} /* RModel::getNeighborIDs */


void RModel::setSurfaceNeighbors(const std::vector<RUVector> &surfaceNeigs)
{
    R_ERROR_ASSERT (surfaceNeigs.size() == this->getNElements());
    this->surfaceNeigs = surfaceNeigs;
} /* RModel::setSurfaceNeighbors */


void RModel::setVolumeNeighbors(const std::vector<RUVector> &volumeNeigs)
{
    R_ERROR_ASSERT (volumeNeigs.size() == this->getNElements());
    this->volumeNeigs = volumeNeigs;
} /* RModel::setVolumeNeighbors */


void RModel::clearSurfaceNeighbors(void)
{
    this->surfaceNeigs.clear();
} /* RModel::clearSurfaceNeighbors */


void RModel::clearVolumeNeighbors(void)
{
    this->volumeNeigs.clear();
} /* RModel::clearVolumeNeighbors */


uint RModel::fixSliverElements(double edgeRatio)
{
    RLogger::info("Fixing sliver elements\n");
    RLogger::indent();
    RLogger::info("Edge (aspect) ratio limit = %g\n",edgeRatio);

    uint nAffected = 0;

    for (uint i=0;i<this->getNElements();i++)
    {
        const RElement &rElement = this->getElement(i);
        double lMax = 0.0;
        double lMin = 0.0;
        bool firstTime = true;
        uint n1 = RConstants::eod;
        uint n2 = RConstants::eod;
        for (uint j=0;j<rElement.size();j++)
        {
            for (uint k=j+1;k<rElement.size();k++)
            {
                double distance = this->nodes[rElement.getNodeId(j)].getDistance(this->nodes[rElement.getNodeId(k)]);
                if (firstTime)
                {
                    lMax = lMin = distance;
                    n1 = rElement.getNodeId(j);
                    n2 = rElement.getNodeId(k);
                    firstTime = false;
                }
                else
                {
                    lMax = std::max(lMax,distance);
                    if (lMin > distance)
                    {
                        lMin = distance;
                        n1 = rElement.getNodeId(j);
                        n2 = rElement.getNodeId(k);
                    }
                }
            }
        }

        if (n1 == RConstants::eod || n2 == RConstants::eod || n1 == n2)
        {
            continue;
        }

        if (lMin <= RConstants::eps || (lMax/lMin) >= edgeRatio)
        {
            try
            {
                this->mergeNodes(n1,n2,true,true);
            }
            catch (const RError &rError)
            {
                RLogger::unindent();
                throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to merge nodes %u and %u. %s",n1,n2,rError.getMessage().toUtf8().constData());
            }

            nAffected++;
        }
    }

    RPoint pointGroup;
    RLine lineGroup;
    RSurface surfaceGroup;

    pointGroup.setName("Downgraded");
    lineGroup.setName("Downgraded");
    surfaceGroup.setName("Downgraded");

    // Move elements to appropriate groups.
    for (uint i=0;i<this->getNLines();i++)
    {
        RLine &rLine = this->getLine(i);
        for (uint j=0;j<rLine.size();j++)
        {
            uint elementID = rLine.get(j);
            if (R_ELEMENT_TYPE_IS_POINT(this->getElement(elementID).getType()))
            {
                rLine.remove(j);
                pointGroup.add(elementID);
                j--;
            }
        }
    }
    for (uint i=0;i<this->getNSurfaces();i++)
    {
        RSurface &rSurface = this->getSurface(i);
        for (uint j=0;j<rSurface.size();j++)
        {
            uint elementID = rSurface.get(j);
            if (R_ELEMENT_TYPE_IS_POINT(this->getElement(elementID).getType()))
            {
                rSurface.remove(j);
                pointGroup.add(elementID);
                j--;
            }
            else if (R_ELEMENT_TYPE_IS_LINE(this->getElement(elementID).getType()))
            {
                rSurface.remove(j);
                lineGroup.add(elementID);
                j--;
            }
        }
    }
    for (uint i=0;i<this->getNVolumes();i++)
    {
        RVolume &rVolume = this->getVolume(i);
        for (uint j=0;j<rVolume.size();j++)
        {
            uint elementID = rVolume.get(j);
            if (R_ELEMENT_TYPE_IS_POINT(this->getElement(elementID).getType()))
            {
                rVolume.remove(j);
                pointGroup.add(elementID);
                j--;
            }
            else if (R_ELEMENT_TYPE_IS_LINE(this->getElement(elementID).getType()))
            {
                rVolume.remove(j);
                lineGroup.add(elementID);
                j--;
            }
            else if (R_ELEMENT_TYPE_IS_SURFACE(this->getElement(elementID).getType()))
            {
                rVolume.remove(j);
                surfaceGroup.add(elementID);
                j--;
            }
        }
    }

    if (pointGroup.size() > 0)
    {
        this->addPoint(pointGroup);
    }
    if (lineGroup.size() > 0)
    {
        this->addLine(lineGroup);
    }
    if (surfaceGroup.size() > 0)
    {
        this->addSurface(surfaceGroup);
    }

    for (uint i=0;i<this->getNPoints();i++)
    {
        if (this->getPoint(i).size() == 0)
        {
            this->removePoint(i);
            i--;
        }
    }
    for (uint i=0;i<this->getNLines();i++)
    {
        if (this->getLine(i).size() == 0)
        {
            this->removeLine(i);
            i--;
        }
    }
    for (uint i=0;i<this->getNSurfaces();i++)
    {
        if (this->getSurface(i).size() == 0)
        {
            this->removeSurface(i);
            i--;
        }
    }
    for (uint i=0;i<this->getNVolumes();i++)
    {
        if (this->getVolume(i).size() == 0)
        {
            this->removeVolume(i);
            i--;
        }
    }

    RLogger::unindent();
    return nAffected;
} /* RModel::fixSliverElements */


QList<uint> RModel::findIntersectedElements(void) const
{
    RLogger::info("Finding intersected elements\n");
    RLogger::indent();

    QVector<bool> intElements;
    intElements.resize(this->getNElements());
    intElements.fill(false);

    RLimitBox limitBox1;

    RProgressInitialize("Finding intersected elements");
    for (uint i=0;i<this->getNElements();i++)
    {
        RProgressPrint(i,this->getNElements());
        this->getElement(i).findLimitBox(this->getNodes(),limitBox1);

#pragma omp parallel for default(shared)
        for (int64_t j=int64_t(i)+1;j<int64_t(this->getNElements());j++)
        {
            RLimitBox limitBox2;
            bool bothIntersected = false;
#pragma omp critical
            bothIntersected = (intElements[i] && intElements[j]);
            if (bothIntersected)
            {
                continue;
            }
            this->getElement(j).findLimitBox(this->getNodes(),limitBox2);
            if (!RLimitBox::areIntersecting(limitBox1,limitBox2))
            {
                continue;
            }

            std::set<RR3Vector> x;
            if (RElement::findIntersectionPoints(this->getElement(i),this->getElement(j),this->getNodes(),x))
            {
#pragma omp critical
                {
//                    RLogger::warning("Elements %u and %u are intersecting.\n",i,j);
                    intElements[i] = intElements[j] = true;
                }
            }
        }
    }
    RProgressFinalize("Done");

    uint nIntersected = 0;
    for (int i=0;i<intElements.size();i++)
    {
        if (intElements[i])
        {
            nIntersected++;
        }
    }

    QList<uint> elementIDs;
    elementIDs.reserve(nIntersected);

    for (int i=0;i<intElements.size();i++)
    {
        if (intElements[i])
        {
            elementIDs.append(i);
        }
    }

    RLogger::unindent();

    return elementIDs;
} /* RModel::findIntersectedElements */


uint RModel::breakIntersectedElements(uint nIterations)
{
    std::vector<uint> elementIDs;
    uint ne = this->getNElements();
    elementIDs.resize(ne);
    for (uint i=0;i<ne;i++)
    {
        elementIDs[i] = i;
    }
    return this->breakIntersectedElements(nIterations,elementIDs);
} /* RModel::breakIntersectedElements */


uint RModel::breakIntersectedElements(uint nIterations, const std::vector<uint> &elementIDs)
{
    uint oldNNodes = this->getNNodes();
    uint nIntersected = 0;
    uint iteration = 0;
    double tolerance = RConstants::findMachineDoubleEpsilon()*100;
//    double tolerance = 100.0*RConstants::eps;

    std::vector<uint> bElementIDs(elementIDs);

    while (iteration < nIterations)
    {
        bool intersectionFound = false;

        iteration++;
        RLogger::info("Iteration %u of %u\n",iteration,nIterations);
        RLogger::indent();

        std::vector< std::set<RR3Vector> > intersectionPoints;
        intersectionPoints.resize(bElementIDs.size());

        // Find intersection points.
        RLogger::info("Finding intersection points\n");
        RLogger::indent();

        RProgressPrintToLog(false);
        RProgressInitialize("Finding intersection points");

        for (uint i=0;i<bElementIDs.size();i++)
        {
            RProgressPrint(i,uint(bElementIDs.size()));

            if (this->getElement(bElementIDs[i]).hasDuplicateNodes())
            {
                continue;
            }

            RLimitBox limitBox1;
            this->getElement(bElementIDs[i]).findLimitBox(this->getNodes(),limitBox1);

#pragma omp parallel for default(shared)
            for (int64_t j=int64_t(i)+1;j<int64_t(bElementIDs.size());j++)
            {
                if (this->getElement(bElementIDs[j]).hasDuplicateNodes())
                {
                    continue;
                }

                RLimitBox limitBox2;
                this->getElement(bElementIDs[j]).findLimitBox(this->getNodes(),limitBox2);
                if (!RLimitBox::areIntersecting(limitBox1,limitBox2))
                {
                    continue;
                }

                std::set<RR3Vector> x;
                if (RElement::findIntersectionPoints(this->getElement(bElementIDs[i]),this->getElement(bElementIDs[j]),this->getNodes(),x))
                {
#pragma omp critical
                    {
                        std::set<RR3Vector>::reverse_iterator it;
                        for (it=x.rbegin();it!=x.rend();++it)
                        {
                            // Insert only nodes which are not in the verticies.
                            bool nodeFound = false;
                            std::set<RR3Vector>::const_iterator cit;
                            for (cit=intersectionPoints[i].begin();cit!=intersectionPoints[i].end();++cit)
                            {
                                if (RR3Vector::findDistance(*it,*cit) < tolerance)
                                {
                                    nodeFound = true;
                                    break;
                                }
                            }
                            if (!nodeFound)
                            {
                                intersectionPoints[i].insert(*it);
                                intersectionFound = true;
                            }
                            nodeFound = false;
                            for (cit=intersectionPoints[j].begin();cit!=intersectionPoints[j].end();++cit)
                            {
                                if (RR3Vector::findDistance(*it,*cit) < tolerance)
                                {
                                    nodeFound = true;
                                    break;
                                }
                            }
                            if (!nodeFound)
                            {
                                intersectionPoints[j].insert(*it);
                                intersectionFound = true;
                            }
                        }
                    }
                }
            }
        }

        RProgressFinalize("Done");
        RLogger::unindent();

        if (!intersectionFound)
        {
            RLogger::info("No intersections were found.\n");
            RLogger::unindent();
            break;
        }
        else
        {
            uint nIntersectedFound = 0;
            for (uint i=0;i<intersectionPoints.size();i++)
            {
                if (intersectionPoints[i].size() > 0)
                {
                    nIntersectedFound++;
                }
            }
            RLogger::info("Number of intersected elements found = %d.\n", nIntersectedFound);
        }

        // Create element -> group book.
        RLogger::info("Creating element to group map\n");
        RLogger::indent();
        std::vector<uint> elementGroupBook;
        elementGroupBook.resize(this->getNElements(),RConstants::eod);

        for (uint i=0;i<this->getNPoints();i++)
        {
            for (uint j=0;j<this->getPoint(i).size();j++)
            {
                elementGroupBook[this->getPoint(i).get(j)] = i;
            }
        }
        for (uint i=0;i<this->getNLines();i++)
        {
            for (uint j=0;j<this->getLine(i).size();j++)
            {
                elementGroupBook[this->getLine(i).get(j)] = i;
            }
        }
        for (uint i=0;i<this->getNSurfaces();i++)
        {
            for (uint j=0;j<this->getSurface(i).size();j++)
            {
                elementGroupBook[this->getSurface(i).get(j)] = i;
            }
        }
        for (uint i=0;i<this->getNVolumes();i++)
        {
            for (uint j=0;j<this->getVolume(i).size();j++)
            {
                elementGroupBook[this->getVolume(i).get(j)] = i;
            }
        }
        RLogger::unindent();

        // Break intersected elements.
        RLogger::info("Breaking intersected elements\n");
        RLogger::indent();

        RProgressPrintToLog(false);
        RProgressInitialize("Breaking intersected elemets");
        for (uint i=0;i<intersectionPoints.size();i++)
        {
            RProgressPrint(i,uint(intersectionPoints.size()));
            if (intersectionPoints[i].size() > 0)
            {
                std::vector<uint> breakNodeIDs;

                std::set<RR3Vector>::const_iterator iter;
                for (iter=intersectionPoints[i].begin();iter!=intersectionPoints[i].end();++iter)
                {
                    this->addNode((*iter)[0],(*iter)[1],(*iter)[2]);
                    breakNodeIDs.push_back(this->getNNodes() - 1);
                }

                std::vector<RElement> newElements;

                this->getElement(bElementIDs[i]).breakWithNodes(this->getNodes(),breakNodeIDs,newElements);
                if (newElements.size() > 0)
                {
                    for (uint j=0;j<newElements.size();j++)
                    {
                        this->addElement(newElements[j],
                                         elementGroupBook[bElementIDs[i]] != RConstants::eod,
                                         elementGroupBook[bElementIDs[i]]);
                        bElementIDs.push_back(this->getNElements()-1);
                    }
                    nIntersected++;
                }
            }
        }

        RProgressFinalize("Done");
        RLogger::unindent();

        // Merge near/duplicate nodes.
        RLogger::info("Merging near/duplicate nodes\n");
        RLogger::indent();
        uint nMerged = 0;
        for (uint i=this->getNNodes()-1;i>=oldNNodes;i--)
        {
            uint nId = this->findNearNode(this->getNode(i),tolerance,false,i);
            if (nId != RConstants::eod)
            {
                this->mergeNodes(nId,i,false,false);
                nMerged ++;
            }
        }
        RLogger::info("Merged near/duplicate nodes = %u\n",nMerged);
        RLogger::unindent();

        // Remove duplicate elements / making them having duplicate nodes (degenerated).
        RLogger::info("Removing duplicate elements\n");
        RLogger::indent();

        for (uint i=0;i<bElementIDs.size();i++)
        {
            const RElement &rElement1 = this->getElement(bElementIDs[i]);
            if (rElement1.hasDuplicateNodes())
            {
                continue;
            }
            for (uint j=i+1;j<bElementIDs.size();j++)
            {
                RElement &rElement2 = this->getElement(bElementIDs[j]);
                if (rElement1 == rElement2)
                {
                    rElement2.setNodeId(1,rElement2.getNodeId(0));
                }
            }
        }

        RLogger::unindent();

        RLogger::unindent();
    }

    // Remove degenerated elements.
    RLogger::info("Removing degenerated elements\n");
    RLogger::indent();
    QList<uint> dElementIDs;
    for (uint i=0;i<this->getNElements();i++)
    {
        if (this->getElement(i).hasDuplicateNodes())
        {
            dElementIDs.push_back(i);
        }
    }
    this->removeElements(dElementIDs,false);
    RLogger::unindent();

    return nIntersected;
} /* RModel::breakIntersectedElements */


bool RModel::boolDifference(uint nIterations, QList<uint> surfaceEntityIDs, uint cuttingSurfaceEntityId)
{
    // First check if all surfaces form closed surface.
    for (int i=0;i<surfaceEntityIDs.size();i++)
    {
        if (!this->checkIfSurfaceIsClosed(surfaceEntityIDs[i]))
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Cut surface \'%s\' is not closed.\n",this->getSurface(surfaceEntityIDs[i]).getName().toUtf8().constData());
        }
    }
    if (!this->checkIfSurfaceIsClosed(cuttingSurfaceEntityId))
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Cutting surface \'%s\' is not closed.\n",this->getSurface(cuttingSurfaceEntityId).getName().toUtf8().constData());
    }

    try
    {
        std::vector<uint> allSurfaceEntityIDs;
        allSurfaceEntityIDs.push_back(cuttingSurfaceEntityId);
        for (int i=0;i<surfaceEntityIDs.size();i++)
        {
            allSurfaceEntityIDs.push_back(surfaceEntityIDs[i]);
        }
        uint nIntersected = this->breakIntersectedElements(nIterations,this->findElementIDs(R_ENTITY_GROUP_SURFACE,allSurfaceEntityIDs));
        if (nIntersected == 0)
        {
            RLogger::info("No intersections were found\n.");
        }
    }
    catch (const RError &error)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to break intersected elements. %s", error.getMessage().toUtf8().constData());
    }

    std::vector<bool> elementRemoveBook;
    elementRemoveBook.resize(this->getNElements(),true);

    const RSurface &rCuttingSurface = this->getSurface(cuttingSurfaceEntityId);

    for (int i=0;i<surfaceEntityIDs.size();i++)
    {
        const RSurface &rSurface = this->getSurface(surfaceEntityIDs[i]);

        std::vector<RR3Vector> elementCenters;
        std::vector<bool> insideBook;

        // Remove cut surface
        elementCenters.resize(rSurface.size());

        for (uint j=0;j<rSurface.size();j++)
        {
            this->getElement(rSurface.get(j)).findCenter(this->nodes,elementCenters[j][0],elementCenters[j][1],elementCenters[j][2]);
        }
        try
        {
            insideBook = rCuttingSurface.pointsInside(this->nodes,this->elements,elementCenters,false);
        }
        catch (const RError &error)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to check if element centers are inside surface. %s",
                         error.getMessage().toUtf8().constData());
        }

        for (uint j=0;j<rSurface.size();j++)
        {
            if (!insideBook[j])
            {
                elementRemoveBook[rSurface.get(j)] = false;
            }
        }

        // Remove cutting surface
        elementCenters.resize(rCuttingSurface.size());

        for (uint j=0;j<rCuttingSurface.size();j++)
        {
            this->getElement(rCuttingSurface.get(j)).findCenter(this->nodes,elementCenters[j][0],elementCenters[j][1],elementCenters[j][2]);
        }

        try
        {
            insideBook = rSurface.pointsInside(this->nodes,this->elements,elementCenters,true);
        }
        catch (const RError &error)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to check if element centers are inside surface. %s",
                         error.getMessage().toUtf8().constData());
        }

        for (uint j=0;j<rCuttingSurface.size();j++)
        {

            if (insideBook[j])
            {
                elementRemoveBook[rCuttingSurface.get(j)] = false;
            }
        }
    }

    QList<uint> elementsToRemove;
    for (uint i=0;i<elementRemoveBook.size();i++)
    {
        if (elementRemoveBook[i])
        {
            elementsToRemove.push_back(i);
        }
    }
//    elementsToRemove.reserve(elementIDsSet.size());

//    std::set<uint>::const_iterator iter;
//    for (iter=elementIDsSet.begin();iter!=elementIDsSet.end();++iter)
//    {
//        if (tmpElementIDsSet.find(*iter) == tmpElementIDsSet.end())
//        {
//            elementsToRemove.push_back(*iter);
//        }
//    }

    this->removeElements(elementsToRemove,false);

    return (elementsToRemove.size() > 0);
} /* RModel::boolDifference */


bool RModel::boolIntersection(uint nIterations, QList<uint> surfaceEntityIDs)
{
    try
    {
        uint nIntersected = this->breakIntersectedElements(nIterations,this->findElementIDs(R_ENTITY_GROUP_SURFACE,surfaceEntityIDs.toVector().toStdVector()));
        if (nIntersected == 0)
        {
            RLogger::info("No intersections were found\n.");
        }
    }
    catch (const RError &error)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to break intersected elements. %s", error.getMessage().toUtf8().constData());
    }

    std::set<uint> elementIDsSet;

    for (int i=0;i<surfaceEntityIDs.size();i++)
    {
        const RSurface &rSurface1 = this->getSurface(surfaceEntityIDs[i]);

        for (int j=0;j<surfaceEntityIDs.size();j++)
        {
            if (i == j)
            {
                continue;
            }
            const RSurface &rSurface2 = this->getSurface(surfaceEntityIDs[j]);

            std::vector<RR3Vector> elementCenters;
            elementCenters.resize(rSurface2.size());

            for (uint k=0;k<rSurface2.size();k++)
            {
                this->getElement(rSurface2.get(k)).findCenter(this->nodes,elementCenters[k][0],elementCenters[k][1],elementCenters[k][2]);
            }

            std::vector<bool> insideBook;
            try
            {
                insideBook = rSurface1.pointsInside(this->nodes,this->elements,elementCenters,true);
            }
            catch (const RError &error)
            {
                throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to check if element centers are inside surface. %s",
                             error.getMessage().toUtf8().constData());
            }

            for (uint k=0;k<rSurface2.size();k++)
            {
                if (!insideBook[k])
                {
                    elementIDsSet.insert(rSurface2.get(k));
                }
            }
        }
    }

    QList<uint> elementsToRemove;
    elementsToRemove.reserve(int(elementIDsSet.size()));

    std::set<uint>::const_iterator iter;
    for (iter=elementIDsSet.begin();iter!=elementIDsSet.end();++iter)
    {
        elementsToRemove.push_back(*iter);
    }

    this->removeElements(elementsToRemove,false);

    return (elementsToRemove.size() > 0);
} /* RModel::boolIntersection */


bool RModel::boolUnion(uint nIterations, QList<uint> surfaceEntityIDs)
{
    try
    {
        uint nIntersected = this->breakIntersectedElements(nIterations,this->findElementIDs(R_ENTITY_GROUP_SURFACE,surfaceEntityIDs.toVector().toStdVector()));
        if (nIntersected == 0)
        {
            RLogger::info("No intersections were found\n.");
        }
    }
    catch (const RError &error)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to break intersected elements. %s", error.getMessage().toUtf8().constData());
    }

    std::set<uint> elementIDsSet;

    for (int i=0;i<surfaceEntityIDs.size();i++)
    {
        const RSurface &rSurface1 = this->getSurface(surfaceEntityIDs[i]);

        for (int j=0;j<surfaceEntityIDs.size();j++)
        {
            if (i == j)
            {
                continue;
            }
            const RSurface &rSurface2 = this->getSurface(surfaceEntityIDs[j]);

            std::vector<RR3Vector> elementCenters;
            elementCenters.resize(rSurface2.size());

            for (uint k=0;k<rSurface2.size();k++)
            {
                this->getElement(rSurface2.get(k)).findCenter(this->nodes,elementCenters[k][0],elementCenters[k][1],elementCenters[k][2]);
            }

            std::vector<bool> insideBook;
            try
            {
                insideBook = rSurface1.pointsInside(this->nodes,this->elements,elementCenters,true);
            }
            catch (const RError &error)
            {
                throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to check if element centers are inside surface. %s",
                             error.getMessage().toUtf8().constData());
            }

            for (uint k=0;k<rSurface2.size();k++)
            {
                if (insideBook[k])
                {
                    elementIDsSet.insert(rSurface2.get(k));
                }
            }
        }
    }

    QList<uint> elementsToRemove;
    elementsToRemove.reserve(int(elementIDsSet.size()));

    std::set<uint>::const_iterator iter;
    for (iter=elementIDsSet.begin();iter!=elementIDsSet.end();++iter)
    {
        elementsToRemove.push_back(*iter);
    }

    this->removeElements(elementsToRemove,false);

    return (elementsToRemove.size() > 0);
} /* RModel::boolUnion */


uint RModel::coarsenSurfaceElements(const std::vector<uint> surfaceIDs, double edgeLength, double elementArea)
{
    std::vector<bool> ecBook;
    ecBook.resize(this->getNElements(),false);
    std::vector<bool> edBook;
    edBook.resize(this->getNElements(),false);

    for (uint i=0;i<surfaceIDs.size();i++)
    {
        const RSurface &rSurface = this->getSurface(surfaceIDs[i]);
        for (uint j=0;j<rSurface.size();j++)
        {
            ecBook[rSurface.get(j)] = true;
        }
    }

    bool elementDeleted = false;

    RProgressInitialize("Coarsening surface elements",true);
    uint nDeleted = 0;
    do
    {
        elementDeleted = false;
        for (uint i=0;i<this->getNElements();i++)
        {
            if (!ecBook[i])
            {
                continue;
            }
            if (edBook[i])
            {
                continue;
            }
            RProgressPrint(1.0);

            const RElement &rElement = this->getElement(i);
            if (!R_ELEMENT_TYPE_IS_SURFACE(rElement.getType()))
            {
                continue;
            }

            double length = 0.0;
            uint np1 = 0;
            uint np2 = 0;
            if (!rElement.findShortestEdge(this->nodes,length,np1,np2))
            {
                continue;
            }
            uint node1 = rElement.getNodeId(np1);
            uint node2 = rElement.getNodeId(np2);

            if (length < edgeLength)
            {
                if (node1 != node2)
                {
                    this->mergeNodes(node1,node2,true,false);
                }
                edBook[i] = true;
                elementDeleted = true;
                nDeleted++;
                continue;
            }

            double area = 0.0;
            if (!rElement.findArea(this->nodes,area))
            {
                continue;
            }

            if (area < elementArea)
            {
                if (node1 != node2)
                {
                    this->mergeNodes(node1,node2,true,false);
                }
                edBook[i] = true;
                elementDeleted = true;
                nDeleted++;
            }
        }
    } while (elementDeleted);

    //! Delete elements with duplicate nodes.
    for (uint i=0;i<this->getNElements();i++)
    {
        const RElement &rElement = this->getElement(i);
        if (rElement.hasDuplicateNodes())
        {
            edBook[i] = true;
            nDeleted++;
        }
    }

    QList<uint> elementIDs;
    elementIDs.reserve(nDeleted);

    for (uint i=0;i<edBook.size();i++)
    {
        if (edBook[i])
        {
            elementIDs.push_back(i);
        }
    }
    RProgressFinalize("Done");

    this->removeElements(elementIDs,false);

    this->purgeUnusedNodes();

    return nDeleted;
} /* RModel::coarsenSurfaceElements */


uint RModel::tetrahedralizeSurface(const std::vector<uint> surfaceIDs)
{
    // Remove all volume elements
    while (this->getNVolumes() > 0)
    {
        this->removeVolume(0);
    }
    this->purgeUnusedElements();

    // Create combined surface.
    RSurface surface;

    uint nElements = 0;
    for (uint i=0;i<surfaceIDs.size();i++)
    {
        nElements += this->getSurface(surfaceIDs[i]).size();
    }

    surface.resize(nElements);

    nElements = 0;
    for (uint i=0;i<surfaceIDs.size();i++)
    {
        const RSurface &rSurface = this->getSurface(surfaceIDs[i]);
        for (uint j=0;j<rSurface.size();j++)
        {
            surface.set(nElements++,rSurface.get(j));
        }
    }

    // Generate tetrahedrons.
    std::vector<RElement> volumeElements;
    try
    {
        volumeElements = surface.tetrahedralize(this->getNodes(),this->getElements());
    }
    catch (const RError &error)
    {
        RLogger::unindent();
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to tetrahedralize surface. %s", error.getMessage().toUtf8().constData());
    }

    // Add tetrahedrons to volume.
    for (uint i=0;i<volumeElements.size();i++)
    {
        this->addElement(volumeElements[i],true,0);
    }

    return uint(volumeElements.size());
} /* RModel::tetrahedralizeSurface */


void RModel::generatePatchSurface(const std::vector<RPatchInput> &patchInput, RPatchBook &book) const
{
    R_ERROR_ASSERT (patchInput.size() == this->getNSurfaces());

    book.clear();

    RDistanceVector<uint> distanceVector(this->getNElements(),RConstants::eod);
    RLogger::info("Patch surface generation\n");
    RLogger::indent();

    for (uint surfaceID=0;surfaceID<this->getNSurfaces();surfaceID++)
    {
        const RSurface &rSurface = this->getSurface(surfaceID);

        if (rSurface.size() == 0)
        {
            continue;
        }
        if (!patchInput[surfaceID].getEmitter() && !patchInput[surfaceID].getReceiver())
        {
            continue;
        }

        RLogger::info("Generating patch on surface \'%s\'\n", rSurface.getName().toUtf8().constData());
        RLogger::indent();
        RLogger::info("Max. patch size  = %g [%s]\n",patchInput[surfaceID].getPatchArea(),RVariable::getUnits(R_VARIABLE_PATCH_AREA).toUtf8().constData());
        RLogger::info("Max. patch size  = %u [%s]\n",patchInput[surfaceID].getPatchSize(),RVariable::getUnits(R_VARIABLE_PATCH_SIZE).toUtf8().constData());
        RLogger::info("Separation angle = %g [%s]\n",patchInput[surfaceID].getSeparationAngle(),RVariable::getUnits(R_VARIABLE_SEPARATION_ANGLE).toUtf8().constData());
        RLogger::unindent();

        // Separation angle in radians.
        double separationAngleRad = patchInput[surfaceID].getSeparationAngle() * RConstants::pi / 180.0;

        RProgressPrintToLog(false);
        RProgressInitialize("Patch generation:");

        for (uint i=0;i<rSurface.size();i++)
        {
            uint elementID = rSurface.get(i);

            if (book.findPatchID(elementID) != RConstants::eod)
            {
                continue;
            }
            RProgressPrint(i,rSurface.size());

            const RElement &rElement = this->getElement(elementID);
            double elementArea = 0.0;

            if (!rElement.findArea(this->nodes,elementArea))
            {
                RLogger::unindent(true);
                RProgressFinalize("Failed");
                throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Could not calculate element area\n");
            }

            // Build distance map.
            this->generateElementDistanceVector(elementID,patchInput[surfaceID].getPatchSize(),separationAngleRad,distanceVector);

            uint patchID = book.createNewPatch(surfaceID);
            double patchArea = 0.0;
            bool addNextElement = true;

            for (uint j=0;j<patchInput[surfaceID].getPatchSize();j++)
            {
                std::vector<uint> distancePositions = distanceVector.getDistancePositions(j);
                for (uint k=0;k<distancePositions.size();k++)
                {
                    if (!this->getElement(distancePositions[k]).findArea(this->nodes,elementArea))
                    {
                        RLogger::unindent(true);
                        RProgressFinalize("Failed");
                        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Could not calculate element area\n");
                    }
                    if (book.findPatchID(distancePositions[k]) != RConstants::eod)
                    {
                        continue;
                    }
                    patchArea += elementArea;
                    book.registerElementID(patchID,distancePositions[k]);

                    if (patchArea > patchInput[surfaceID].getPatchArea())
                    {
                        addNextElement = false;
                        break;
                    }
                }
                if (!addNextElement)
                {
                    break;
                }
            }
        }
        RProgressFinalize("Done");
    }
    RLogger::unindent(true);
} /* RModel::generatePatchSurface */


void RModel::generatePatchInputVector(std::vector<RPatchInput> &patchInput) const
{
    patchInput.resize(this->getNSurfaces(),RPatchInput());
    std::fill(patchInput.begin(),patchInput.end(),RPatchInput());

    for (uint i=0;i<this->getNSurfaces();i++)
    {
        if (!this->getSurface(i).hasBoundaryCondition(R_BOUNDARY_CONDITION_RADIATION))
        {
            continue;
        }
        const RBoundaryCondition &rBoundaryCondition = this->getSurface(i).getBoundaryCondition(R_BOUNDARY_CONDITION_RADIATION);

        uint bcPosition;
        if ((bcPosition = rBoundaryCondition.findComponentPosition(R_VARIABLE_PATCH_AREA)) != RConstants::eod)
        {
            patchInput[i].setPatchArea(rBoundaryCondition.getComponent(bcPosition).getValue(0));
        }
        if ((bcPosition = rBoundaryCondition.findComponentPosition(R_VARIABLE_PATCH_SIZE)) != RConstants::eod)
        {
            patchInput[i].setPatchSize(uint(std::ceil(rBoundaryCondition.getComponent(bcPosition).getValue(0))));
        }
        if ((bcPosition = rBoundaryCondition.findComponentPosition(R_VARIABLE_SEPARATION_ANGLE)) != RConstants::eod)
        {
            patchInput[i].setSeparationAngle(rBoundaryCondition.getComponent(bcPosition).getValue(0));
        }
        if ((bcPosition = rBoundaryCondition.findComponentPosition(R_VARIABLE_RADIATION_EMITTER)) != RConstants::eod)
        {
            patchInput[i].setEmitter(rBoundaryCondition.getComponent(bcPosition).getValue(0) != 0.0);
        }
        if ((bcPosition = rBoundaryCondition.findComponentPosition(R_VARIABLE_RADIATION_RECEIVER)) != RConstants::eod)
        {
            patchInput[i].setReceiver(rBoundaryCondition.getComponent(bcPosition).getValue(0) != 0.0);
        }
    }
} /* RModel::generatePatchInputVector */


void RModel::findPatchCenter(const RPatch &rPatch, double &cx, double &cy, double &cz) const
{
    const RUVector &rElementIDs = rPatch.getElementIDs();

    cx = cy = cz = 0.0;

    double x,y,z;
    uint ne = uint(rElementIDs.size());
    for (uint i=0;i<ne;i++)
    {
        const RElement &rElement = this->getElement(rElementIDs[i]);
        rElement.findCenter(this->getNodes(),x,y,z);
        cx += x;
        cy += y;
        cz += z;
    }
    if (ne > 0)
    {
        cx /= double(ne);
        cy /= double(ne);
        cz /= double(ne);
    }
} /* RModel::findPatchCenter */


void RModel::findPatchNormal(const RPatch &rPatch, double &nx, double &ny, double &nz) const
{
    const RUVector &rElementIDs = rPatch.getElementIDs();

    nx = ny = nz = 0.0;

    double x,y,z;
    uint ne = uint(rElementIDs.size());
    for (uint i=0;i<ne;i++)
    {
        const RElement &rElement = this->getElement(rElementIDs[i]);
        rElement.findNormal(this->getNodes(),x,y,z);
        nx += x;
        ny += y;
        nz += z;
    }
    if (ne > 0)
    {
        nx /= double(ne);
        ny /= double(ne);
        nz /= double(ne);
    }
    double len = sqrt(nx*nx+ny*ny+nz*nz);
    if (len > 0.0)
    {
        nx /= len;
        ny /= len;
        nz /= len;
    }
} /* RModel::findPatchNormal */


void RModel::findPatchArea(const RPatch &rPatch, double area) const
{
    const RUVector &rElementIDs = rPatch.getElementIDs();

    area = 0.0;

    double a;
    uint ne = uint(rElementIDs.size());
    for (uint i=0;i<ne;i++)
    {
        const RElement &rElement = this->getElement(rElementIDs[i]);
        rElement.findArea(this->getNodes(),a);
        area += a;
    }
} /* RModel::findPatchArea */


QString RModel::findRecentViewFactorMatrixFile(void) const
{
    QString viewFactorMatrixFile = this->getProblemSetup().getRadiationSetup().getViewFactorMatrixFile();
    if (!viewFactorMatrixFile.isEmpty())
    {
        viewFactorMatrixFile = RRadiationSetup::findRecentViewFactorMatrixFile(viewFactorMatrixFile, this->getTimeSolver().getCurrentTimeStep());
    }
    return viewFactorMatrixFile;
} /* RModel::findRecentViewFactorMatrixFile */


void RModel::generateViewFactorMatrixHeader(RViewFactorMatrixHeader &viewFactorMatrixHeader) const
{
    viewFactorMatrixHeader.setHemicubeResolution(this->getProblemSetup().getRadiationSetup().getResolution());
    this->generatePatchInputVector(viewFactorMatrixHeader.getPatchInput());
    viewFactorMatrixHeader.setNElements(this->getNElements());
} /* RModel::generateViewFactorMatrixHeade */


QString RModel::writeViewFactorMatrix(const RViewFactorMatrix &viewFactorMatrix, const QString &fileName) const
{
    // Write view-factor matrix to file
    uint currentTimeStep = 0;
    if (this->getTimeSolver().getEnabled())
    {
        currentTimeStep = this->getTimeSolver().getCurrentTimeStep()+1;
    }
    QString newViewFactorMatrixFile = RFileManager::getFileNameWithTimeStep(fileName,currentTimeStep);
    QString linkViewFactorMatrixFile = RFileManager::getFileNameWithOutTimeStep(newViewFactorMatrixFile);

    RLogger::info("Writing view-factor matrix to file \'%s\'\n",newViewFactorMatrixFile.toUtf8().constData());
    viewFactorMatrix.write(newViewFactorMatrixFile,linkViewFactorMatrixFile);

    return linkViewFactorMatrixFile;
} /* RModel::writeViewFactorMatrix */


RBoundaryCondition RModel::generateDefaultBoundayCondition(RBoundaryConditionType type, REntityGroupType entityGroupType, uint entityID) const
{
    RBoundaryCondition bc(type);

    if (type == R_BOUNDARY_CONDITION_RADIATION && entityGroupType == R_ENTITY_GROUP_SURFACE)
    {
        uint cPos;

        // Enable radiation emitter
        if ((cPos= bc.findComponentPosition(R_VARIABLE_RADIATION_EMITTER)) != RConstants::eod)
        {
            bc.getComponent(cPos).add(0,1.0);
        }
        // Enable radiation receiver
        if ((cPos= bc.findComponentPosition(R_VARIABLE_RADIATION_RECEIVER)) != RConstants::eod)
        {
            bc.getComponent(cPos).add(0,1.0);
        }
        // Set patch area
        if ((cPos= bc.findComponentPosition(R_VARIABLE_PATCH_AREA)) != RConstants::eod)
        {
            double surfaceArea = 0.0;
            double elementArea;
            for (uint i=0;i<this->getSurface(entityID).size();i++)
            {
                const RElement &rElement = this->getElement(this->getSurface(entityID).get(i));
                if (rElement.findArea(this->nodes,elementArea))
                {
                    surfaceArea += elementArea;
                }
            }
            bc.getComponent(cPos).add(0,10.0*surfaceArea/double(this->getSurface(entityID).size()));
        }
        // Set patch size
        if ((cPos= bc.findComponentPosition(R_VARIABLE_PATCH_SIZE)) != RConstants::eod)
        {
            bc.getComponent(cPos).add(0,10.0);
        }
        // Set separation angle
        if ((cPos= bc.findComponentPosition(R_VARIABLE_SEPARATION_ANGLE)) != RConstants::eod)
        {
            bc.getComponent(cPos).add(0,45.0);
        }
    }

    return bc;
} /* RModel::generateDefaultBoundayCondition */


QString RModel::generateNextEntityName(REntityGroupType groupType, const QString &suggestedName) const
{
    QString entityName;
    uint cnt = 0;
    switch (groupType)
    {
        case R_ENTITY_GROUP_POINT:
        {
            QString templateName(suggestedName.isEmpty() ? RPoint::defaultName + " " : suggestedName);
            bool nameFound = false;
            do
            {
                entityName = templateName + QString::number(++cnt);
                nameFound = false;
                for (uint i=0;i<this->getNPoints();i++)
                {
                    if (this->getPoint(i).getName() == entityName)
                    {
                        nameFound = true;
                        break;
                    }
                }
            } while (nameFound);
            break;
        }
        case R_ENTITY_GROUP_LINE:
        {
            QString templateName(suggestedName.isEmpty() ? RLine::defaultName + " " : suggestedName);
            bool nameFound = false;
            do
            {
                entityName = templateName + QString::number(++cnt);
                nameFound = false;
                for (uint i=0;i<this->getNLines();i++)
                {
                    if (this->getLine(i).getName() == entityName)
                    {
                        nameFound = true;
                        break;
                    }
                }
            } while (nameFound);
            break;
        }
        case R_ENTITY_GROUP_SURFACE:
        {
            QString templateName(suggestedName.isEmpty() ? RSurface::defaultName + " " : suggestedName);
            bool nameFound = false;
            do
            {
                entityName = templateName + QString::number(++cnt);
                nameFound = false;
                for (uint i=0;i<this->getNSurfaces();i++)
                {
                    if (this->getSurface(i).getName() == entityName)
                    {
                        nameFound = true;
                        break;
                    }
                }
            } while (nameFound);
            break;
        }
        case R_ENTITY_GROUP_VOLUME:
        {
            QString templateName(suggestedName.isEmpty() ? RVolume::defaultName + " " : suggestedName);
            bool nameFound = false;
            do
            {
                entityName = templateName + QString::number(++cnt);
                nameFound = false;
                for (uint i=0;i<this->getNVolumes();i++)
                {
                    if (this->getVolume(i).getName() == entityName)
                    {
                        nameFound = true;
                        break;
                    }
                }
            } while (nameFound);
            break;
        }
        case R_ENTITY_GROUP_VECTOR_FIELD:
        {
            QString templateName(suggestedName.isEmpty() ? RVectorField::defaultName + " " : suggestedName);
            bool nameFound = false;
            do
            {
                entityName = templateName + QString::number(++cnt);
                nameFound = false;
                for (uint i=0;i<this->getNVectorFields();i++)
                {
                    if (this->getVectorField(i).getName() == entityName)
                    {
                        nameFound = true;
                        break;
                    }
                }
            } while (nameFound);
            break;
        }
        case R_ENTITY_GROUP_SCALAR_FIELD:
        {
            QString templateName(suggestedName.isEmpty() ? RScalarField::defaultName + " " : suggestedName);
            bool nameFound = false;
            do
            {
                entityName = templateName + QString::number(++cnt);
                nameFound = false;
                for (uint i=0;i<this->getNScalarFields();i++)
                {
                    if (this->getScalarField(i).getName() == entityName)
                    {
                        nameFound = true;
                        break;
                    }
                }
            } while (nameFound);
            break;
        }
        case R_ENTITY_GROUP_STREAM_LINE:
        {
            QString templateName(suggestedName.isEmpty() ? RStreamLine::defaultName + " " : suggestedName);
            bool nameFound = false;
            do
            {
                entityName = templateName + QString::number(++cnt);
                nameFound = false;
                for (uint i=0;i<this->getNStreamLines();i++)
                {
                    if (this->getStreamLine(i).getName() == entityName)
                    {
                        nameFound = true;
                        break;
                    }
                }
            } while (nameFound);
            break;
        }
        case R_ENTITY_GROUP_CUT:
        {
            QString templateName(suggestedName.isEmpty() ? RCut::defaultName + " " : suggestedName);
            bool nameFound = false;
            do
            {
                entityName = templateName + QString::number(++cnt);
                nameFound = false;
                for (uint i=0;i<this->getNCuts();i++)
                {
                    if (this->getCut(i).getName() == entityName)
                    {
                        nameFound = true;
                        break;
                    }
                }
            } while (nameFound);
            break;
        }
        case R_ENTITY_GROUP_ISO:
        {
            QString templateName(suggestedName.isEmpty() ? RIso::defaultName + " " : suggestedName);
            bool nameFound = false;
            do
            {
                entityName = templateName + QString::number(++cnt);
                nameFound = false;
                for (uint i=0;i<this->getNIsos();i++)
                {
                    if (this->getIso(i).getName() == entityName)
                    {
                        nameFound = true;
                        break;
                    }
                }
            } while (nameFound);
            break;
        }
        default:
        {
            break;
        }
    }
    return entityName;
} /* RModel::generateNextEntityName */


void RModel::convertElementToNodeVector(const RRVector &elementValues,
                                        const RBVector &setValues,
                                        RRVector &nodeValues,
                                        bool onlySetValues)
{
    nodeValues.resize(this->getNNodes(),0.0);

    if (onlySetValues)
    {
        for (uint i=0;i<setValues.size();i++)
        {
            if (setValues[i])
            {
                const RElement &rElement = this->getElement(i);
                for (uint j=0;j<rElement.size();j++)
                {
                    nodeValues[rElement.getNodeId(j)] = 0.0;
                }
            }
        }
    }
    else
    {
        nodeValues.fill(0.0);
    }

    std::vector<double> nodeDistance;
    nodeDistance.resize(this->getNNodes(),0.0);

    if (!onlySetValues)
    {
        // Volume elements.
        for (uint i=0;i<this->getNVolumes();i++)
        {
            const RVolume &rVolume = this->getVolume(i);
            for (uint j=0;j<rVolume.size();j++)
            {
                uint elementID = rVolume.get(j);
                const RElement &element = this->getElement(elementID);
                if (!setValues[elementID])
                {
                    RR3Vector center;
                    element.findCenter(this->nodes,center[0],center[1],center[2]);
                    for (uint j=0;j<element.size();j++)
                    {
                        double d = this->nodes[element.getNodeId(j)].getDistance(RNode(center));
                        d = (d < RConstants::eps) ? 1.0 / RConstants::eps : 1.0 / d;

                        nodeValues[element.getNodeId(j)] += d*elementValues[elementID];
                        nodeDistance[element.getNodeId(j)] += d;
                    }
                }
            }
        }
        // Surface elements.
        for (uint i=0;i<this->getNSurfaces();i++)
        {
            const RSurface &rSurface = this->getSurface(i);
            for (uint j=0;j<rSurface.size();j++)
            {
                uint elementID = rSurface.get(j);
                const RElement &element = this->getElement(elementID);
                if (!setValues[elementID])
                {
                    if (rSurface.getThickness() > 0.0)
                    {
                        RR3Vector center;
                        element.findCenter(this->nodes,center[0],center[1],center[2]);
                        for (uint j=0;j<element.size();j++)
                        {
                            double d = this->nodes[element.getNodeId(j)].getDistance(RNode(center));
                            d = (d < RConstants::eps) ? 1.0 / RConstants::eps : 1.0 / d;

                            nodeValues[element.getNodeId(j)] += d*elementValues[elementID];
                            nodeDistance[element.getNodeId(j)] += d;
                        }
                    }
                }
            }
        }
        // Line elements.
        for (uint i=0;i<this->getNLines();i++)
        {
            const RLine &rLine = this->getLine(i);
            for (uint j=0;j<rLine.size();j++)
            {
                uint elementID = rLine.get(j);
                const RElement &element = this->getElement(elementID);
                if (!setValues[elementID])
                {
                    if (rLine.getCrossArea() > 0.0)
                    {
                        RR3Vector center;
                        element.findCenter(this->nodes,center[0],center[1],center[2]);
                        for (uint j=0;j<element.size();j++)
                        {
                            double d = this->nodes[element.getNodeId(j)].getDistance(RNode(center));
                            d = (d < RConstants::eps) ? 1.0 / RConstants::eps : 1.0 / d;

                            nodeValues[element.getNodeId(j)] += d*elementValues[elementID];
                            nodeDistance[element.getNodeId(j)] += d;
                        }
                    }
                }
            }
        }
        // Point elements.
        for (uint i=0;i<this->getNPoints();i++)
        {
            const RPoint &rPoint = this->getPoint(i);
            for (uint j=0;j<rPoint.size();j++)
            {
                uint elementID = rPoint.get(j);
                const RElement &element = this->getElement(elementID);
                if (!setValues[elementID])
                {
                    if (rPoint.getVolume() > 0.0)
                    {
                        for (uint j=0;j<element.size();j++)
                        {
                            nodeValues[element.getNodeId(j)] += elementValues[elementID] / RConstants::eps;
                            nodeDistance[element.getNodeId(j)] += 1.0 / RConstants::eps;
                        }
                    }
                }
            }
        }
    }

    // Volume elements.
    for (uint i=0;i<this->getNVolumes();i++)
    {
        const RVolume &rVolume = this->getVolume(i);
        for (uint j=0;j<rVolume.size();j++)
        {
            uint elementID = rVolume.get(j);
            const RElement &element = this->getElement(elementID);
            if (setValues[elementID])
            {
                for (uint j=0;j<element.size();j++)
                {
                    nodeValues[element.getNodeId(j)] = elementValues[elementID];
                    nodeDistance[element.getNodeId(j)] = 1.0;
                }
            }
        }
    }
    // Surface elements.
    for (uint i=0;i<this->getNSurfaces();i++)
    {
        const RSurface &rSurface = this->getSurface(i);
        for (uint j=0;j<rSurface.size();j++)
        {
            uint elementID = rSurface.get(j);
            const RElement &element = this->getElement(elementID);
            if (setValues[elementID])
            {
                for (uint j=0;j<element.size();j++)
                {
                    nodeValues[element.getNodeId(j)] = elementValues[elementID];
                    nodeDistance[element.getNodeId(j)] = 1.0;
                }
            }
        }
    }
    // Line elements.
    for (uint i=0;i<this->getNLines();i++)
    {
        const RLine &rLine = this->getLine(i);
        for (uint j=0;j<rLine.size();j++)
        {
            uint elementID = rLine.get(j);
            const RElement &element = this->getElement(elementID);
            if (setValues[elementID])
            {
                for (uint j=0;j<element.size();j++)
                {
                    nodeValues[element.getNodeId(j)] = elementValues[elementID];
                    nodeDistance[element.getNodeId(j)] = 1.0;
                }
            }
        }
    }
    // Point elements.
    for (uint i=0;i<this->getNPoints();i++)
    {
        const RPoint &rPoint = this->getPoint(i);
        for (uint j=0;j<rPoint.size();j++)
        {
            uint elementID = rPoint.get(j);
            const RElement &element = this->getElement(elementID);
            if (setValues[elementID])
            {
                for (uint j=0;j<element.size();j++)
                {
                    nodeValues[element.getNodeId(j)] = elementValues[elementID];
                    nodeDistance[element.getNodeId(j)] = 1.0;
                }
            }
        }
    }

    if (!onlySetValues)
    {
        for (uint i=0;i<this->getNNodes();i++)
        {
            if (nodeDistance[i] == 0.0)
            {
                nodeValues[i] = 0.0;
            }
            else
            {
                nodeValues[i] /= double(nodeDistance[i]);
            }
        }
    }
} /* RModel::convertElementToNodeVector */


void RModel::convertNodeToElementVector(const RRVector &nodeValues,
                                        RRVector &elementValues)
{
    elementValues.resize(this->getNElements(),0.0);

    // Point elements.
    for (uint i=0;i<this->getNPoints();i++)
    {
        const RPoint &rPoint = this->getPoint(i);
        for (uint j=0;j<rPoint.size();j++)
        {
            if (rPoint.getVolume() <= 0.0)
            {
                continue;
            }
            uint elementID = rPoint.get(j);
            elementValues[elementID] = 0.0;
            const RElement &element = this->getElement(elementID);
            for (uint j=0;j<element.size();j++)
            {
                elementValues[elementID] += nodeValues[element.getNodeId(j)];
            }
            elementValues[elementID] /= element.size();
        }
    }
    // Line elements.
    for (uint i=0;i<this->getNLines();i++)
    {
        const RLine &rLine = this->getLine(i);
        for (uint j=0;j<rLine.size();j++)
        {
            if (rLine.getCrossArea() <= 0.0)
            {
                continue;
            }
            uint elementID = rLine.get(j);
            elementValues[elementID] = 0.0;
            const RElement &element = this->getElement(elementID);
            for (uint j=0;j<element.size();j++)
            {
                elementValues[elementID] += nodeValues[element.getNodeId(j)];
            }
            elementValues[elementID] /= element.size();
        }
    }
    // Surface elements.
    for (uint i=0;i<this->getNSurfaces();i++)
    {
        const RSurface &rSurface = this->getSurface(i);
        for (uint j=0;j<rSurface.size();j++)
        {
            if (rSurface.getThickness() <= 0.0)
            {
                continue;
            }
            uint elementID = rSurface.get(j);
            elementValues[elementID] = 0.0;
            const RElement &element = this->getElement(elementID);
            for (uint j=0;j<element.size();j++)
            {
                elementValues[elementID] += nodeValues[element.getNodeId(j)];
            }
            elementValues[elementID] /= element.size();
        }
    }
    // Volume elements.
    for (uint i=0;i<this->getNVolumes();i++)
    {
        const RVolume &rVolume = this->getVolume(i);
        for (uint j=0;j<rVolume.size();j++)
        {
            uint elementID = rVolume.get(j);
            elementValues[elementID] = 0.0;
            const RElement &element = this->getElement(elementID);
            for (uint j=0;j<element.size();j++)
            {
                elementValues[elementID] += nodeValues[element.getNodeId(j)];
            }
            elementValues[elementID] /= element.size();
        }
    }
} /* RModel::convertNodeToElementVector */


void RModel::clearBoundaryConditions(void)
{
    for (uint i=0;i<this->getNPoints();i++)
    {
        this->getPoint(i).removeAllBoundaryConditions();
    }
    for (uint i=0;i<this->getNLines();i++)
    {
        this->getLine(i).removeAllBoundaryConditions();
    }
    for (uint i=0;i<this->getNSurfaces();i++)
    {
        this->getSurface(i).removeAllBoundaryConditions();
    }
    for (uint i=0;i<this->getNVolumes();i++)
    {
        this->getVolume(i).removeAllBoundaryConditions();
    }
} /* RModel::clearBoundaryConditions */


void RModel::clearInitialConditions(void)
{
    for (uint i=0;i<this->getNPoints();i++)
    {
        this->getPoint(i).removeAllInitialConditions();
    }
    for (uint i=0;i<this->getNLines();i++)
    {
        this->getLine(i).removeAllInitialConditions();
    }
    for (uint i=0;i<this->getNSurfaces();i++)
    {
        this->getSurface(i).removeAllInitialConditions();
    }
    for (uint i=0;i<this->getNVolumes();i++)
    {
        this->getVolume(i).removeAllInitialConditions();
    }
} /* RModel::clearInitialConditions */


void RModel::clearEnvironmentConditions(void)
{
    for (uint i=0;i<this->getNPoints();i++)
    {
        this->getPoint(i).removeAllEnvironmentConditions();
    }
    for (uint i=0;i<this->getNLines();i++)
    {
        this->getLine(i).removeAllEnvironmentConditions();
    }
    for (uint i=0;i<this->getNSurfaces();i++)
    {
        this->getSurface(i).removeAllEnvironmentConditions();
    }
    for (uint i=0;i<this->getNVolumes();i++)
    {
        this->getVolume(i).removeAllEnvironmentConditions();
    }
} /* RModel::clearEnvironmentConditions */


void RModel::clearEntityVariableData(void)
{
    for (uint i=0;i<this->getNEntityGroups();i++)
    {
        this->getEntityGroupPtr(i)->getData().getVariableData().clear();
    }
} /* RModel::clearEntityVariableData */


void RModel::clearEntityVariableData(RVariableType variableType)
{
    for (uint i=0;i<this->getNEntityGroups();i++)
    {
        this->getEntityGroupPtr(i)->getData().getVariableData().erase(variableType);
    }
} /* RModel::clearEntityVariableData */


QString RModel::getDefaultFileExtension(bool binary)
{
    if (binary)
    {
        return "rbm";
    }
    else
    {
        return "ram";
    }
} /* RModel::getDefaultFileExtension */


void RModel::writeLink(const QString &linkFileName, const QString &targetFileName)
{
    if (linkFileName.isEmpty() || targetFileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    bool binary = true;
    QString ext = RFileManager::getExtension(linkFileName);

    if (ext == RModel::getDefaultFileExtension(false))
    {
        binary = false;
    }
    else if (ext == RModel::getDefaultFileExtension(true))
    {
        binary = true;
    }
    else
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF, "Unknown extension \"" + ext + "\".");
    }

    RFileManager::writeLink(linkFileName,targetFileName,_version,binary);
} /* RModel::writeLink */


QString RModel::readAscii(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    RLogger::info("Reading ascii model file \'%s\'\n",fileName.toUtf8().constData());

    RFile modelFile(fileName,RFile::ASCII);

    if (!modelFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    RFileHeader fileHeader;

    RFileIO::readAscii(modelFile,fileHeader);
    if (fileHeader.getType() == R_FILE_TYPE_LINK)
    {
        QString targetFileName(RFileManager::findLinkTargetFileName(fileName,fileHeader.getInformation()));
        RLogger::info("File \'%s\' is a link file pointing to \'%s\'\n",fileName.toUtf8().constData(),targetFileName.toUtf8().constData());
        return targetFileName;
    }
    if (fileHeader.getType() != R_FILE_TYPE_MODEL)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF,"File type of the file \'" + fileName + "\' is not MODEL.");
    }

    // Set file version
    modelFile.setVersion(fileHeader.getVersion());

    // Reading mesh/model values

    RFileIO::readAscii(modelFile,this->name);
    RFileIO::readAscii(modelFile,this->description);
    uint nNodes = 0;
    RFileIO::readAscii(modelFile,nNodes);
    this->nodes.resize(nNodes);
    for (uint i=0;i<nNodes;i++)
    {
        RFileIO::readAscii(modelFile,this->nodes[i]);
    }
    uint nElements = 0;
    RFileIO::readAscii(modelFile,nElements);
    this->elements.resize(nElements);
    for (uint i=0;i<nElements;i++)
    {
        RFileIO::readAscii(modelFile,this->elements[i]);
    }
    uint nPoints = 0;
    RFileIO::readAscii(modelFile,nPoints);
    this->points.resize(nPoints);
    for (uint i=0;i<nPoints;i++)
    {
        RFileIO::readAscii(modelFile,this->points[i]);
    }
    uint nLines = 0;
    RFileIO::readAscii(modelFile,nLines);
    this->lines.resize(nLines);
    for (uint i=0;i<nLines;i++)
    {
        RFileIO::readAscii(modelFile,this->lines[i]);
    }
    uint nSurfaces = 0;
    RFileIO::readAscii(modelFile,nSurfaces);
    this->surfaces.resize(nSurfaces);
    for (uint i=0;i<nSurfaces;i++)
    {
        RFileIO::readAscii(modelFile,this->surfaces[i]);
    }
    uint nVolumes = 0;
    RFileIO::readAscii(modelFile,nVolumes);
    this->volumes.resize(nVolumes);
    for (uint i=0;i<nVolumes;i++)
    {
        RFileIO::readAscii(modelFile,this->volumes[i]);
    }
    uint nVectorFields = 0;
    RFileIO::readAscii(modelFile,nVectorFields);
    this->vectorFields.resize(nVectorFields);
    for (uint i=0;i<nVectorFields;i++)
    {
        RFileIO::readAscii(modelFile,this->vectorFields[i]);
    }
    uint nScalarFields = 0;
    RFileIO::readAscii(modelFile,nScalarFields);
    this->scalarFields.resize(nScalarFields);
    for (uint i=0;i<nScalarFields;i++)
    {
        RFileIO::readAscii(modelFile,this->scalarFields[i]);
    }
    uint nStreamLines = 0;
    RFileIO::readAscii(modelFile,nStreamLines);
    this->streamLines.resize(nStreamLines);
    for (uint i=0;i<nStreamLines;i++)
    {
        RFileIO::readAscii(modelFile,this->streamLines[i]);
    }
    uint nCuts = 0;
    RFileIO::readAscii(modelFile,nCuts);
    this->cuts.resize(nCuts);
    for (uint i=0;i<nCuts;i++)
    {
        RFileIO::readAscii(modelFile,this->cuts[i]);
    }
    uint nIsos = 0;
    RFileIO::readAscii(modelFile,nIsos);
    this->isos.resize(nIsos);
    for (uint i=0;i<nIsos;i++)
    {
        RFileIO::readAscii(modelFile,this->isos[i]);
    }
    RFileIO::readAscii(modelFile,this->modelData);

    // Reading problem values

    RFileIO::readAscii(modelFile,this->taskTree);
    RFileIO::readAscii(modelFile,this->timeSolver);
    RFileIO::readAscii(modelFile,this->getMatrixSolverConf(RMatrixSolverConf::CG));
    if (modelFile.getVersion() > RVersion(0,3,2))
    {
        RFileIO::readAscii(modelFile,this->getMatrixSolverConf(RMatrixSolverConf::GMRES));
    }
    RFileIO::readAscii(modelFile,this->monitoringPointManager);
    RFileIO::readAscii(modelFile,this->problemSetup);

    // Reading results values

    RFileIO::readAscii(modelFile,this->RResults::nnodes);
    RFileIO::readAscii(modelFile,this->RResults::nelements);
    uint nVariables = 0;
    RFileIO::readAscii(modelFile,nVariables);
    this->RResults::variables.resize(nVariables);
    for (uint i=0;i<this->RResults::variables.size();i++)
    {
        RFileIO::readAscii(modelFile,this->RResults::variables[i]);
    }

    // Reading neighbor information.
    uint nSurfaceNeigs = 0;
    RFileIO::readAscii(modelFile,nSurfaceNeigs);
    this->surfaceNeigs.resize(nSurfaceNeigs);
    for (uint i=0;i<this->surfaceNeigs.size();i++)
    {
        RFileIO::readAscii(modelFile,this->surfaceNeigs[i]);
    }
    uint nVolumeNeigs = 0;
    RFileIO::readAscii(modelFile,nVolumeNeigs);
    this->volumeNeigs.resize(nVolumeNeigs);
    for (uint i=0;i<this->volumeNeigs.size();i++)
    {
        RFileIO::readAscii(modelFile,this->volumeNeigs[i]);
    }

    modelFile.close();

    return QString();
} /* RModel::readAscii */


QString RModel::readBinary(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    RLogger::info("Reading binary model file \'%s\'\n",fileName.toUtf8().constData());

    RFile modelFile(fileName,RFile::BINARY);

    if (!modelFile.open(QIODevice::ReadOnly))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    RFileHeader fileHeader;

    RFileIO::readBinary(modelFile,fileHeader);
    if (fileHeader.getType() == R_FILE_TYPE_LINK)
    {
        QString targetFileName(RFileManager::findLinkTargetFileName(fileName,fileHeader.getInformation()));
        RLogger::info("File \'%s\' is a link file pointing to \'%s\'\n",fileName.toUtf8().constData(),targetFileName.toUtf8().constData());
        return targetFileName;
    }
    if (fileHeader.getType() != R_FILE_TYPE_MODEL)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF,"File type of the file \'" + fileName + "\' is not MODEL.");
    }

    // Set file version
    modelFile.setVersion(fileHeader.getVersion());

    // Reading mesh/model values

    RFileIO::readBinary(modelFile,this->name);
    RFileIO::readBinary(modelFile,this->description);
    uint nNodes = 0;
    RFileIO::readBinary(modelFile,nNodes);
    this->nodes.resize(nNodes);
    for (uint i=0;i<nNodes;i++)
    {
        RFileIO::readBinary(modelFile,this->nodes[i]);
    }
    uint nElements = 0;
    RFileIO::readBinary(modelFile,nElements);
    this->elements.resize(nElements);
    for (uint i=0;i<nElements;i++)
    {
        RFileIO::readBinary(modelFile,this->elements[i]);
    }
    uint nPoints = 0;
    RFileIO::readBinary(modelFile,nPoints);
    this->points.resize(nPoints);
    for (uint i=0;i<nPoints;i++)
    {
        RFileIO::readBinary(modelFile,this->points[i]);
    }
    uint nLines = 0;
    RFileIO::readBinary(modelFile,nLines);
    this->lines.resize(nLines);
    for (uint i=0;i<nLines;i++)
    {
        RFileIO::readBinary(modelFile,this->lines[i]);
    }
    uint nSurfaces = 0;
    RFileIO::readBinary(modelFile,nSurfaces);
    this->surfaces.resize(nSurfaces);
    for (uint i=0;i<nSurfaces;i++)
    {
        RFileIO::readBinary(modelFile,this->surfaces[i]);
    }
    uint nVolumes = 0;
    RFileIO::readBinary(modelFile,nVolumes);
    this->volumes.resize(nVolumes);
    for (uint i=0;i<nVolumes;i++)
    {
        RFileIO::readBinary(modelFile,this->volumes[i]);
    }
    uint nVectorFields = 0;
    RFileIO::readBinary(modelFile,nVectorFields);
    this->vectorFields.resize(nVectorFields);
    for (uint i=0;i<nVectorFields;i++)
    {
        RFileIO::readBinary(modelFile,this->vectorFields[i]);
    }
    uint nScalarFields = 0;
    RFileIO::readBinary(modelFile,nScalarFields);
    this->scalarFields.resize(nScalarFields);
    for (uint i=0;i<nScalarFields;i++)
    {
        RFileIO::readBinary(modelFile,this->scalarFields[i]);
    }
    uint nStreamLines = 0;
    RFileIO::readBinary(modelFile,nStreamLines);
    this->streamLines.resize(nStreamLines);
    for (uint i=0;i<nStreamLines;i++)
    {
        RFileIO::readBinary(modelFile,this->streamLines[i]);
    }
    uint nCuts = 0;
    RFileIO::readBinary(modelFile,nCuts);
    this->cuts.resize(nCuts);
    for (uint i=0;i<nCuts;i++)
    {
        RFileIO::readBinary(modelFile,this->cuts[i]);
    }
    uint nIsos = 0;
    RFileIO::readBinary(modelFile,nIsos);
    this->isos.resize(nIsos);
    for (uint i=0;i<nIsos;i++)
    {
        RFileIO::readBinary(modelFile,this->isos[i]);
    }
    RFileIO::readBinary(modelFile,this->modelData);

    // Reading problem values

    RFileIO::readBinary(modelFile,this->taskTree);
    RFileIO::readBinary(modelFile,this->timeSolver);
    RFileIO::readBinary(modelFile,this->getMatrixSolverConf(RMatrixSolverConf::CG));
    if (modelFile.getVersion() > RVersion(0,3,2))
    {
        RFileIO::readBinary(modelFile,this->getMatrixSolverConf(RMatrixSolverConf::GMRES));
    }
    RFileIO::readBinary(modelFile,this->monitoringPointManager);
    RFileIO::readBinary(modelFile,this->problemSetup);

    // Reading results values

    RFileIO::readBinary(modelFile,this->RResults::nnodes);
    RFileIO::readBinary(modelFile,this->RResults::nelements);
    uint nVariables = 0;
    RFileIO::readBinary(modelFile,nVariables);
    this->RResults::variables.resize(nVariables);
    for (uint i=0;i<this->RResults::variables.size();i++)
    {
        RFileIO::readBinary(modelFile,this->RResults::variables[i]);
    }

    // Reading neighbor information.
    uint nSurfaceNeigs = 0;
    RFileIO::readBinary(modelFile,nSurfaceNeigs);
    this->surfaceNeigs.resize(nSurfaceNeigs);
    for (uint i=0;i<this->surfaceNeigs.size();i++)
    {
        RFileIO::readBinary(modelFile,this->surfaceNeigs[i]);
    }
    uint nVolumeNeigs = 0;
    RFileIO::readBinary(modelFile,nVolumeNeigs);
    this->volumeNeigs.resize(nVolumeNeigs);
    for (uint i=0;i<this->volumeNeigs.size();i++)
    {
        RFileIO::readBinary(modelFile,this->volumeNeigs[i]);
    }

    modelFile.close();

    return QString();
} /* RModel::readBinary */


void RModel::writeAscii(const QString &fileName) const
{
    RProgressInitialize("Writing model file");

    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    RLogger::info("Writing ascii model file \'%s\'\n",fileName.toUtf8().constData());

    RSaveFile modelFile(fileName,RSaveFile::ASCII);

    if (!modelFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    uint nsteps = this->getNNodes()
                        + this->getNElements()
                        + this->getNPoints()
                        + this->getNLines()
                        + this->getNSurfaces()
                        + this->getNVolumes()
                        + this->getNVectorFields()
                        + this->getNScalarFields()
                        + this->getNStreamLines()
                        + this->getNCuts()
                        + this->getNIsos();
    uint cstep = 0;

    RFileIO::writeAscii(modelFile,RFileHeader(R_FILE_TYPE_MODEL,_version));

    // Writing mesh/model values

    RFileIO::writeAscii(modelFile,"\"" + this->name + "\"");
    RFileIO::writeAscii(modelFile,"\"" + this->description + "\"");
    RFileIO::writeAscii(modelFile,this->getNNodes());
    for (uint i=0;i<this->getNNodes();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeAscii(modelFile,this->nodes[i],false);
        RFileIO::writeNewLineAscii(modelFile);
    }
    RFileIO::writeAscii(modelFile,this->getNElements());
    for (uint i=0;i<this->getNElements();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeAscii(modelFile,this->elements[i],false);
        RFileIO::writeNewLineAscii(modelFile);
    }
    RFileIO::writeAscii(modelFile,this->getNPoints());
    for (uint i=0;i<this->getNPoints();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeAscii(modelFile,this->points[i],false);
        RFileIO::writeNewLineAscii(modelFile);
    }
    RFileIO::writeAscii(modelFile,this->getNLines());
    for (uint i=0;i<this->getNLines();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeAscii(modelFile,this->lines[i],false);
        RFileIO::writeNewLineAscii(modelFile);
    }
    RFileIO::writeAscii(modelFile,this->getNSurfaces());
    for (uint i=0;i<this->getNSurfaces();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeAscii(modelFile,this->surfaces[i],false);
        RFileIO::writeNewLineAscii(modelFile);
    }
    RFileIO::writeAscii(modelFile,this->getNVolumes());
    for (uint i=0;i<this->getNVolumes();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeAscii(modelFile,this->volumes[i],false);
        RFileIO::writeNewLineAscii(modelFile);
    }
    RFileIO::writeAscii(modelFile,this->getNVectorFields());
    for (uint i=0;i<this->getNVectorFields();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeAscii(modelFile,this->vectorFields[i],false);
        RFileIO::writeNewLineAscii(modelFile);
    }
    RFileIO::writeAscii(modelFile,this->getNScalarFields());
    for (uint i=0;i<this->getNScalarFields();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeAscii(modelFile,this->scalarFields[i],false);
        RFileIO::writeNewLineAscii(modelFile);
    }
    RFileIO::writeAscii(modelFile,this->getNStreamLines());
    for (uint i=0;i<this->getNStreamLines();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeAscii(modelFile,this->streamLines[i],false);
        RFileIO::writeNewLineAscii(modelFile);
    }
    RFileIO::writeAscii(modelFile,this->getNCuts());
    for (uint i=0;i<this->getNCuts();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeAscii(modelFile,this->cuts[i],false);
        RFileIO::writeNewLineAscii(modelFile);
    }
    RFileIO::writeAscii(modelFile,this->getNIsos());
    for (uint i=0;i<this->getNIsos();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeAscii(modelFile,this->isos[i],false);
        RFileIO::writeNewLineAscii(modelFile);
    }
    RFileIO::writeAscii(modelFile,this->modelData);

    // Writing problem values

    RFileIO::writeAscii(modelFile,this->taskTree);
    RFileIO::writeAscii(modelFile,this->timeSolver);
    RFileIO::writeAscii(modelFile,this->getMatrixSolverConf(RMatrixSolverConf::CG));
    RFileIO::writeAscii(modelFile,this->getMatrixSolverConf(RMatrixSolverConf::GMRES));
    RFileIO::writeAscii(modelFile,this->monitoringPointManager);
    RFileIO::writeAscii(modelFile,this->problemSetup);

    // Reading results values

    RFileIO::writeAscii(modelFile,this->RResults::nnodes);
    RFileIO::writeAscii(modelFile,this->RResults::nelements);
    RFileIO::writeAscii(modelFile,(uint)this->RResults::variables.size());
    for (uint i=0;i<this->RResults::variables.size();i++)
    {
        RFileIO::writeAscii(modelFile,this->RResults::variables[i]);
    }

    // Writing neighbor information.
    RFileIO::writeAscii(modelFile,(uint)this->surfaceNeigs.size());
    for (uint i=0;i<this->surfaceNeigs.size();i++)
    {
        RFileIO::writeAscii(modelFile,this->surfaceNeigs[i]);
    }
    RFileIO::writeAscii(modelFile,(uint)this->volumeNeigs.size());
    for (uint i=0;i<this->volumeNeigs.size();i++)
    {
        RFileIO::writeAscii(modelFile,this->volumeNeigs[i]);
    }

    modelFile.commit();

    RProgressFinalize("Done");
} /* RModel::writeAscii */


void RModel::writeBinary(const QString &fileName) const
{
    RProgressInitialize("Writing model file");

    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    RLogger::info("Writing binary model file \'%s\'\n",fileName.toUtf8().constData());

    RSaveFile modelFile(fileName,RSaveFile::BINARY);

    if (!modelFile.open(QIODevice::WriteOnly))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    uint nsteps = this->getNNodes()
                        + this->getNElements()
                        + this->getNPoints()
                        + this->getNLines()
                        + this->getNSurfaces()
                        + this->getNVolumes()
                        + this->getNVectorFields()
                        + this->getNScalarFields()
                        + this->getNStreamLines()
                        + this->getNCuts()
                        + this->getNIsos();
    uint cstep = 0;

    RFileIO::writeBinary(modelFile,RFileHeader(R_FILE_TYPE_MODEL,_version));

    // Writing mesh/model values

    RFileIO::writeBinary(modelFile,this->name);
    RFileIO::writeBinary(modelFile,this->description);
    RFileIO::writeBinary(modelFile,this->getNNodes());
    for (uint i=0;i<this->getNNodes();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeBinary(modelFile,this->nodes[i]);
    }
    RFileIO::writeBinary(modelFile,this->getNElements());
    for (uint i=0;i<this->getNElements();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeBinary(modelFile,this->elements[i]);
    }
    RFileIO::writeBinary(modelFile,this->getNPoints());
    for (uint i=0;i<this->getNPoints();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeBinary(modelFile,this->points[i]);
    }
    RFileIO::writeBinary(modelFile,this->getNLines());
    for (uint i=0;i<this->getNLines();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeBinary(modelFile,this->lines[i]);
    }
    RFileIO::writeBinary(modelFile,this->getNSurfaces());
    for (uint i=0;i<this->getNSurfaces();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeBinary(modelFile,this->surfaces[i]);
    }
    RFileIO::writeBinary(modelFile,this->getNVolumes());
    for (uint i=0;i<this->getNVolumes();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeBinary(modelFile,this->volumes[i]);
    }
    RFileIO::writeBinary(modelFile,this->getNVectorFields());
    for (uint i=0;i<this->getNVectorFields();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeBinary(modelFile,this->vectorFields[i]);
    }
    RFileIO::writeBinary(modelFile,this->getNScalarFields());
    for (uint i=0;i<this->getNScalarFields();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeBinary(modelFile,this->scalarFields[i]);
    }
    RFileIO::writeBinary(modelFile,this->getNStreamLines());
    for (uint i=0;i<this->getNStreamLines();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeBinary(modelFile,this->streamLines[i]);
    }
    RFileIO::writeBinary(modelFile,this->getNCuts());
    for (uint i=0;i<this->getNCuts();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeBinary(modelFile,this->cuts[i]);
    }
    RFileIO::writeBinary(modelFile,this->getNIsos());
    for (uint i=0;i<this->getNIsos();i++)
    {
        RProgressPrint(cstep++,nsteps);
        RFileIO::writeBinary(modelFile,this->isos[i]);
    }
    RFileIO::writeBinary(modelFile,this->modelData);

    // Writing problem values

    RFileIO::writeBinary(modelFile,this->taskTree);
    RFileIO::writeBinary(modelFile,this->timeSolver);
    RFileIO::writeBinary(modelFile,this->getMatrixSolverConf(RMatrixSolverConf::CG));
    RFileIO::writeBinary(modelFile,this->getMatrixSolverConf(RMatrixSolverConf::GMRES));
    RFileIO::writeBinary(modelFile,this->monitoringPointManager);
    RFileIO::writeBinary(modelFile,this->problemSetup);

    // Writing results values

    RFileIO::writeBinary(modelFile,this->RResults::nnodes);
    RFileIO::writeBinary(modelFile,this->RResults::nelements);
    RFileIO::writeBinary(modelFile,(uint)this->RResults::variables.size());
    for (uint i=0;i<this->RResults::variables.size();i++)
    {
        RFileIO::writeBinary(modelFile,this->RResults::variables[i]);
    }

    // Writing neighbor information.
    RFileIO::writeBinary(modelFile,(uint)this->surfaceNeigs.size());
    for (uint i=0;i<this->surfaceNeigs.size();i++)
    {
        RFileIO::writeBinary(modelFile,this->surfaceNeigs[i]);
    }
    RFileIO::writeBinary(modelFile,(uint)this->volumeNeigs.size());
    for (uint i=0;i<this->volumeNeigs.size();i++)
    {
        RFileIO::writeBinary(modelFile,this->volumeNeigs[i]);
    }

    modelFile.commit();

    RProgressFinalize("Done");
} /* RModel::writeBinary */


std::vector<RUVector> RModel::findSurfaceNeighbors(void) const
{
    std::vector<RUVector> neigs;

    RLogger::info("Finding surface neighbors\n");
    RLogger::indent();
    RProgressInitialize("Finding surface neighbors");

    neigs.resize(this->getNElements());

    RUVector surfaceElements;
    surfaceElements.reserve(this->getNElements());
    for (uint i=0;i<this->getNElements();i++)
    {
        if (R_ELEMENT_TYPE_IS_SURFACE(this->getElement(i).getType()))
        {
            surfaceElements.push_back(i);
            neigs[i].reserve(RElement::getNNeighbors(this->getElement(i).getType()));
        }
    }

    uint nProcessed = 0;
#pragma omp parallel for default(shared)
    for (int64_t i=0;i<int64_t(surfaceElements.size());i++)
    {
        uint elementID1 = surfaceElements[i];
#pragma omp critical
        {
            RProgressPrint(++nProcessed,uint(surfaceElements.size()));
        }
        RLimitBox lb1,lb2;
        this->getElement(elementID1).findLimitBox(this->getNodes(),lb1);

        for (uint j=i+1;j<surfaceElements.size();j++)
        {
            uint elementID2 = surfaceElements[j];
            this->getElement(elementID2).findLimitBox(this->getNodes(),lb2);
            if (!RLimitBox::areIntersecting(lb1,lb2))
            {
                continue;
            }

            if (this->getElement(elementID1).isNeighbor(this->getElement(elementID2)))
            {
#pragma omp critical
                {
                    neigs[elementID1].push_back(elementID2);
                    neigs[elementID2].push_back(elementID1);
                }
            }
        }
    }

    RProgressFinalize();
    RLogger::unindent();
    return neigs;
} /* RModel::findSurfaceNeighbors */

std::vector<RUVector> RModel::findVolumeNeighbors(void) const
{
    std::vector<RUVector> neigs;

    RLogger::info("Finding volume neighbors\n");
    RLogger::indent();
    RProgressInitialize("Finding volume neighbors");

    neigs.resize(this->getNElements());

    RUVector volumeElements;
    RUVector volumeNeighborCount;
    volumeElements.reserve(this->getNElements());
    volumeNeighborCount.resize(this->getNElements());
    for (uint i=0;i<this->getNElements();i++)
    {
        if (R_ELEMENT_TYPE_IS_VOLUME(this->getElement(i).getType()))
        {
            volumeElements.push_back(i);
            volumeNeighborCount[i] = RElement::getNNeighbors(this->getElement(i).getType());
            neigs[i].reserve(volumeNeighborCount[i]);
        }
    }

    uint nProcessed = 0;
#pragma omp parallel for default(shared)
    for (int64_t i=0;i<int64_t(volumeElements.size());i++)
    {
#pragma omp critical
        {
            RProgressPrint(++nProcessed,uint(volumeElements.size()));
        }
        uint elementID1 = volumeElements[i];

        if (neigs[elementID1].size() >= volumeNeighborCount[elementID1])
        {
            continue;
        }

        RLimitBox lb1,lb2;
        this->getElement(elementID1).findLimitBox(this->getNodes(),lb1);

        for (uint j=i+1;j<volumeElements.size();j++)
        {
            uint elementID2 = volumeElements[j];

            if (neigs[elementID2].size() >= volumeNeighborCount[elementID2])
            {
                continue;
            }

            this->getElement(elementID2).findLimitBox(this->getNodes(),lb2);
            if (!RLimitBox::areIntersecting(lb1,lb2))
            {
                continue;
            }

            bool skip = false;
            if (this->getElement(elementID1).isNeighbor(this->getElement(elementID2)))
            {
#pragma omp critical
                {
                    neigs[elementID1].push_back(elementID2);
                    neigs[elementID2].push_back(elementID1);
                    if (neigs[elementID1].size() == volumeNeighborCount[elementID1])
                    {
                        skip = true;
                    }
                }
            }
            if (skip)
            {
                break;
            }
        }
    }

    RProgressFinalize();
    RLogger::unindent();

    return neigs;
} /* RModel::findVolumeNeighbors */


void RModel::markSurfaceNeighbors(uint elementID,
                                 double angle,
                                 const std::vector<RUVector> &neighbors,
                                 std::vector<uint> &marks)
{
    uint cElementID;
    uint nElementID;
    double cnAngle;
    bool cposChanged;
    int cpos = 0;
    std::vector<uint> stack;
    stack.resize(this->getNElements());
    stack[cpos] = elementID;
    RRVector cNormal(3);
    RRVector nNormal(3);

    while (cpos >= 0)
    {
        cposChanged = false;
        cElementID = stack[cpos];
        for (uint i=0;i<neighbors[cElementID].size();i++)
        {
            nElementID = neighbors[cElementID][i];

            if (marks[cElementID] == marks[nElementID])
            {
                continue;
            }

            if (!this->getElement(cElementID).findNormal(this->nodes,cNormal[0],cNormal[1],cNormal[2]))
            {
                RLogger::warning("Failed to find normal for element \'%u\'.\n",cElementID);
                continue;
            }
            if (!this->getElement(nElementID).findNormal(this->nodes,nNormal[0],nNormal[1],nNormal[2]))
            {
                RLogger::warning("Failed to find normal for element \'%u\'.\n",nElementID);
                continue;
            }

            cnAngle = R_RAD_TO_DEG(RRVector::angle(cNormal,nNormal));

            if (cnAngle <= angle || cnAngle >= (360.0 - angle))
            {
                marks[nElementID] = marks[cElementID];
                stack[++cpos] = nElementID;
                cposChanged = true;
                break;
            }
        }
        if (!cposChanged)
        {
            cpos--;
        }
    }
} /* RModel::markSurfaceNeighbors */


void RModel::removeEntityGroupIdReferences(uint entityGroupId)
{
    for (uint i=0;i<this->getNVectorFields();i++)
    {
        this->getVectorField(i).removeElementGroupID(entityGroupId,true);
    }
    for (uint i=0;i<this->getNScalarFields();i++)
    {
        this->getScalarField(i).removeElementGroupID(entityGroupId,true);
    }
    for (uint i=0;i<this->getNCuts();i++)
    {
        this->getCut(i).removeElementGroupID(entityGroupId,true);
    }
    for (uint i=0;i<this->getNIsos();i++)
    {
        this->getIso(i).removeElementGroupID(entityGroupId,true);
    }
} /* RModel::removeEntityGroupIdReferences */


void RModel::generateElementDistanceVector(uint startElementID, uint maximumDistance, double separationAngle, RDistanceVector<uint> &distanceVector) const
{
    distanceVector.resize(this->getNElements(),RConstants::eod);
    distanceVector.fill(RConstants::eod);

    std::stack<uint> elementStack;

    uint elementID = startElementID;
    uint distance = 0;

    RR3Vector n1;
    RR3Vector n2;

    do
    {
        bool popPrevious = true;
        if (distance <= maximumDistance)
        {
            distanceVector[elementID] = distance;
            if (distance < maximumDistance)
            {
                this->getElement(elementID).findNormal(this->nodes,n1[0],n1[1],n1[2]);
                const std::vector<uint> *neighborIDs = this->getNeighborIDs(elementID);
                if (neighborIDs)
                {
                    for (uint i=0;i<neighborIDs->size();i++)
                    {
                        if (distanceVector[neighborIDs->at(i)] > distanceVector[elementID] + 1)
                        {
                            this->getElement(neighborIDs->at(i)).findNormal(this->nodes,n2[0],n2[1],n2[2]);
                            if (RRVector::angle(n1,n2) < separationAngle)
                            {
                                elementStack.push(elementID);
                                elementID = neighborIDs->at(i);
                                distance ++;
                                popPrevious = false;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if (popPrevious && elementStack.size() > 0)
        {
            elementID = elementStack.top();
            distance = distanceVector[elementID];
            elementStack.pop();
        }
    } while (elementID != startElementID);
} /* RModel::generateElementDistanceVector */


uint RModel::findNearNode(const RNode &node, double tolerance, bool findNearest, uint nodeID) const
{
    uint nn = this->getNNodes();
    uint nearNode = RConstants::eod;
    double minDistance = DBL_MAX;

    for (uint i=0;i<nn;i++)
    {
        if (i == nodeID)
        {
            continue;
        }
        if (tolerance == 0.0)
        {
            if (this->getNode(i) == node)
            {
                nearNode = i;
                break;
            }
        }
        else
        {
            double distance = this->getNode(i).getDistance(node);
            if (distance < tolerance)
            {
                if (distance < minDistance)
                {
                    minDistance = distance;
                    nearNode = i;
                    if (!findNearest)
                    {
                        break;
                    }
                }
            }
        }
    }

    return nearNode;
} /* RModel::findNearNode */
