/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolverradiativeheat.cpp                                 *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   6-th November 2015                                       *
 *                                                                   *
 *  DESCRIPTION: Radiative Heat-transfer solver class definition     *
 *********************************************************************/

#include "rsolverradiativeheat.h"
#include "rmatrixsolver.h"
#include "rhemicube.h"

void RSolverRadiativeHeat::_init(const RSolverRadiativeHeat *pRadiativeHeatSolver)
{
    if (pRadiativeHeatSolver)
    {
        this->elementTemperature = pRadiativeHeatSolver->elementTemperature;
        this->elementRadiativeHeat = pRadiativeHeatSolver->elementRadiativeHeat;
        this->patchHeat = pRadiativeHeatSolver->patchHeat;
        this->viewFactorMatrix = pRadiativeHeatSolver->viewFactorMatrix;
        this->patchHeatNorm = pRadiativeHeatSolver->patchHeatNorm;
        this->oldPatchHeatNorm = pRadiativeHeatSolver->oldPatchHeatNorm;
    }
}

RSolverRadiativeHeat::RSolverRadiativeHeat(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData)
    : RSolverGeneric(pModel,modelFileName,convergenceFileName,sharedData)
    , patchHeatNorm(0.0)
    , oldPatchHeatNorm(0.0)
{
    this->problemType = R_PROBLEM_RADIATIVE_HEAT;
    this->_init();
}

RSolverRadiativeHeat::RSolverRadiativeHeat(const RSolverRadiativeHeat &radiativeHeatSolver)
    : RSolverGeneric(radiativeHeatSolver)
{
    this->_init(&radiativeHeatSolver);
}

RSolverRadiativeHeat::~RSolverRadiativeHeat()
{

}

RSolverRadiativeHeat &RSolverRadiativeHeat::operator =(const RSolverRadiativeHeat &radiativeHeatSolver)
{
    RSolverGeneric::operator =(radiativeHeatSolver);
    this->_init(&radiativeHeatSolver);
    return (*this);
}

bool RSolverRadiativeHeat::hasConverged(void) const
{
    if (this->oldPatchHeatNorm < RConstants::eps)
    {
        return (this->patchHeatNorm < RConstants::eps);
    }
    double convergenceRate = std::abs(1.0 - this->patchHeatNorm/this->oldPatchHeatNorm);

    RLogger::info("Convergence rate = %g\n",convergenceRate);

    return (convergenceRate < RConstants::eps);
}

double RSolverRadiativeHeat::findTemperatureScale(void) const
{
    return 1.0;
}

void RSolverRadiativeHeat::updateScales(void)
{
    this->scales.setMetre(this->findMeshScale());
    this->scales.setKelvin(this->findTemperatureScale());
}

void RSolverRadiativeHeat::recover(void)
{
    uint heatRadiationVariablePosition = this->pModel->findVariable(R_VARIABLE_HEAT_RADIATION);

    if (heatRadiationVariablePosition != RConstants::eod)
    {
        const RVariable &rVariable = this->pModel->getVariable(heatRadiationVariablePosition);
        if (rVariable.getApplyType() == R_VARIABLE_APPLY_ELEMENT)
        {
            this->elementRadiativeHeat = rVariable.getValues();
        }
    }
}

