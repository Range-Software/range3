/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolverfluidheat.cpp                                     *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   28-th January 2016                                       *
 *                                                                   *
 *  DESCRIPTION: Fluid - heat transfer solver class definition       *
 *********************************************************************/

#include "rsolverfluidheat.h"

void RSolverFluidHeat::_init(const RSolverFluidHeat *pSolver)
{
    if (pSolver)
    {

    }
}

RSolverFluidHeat::RSolverFluidHeat(RModel *pModel, const QString &modelFileName, RSolverSharedData &sharedData)
    : RSolverFluid(pModel,modelFileName,sharedData)
{
    this->problemType = R_PROBLEM_FLUID_HEAT;
    this->_init();
}

RSolverFluidHeat::RSolverFluidHeat(const RSolverFluidHeat &solver)
    : RSolverFluid(solver)
{
    this->_init(&solver);
}

RSolverFluidHeat::~RSolverFluidHeat()
{

}

RSolverFluidHeat &RSolverFluidHeat::operator =(const RSolverFluidHeat &solver)
{
    RSolverFluid::operator =(solver);
    this->_init(&solver);
    return (*this);
}

bool RSolverFluidHeat::hasConverged(void) const
{
    return true;
}

void RSolverFluidHeat::updateScales(void)
{
    if (this->taskIteration > 0)
    {
        return;
    }

    RRVector eRo;
    RRVector eU;

    this->generateMaterialVecor(R_MATERIAL_PROPERTY_DENSITY,eRo);
    this->generateMaterialVecor(R_MATERIAL_PROPERTY_DYNAMIC_VISCOSITY,eU);

    this->avgRo = 0.0;
    this->avgU = 0.0;
    uint n = 0;
    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        if (eRo[i] != 0.0 && eU[i] != 0.0)
        {
            this->avgRo += eRo[i];
            this->avgU += eU[i];
            n++;
        }
    }
    if (n > 0)
    {
        this->avgRo /= double(n);
        this->avgU /= double(n);
    }

    this->scales.setMetre(this->findMeshScale());
    this->scales.setSecond(this->findReScale());
    this->scales.setKilogram(this->findWeightScale());
}

void RSolverFluidHeat::recover(void)
{

}

void RSolverFluidHeat::prepare(void)
{

}

void RSolverFluidHeat::solve(void)
{

}

void RSolverFluidHeat::process(void)
{

}

void RSolverFluidHeat::store(void)
{

}

void RSolverFluidHeat::statistics(void)
{

}