void RSolverRadiativeHeat::prepareViewFactors(void)
{
    if (this->checkViewFactorHeader(this->viewFactorMatrix.getHeader()))
    {
        return;
    }

    bool reculateViewFactors = false;

    QString viewFactorMatrixFile = this->pModel->getProblemSetup().getRadiationSetup().getViewFactorMatrixFile();
    if (viewFactorMatrixFile.isEmpty())
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Missing view-factor matrix file name.");
    }

    QString recentViewFactorMatrixFile = RRadiationSetup::findRecentViewFactorMatrixFile(viewFactorMatrixFile,
                                                                                         this->pModel->getTimeSolver().getCurrentTimeStep());

    if (recentViewFactorMatrixFile.isEmpty())
    {
        reculateViewFactors = true;
    }
    else
    {
        RViewFactorMatrixHeader viewFactorMatrixHeader;
        try
        {
            RViewFactorMatrix::readHeader(recentViewFactorMatrixFile,viewFactorMatrixHeader);
        }
        catch (const RError &error)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,
                         "Failed to read view factor matrix header from file \'%s\'. %s",
                         recentViewFactorMatrixFile.toUtf8().constData(),
                         error.getMessage().toUtf8().constData());
        }

        if (!this->checkViewFactorHeader(viewFactorMatrixHeader))
        {
            reculateViewFactors = true;
        }
        else
        {
            try
            {
                this->viewFactorMatrix.read(recentViewFactorMatrixFile);
                if (!this->checkViewFactorHeader(this->viewFactorMatrix.getHeader()))
                {
                    reculateViewFactors = true;
                }
            }
            catch (const RError &error)
            {
                throw RError(R_ERROR_APPLICATION,R_ERROR_REF,
                             "Failed to read view factor matrix header from file \'%s\'. %s",
                             recentViewFactorMatrixFile.toUtf8().constData(),
                             error.getMessage().toUtf8().constData());
            }
        }
    }

    if (reculateViewFactors)
    {
        // Generate patch surface
        this->pModel->generatePatchInputVector(this->viewFactorMatrix.getHeader().getPatchInput());
        this->pModel->generatePatchSurface(this->viewFactorMatrix.getHeader().getPatchInput(),
                                           this->viewFactorMatrix.getPatchBook());

        // Calculate view-factors
        RHemiCube::calculateViewFactors(*this->pModel,this->viewFactorMatrix);

        // Write view-factor matrix to file
        viewFactorMatrixFile = this->pModel->writeViewFactorMatrix(this->viewFactorMatrix,viewFactorMatrixFile);

        this->pModel->getProblemSetup().getRadiationSetup().setViewFactorMatrixFile(viewFactorMatrixFile);
    }
}

void RSolverRadiativeHeat::prepare(void)
{
    RBVector temperatureExplicitFlags;
    RRVector elementAmbientTemperature;
    RRVector elementEmissivity;

    this->generateNodeBook(R_PROBLEM_HEAT);
    this->generateVariableVector(R_VARIABLE_TEMPERATURE,this->elementTemperature,temperatureExplicitFlags,true,this->firstRun,this->firstRun);
    this->generateVariableVector(R_VARIABLE_TEMPERATURE,elementAmbientTemperature,temperatureExplicitFlags,false,false,true);

    // Initialize view-factor matrix.
    this->prepareViewFactors();
    this->generateMaterialVecor(R_MATERIAL_PROPERTY_EMISSIVITY,elementEmissivity);

    const RPatchBook &rPatchBook = this->viewFactorMatrix.getPatchBook();

    // Initialize radiation heat.
    this->elementRadiativeHeat.resize(this->pModel->getNElements(),0.0);
    this->patchHeat.resize(rPatchBook.getNPatches(),0.0);

    // Resize computatioal arrays.
    this->b.resize(rPatchBook.getNPatches());
    this->x.resize(rPatchBook.getNPatches());

    this->A.clear();
    this->b.fill(0.0);
    this->x.fill(0.0);

    RRVector patchEmissivity(rPatchBook.getNPatches(),0.0);
    RRVector patchTemperature(rPatchBook.getNPatches(),0.0);
    RRVector patchAmbientTemperature(rPatchBook.getNPatches(),0.0);

    // Find patch emissivity and temperature.
    for (uint i=0;i<this->pModel->getNSurfaces();i++)
    {
        const RSurface &rSurface = this->pModel->getSurface(i);
        for (uint j=0;j<rSurface.size();j++)
        {
            uint elementID = rSurface.get(j);
            uint patchID = rPatchBook.findPatchID(elementID);
            if (patchID != RConstants::eod)
            {
                patchEmissivity[patchID] += elementEmissivity[elementID];
                patchTemperature[patchID] += this->elementTemperature[elementID];
                patchAmbientTemperature[patchID] += elementAmbientTemperature[elementID];
            }
        }
    }
    for (uint i=0;i<rPatchBook.getNPatches();i++)
    {
        uint nElements = uint(rPatchBook.getPatch(i).getElementIDs().size());
        if (nElements > 0)
        {
            patchEmissivity[i] /= double(nElements);
            patchTemperature[i] /= double(nElements);
            patchAmbientTemperature[i] /= double(nElements);
        }
    }

    // Prepare patch elements.
    #pragma omp parallel for default(shared)
    for (int64_t i=0;i<int64_t(rPatchBook.getNPatches());i++)
    {
        RRVector viewFactors = this->viewFactorMatrix.getRow(i).getViewFactors().getValues(rPatchBook.getNPatches());

        double Fsum = 0.0;
        for (uint j=0;j<rPatchBook.getNPatches();j++)
        {
            double dij = (i == j ? 1.0 : 0.0);

            double Fij = viewFactors[j];
            double Ej = patchEmissivity[j];
            Fsum += Fij;

            double Aij = 0.0;
            if (Ej != 0.0)
            {
                Aij = dij/Ej - Fij*(1.0-Ej)/Ej;
            }
            double Bij = (dij-Fij)*RSolverGeneric::sigma;

            #pragma omp critical
            {
                this->A.addValue(i,j,Aij);
                this->b[j] -= Bij * std::pow(patchTemperature[j],4);
            }
        }

        // Ambient radiative heat flux
        this->b[i] += (1.0 - Fsum) * RSolverGeneric::sigma * std::pow(patchAmbientTemperature[i],4);
        // Subtract resulting heat from convection-conduction equation to ensure energy balance
//        this->b[i] -= this->patchHeat[i];
    }
}

void RSolverRadiativeHeat::solve(void)
{
    try
    {
        RLogger::indent();
        RMatrixSolver matrixSolver(this->pModel->getMatrixSolverConf(RMatrixSolverConf::CG));
        matrixSolver.solve(this->A,this->b,this->x,R_MATRIX_PRECONDITIONER_JACOBI,1);
        RLogger::unindent();
    }
    catch (RError error)
    {
        RLogger::unindent();
        throw error;
    }

    const RPatchBook &rPatchBook = this->viewFactorMatrix.getPatchBook();

    for (uint i=0;i<rPatchBook.getNPatches();i++)
    {
        this->patchHeat[i] = this->x[i];
    }
    this->oldPatchHeatNorm = this->patchHeatNorm;
    this->patchHeatNorm = RRVector::norm(this->patchHeat);
}

void RSolverRadiativeHeat::process(void)
{
    const RPatchBook &rPatchBook = this->viewFactorMatrix.getPatchBook();

    for (uint i=0;i<rPatchBook.getNPatches();i++)
    {
        const RUVector &elementIDs = rPatchBook.getPatch(i).getElementIDs();

        // Distribute heat to elements based on their size portion.
        double patchArea = 0.0;
        for (uint j=0;j<elementIDs.size();j++)
        {
            double elementArea;
            if (this->pModel->getElement(elementIDs[j]).findArea(this->pModel->getNodes(),elementArea))
            {
                patchArea += elementArea;
            }
        }
        for (uint j=0;j<elementIDs.size();j++)
        {
            double elementArea;
            double ratio = 0.0;
            if (this->pModel->getElement(elementIDs[j]).findArea(this->pModel->getNodes(),elementArea))
            {
                ratio = elementArea / patchArea;
            }
            this->elementRadiativeHeat[elementIDs[j]] = this->patchHeat[i] * ratio;
        }
    }
}

void RSolverRadiativeHeat::store(void)
{
    RLogger::info("Storing results\n");
    RLogger::indent();

    // Heat flux
    uint heatPos = this->pModel->findVariable(R_VARIABLE_HEAT_RADIATION);
    if (heatPos == RConstants::eod)
    {
        heatPos = this->pModel->addVariable(R_VARIABLE_HEAT_RADIATION);
        this->pModel->getVariable(heatPos).getVariableData().setMinMaxDisplayValue(
                    RStatistics::findMinimumValue(this->elementRadiativeHeat),
                    RStatistics::findMaximumValue(this->elementRadiativeHeat));
    }
    RVariable &heat =  this->pModel->getVariable(heatPos);

    heat.setApplyType(R_VARIABLE_APPLY_ELEMENT);
    heat.resize(1,this->pModel->getNElements());
    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        heat.setValue(0,i,this->elementRadiativeHeat[i]);
    }

    RLogger::unindent();
}

void RSolverRadiativeHeat::statistics(void)
{
    this->printStats(R_VARIABLE_HEAT_RADIATION);
    this->processMonitoringPoints();
}

bool RSolverRadiativeHeat::checkViewFactorHeader(const RViewFactorMatrixHeader &viewFactorMatrixHeader) const
{
    if (viewFactorMatrixHeader.getPatchInput().size() != this->pModel->getNSurfaces())
    {
        return false;
    }
    if (viewFactorMatrixHeader.getNElements() != this->pModel->getNElements())
    {
        return false;
    }
    if (viewFactorMatrixHeader.getHemicubeResolution() != this->pModel->getProblemSetup().getRadiationSetup().getResolution())
    {
        return false;
    }
    std::vector<RPatchInput> patchInput;
    this->pModel->generatePatchInputVector(patchInput);
    if (viewFactorMatrixHeader.getPatchInput() != patchInput)
    {
        return false;
    }
    return true;
}
