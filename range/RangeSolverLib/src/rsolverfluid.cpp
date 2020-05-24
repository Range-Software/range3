/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolverfluid.cpp                                         *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   20-th November 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Fluid solver class definition                       *
 *********************************************************************/

#include <omp.h>

#include "rsolverfluid.h"
#include "rmatrixsolver.h"

//#define _OPTIMAL_ASSEMBLY_

static const double inv6 = 1.0 / 6.0;

class FluidMatrixContainer
{
    public:

        bool initialized;

        // Element level matricies
        RRMatrix me;    // m
        RRMatrix ce;    // c
        RRMatrix ke;    // k
        RRMatrix ge;    // g
        RRMatrix geT;   // gT
        RRMatrix cpe;   // c+
        RRMatrix cte;   // c~
        RRMatrix ctpe;  // c~+
        RRMatrix kte;   // k~
        RRMatrix ktpe;  // k~+
        RRMatrix ktppe; // k~++
        RRMatrix yte;   // y~
        RRMatrix ytpe;  // y~+
        RRMatrix bte;   // B
        RRMatrix ye;    // y
        RRMatrix ype;   // y+
        RRMatrix the;   // 0
        RRMatrix epe;   // e

        // Element level vectors
        RRVector fv;    // f
        RRVector ftv;   // f~
        RRVector etv;   // e~
        RRVector mv;    // m
        RRVector cv;    // c
        RRVector kv;    // k
        RRVector gv;    // g
        RRVector gvT;   // gT
        RRVector ctv;   // c~
        RRVector ktv;   // k~
        RRVector ytv;   // y~
        RRVector btv;   // B
        RRVector yv;    // y
        RRVector thv;   // 0
        RRVector ev;    // e

        // Element level equations
        RRMatrix Ae11;
        RRMatrix Ae12;
        RRMatrix Ae21;
        RRMatrix Ae22;
        RRVector be1;
        RRVector be2;

    public:

        FluidMatrixContainer() : initialized(false)
        {

        }

        void resize(uint nen)
        {
            this->me.resize(nen*3,nen*3,0.0);
            this->ce.resize(nen*3,nen*3,0.0);
            this->ke.resize(nen*3,nen*3,0.0);
            this->ge.resize(nen*3,nen,0.0);
            this->geT.resize(nen,nen*3,0.0);
            this->cpe.resize(nen*3,nen*3,0.0);
            this->cte.resize(nen*3,nen*3,0.0);
            this->ctpe.resize(nen*3,nen*3,0.0);
            this->kte.resize(nen*3,nen*3,0.0);
            this->ktpe.resize(nen*3,nen*3,0.0);
            this->ktppe.resize(nen*3,nen*3,0.0);
            this->yte.resize(nen*3,nen,0.0);
            this->ytpe.resize(nen*3,nen*3,0.0);
            this->bte.resize(nen,nen*3,0.0);
            this->ye.resize(nen,nen*3,0.0);
            this->ype.resize(nen,nen*3,0.0);
            this->the.resize(nen,nen,0.0);
            this->epe.resize(nen*3,nen*3,0.0);

            this->fv.resize(nen*3,0.0);
            this->ftv.resize(nen*3,0.0);
            this->etv.resize(nen,0.0);
            this->mv.resize(nen*3,0.0);
            this->cv.resize(nen*3,0.0);
            this->kv.resize(nen*3,0.0);
            this->gv.resize(nen*3,0.0);
            this->gvT.resize(nen,0.0);
            this->ctv.resize(nen*3,0.0);
            this->ktv.resize(nen*3,0.0);
            this->ytv.resize(nen*3,0.0);
            this->btv.resize(nen,0.0);
            this->yv.resize(nen,0.0);
            this->thv.resize(nen,0.0);
            this->ev.resize(nen*3,0.0);

            this->Ae11.resize(nen*3,nen*3,0.0);
            this->Ae12.resize(nen*3,nen,0.0);
            this->Ae21.resize(nen,nen*3,0.0);
            this->Ae22.resize(nen,nen,0.0);
            this->be1.resize(nen*3,0.0);
            this->be2.resize(nen,0.0);

            this->initialized = true;
        }

        void clear(void)
        {
            this->me.fill(0.0);
            this->ce.fill(0.0);
            this->ke.fill(0.0);
            this->ge.fill(0.0);
            this->geT.fill(0.0);
            this->cpe.fill(0.0);
            this->cte.fill(0.0);
            this->ctpe.fill(0.0);
            this->kte.fill(0.0);
            this->ktpe.fill(0.0);
            this->ktppe.fill(0.0);
            this->yte.fill(0.0);
            this->ytpe.fill(0.0);
            this->bte.fill(0.0);
            this->ye.fill(0.0);
            this->ype.fill(0.0);
            this->the.fill(0.0);
            this->epe.fill(0.0);

            this->fv.fill(0.0);
            this->ftv.fill(0.0);
            this->etv.fill(0.0);
            this->mv.fill(0.0);
            this->cv.fill(0.0);
            this->kv.fill(0.0);
            this->gv.fill(0.0);
            this->gvT.fill(0.0);
            this->ctv.fill(0.0);
            this->ktv.fill(0.0);
            this->ytv.fill(0.0);
            this->btv.fill(0.0);
            this->yv.fill(0.0);
            this->thv.fill(0.0);
            this->ev.fill(0.0);

            this->Ae11.fill(0.0);
            this->Ae12.fill(0.0);
            this->Ae21.fill(0.0);
            this->Ae22.fill(0.0);
            this->be1.fill(0.0);
            this->be2.fill(0.0);
        }
};

void RSolverFluid::_init(const RSolverFluid *pSolver)
{
    if (pSolver)
    {
        this->freePressureNodeHeight = pSolver->freePressureNodeHeight;
        this->elementScales = pSolver->elementScales;
        this->elementPressure = pSolver->elementPressure;
        this->elementVelocity = pSolver->elementVelocity;
        this->elementGravity = pSolver->elementGravity;
        this->nodePressure = pSolver->nodePressure;
        this->nodeVelocity = pSolver->nodeVelocity;
        this->nodeVelocityOld = pSolver->nodeVelocityOld;
        this->nodeAcceleration = pSolver->nodeAcceleration;
        this->streamVelocity = pSolver->streamVelocity;
        this->invStreamVelocity = pSolver->invStreamVelocity;
        this->elementDensity = pSolver->elementDensity;
        this->elementViscosity = pSolver->elementViscosity;
        this->avgRo = pSolver->avgRo;
        this->avgU = pSolver->avgU;
        this->cvgV = pSolver->cvgV;
        this->cvgP = pSolver->cvgP;
    }
    else
    {
        this->nodeAcceleration.x.resize(this->pModel->getNNodes(),0.0);
        this->nodeAcceleration.y.resize(this->pModel->getNNodes(),0.0);
        this->nodeAcceleration.z.resize(this->pModel->getNNodes(),0.0);
        this->nodeVelocity.x.resize(this->pModel->getNNodes(),0.0);
        this->nodeVelocity.y.resize(this->pModel->getNNodes(),0.0);
        this->nodeVelocity.z.resize(this->pModel->getNNodes(),0.0);
        this->nodeVelocityOld.x.resize(this->pModel->getNNodes(),0.0);
        this->nodeVelocityOld.y.resize(this->pModel->getNNodes(),0.0);
        this->nodeVelocityOld.z.resize(this->pModel->getNNodes(),0.0);
        this->nodePressure.resize(this->pModel->getNNodes(),0.0);
    }
}

RSolverFluid::RSolverFluid(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData)
    : RSolverGeneric(pModel,modelFileName,convergenceFileName,sharedData)
    , streamVelocity(1.0)
    , invStreamVelocity(1.0)
    , cvgV(0.0)
    , cvgP(0.0)
{
    this->problemType = R_PROBLEM_FLUID;
    this->_init();
}

RSolverFluid::RSolverFluid(const RSolverFluid &solver)
    : RSolverGeneric(solver)
{
    this->_init(&solver);
}

RSolverFluid::~RSolverFluid()
{
    this->clearShapeDerivatives();
}

RSolverFluid &RSolverFluid::operator =(const RSolverFluid &solver)
{
    RSolverGeneric::operator =(solver);
    this->_init(&solver);
    return (*this);
}

bool RSolverFluid::hasConverged(void) const
{
    return false;
}

void RSolverFluid::updateScales(void)
{
    this->nodeVelocity.x.resize(this->pModel->getNNodes(),0.0);
    this->nodeVelocity.y.resize(this->pModel->getNNodes(),0.0);
    this->nodeVelocity.z.resize(this->pModel->getNNodes(),0.0);
    this->elementVelocity.x.resize(this->pModel->getNElements(),0.0);
    this->elementVelocity.y.resize(this->pModel->getNElements(),0.0);
    this->elementVelocity.z.resize(this->pModel->getNElements(),0.0);
    this->nodeAcceleration.x.resize(this->pModel->getNNodes(),0.0);
    this->nodeAcceleration.y.resize(this->pModel->getNNodes(),0.0);
    this->nodeAcceleration.z.resize(this->pModel->getNNodes(),0.0);
    this->nodeVelocityOld.x.resize(this->pModel->getNNodes(),0.0);
    this->nodeVelocityOld.y.resize(this->pModel->getNNodes(),0.0);
    this->nodeVelocityOld.z.resize(this->pModel->getNNodes(),0.0);
    this->elementPressure.resize(this->pModel->getNElements(),0.0);

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
    this->scales.setSecond(this->findTimeScale());
    this->scales.setKilogram(this->findWeightScale());
}

void RSolverFluid::recover(void)
{
    this->recoveryStopWatch.reset();
    this->recoveryStopWatch.resume();

    this->recoverVariable(R_VARIABLE_VELOCITY,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),0,this->nodeVelocity.x,0.0);
    this->recoverVariable(R_VARIABLE_VELOCITY,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),1,this->nodeVelocity.y,0.0);
    this->recoverVariable(R_VARIABLE_VELOCITY,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),2,this->nodeVelocity.z,0.0);
//    this->recoverVariable(R_VARIABLE_ACCELERATION,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),0,this->nodeAcceleration.x);
//    this->recoverVariable(R_VARIABLE_ACCELERATION,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),1,this->nodeAcceleration.y);
//    this->recoverVariable(R_VARIABLE_ACCELERATION,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),2,this->nodeAcceleration.z);
    this->recoverVariable(R_VARIABLE_PRESSURE,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),0,this->nodePressure,0.0);

    this->recoveryStopWatch.pause();
}

void RSolverFluid::prepare(void)
{
    RLogger::info("Building matrix system\n");
    RLogger::indent();

    this->buildStopWatch.reset();

    if (this->taskIteration == 0 || this->meshChanged)
    {
        this->generateNodeBook();
        this->generateMaterialVecor(R_MATERIAL_PROPERTY_DENSITY,this->elementDensity);
        this->generateMaterialVecor(R_MATERIAL_PROPERTY_DYNAMIC_VISCOSITY,this->elementViscosity);

        this->findInputVectors();
    }

    this->pModel->convertNodeToElementVector(this->nodePressure,this->elementPressure);
    this->pModel->convertNodeToElementVector(this->nodeVelocity.x,this->elementVelocity.x);
    this->pModel->convertNodeToElementVector(this->nodeVelocity.y,this->elementVelocity.y);
    this->pModel->convertNodeToElementVector(this->nodeVelocity.z,this->elementVelocity.z);

    if (this->meshChanged)
    {
        this->computeElementScales();
        this->computeShapeDerivatives();
    }
    if (this->taskIteration == 0)
    {
        this->computeFreePressureNodeHeight();
        this->streamVelocity = RSolverFluid::computeStreamVelocity(*this->pModel,this->nodeVelocity,false);
        this->invStreamVelocity = 1.0 / this->streamVelocity;
    }

    RRVector elementFreePressure;
    RBVector elementFreePressureSetValues;
    this->computeElementFreePressure(elementFreePressure,elementFreePressureSetValues);

    this->A.clear();
    this->A.setNRows(this->nodeBook.getNEnabled());
    this->A.reserveNColumns(100);
    this->b.resize(this->nodeBook.getNEnabled());
    this->b.fill(0.0);
    this->x.resize(this->nodeBook.getNEnabled());
    this->x.fill(0.0);

#ifdef _OPTIMAL_ASSEMBLY_
    int np = omp_get_max_threads();

    QVector<RSparseMatrix> Ap;
    Ap.resize(np);
    QVector<RRVector> bp;
    bp.resize(np);

    for (int i=0;i<np;i++)
    {
        Ap[i].setNRows(this->nodeBook.getNEnabled());
        Ap[i].reserveNColumns(100);
        bp[i].resize(this->nodeBook.getNEnabled());
        bp[i].fill(0.0);
    }
#endif /* _OPTIMAL_ASSEMBLY_ */

    bool abort = false;

    RMatrixManager<FluidMatrixContainer> matrixManager;

    if (this->meshChanged)
    {
        this->elementNormals.resize(this->pModel->getNElements(),RR3Vector(0.0,0.0,0.0));
        this->elementGravityMagnitude.resize(this->pModel->getNElements());
    }

    this->buildStopWatch.resume();

    // Compute element matrices
    #pragma omp parallel for default(shared) private(matrixManager)
    for (int64_t i=0;i<int64_t(this->pModel->getNElements());i++)
    {
        uint elementID = uint(i);

        const RElement &element = this->pModel->getElement(elementID);

        #pragma omp flush (abort)
        if (abort)
        {
            continue;
        }
        try
        {
            uint nen = element.size();
            uint nInp = RElement::getNIntegrationPoints(element.getType());

            RRMatrix Ae(nen*4,nen*4,0.0);
            RRVector be(nen*4,0.0);

            if (R_ELEMENT_TYPE_IS_SURFACE(element.getType()))
            {
                if (!elementFreePressureSetValues[elementID])
                {
                    continue;
                }

                RR3Vector &normal = this->elementNormals[elementID];
                if (this->meshChanged)
                {
                    // no need to recalculate if mesh does not change !!!
                    element.findNormal(this->pModel->getNodes(),normal[0],normal[1],normal[2]);
                    this->elementGravityMagnitude[elementID] = std::sqrt(  std::pow(elementGravity.x[elementID],2)
                                                                         + std::pow(elementGravity.y[elementID],2)
                                                                         + std::pow(elementGravity.z[elementID],2));
                }

                double ro = this->elementDensity[elementID];

                double fp = elementFreePressure[elementID];
                double gm = this->elementGravityMagnitude[elementID];

                for (uint intPoint=0;intPoint<nInp;intPoint++)
                {
                    const RElementShapeFunction &shapeFunc = RElement::getShapeFunction(element.getType(),intPoint);
                    const RRVector &N = shapeFunc.getN();
                    double detJ = this->shapeDerivations[elementID]->getJacobian(intPoint);
                    double integValue = detJ * shapeFunc.getW();

                    for (uint m=0;m<element.size();m++)
                    {
                        double nh = this->freePressureNodeHeight[element.getNodeId(m)];

                        // Pressure vector
                        double value = N[m] * (fp + ro * gm * nh) * integValue;
                        be[4*m+0] -= value * normal[0];
                        be[4*m+1] -= value * normal[1];
                        be[4*m+2] -= value * normal[2];
                    }
                }
                if (this->pModel->getTimeSolver().getEnabled())
                {
                    be *= this->pModel->getTimeSolver().getCurrentTimeStepSize();
                }
            }

            if (R_ELEMENT_TYPE_IS_VOLUME(element.getType()))
            {
                if (!this->computableElements[elementID])
                {
                    continue;
                }
                this->computeElement(elementID,Ae,be,matrixManager);
            }
            this->applyLocalRotations(elementID,Ae);
#ifdef _OPTIMAL_ASSEMBLY_
            this->assemblyMatrix(elementID,Ae,be,Ap[omp_get_thread_num()],bp[omp_get_thread_num()]);
#else
            this->assemblyMatrix(elementID,Ae,be);
#endif
        }
        catch (const RError &rError)
        {
            #pragma omp critical
            {
                RLogger::error("%s\n",rError.getMessage().toUtf8().constData());
                abort = true;
            }
            #pragma omp flush (abort)
        }
    }

#ifdef _OPTIMAL_ASSEMBLY_
#pragma omp parallel for default(shared)
    for (int64_t i=0;i<int64_t(this->A.getNRows());i++)
    {
        for (int j=0;j<np;j++)
        {
            A.getVector(uint(i)).addVector(Ap[j].getVector(uint(i)));
            this->b[uint(i)] += bp[j][uint(i)];
        }
    }
#endif

    this->buildStopWatch.pause();

    if (abort)
    {
        RLogger::unindent();
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to prepare matrix system.");
    }

    RLogger::unindent();
}

void RSolverFluid::solve(void)
{
    RLogger::info("Solving matrix system\n");
    RLogger::indent();

    this->solverStopWatch.reset();
    this->solverStopWatch.resume();

    try
    {
        RLogger::indent();
        RMatrixSolver matrixSolver(this->pModel->getMatrixSolverConf(RMatrixSolverConf::GMRES));
        matrixSolver.solve(this->A,this->b,this->x,R_MATRIX_PRECONDITIONER_JACOBI,1);
        RLogger::unindent();
    }
    catch (RError error)
    {
        RLogger::unindent();
        throw error;
    }

    this->solverStopWatch.pause();

    this->nodeVelocity.x.resize(this->pModel->getNNodes(),0.0);
    this->nodeVelocity.y.resize(this->pModel->getNNodes(),0.0);
    this->nodeVelocity.z.resize(this->pModel->getNNodes(),0.0);
    this->nodePressure.resize(this->pModel->getNNodes(),0.0);
    this->nodeAcceleration.x.resize(this->pModel->getNNodes(),0.0);
    this->nodeAcceleration.y.resize(this->pModel->getNNodes(),0.0);
    this->nodeAcceleration.z.resize(this->pModel->getNNodes(),0.0);

    this->updateStopWatch.reset();
    this->updateStopWatch.resume();

    double uOld = 0.0;
    for (uint i=0;i<this->nodeVelocity.x.size();i++)
    {
        RR3Vector vec(this->nodeVelocity.x[i],this->nodeVelocity.y[i],this->nodeVelocity.z[i]);
        uOld += RRVector::dot(vec,vec);
    }
    uOld = std::sqrt(uOld);
    double pOld = RRVector::norm(this->nodePressure);

    if (!this->pModel->getTimeSolver().getEnabled())
    {
        this->nodeVelocityOld.x = this->nodeVelocity.x;
        this->nodeVelocityOld.y = this->nodeVelocity.y;
        this->nodeVelocityOld.z = this->nodeVelocity.z;
    }

    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        uint position = 0;
        double dvx = 0.0;
        double dvy = 0.0;
        double dvz = 0.0;
        double dp = 0.0;

        if (this->nodeBook.getValue(4*i+0,position))
        {
            dvx = this->x[position];
        }
        if (this->nodeBook.getValue(4*i+1,position))
        {
            dvy = this->x[position];
        }
        if (this->nodeBook.getValue(4*i+2,position))
        {
            dvz = this->x[position];
        }
        if (this->nodeBook.getValue(4*i+3,position))
        {
            dp = this->x[position];
        }
        if (this->localRotations[i].isActive())
        {
            RR3Vector v(dvx,dvy,dvz);
            this->localRotations[i].rotateResultsVector(v);
            dvx = v[0];
            dvy = v[1];
            dvz = v[2];
        }
        this->nodeVelocity.x[i] += dvx;
        this->nodeVelocity.y[i] += dvy;
        this->nodeVelocity.z[i] += dvz;
        this->nodePressure[i] += dp;
    }
    if (this->pModel->getTimeSolver().getEnabled())
    {
        double dt = this->pModel->getTimeSolver().getCurrentTimeStepSize();
        for (uint i=0;i<this->pModel->getNNodes();i++)
        {
            this->nodeAcceleration.x[i] = (this->nodeVelocity.x[i] - this->nodeVelocityOld.x[i]) / dt;
            this->nodeAcceleration.y[i] = (this->nodeVelocity.y[i] - this->nodeVelocityOld.y[i]) / dt;
            this->nodeAcceleration.z[i] = (this->nodeVelocity.z[i] - this->nodeVelocityOld.z[i]) / dt;
        }
    }

    double u = 0.0;
    for (uint i=0;i<this->nodeVelocity.x.size();i++)
    {
        RR3Vector vec(this->nodeVelocity.x[i],this->nodeVelocity.y[i],this->nodeVelocity.z[i]);
        u += RRVector::dot(vec,vec);
    }
    u = std::sqrt(u);
    double p = RRVector::norm(this->nodePressure);

    this->cvgV = (u - uOld) / this->scales.findScaleFactor(R_VARIABLE_VELOCITY);
    this->cvgP = (p - pOld) / this->scales.findScaleFactor(R_VARIABLE_PRESSURE);

    this->updateStopWatch.pause();

    RLogger::unindent();
}

void RSolverFluid::process(void)
{

}

void RSolverFluid::store(void)
{
    RLogger::info("Storing results\n");
    RLogger::indent();

    // Velocity
    uint velocityPos = this->pModel->findVariable(R_VARIABLE_VELOCITY);
    if (velocityPos == RConstants::eod)
    {
        velocityPos = this->pModel->addVariable(R_VARIABLE_VELOCITY);

        double umin = 0.0;
        double umax = 0.0;
        for (uint i=0;i<this->pModel->getNNodes();i++)
        {
            double u = RR3Vector(this->nodeVelocity.x[i],
                                 this->nodeVelocity.y[i],
                                 this->nodeVelocity.z[i]).length();
            if (i == 0)
            {
                umin = umax = u;
            }
            else
            {
                umin = std::min(umin,u);
                umax = std::max(umax,u);
            }
        }

        this->pModel->getVariable(velocityPos).getVariableData().setMinMaxDisplayValue(umin,umax);
    }
    RVariable &velocity =  this->pModel->getVariable(velocityPos);

    velocity.setApplyType(R_VARIABLE_APPLY_NODE);
    velocity.resize(3,this->pModel->getNNodes());
    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        velocity.setValue(0,i,this->nodeVelocity.x[i]);
        velocity.setValue(1,i,this->nodeVelocity.y[i]);
        velocity.setValue(2,i,this->nodeVelocity.z[i]);
    }

//    // Acceleration
//    uint accelerationPos = this->pModel->findVariable(R_VARIABLE_ACCELERATION);
//    if (accelerationPos == RConstants::eod)
//    {
//        accelerationPos = this->pModel->addVariable(R_VARIABLE_ACCELERATION);

//        double amin = 0.0;
//        double amax = 0.0;
//        for (uint i=0;i<this->nodeVelocity.x.size();i++)
//        {
//            double u = RR3Vector(this->nodeAcceleration.x[i],
//                                 this->nodeAcceleration.y[i],
//                                 this->nodeAcceleration.z[i]).length();
//            if (i == 0)
//            {
//                amin = amax = u;
//            }
//            else
//            {
//                amin = std::min(amin,u);
//                amax = std::max(amax,u);
//            }
//        }

//        this->pModel->getVariable(accelerationPos).getVariableData().setMinMaxDisplayValue(amin,amax);
//    }
//    RVariable &acceleration =  this->pModel->getVariable(accelerationPos);

//    acceleration.setApplyType(R_VARIABLE_APPLY_NODE);
//    acceleration.resize(3,this->pModel->getNNodes());
//    for (uint i=0;i<this->pModel->getNNodes();i++)
//    {
//        acceleration.setValue(0,i,this->nodeAcceleration.x[i]);
//        acceleration.setValue(1,i,this->nodeAcceleration.y[i]);
//        acceleration.setValue(2,i,this->nodeAcceleration.z[i]);
//    }

    // Pressure
    uint pressurePos = this->pModel->findVariable(R_VARIABLE_PRESSURE);
    if (pressurePos == RConstants::eod)
    {
        pressurePos = this->pModel->addVariable(R_VARIABLE_PRESSURE);

        this->pModel->getVariable(pressurePos).getVariableData().setMinMaxDisplayValue(
                    RStatistics::findMinimumValue(this->nodePressure),
                    RStatistics::findMaximumValue(this->nodePressure));
    }
    RVariable &pressure =  this->pModel->getVariable(pressurePos);

    pressure.setApplyType(R_VARIABLE_APPLY_NODE);
    pressure.resize(1,this->pModel->getNNodes());
    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        pressure.setValue(0,i,this->nodePressure[i]);
    }

    RLogger::unindent();
}

void RSolverFluid::statistics(void)
{
    static uint counter = 0;
    static double oldResidual = 0.0;

    double scale = std::pow(this->scales.getSecond(),2) / this->scales.getKilogram();
    double residual = RRVector::norm(this->b)*scale;
    double convergence = residual - oldResidual;
    oldResidual = residual;

    std::vector<RIterationInfoValue> cvgValues;
    cvgValues.push_back(RIterationInfoValue("Solver residual",residual));
    cvgValues.push_back(RIterationInfoValue("Solver convergence",convergence));
    cvgValues.push_back(RIterationInfoValue("Velocity convergence",this->cvgV));
    cvgValues.push_back(RIterationInfoValue("Pressure convergence",this->cvgP));

    RIterationInfo::writeToFile(this->convergenceFileName,counter,cvgValues);

    this->printStats(R_VARIABLE_VELOCITY);
    this->printStats(R_VARIABLE_PRESSURE);
    this->processMonitoringPoints();

    RLogger::info("Convergence:   %-13g\n",residual);

    RLogger::info("Build time:        %9u [ms]\n",this->buildStopWatch.getMiliSeconds());
    RLogger::info("Solver time:       %9u [ms]\n",this->solverStopWatch.getMiliSeconds());
    RLogger::info("Update time:       %9u [ms]\n",this->updateStopWatch.getMiliSeconds());

    counter++;
}

void RSolverFluid::findInputVectors(void)
{
    RBVector elementVelocitySetValues(this->pModel->getNElements(),false);
    RBVector elementPressureSetValues(this->pModel->getNElements(),false);

    this->elementVelocity.x.resize(this->pModel->getNElements(),0.0);
    this->elementVelocity.y.resize(this->pModel->getNElements(),0.0);
    this->elementVelocity.z.resize(this->pModel->getNElements(),0.0);
    this->nodeAcceleration.x.resize(this->pModel->getNNodes(),0.0);
    this->nodeAcceleration.y.resize(this->pModel->getNNodes(),0.0);
    this->nodeAcceleration.z.resize(this->pModel->getNNodes(),0.0);
    this->nodeVelocityOld.x.resize(this->pModel->getNNodes(),0.0);
    this->nodeVelocityOld.y.resize(this->pModel->getNNodes(),0.0);
    this->nodeVelocityOld.z.resize(this->pModel->getNNodes(),0.0);
    this->elementPressure.resize(this->pModel->getNElements(),0.0);

    // Apply initial conditions
    for (uint i=0;i<this->pModel->getNElementGroups();i++)
    {
        const RElementGroup *pElementGroup = this->pModel->getElementGroupPtr(i);
        if (!pElementGroup)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Element group could not be found (%u of %u).",i,this->pModel->getNElementGroups());
        }

        if (this->firstRun)
        {
            // Apply initial conditions
            for (uint j=0;j<pElementGroup->getNInitialConditions();j++)
            {
                RR3Vector velocity(0.0,0.0,0.0);
                double pressure = 0.0;

                bool velocitySet = false;
                bool pressureSet = false;

                const RInitialCondition &ic = pElementGroup->getInitialCondition(j);

                if (ic.getType() == R_INITIAL_CONDITION_VELOCITY)
                {
                    uint icComponentPosition = ic.findComponentPosition(R_VARIABLE_VELOCITY_X);
                    if (icComponentPosition != RConstants::eod)
                    {
                        const RConditionComponent &conditionComponent = ic.getComponent(icComponentPosition);
                        velocity[0] = conditionComponent.get(this->pModel->getTimeSolver().getCurrentTime());
                    }

                    icComponentPosition = ic.findComponentPosition(R_VARIABLE_VELOCITY_Y);
                    if (icComponentPosition != RConstants::eod)
                    {
                        const RConditionComponent &conditionComponent = ic.getComponent(icComponentPosition);
                        velocity[1] = conditionComponent.get(this->pModel->getTimeSolver().getCurrentTime());
                    }

                    icComponentPosition = ic.findComponentPosition(R_VARIABLE_VELOCITY_Z);
                    if (icComponentPosition != RConstants::eod)
                    {
                        const RConditionComponent &conditionComponent = ic.getComponent(icComponentPosition);
                        velocity[2] = conditionComponent.get(this->pModel->getTimeSolver().getCurrentTime());
                    }
                    velocitySet = true;
                }
                else if (ic.getType() == R_INITIAL_CONDITION_PRESSURE)
                {
                    uint icComponentPosition = ic.findComponentPosition(R_VARIABLE_PRESSURE);
                    if (icComponentPosition == RConstants::eod)
                    {
                        continue;
                    }
                    const RConditionComponent &conditionComponent = ic.getComponent(icComponentPosition);
                    pressure = conditionComponent.get(this->pModel->getTimeSolver().getCurrentTime());
                    pressureSet = true;
                }

                if (!velocitySet && !pressureSet)
                {
                    continue;
                }

                for (uint k=0;k<pElementGroup->size();k++)
                {
                    if (velocitySet)
                    {
                        this->elementVelocity.x[pElementGroup->get(k)] = velocity[0];
                        this->elementVelocity.y[pElementGroup->get(k)] = velocity[1];
                        this->elementVelocity.z[pElementGroup->get(k)] = velocity[2];
                        elementVelocitySetValues[pElementGroup->get(k)] = true;
                    }
                    if (pressureSet)
                    {
                        this->elementPressure[pElementGroup->get(k)] = pressure;
                        elementPressureSetValues[pElementGroup->get(k)] = true;
                    }
                }
            }
        }
    }

    RBVector elementWall(this->pModel->getNElements(),false);
    RBVector elementFrictionlessWall(this->pModel->getNElements(),false);

    // Apply boundary conditions
    for (uint i=0;i<this->pModel->getNElementGroups();i++)
    {
        const RElementGroup *pElementGroup = this->pModel->getElementGroupPtr(i);
        if (!pElementGroup)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Element group could not be found (%u of %u).",i,this->pModel->getNElementGroups());
        }
        // Apply boundary conditions
        for (uint j=0;j<pElementGroup->getNBoundaryConditions();j++)
        {
            RR3Vector velocity(0.0,0.0,0.0);
            double pressure = 0.0;

            bool wallSet = false;
            bool frictionlessWallSet = false;
            bool velocitySet = false;
            bool pressureSet = false;

            const RBoundaryCondition &bc = pElementGroup->getBoundaryCondition(j);

            if (bc.getType() == R_BOUNDARY_CONDITION_WALL)
            {
                wallSet = true;
                velocitySet = true;
            }
            else if (bc.getType() == R_BOUNDARY_CONDITION_WALL_FRICTIONLESS)
            {
                frictionlessWallSet = true;
            }
            else if (bc.getType() == R_BOUNDARY_CONDITION_INFLOW_VELOCITY)
            {
                uint bcComponentPosition = bc.findComponentPosition(R_VARIABLE_VELOCITY);
                if (bcComponentPosition == RConstants::eod)
                {
                    continue;
                }
                const RConditionComponent &conditionComponent = bc.getComponent(bcComponentPosition);
                double value = conditionComponent.get(this->pModel->getTimeSolver().getCurrentTime());
                RR3Vector normal;

                const RSurface *pSurface = static_cast<const RSurface*>(pElementGroup);
                pSurface->findAverageNormal(this->pModel->getNodes(),this->pModel->getElements(),normal);

                if (pSurface->size() > 0)
                {
                    if (!this->inwardElements[pSurface->get(0)])
                    {
                        normal *= -1.0;
                    }
                }

                velocity[0] = value * normal[0];
                velocity[1] = value * normal[1];
                velocity[2] = value * normal[2];
                velocitySet = true;
            }
            else if (bc.getType() == R_BOUNDARY_CONDITION_INFLOW_VOLURATE)
            {
                uint bcComponentPosition = bc.findComponentPosition(R_VARIABLE_VOLUME_FLOW_RATE);
                if (bcComponentPosition == RConstants::eod)
                {
                    continue;
                }
                const RConditionComponent &conditionComponent = bc.getComponent(bcComponentPosition);
                double volumetricFlowRate = conditionComponent.get(this->pModel->getTimeSolver().getCurrentTime());

                const RSurface *pSurface = static_cast<const RSurface*>(pElementGroup);

                double area = pSurface->findArea(this->pModel->getNodes(),this->pModel->getElements());
                double value = volumetricFlowRate / area;

                RR3Vector normal;
                pSurface->findAverageNormal(this->pModel->getNodes(),this->pModel->getElements(),normal);

                if (pSurface->size() > 0)
                {
                    if (!this->inwardElements[pSurface->get(0)])
                    {
                        normal *= -1.0;
                    }
                }

                velocity[0] = value * normal[0];
                velocity[1] = value * normal[1];
                velocity[2] = value * normal[2];
                velocitySet = true;
            }
            else if (bc.getType() == R_BOUNDARY_CONDITION_PRESSURE_EXPLICIT)
            {
                uint bcComponentPosition = bc.findComponentPosition(R_VARIABLE_PRESSURE);
                if (bcComponentPosition == RConstants::eod)
                {
                    continue;
                }
                const RConditionComponent &conditionComponent = bc.getComponent(bcComponentPosition);
                pressure = conditionComponent.get(this->pModel->getTimeSolver().getCurrentTime());
                pressureSet = true;
            }
//            else if (bc.getType() == R_BOUNDARY_CONDITION_PRESSURE_IMPLICIT)
//            {
//                uint bcComponentPosition = bc.findComponentPosition(R_VARIABLE_PRESSURE);
//                if (bcComponentPosition == RConstants::eod)
//                {
//                    continue;
//                }
//                const RConditionComponent &conditionComponent = bc.getComponent(bcComponentPosition);
//                pressure = conditionComponent.get(this->pModel->getTimeSolver().getCurrentTime());
//                pressureSet = true;
//            }

            if (!wallSet && !frictionlessWallSet && !velocitySet && !pressureSet)
            {
                continue;
            }

            for (uint k=0;k<pElementGroup->size();k++)
            {
                elementWall[pElementGroup->get(k)] = wallSet;
                elementFrictionlessWall[pElementGroup->get(k)] = frictionlessWallSet;

                if (velocitySet)
                {
                    this->elementVelocity.x[pElementGroup->get(k)] = velocity[0];
                    this->elementVelocity.y[pElementGroup->get(k)] = velocity[1];
                    this->elementVelocity.z[pElementGroup->get(k)] = velocity[2];
                    elementVelocitySetValues[pElementGroup->get(k)] = true;
                }
                if (pressureSet)
                {
                    this->elementPressure[pElementGroup->get(k)] = pressure;
                    elementPressureSetValues[pElementGroup->get(k)] = true;
                }
            }
        }
    }

    RBVector elementGravitySetValues;

    this->generateVariableVector(R_VARIABLE_G_ACCELERATION_X,this->elementGravity.x,elementGravitySetValues,true,true,true);
    this->generateVariableVector(R_VARIABLE_G_ACCELERATION_Y,this->elementGravity.y,elementGravitySetValues,true,true,true);
    this->generateVariableVector(R_VARIABLE_G_ACCELERATION_Z,this->elementGravity.z,elementGravitySetValues,true,true,true);

    this->pModel->convertElementToNodeVector(this->elementVelocity.x,elementVelocitySetValues,this->nodeVelocity.x,true);
    this->pModel->convertElementToNodeVector(this->elementVelocity.y,elementVelocitySetValues,this->nodeVelocity.y,true);
    this->pModel->convertElementToNodeVector(this->elementVelocity.z,elementVelocitySetValues,this->nodeVelocity.z,true);
    this->pModel->convertElementToNodeVector(this->elementPressure,elementPressureSetValues,this->nodePressure,true);

    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        const RElement &rElement = this->pModel->getElement(i);
        if (elementWall[i])
        {
            for (uint j=0;j<rElement.size();j++)
            {
                this->nodeVelocity.x[rElement.getNodeId(j)] = 0.0;
                this->nodeVelocity.y[rElement.getNodeId(j)] = 0.0;
                this->nodeVelocity.z[rElement.getNodeId(j)] = 0.0;
            }
        }
        if (elementFrictionlessWall[i])
        {
            RR3Vector elementNormal;
            rElement.findNormal(this->pModel->getNodes(),elementNormal[0],elementNormal[1],elementNormal[2]);

            bool hasX = false;
            bool hasY = false;
            bool hasZ = false;

            if (std::fabs(elementNormal[0]) > std::fabs(elementNormal[1]) && std::fabs(elementNormal[0]) > std::fabs(elementNormal[2]))
            {
                hasX = true;
            }
            else
            {
                if (std::fabs(elementNormal[1]) > std::fabs(elementNormal[0]) && std::fabs(elementNormal[1]) > std::fabs(elementNormal[2]))
                {
                    hasY = true;
                }
                else
                {
                    hasZ = true;
                }
            }

            for (uint j=0;j<rElement.size();j++)
            {
                if (hasX)
                {
                    this->nodeVelocity.x[rElement.getNodeId(j)] = 0.0;
                }
                if (hasY)
                {
                    this->nodeVelocity.y[rElement.getNodeId(j)] = 0.0;
                }
                if (hasZ)
                {
                    this->nodeVelocity.z[rElement.getNodeId(j)] = 0.0;
                }
            }
        }
    }

    this->nodeVelocityOld.x = this->nodeVelocity.x;
    this->nodeVelocityOld.y = this->nodeVelocity.y;
    this->nodeVelocityOld.z = this->nodeVelocity.z;

    this->nodeAcceleration.x.resize(this->pModel->getNNodes(),0.0);
    this->nodeAcceleration.y.resize(this->pModel->getNNodes(),0.0);
    this->nodeAcceleration.z.resize(this->pModel->getNNodes(),0.0);
}

void RSolverFluid::generateNodeBook(void)
{
    this->nodeBook.resize(this->pModel->getNNodes()*4);
    this->nodeBook.initialize();

    for (uint i=0;i<this->pModel->getNElementGroups();i++)
    {
        const RElementGroup *pElementGroup = this->pModel->getElementGroupPtr(i);
        if (!pElementGroup)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Element group could not be found (%u of %u).",i,this->pModel->getNElementGroups());
        }
        bool hasVelocityX = false;
        bool hasVelocityY = false;
        bool hasVelocityZ = false;
        bool hasFriction = false;
        bool hasPressure = false;
        for (uint j=0;j<pElementGroup->getNBoundaryConditions();j++)
        {
            const RBoundaryCondition &bc = pElementGroup->getBoundaryCondition(j);
            if (RBoundaryCondition::getProblemTypeMask(bc.getType()) & R_PROBLEM_FLUID)
            {
                if (bc.getType() == R_BOUNDARY_CONDITION_INFLOW_VELOCITY ||
                    bc.getType() == R_BOUNDARY_CONDITION_INFLOW_VOLURATE ||
                    bc.getType() == R_BOUNDARY_CONDITION_WALL)
                {
                    hasVelocityX = true;
                    hasVelocityY = true;
                    hasVelocityZ = true;
                }
                if (bc.getType() == R_BOUNDARY_CONDITION_WALL_FRICTIONLESS)
                {
                    hasFriction = true;
                }
                if (bc.getType() == R_BOUNDARY_CONDITION_PRESSURE_EXPLICIT)
                {
                    hasPressure = true;
                }
            }
        }
        if (!hasVelocityX && !hasVelocityY && !hasVelocityZ && !hasFriction && !hasPressure)
        {
            continue;
        }
        for (int64_t j=0;j<pElementGroup->size();j++)
        {
            uint elementID = pElementGroup->get(uint(j));
            const RElement &rElement = this->pModel->getElement(elementID);

            bool elementHasVelocityX = hasVelocityX;
            bool elementHasVelocityY = hasVelocityY;
            bool elementHasVelocityZ = hasVelocityZ;

            if (hasFriction)
            {
                RR3Vector elementNormal;
                rElement.findNormal(this->pModel->getNodes(),elementNormal[0],elementNormal[1],elementNormal[2]);

                if (std::fabs(elementNormal[0]) > std::fabs(elementNormal[1]) && std::fabs(elementNormal[0]) > std::fabs(elementNormal[2]))
                {
                    elementHasVelocityX = true;
                }
                else
                {
                    if (std::fabs(elementNormal[1]) > std::fabs(elementNormal[0]) && std::fabs(elementNormal[1]) > std::fabs(elementNormal[2]))
                    {
                        elementHasVelocityY = true;
                    }
                    else
                    {
                        elementHasVelocityZ = true;
                    }
                }
            }
            for (uint k=0;k<rElement.size();k++)
            {
                uint nodeId = rElement.getNodeId(k);
                if (elementHasVelocityX)
                {
                    this->nodeBook.disable(4*nodeId+0,true);
                }
                if (elementHasVelocityY)
                {
                    this->nodeBook.disable(4*nodeId+1,true);
                }
                if (elementHasVelocityZ)
                {
                    this->nodeBook.disable(4*nodeId+2,true);
                }
                if (hasPressure)
                {
                    this->nodeBook.disable(4*nodeId+3,true);
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
            this->nodeBook.disable(4*i+0,true);
            this->nodeBook.disable(4*i+1,true);
            this->nodeBook.disable(4*i+2,true);
            this->nodeBook.disable(4*i+3,true);
        }
    }
}

void RSolverFluid::computeFreePressureNodeHeight(void)
{
    this->freePressureNodeHeight.resize(this->pModel->getNNodes());
    this->freePressureNodeHeight.fill(0.0);

    // Based on average gravity vector on surface elements.

    RR3Vector g(0.0,0.0,0.0);

    for (uint i=0;i<this->pModel->getNSurfaces();i++)
    {
        const RSurface &rSurface = this->pModel->getSurface(i);

        if (!rSurface.hasBoundaryCondition(R_BOUNDARY_CONDITION_PRESSURE_IMPLICIT))
        {
            continue;
        }

        if (rSurface.hasEnvironmentCondition(R_ENVIRONMENT_CONDITION_G_ACCELERATION))
        {
            const REnvironmentCondition &ec = rSurface.getEnvironmentCondition(R_ENVIRONMENT_CONDITION_G_ACCELERATION);
            uint componentPosition = 0;
            componentPosition = ec.findComponentPosition(R_VARIABLE_G_ACCELERATION_X);
            if (componentPosition != RConstants::eod)
            {
                const RConditionComponent &conditionComponent = ec.getComponent(componentPosition);
                g[0] += conditionComponent.get(this->pModel->getTimeSolver().getCurrentTime());
            }
            componentPosition = ec.findComponentPosition(R_VARIABLE_G_ACCELERATION_Y);
            if (componentPosition != RConstants::eod)
            {
                const RConditionComponent &conditionComponent = ec.getComponent(componentPosition);
                g[1] += conditionComponent.get(this->pModel->getTimeSolver().getCurrentTime());
            }
            componentPosition = ec.findComponentPosition(R_VARIABLE_G_ACCELERATION_Z);
            if (componentPosition != RConstants::eod)
            {
                const RConditionComponent &conditionComponent = ec.getComponent(componentPosition);
                g[2] += conditionComponent.get(this->pModel->getTimeSolver().getCurrentTime());
            }
        }
    }
    g.normalize();

    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        const RNode &rNode = this->pModel->getNode(i);
        this->freePressureNodeHeight[i] = rNode.getX()*g[0]
                                        + rNode.getY()*g[1]
                                        + rNode.getZ()*g[2];
    }

    double md = RStatistics::findMinimumValue(this->freePressureNodeHeight);

    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        this->freePressureNodeHeight[i] -= md;
        this->freePressureNodeHeight[i] = std::fabs(this->freePressureNodeHeight[i]);
    }
}

void RSolverFluid::computeShapeDerivatives(void)
{
    this->shapeDerivations.resize(this->pModel->getNElements(),nullptr);

    for (uint i=0;i<this->pModel->getNElements();i++)
    {
        uint elementID = i;

        const RElement &rElement = this->pModel->getElement(elementID);
//        if (R_ELEMENT_TYPE_IS_VOLUME(rElement.getType()))
        {
            if (!this->computableElements[elementID])
            {
                continue;
            }
            if (!this->shapeDerivations[elementID])
            {
                this->shapeDerivations[elementID] = new RElementShapeDerivation(rElement,this->pModel->getNodes(),R_PROBLEM_FLUID);
            }
        }
    }
}

void RSolverFluid::clearShapeDerivatives(void)
{
    for (uint i=0;i<this->shapeDerivations.size();i++)
    {
        delete this->shapeDerivations[i];
    }
}

void RSolverFluid::computeElement(unsigned int elementID, RRMatrix &Ae, RRVector &be, RMatrixManager<FluidMatrixContainer> &matrixManager)
{
    if (RElement::hasConstantDerivative(this->pModel->getElement(elementID).getType()))
    {
        this->computeElementConstantDerivative(elementID,Ae,be,matrixManager);
    }
    else
    {
        this->computeElementGeneral(elementID,Ae,be,matrixManager);
    }
}

void RSolverFluid::computeElementGeneral(unsigned int elementID, RRMatrix &Ae, RRVector &be, RMatrixManager<FluidMatrixContainer> &matrixManager)
{
    bool unsteady = (this->pModel->getTimeSolver().getEnabled());

    const RElement &element = this->pModel->getElement(elementID);
    uint nen = element.size();
    uint nInp = RElement::getNIntegrationPoints(element.getType());

    double ro = this->elementDensity[elementID];
    double invro = 1.0 / ro;
    double u = this->elementViscosity[elementID];

    Ae.fill(0.0);
    be.fill(0.0);

    FluidMatrixContainer &matrixCotainer = matrixManager.getMatricies(element.getType());
    matrixCotainer.clear();

    // Element level matricies
    RRMatrix &me = matrixCotainer.me;
    RRMatrix &ce = matrixCotainer.ce;
    RRMatrix &ke = matrixCotainer.ke;
    RRMatrix &ge = matrixCotainer.ge;
    RRMatrix &geT = matrixCotainer.geT;
    RRMatrix &cpe = matrixCotainer.cpe;
    RRMatrix &cte = matrixCotainer.cte;
    RRMatrix &ctpe = matrixCotainer.ctpe;
    RRMatrix &kte = matrixCotainer.kte;
    RRMatrix &ktpe = matrixCotainer.ktpe;
    RRMatrix &ktppe = matrixCotainer.ktppe;
    RRMatrix &yte = matrixCotainer.yte;
    RRMatrix &ytpe = matrixCotainer.ytpe;
    RRMatrix &bte = matrixCotainer.bte;
    RRMatrix &ye = matrixCotainer.ye;
    RRMatrix &ype = matrixCotainer.ype;
    RRMatrix &the = matrixCotainer.the;
    RRMatrix &epe = matrixCotainer.epe;

    // Element level vectors
    RRVector &fv = matrixCotainer.fv;
    RRVector &ftv = matrixCotainer.ftv;
    RRVector &etv = matrixCotainer.etv;
    RRVector &mv = matrixCotainer.mv;
    RRVector &cv = matrixCotainer.cv;
    RRVector &kv = matrixCotainer.kv;
    RRVector &gv = matrixCotainer.gv;
    RRVector &gvT = matrixCotainer.gvT;
    RRVector &ctv = matrixCotainer.ctv;
    RRVector &ktv = matrixCotainer.ktv;
    RRVector &ytv = matrixCotainer.ytv;
    RRVector &btv = matrixCotainer.btv;
    RRVector &yv = matrixCotainer.yv;
    RRVector &thv = matrixCotainer.thv;
    RRVector &ev = matrixCotainer.ev;

    // Element level equations
    RRMatrix &Ae11 = matrixCotainer.Ae11;
    RRMatrix &Ae12 = matrixCotainer.Ae12;
    RRMatrix &Ae21 = matrixCotainer.Ae21;
    RRMatrix &Ae22 = matrixCotainer.Ae22;
    RRVector &be1 = matrixCotainer.be1;
    RRVector &be2 = matrixCotainer.be2;

    double mVh = this->streamVelocity;
    double invmVh = this->invStreamVelocity;
    double alpha = this->pModel->getTimeSolver().getTimeMarchApproximationCoefficient();
    double dt = this->pModel->getTimeSolver().getCurrentTimeStepSize();
    double alphaDt = alpha * dt;

    // Element level input -------------------------------------------
    RR3Vector ve(this->elementVelocity.x[elementID],
                 this->elementVelocity.y[elementID],
                 this->elementVelocity.z[elementID]);
    RR3Vector veo(0.0,0.0,0.0);
    for (uint i=0;i<nen;i++)
    {
        veo[0] += this->nodeVelocityOld.x[element.getNodeId(i)];
        veo[1] += this->nodeVelocityOld.y[element.getNodeId(i)];
        veo[2] += this->nodeVelocityOld.z[element.getNodeId(i)];
    }
    veo *= 1.0/double(nen);
    RRVector ax(nen,0.0);
    RRVector ay(nen,0.0);
    RRVector az(nen,0.0);
    for (uint i=0;i<nen;i++)
    {
        ax[i] = this->nodeAcceleration.x[element.getNodeId(i)];
        ay[i] = this->nodeAcceleration.y[element.getNodeId(i)];
        az[i] = this->nodeAcceleration.z[element.getNodeId(i)];
    }
    RR3Vector g(this->elementGravity.x[elementID],
                this->elementGravity.y[elementID],
                this->elementGravity.z[elementID]);
    double p = this->elementPressure[elementID];
    // element level velocity magnitude
    double mvh = veo.length();
    double invmvh = 1.0 / mvh;
    // element level velocity direction
    RR3Vector s(veo);
    s.normalize();

    for (uint intPoint=0;intPoint<nInp;intPoint++)
    {
        const RElementShapeFunction &shapeFunc = RElement::getShapeFunction(element.getType(),intPoint);
        const RRVector &N = shapeFunc.getN();
        const RRMatrix &B = this->shapeDerivations[elementID]->getDerivative(intPoint);
        double detJ = this->shapeDerivations[elementID]->getJacobian(intPoint);

        double integValue = detJ * shapeFunc.getW();

        // velocity divergence
        RRVector vdiv(nen,0.0);
        // partial derivatives
        RR3Vector vex(0.0,0.0,0.0), vey(0.0,0.0,0.0), vez(0.0,0.0,0.0);
        double px = 0.0, py = 0.0, pz = 0.0;
        // element length scale
        double h = 0.0;
        double hn = this->elementScales[elementID];

        for (uint m=0;m<nen;m++)
        {
            vdiv[m] += ve[0] * B[m][0] + ve[1] * B[m][1] + ve[2] * B[m][2];

            vex[0] += B[m][0] * this->nodeVelocity.x[element.getNodeId(m)];
            vex[1] += B[m][0] * this->nodeVelocity.y[element.getNodeId(m)];
            vex[2] += B[m][0] * this->nodeVelocity.z[element.getNodeId(m)];

            vey[0] += B[m][1] * this->nodeVelocity.x[element.getNodeId(m)];
            vey[1] += B[m][1] * this->nodeVelocity.y[element.getNodeId(m)];
            vey[2] += B[m][1] * this->nodeVelocity.z[element.getNodeId(m)];

            vez[0] += B[m][2] * this->nodeVelocity.x[element.getNodeId(m)];
            vez[1] += B[m][2] * this->nodeVelocity.y[element.getNodeId(m)];
            vez[2] += B[m][2] * this->nodeVelocity.z[element.getNodeId(m)];

            px += B[m][0] * this->nodePressure[element.getNodeId(m)];
            py += B[m][1] * this->nodePressure[element.getNodeId(m)];
            pz += B[m][2] * this->nodePressure[element.getNodeId(m)];

            h += std::fabs(s[0]*B[m][0] + s[1]*B[m][1] + s[2]*B[m][2]);
        }
        if (h != 0.0)
        {
            h = 2.0/h;
        }
        // Reynolds numbers
        double roD2u = 0.5 * ro / u;
        double Re = roD2u * mvh * h;
        double Ren = roD2u * mVh * hn;

        // SUPG stabilization parameter
        double Tsupg = 0.0;
        if (mvh > 0.0)
        {
            if (Re > 0.0 && Re <= 3.0)
            {
                Tsupg = h * Re *invmvh * inv6;
            }
            else
            {
                Tsupg = h * invmvh * 0.5;
            }
        }

        // PSPG stabilization parameter
        double Tpspg = 0.0;
        if (mVh > 0.0)
        {
            if (Ren > 0.0 && Ren <= 3.0)
            {
                Tpspg = hn * Ren * invmVh * inv6;
            }
            else
            {
                Tpspg = hn * invmVh * 0.5;
            }
        }

        // LSIC stabilization parameter
        double Tlsic = mvh*h * 0.5;

        double value = 0.0;
        for (uint m=0;m<nen;m++)
        {
            for (uint n=0;n<nen;n++)
            {
                // m matrix
                if (unsteady)
                {
                    value = ro * N[m]*N[n];
                    me[3*m+0][3*m+0] = value;
                    me[3*m+1][3*m+1] = value;
                    me[3*m+2][3*m+2] = value;
                }
                // c matrix
                value = ro * N[m] * vdiv[n];
                ce[3*m+0][3*n+0] = value;
                ce[3*m+1][3*n+1] = value;
                ce[3*m+2][3*n+2] = value;
                // k matrix
                value = ro * u;
                double tmpValue = B[m][0] * B[n][0] + B[m][1] * B[n][1] + B[m][2] * B[n][2];
                ke[3*m+0][3*n+0] = value * (tmpValue + B[m][0] * B[n][0]);
                ke[3*m+1][3*n+0] = value * (           B[m][0] * B[n][1]);
                ke[3*m+2][3*n+0] = value * (           B[m][0] * B[n][2]);
                ke[3*m+0][3*n+1] = value * (           B[m][1] * B[n][0]);
                ke[3*m+1][3*n+1] = value * (tmpValue + B[m][1] * B[n][1]);
                ke[3*m+2][3*n+1] = value * (           B[m][1] * B[n][2]);
                ke[3*m+0][3*n+2] = value * (           B[m][2] * B[n][0]);
                ke[3*m+1][3*n+2] = value * (           B[m][2] * B[n][1]);
                ke[3*m+2][3*n+2] = value * (tmpValue + B[m][2] * B[n][2]);
                // g matrix
                ge[3*m+0][n] = B[m][0] * N[n];
                ge[3*m+1][n] = B[m][1] * N[n];
                ge[3*m+2][n] = B[m][2] * N[n];
                // gT matrix
                geT[n][3*m+0] = ge[3*m+0][n];
                geT[n][3*m+1] = ge[3*m+1][n];
                geT[n][3*m+2] = ge[3*m+2][n];
                // c+ matrix
                value = ro * N[m] * N[n];
                cpe[3*m+0][3*n+0] = value * vex[0];
                cpe[3*m+0][3*n+1] = value * vey[0];
                cpe[3*m+0][3*n+2] = value * vez[0];
                cpe[3*m+1][3*n+0] = value * vex[1];
                cpe[3*m+1][3*n+1] = value * vey[1];
                cpe[3*m+1][3*n+2] = value * vez[1];
                cpe[3*m+2][3*n+0] = value * vex[2];
                cpe[3*m+2][3*n+1] = value * vey[2];
                cpe[3*m+2][3*n+2] = value * vez[2];
                // c~ matrix
                if (unsteady)
                {
                    value = Tsupg * ro * vdiv[m] * N[n];
                    cte[3*m+0][3*n+0] = value;
                    cte[3*m+1][3*n+1] = value;
                    cte[3*m+2][3*n+2] = value;
                }
                // c~+ matrix
                if (unsteady)
                {
                    value = Tsupg * ro * N[n];
                    ctpe[3*m+0][3*n+0] = value * B[m][0] * ax[n];
                    ctpe[3*m+1][3*n+0] = value * B[m][0] * ay[n];
                    ctpe[3*m+2][3*n+0] = value * B[m][0] * az[n];
                    ctpe[3*m+0][3*n+1] = value * B[m][1] * ax[n];
                    ctpe[3*m+1][3*n+1] = value * B[m][1] * ay[n];
                    ctpe[3*m+2][3*n+1] = value * B[m][1] * az[n];
                    ctpe[3*m+0][3*n+2] = value * B[m][2] * ax[n];
                    ctpe[3*m+1][3*n+2] = value * B[m][2] * ay[n];
                    ctpe[3*m+2][3*n+2] = value * B[m][2] * az[n];
                }
                // k~ matrix
                value = Tsupg * ro * vdiv[m] * vdiv[n];
                kte[3*m+0][3*n+0] = value;
                kte[3*m+1][3*n+1] = value;
                kte[3*m+2][3*n+2] = value;
                // k~+ matrix
                value = Tsupg * ro * vdiv[m] * N[n];
                ktpe[3*m+0][3*n+0] = value * vex[0];
                ktpe[3*m+0][3*n+1] = value * vey[0];
                ktpe[3*m+0][3*n+2] = value * vez[0];
                ktpe[3*m+1][3*n+0] = value * vex[1];
                ktpe[3*m+1][3*n+1] = value * vey[1];
                ktpe[3*m+1][3*n+2] = value * vez[1];
                ktpe[3*m+2][3*n+0] = value * vex[2];
                ktpe[3*m+2][3*n+1] = value * vey[2];
                ktpe[3*m+2][3*n+2] = value * vez[2];
                // k~++ matrix
                value = Tsupg * ro * N[n];
                ktppe[3*m+0][3*n+0] = value * B[m][0] * (ve[0]*vex[0] + ve[1]*vey[0] + ve[2]*vez[0]);
                ktppe[3*m+0][3*n+1] = value * B[m][1] * (ve[0]*vex[0] + ve[1]*vey[0] + ve[2]*vez[0]);
                ktppe[3*m+0][3*n+2] = value * B[m][2] * (ve[0]*vex[0] + ve[1]*vey[0] + ve[2]*vez[0]);

                ktppe[3*m+1][3*n+0] = value * B[m][0] * (ve[0]*vex[1] + ve[1]*vey[1] + ve[2]*vez[1]);
                ktppe[3*m+1][3*n+1] = value * B[m][1] * (ve[0]*vex[1] + ve[1]*vey[1] + ve[2]*vez[1]);
                ktppe[3*m+1][3*n+2] = value * B[m][2] * (ve[0]*vex[1] + ve[1]*vey[1] + ve[2]*vez[1]);

                ktppe[3*m+2][3*n+0] = value * B[m][0] * (ve[0]*vex[2] + ve[1]*vey[2] + ve[2]*vez[2]);
                ktppe[3*m+2][3*n+1] = value * B[m][1] * (ve[0]*vex[2] + ve[1]*vey[2] + ve[2]*vez[2]);
                ktppe[3*m+2][3*n+2] = value * B[m][2] * (ve[0]*vex[2] + ve[1]*vey[2] + ve[2]*vez[2]);
                // y~ matrix
                value = Tsupg * vdiv[m];
                yte[3*m+0][n] = value * B[n][0];
                yte[3*m+1][n] = value * B[n][1];
                yte[3*m+2][n] = value * B[n][2];
                // y~+ matrix
                value = Tsupg * N[n];
                ytpe[3*m+0][3*n+0] = value * B[m][0] * px;
                ytpe[3*m+0][3*n+1] = value * B[m][1] * px;
                ytpe[3*m+0][3*n+2] = value * B[m][2] * px;
                ytpe[3*m+1][3*n+0] = value * B[m][0] * py;
                ytpe[3*m+1][3*n+1] = value * B[m][1] * py;
                ytpe[3*m+1][3*n+2] = value * B[m][2] * py;
                ytpe[3*m+2][3*n+0] = value * B[m][0] * pz;
                ytpe[3*m+2][3*n+1] = value * B[m][1] * pz;
                ytpe[3*m+2][3*n+2] = value * B[m][2] * pz;
                // B matrix
                value = Tpspg * N[n];
                bte[m][3*n+0] = value * B[m][0];
                bte[m][3*n+1] = value * B[m][1];
                bte[m][3*n+2] = value * B[m][2];
                // y matrix
                value = Tpspg * vdiv[n];
                ye[m][3*n+0] = value * B[m][0];
                ye[m][3*n+1] = value * B[m][1];
                ye[m][3*n+2] = value * B[m][2];
                // y+ matrix
                value = Tpspg * N[n];
                ype[m][3*n+0] = value * (B[m][0]*vex[0] + B[m][1]*vex[1] + B[m][2]*vex[2]);
                ype[m][3*n+1] = value * (B[m][0]*vey[0] + B[m][1]*vey[1] + B[m][2]*vey[2]);
                ype[m][3*n+2] = value * (B[m][0]*vez[0] + B[m][1]*vez[1] + B[m][2]*vez[2]);
                // 0 matrix
                the[m][n] = Tpspg * ((B[m][0]*B[n][0] + B[m][1]*B[n][1] + B[m][2]*B[n][2]) * invro);
                // ep matrix
                value = Tlsic * ro;
                epe[3*m+0][3*n+0] = B[m][0] * B[n][0] * value;
                epe[3*m+0][3*n+1] = B[m][0] * B[n][1] * value;
                epe[3*m+0][3*n+2] = B[m][0] * B[n][2] * value;
                epe[3*m+1][3*n+0] = B[m][1] * B[n][0] * value;
                epe[3*m+1][3*n+1] = B[m][1] * B[n][1] * value;
                epe[3*m+1][3*n+2] = B[m][1] * B[n][2] * value;
                epe[3*m+2][3*n+0] = B[m][2] * B[n][0] * value;
                epe[3*m+2][3*n+1] = B[m][2] * B[n][1] * value;
                epe[3*m+2][3*n+2] = B[m][2] * B[n][2] * value;
            }
            // m vector
            value = N[m] * ro;
            if (unsteady)
            {
                mv[3*m+0] = value * ax[m];
                mv[3*m+1] = value * ay[m];
                mv[3*m+2] = value * az[m];
            }
            // c vector
            cv[3*m+0] = value * (ve[0]*vex[0] + ve[1]*vey[0] + ve[2]*vez[0]);
            cv[3*m+1] = value * (ve[0]*vex[1] + ve[1]*vey[1] + ve[2]*vez[1]);
            cv[3*m+2] = value * (ve[0]*vex[2] + ve[1]*vey[2] + ve[2]*vez[2]);
            // k vector
            kv[3*m+0] = u * (  B[m][0]*vex[0] + B[m][1]*vey[0] + B[m][2]*vez[0]
                             + B[m][0]*vex[0] + B[m][1]*vex[1] + B[m][2]*vex[2]);
            kv[3*m+1] = u * (  B[m][0]*vex[1] + B[m][1]*vey[1] + B[m][2]*vez[1]
                             + B[m][0]*vey[0] + B[m][1]*vey[1] + B[m][2]*vey[2]);
            kv[3*m+2] = u * (  B[m][0]*vex[2] + B[m][1]*vey[2] + B[m][2]*vez[2]
                             + B[m][0]*vez[0] + B[m][1]*vez[1] + B[m][2]*vez[2]);
            // g vector
            gv[3*m+0] = B[m][0] * p;
            gv[3*m+1] = B[m][1] * p;
            gv[3*m+2] = B[m][2] * p;
            // gT vector
            gvT[m] = N[m] * (vex[0] + vey[1] + vez[2]);
            // c~ vector
            value = Tsupg * vdiv[m];
            if (unsteady)
            {
                ctv[3*m+0] = value * ro * ax[m];
                ctv[3*m+1] = value * ro * ay[m];
                ctv[3*m+2] = value * ro * az[m];
            }
            // k~ vector
            ktv[3*m+0] = value * ro * (ve[0]*vex[0] + ve[1]*vey[0] + ve[2]*vez[0]);
            ktv[3*m+1] = value * ro * (ve[0]*vex[1] + ve[1]*vey[1] + ve[2]*vez[1]);
            ktv[3*m+2] = value * ro * (ve[0]*vex[2] + ve[1]*vey[2] + ve[2]*vez[2]);
            // y~ vector
            ytv[3*m+0] = value * px;
            ytv[3*m+1] = value * py;
            ytv[3*m+2] = value * pz;
            // B vector
            if (unsteady)
            {
                btv[m] = Tpspg * (B[m][0]*ax[m] + B[m][1]*ay[m] + B[m][2]*az[m]);
            }
            // y vector
            yv[m] = Tpspg * (  B[m][0] * (ve[0]*vex[0] + ve[1]*vey[0] + ve[2]*vez[0])
                             + B[m][1] * (ve[0]*vex[1] + ve[1]*vey[1] + ve[2]*vez[1])
                             + B[m][2] * (ve[0]*vex[2] + ve[1]*vey[2] + ve[2]*vez[2]));
            // 0 vector
            thv[m] = Tpspg * ((B[m][0]*px + B[m][1]*py + B[m][2]*pz) * invro);
            // e vector
            value = Tlsic * ro * (vex[0] + vey[1] + vez[2]);
            ev[3*m+0] = value * B[m][0];
            ev[3*m+1] = value * B[m][1];
            ev[3*m+2] = value * B[m][2];
            // f vector
            value = ro * N[m];
            fv[3*m+0] = value * g[0];
            fv[3*m+1] = value * g[1];
            fv[3*m+2] = value * g[2];
            // f~ vector
            value = Tsupg * ro * vdiv[m];
            ftv[3*m+0] = value * g[0];
            ftv[3*m+1] = value * g[1];
            ftv[3*m+2] = value * g[2];
            // e~ vector
            etv[m] = Tpspg * (B[m][0]*g[0] + B[m][1]*g[1] + B[m][2]*g[2]);
        }

        // Assembly element level matrixes
        for (uint m=0;m<nen*3;m++)
        {
            for (uint n=0;n<nen*3;n++)
            {
                if (unsteady)
                {
                    Ae11[m][n] = me[m][n] + cte[m][n]
                               + alphaDt * (
                                              ce[m][n]    + cpe[m][n]
                                            + ctpe[m][n]  + ke[m][n]
                                            + kte[m][n]   + ktpe[m][n]
                                            + ktppe[m][n] + ytpe[m][n] )
                               + dt * epe[m][n];
                }
                else
                {
                    Ae11[m][n] = ce[m][n]    + cpe[m][n]
                               + ke[m][n]
                               + kte[m][n]   + ktpe[m][n]
                               + ktppe[m][n] + ytpe[m][n]
                               + epe[m][n];
                }
            }
            for (uint n=0;n<nen;n++)
            {
                if (unsteady)
                {
                    Ae12[m][n] = -dt * (ge[m][n] + yte[m][n]);
                    Ae21[n][m] = bte[n][m]
                               + dt * geT[n][m]
                               + alphaDt * (ye[n][m] + ype[n][m]);
                }
                else
                {
                    Ae12[m][n] = -ge[m][n] - yte[m][n];
                    Ae21[n][m] =  geT[n][m] + ye[n][m] + ype[n][m];
                }
            }
            if (unsteady)
            {
                be1[m] = dt * (fv[m] + ftv[m] - (  mv[m]  + ctv[m]
                                                 + cv[m]  + kv[m]
                                                 - gv[m]  + ktv[m]
                                                 - ytv[m] + ev[m]) );
            }
            else
            {
                be1[m] = fv[m] + ftv[m] - (  cv[m]  + kv[m]
                                           - gv[m]  + ktv[m]
                                           - ytv[m] + ev[m]);
            }

            for (uint k=0;k<nen;k++)
            {
                for (uint l=0;l<nen;l++)
                {
                    if (unsteady)
                    {
                        Ae22[k][l] = dt * the[k][l];
                    }
                    else
                    {
                        Ae22[k][l] = the[k][l];
                    }
                }
                if (unsteady)
                {
                    be2[k] = dt * (etv[k] - (  btv[k] + gvT[k]
                                             + yv[k] + thv[k]) );
                }
                else
                {
                    be2[k] = etv[k] - (gvT[k] + yv[k] + thv[k]);
                }
            }
        }

        for (uint m=0;m<nen;m++)
        {
            for (uint n=0;n<nen;n++)
            {
                Ae[4*m+0][4*n+0] += Ae11[3*m+0][3*n+0] * integValue;
                Ae[4*m+0][4*n+1] += Ae11[3*m+0][3*n+1] * integValue;
                Ae[4*m+0][4*n+2] += Ae11[3*m+0][3*n+2] * integValue;
                Ae[4*m+1][4*n+0] += Ae11[3*m+1][3*n+0] * integValue;
                Ae[4*m+1][4*n+1] += Ae11[3*m+1][3*n+1] * integValue;
                Ae[4*m+1][4*n+2] += Ae11[3*m+1][3*n+2] * integValue;
                Ae[4*m+2][4*n+0] += Ae11[3*m+2][3*n+0] * integValue;
                Ae[4*m+2][4*n+1] += Ae11[3*m+2][3*n+1] * integValue;
                Ae[4*m+2][4*n+2] += Ae11[3*m+2][3*n+2] * integValue;

                Ae[4*m+0][4*n+3] += Ae12[3*m+0][n] * integValue;
                Ae[4*m+1][4*n+3] += Ae12[3*m+1][n] * integValue;
                Ae[4*m+2][4*n+3] += Ae12[3*m+2][n] * integValue;

                Ae[4*m+3][4*n+0] += Ae21[m][3*n+0] * integValue;
                Ae[4*m+3][4*n+1] += Ae21[m][3*n+1] * integValue;
                Ae[4*m+3][4*n+2] += Ae21[m][3*n+2] * integValue;

                Ae[4*m+3][4*n+3] += Ae22[m][n] * integValue;
            }
            be[4*m+0] += be1[3*m+0] * integValue;
            be[4*m+1] += be1[3*m+1] * integValue;
            be[4*m+2] += be1[3*m+2] * integValue;

            be[4*m+3] += be2[m] * integValue;
        }
    }
}

void RSolverFluid::computeElementConstantDerivative(unsigned int elementID, RRMatrix &Ae, RRVector &be, RMatrixManager<FluidMatrixContainer> &matrixManager)
{
    bool unsteady = (this->pModel->getTimeSolver().getEnabled());

    const RElement &element = this->pModel->getElement(elementID);
    uint nen = element.size();

    double ro = this->elementDensity[elementID];
    double invro = 1.0 / ro;
    double u = this->elementViscosity[elementID];

    Ae.fill(0.0);
    be.fill(0.0);

    FluidMatrixContainer &matrixCotainer = matrixManager.getMatricies(element.getType());
    matrixCotainer.clear();

    // Element level matricies
    RRMatrix &me = matrixCotainer.me;
    RRMatrix &ce = matrixCotainer.ce;
    RRMatrix &ke = matrixCotainer.ke;
    RRMatrix &ge = matrixCotainer.ge;
    RRMatrix &geT = matrixCotainer.geT;
    RRMatrix &cpe = matrixCotainer.cpe;
    RRMatrix &cte = matrixCotainer.cte;
    RRMatrix &ctpe = matrixCotainer.ctpe;
    RRMatrix &kte = matrixCotainer.kte;
    RRMatrix &ktpe = matrixCotainer.ktpe;
    RRMatrix &ktppe = matrixCotainer.ktppe;
    RRMatrix &yte = matrixCotainer.yte;
    RRMatrix &ytpe = matrixCotainer.ytpe;
    RRMatrix &bte = matrixCotainer.bte;
    RRMatrix &ye = matrixCotainer.ye;
    RRMatrix &ype = matrixCotainer.ype;
    RRMatrix &the = matrixCotainer.the;
    RRMatrix &epe = matrixCotainer.epe;

    // Element level vectors
    RRVector &fv = matrixCotainer.fv;
    RRVector &ftv = matrixCotainer.ftv;
    RRVector &etv = matrixCotainer.etv;
    RRVector &mv = matrixCotainer.mv;
    RRVector &cv = matrixCotainer.cv;
    RRVector &kv = matrixCotainer.kv;
    RRVector &gv = matrixCotainer.gv;
    RRVector &gvT = matrixCotainer.gvT;
    RRVector &ctv = matrixCotainer.ctv;
    RRVector &ktv = matrixCotainer.ktv;
    RRVector &ytv = matrixCotainer.ytv;
    RRVector &btv = matrixCotainer.btv;
    RRVector &yv = matrixCotainer.yv;
    RRVector &thv = matrixCotainer.thv;
    RRVector &ev = matrixCotainer.ev;

    // Element level equations
    RRMatrix &Ae11 = matrixCotainer.Ae11;
    RRMatrix &Ae12 = matrixCotainer.Ae12;
    RRMatrix &Ae21 = matrixCotainer.Ae21;
    RRMatrix &Ae22 = matrixCotainer.Ae22;
    RRVector &be1 = matrixCotainer.be1;
    RRVector &be2 = matrixCotainer.be2;

    double mVh = this->streamVelocity;
    double invmVh = this->invStreamVelocity;
    double alpha = this->pModel->getTimeSolver().getTimeMarchApproximationCoefficient();
    double dt = this->pModel->getTimeSolver().getCurrentTimeStepSize();
    double alphaDt = alpha * dt;

    // Element level input -------------------------------------------
    RR3Vector ve(this->elementVelocity.x[elementID],
                 this->elementVelocity.y[elementID],
                 this->elementVelocity.z[elementID]);
    RR3Vector veo(0.0,0.0,0.0);
    for (uint i=0;i<nen;i++)
    {
        veo[0] += this->nodeVelocityOld.x[element.getNodeId(i)];
        veo[1] += this->nodeVelocityOld.y[element.getNodeId(i)];
        veo[2] += this->nodeVelocityOld.z[element.getNodeId(i)];
    }
    veo *= 1.0/double(nen);
    RRVector ax(nen,0.0);
    RRVector ay(nen,0.0);
    RRVector az(nen,0.0);
    for (uint i=0;i<nen;i++)
    {
        ax[i] = this->nodeAcceleration.x[element.getNodeId(i)];
        ay[i] = this->nodeAcceleration.y[element.getNodeId(i)];
        az[i] = this->nodeAcceleration.z[element.getNodeId(i)];
    }

    RR3Vector g(this->elementGravity.x[elementID],
                this->elementGravity.y[elementID],
                this->elementGravity.z[elementID]);
    double p = this->elementPressure[elementID];
    // element level velocity magnitude
    double mvh = veo.length();
    double invmvh = 1.0 / mvh;
    // element level velocity direction
    RR3Vector s(veo);
    s.normalize();

    const RRVector &iN = RElement::getMassVector(element.getType());
    const RRMatrix &iNiN = RElement::getMassMatrix(element.getType());
    double wt = RElement::getTotalWeightFactor(element.getType());
    const RRMatrix &B = this->shapeDerivations[elementID]->getDerivative(0);

    // velocity divergence
    RRVector vdiv(nen,0.0);
    // partial derivatives
    RR3Vector vex(0.0,0.0,0.0), vey(0.0,0.0,0.0), vez(0.0,0.0,0.0);
    double px = 0.0, py = 0.0, pz = 0.0;
    // element length scale
    double h = 0.0;
    double hn = this->elementScales[elementID];

    for (uint m=0;m<nen;m++)
    {
        vdiv[m] += ve[0] * B[m][0] + ve[1] * B[m][1] + ve[2] * B[m][2];

        vex[0] += B[m][0] * this->nodeVelocity.x[element.getNodeId(m)];
        vex[1] += B[m][0] * this->nodeVelocity.y[element.getNodeId(m)];
        vex[2] += B[m][0] * this->nodeVelocity.z[element.getNodeId(m)];

        vey[0] += B[m][1] * this->nodeVelocity.x[element.getNodeId(m)];
        vey[1] += B[m][1] * this->nodeVelocity.y[element.getNodeId(m)];
        vey[2] += B[m][1] * this->nodeVelocity.z[element.getNodeId(m)];

        vez[0] += B[m][2] * this->nodeVelocity.x[element.getNodeId(m)];
        vez[1] += B[m][2] * this->nodeVelocity.y[element.getNodeId(m)];
        vez[2] += B[m][2] * this->nodeVelocity.z[element.getNodeId(m)];

        px += B[m][0] * this->nodePressure[element.getNodeId(m)];
        py += B[m][1] * this->nodePressure[element.getNodeId(m)];
        pz += B[m][2] * this->nodePressure[element.getNodeId(m)];

        h += std::fabs(s[0]*B[m][0] + s[1]*B[m][1] + s[2]*B[m][2]);
    }
    if (h != 0.0)
    {
        h = 2.0/h;
    }
    // Reynolds numbers
    double roD2u = 0.5 * ro / u;
    double Re = roD2u * mvh * h;
    double Ren = roD2u * mVh * hn;

    // SUPG stabilization parameter
    double Tsupg = 0.0;
    if (mvh > 0.0)
    {
        if (Re > 0.0 && Re <= 3.0)
        {
            Tsupg = h * Re * invmvh * inv6;
        }
        else
        {
            Tsupg = h * invmvh * 0.5;
        }
    }

    // PSPG stabilization parameter
    double Tpspg = 0.0;
    if (mVh > 0.0)
    {
        if (Ren > 0.0 && Ren <= 3.0)
        {
            Tpspg = hn * Ren * invmVh * inv6;
        }
        else
        {
            Tpspg = hn * invmVh * 0.5;
        }
    }

    // LSIC stabilization parameter
    double Tlsic = mvh*h * 0.5;

    double value = 0.0;
    for (uint m=0;m<nen;m++)
    {
        for (uint n=0;n<nen;n++)
        {
            // m matrix
            if (unsteady)
            {
                value = ro * iNiN[m][n];
                me[3*m+0][3*n+0] = value;
                me[3*m+1][3*n+1] = value;
                me[3*m+2][3*n+2] = value;
            }
            // c matrix
            value = ro * iN[m] * vdiv[n];
            ce[3*m+0][3*n+0] = value;
            ce[3*m+1][3*n+1] = value;
            ce[3*m+2][3*n+2] = value;
            // k matrix
            value = ro * u * wt;
            double tmpValue = B[m][0] * B[n][0] + B[m][1] * B[n][1] + B[m][2] * B[n][2];
            ke[3*m+0][3*n+0] = value * (tmpValue + B[m][0] * B[n][0]);
            ke[3*m+1][3*n+0] = value * (           B[m][0] * B[n][1]);
            ke[3*m+2][3*n+0] = value * (           B[m][0] * B[n][2]);
            ke[3*m+0][3*n+1] = value * (           B[m][1] * B[n][0]);
            ke[3*m+1][3*n+1] = value * (tmpValue + B[m][1] * B[n][1]);
            ke[3*m+2][3*n+1] = value * (           B[m][1] * B[n][2]);
            ke[3*m+0][3*n+2] = value * (           B[m][2] * B[n][0]);
            ke[3*m+1][3*n+2] = value * (           B[m][2] * B[n][1]);
            ke[3*m+2][3*n+2] = value * (tmpValue + B[m][2] * B[n][2]);
            // g matrix
            ge[3*m+0][n] = B[m][0] * iN[n];
            ge[3*m+1][n] = B[m][1] * iN[n];
            ge[3*m+2][n] = B[m][2] * iN[n];
            // gT matrix
            geT[n][3*m+0] = ge[3*m+0][n];
            geT[n][3*m+1] = ge[3*m+1][n];
            geT[n][3*m+2] = ge[3*m+2][n];
            // c+ matrix
            value = ro * iNiN[m][n];
            cpe[3*m+0][3*n+0] = value * vex[0];
            cpe[3*m+0][3*n+1] = value * vey[0];
            cpe[3*m+0][3*n+2] = value * vez[0];
            cpe[3*m+1][3*n+0] = value * vex[1];
            cpe[3*m+1][3*n+1] = value * vey[1];
            cpe[3*m+1][3*n+2] = value * vez[1];
            cpe[3*m+2][3*n+0] = value * vex[2];
            cpe[3*m+2][3*n+1] = value * vey[2];
            cpe[3*m+2][3*n+2] = value * vez[2];
            // c~ matrix
            if (unsteady)
            {
                value = Tsupg * ro * vdiv[m] * iN[n];
                cte[3*m+0][3*n+0] = value;
                cte[3*m+1][3*n+1] = value;
                cte[3*m+2][3*n+2] = value;
            }
            // c~+ matrix
            if (unsteady)
            {
                value = Tsupg * ro * iN[n];
                ctpe[3*m+0][3*n+0] = value * B[m][0] * ax[n];
                ctpe[3*m+1][3*n+0] = value * B[m][0] * ay[n];
                ctpe[3*m+2][3*n+0] = value * B[m][0] * az[n];
                ctpe[3*m+0][3*n+1] = value * B[m][1] * ax[n];
                ctpe[3*m+1][3*n+1] = value * B[m][1] * ay[n];
                ctpe[3*m+2][3*n+1] = value * B[m][1] * az[n];
                ctpe[3*m+0][3*n+2] = value * B[m][2] * ax[n];
                ctpe[3*m+1][3*n+2] = value * B[m][2] * ay[n];
                ctpe[3*m+2][3*n+2] = value * B[m][2] * az[n];
            }
            // k~ matrix
            value = Tsupg * ro * vdiv[m] * vdiv[n] * wt;
            kte[3*m+0][3*n+0] = value;
            kte[3*m+1][3*n+1] = value;
            kte[3*m+2][3*n+2] = value;
            // k~+ matrix
            value = Tsupg * ro * vdiv[m] * iN[n];
            ktpe[3*m+0][3*n+0] = value * vex[0];
            ktpe[3*m+0][3*n+1] = value * vey[0];
            ktpe[3*m+0][3*n+2] = value * vez[0];
            ktpe[3*m+1][3*n+0] = value * vex[1];
            ktpe[3*m+1][3*n+1] = value * vey[1];
            ktpe[3*m+1][3*n+2] = value * vez[1];
            ktpe[3*m+2][3*n+0] = value * vex[2];
            ktpe[3*m+2][3*n+1] = value * vey[2];
            ktpe[3*m+2][3*n+2] = value * vez[2];
            // k~++ matrix
            value = Tsupg * ro * iN[n];
            ktppe[3*m+0][3*n+0] = value * B[m][0] * (ve[0]*vex[0] + ve[1]*vey[0] + ve[2]*vez[0]);
            ktppe[3*m+0][3*n+1] = value * B[m][1] * (ve[0]*vex[0] + ve[1]*vey[0] + ve[2]*vez[0]);
            ktppe[3*m+0][3*n+2] = value * B[m][2] * (ve[0]*vex[0] + ve[1]*vey[0] + ve[2]*vez[0]);

            ktppe[3*m+1][3*n+0] = value * B[m][0] * (ve[0]*vex[1] + ve[1]*vey[1] + ve[2]*vez[1]);
            ktppe[3*m+1][3*n+1] = value * B[m][1] * (ve[0]*vex[1] + ve[1]*vey[1] + ve[2]*vez[1]);
            ktppe[3*m+1][3*n+2] = value * B[m][2] * (ve[0]*vex[1] + ve[1]*vey[1] + ve[2]*vez[1]);

            ktppe[3*m+2][3*n+0] = value * B[m][0] * (ve[0]*vex[2] + ve[1]*vey[2] + ve[2]*vez[2]);
            ktppe[3*m+2][3*n+1] = value * B[m][1] * (ve[0]*vex[2] + ve[1]*vey[2] + ve[2]*vez[2]);
            ktppe[3*m+2][3*n+2] = value * B[m][2] * (ve[0]*vex[2] + ve[1]*vey[2] + ve[2]*vez[2]);
            // y~ matrix
            value = Tsupg * vdiv[m] * wt;
            yte[3*m+0][n] = value * B[n][0];
            yte[3*m+1][n] = value * B[n][1];
            yte[3*m+2][n] = value * B[n][2];
            // y~+ matrix
            value = Tsupg * iN[n];
            ytpe[3*m+0][3*n+0] = value * B[m][0] * px;
            ytpe[3*m+0][3*n+1] = value * B[m][1] * px;
            ytpe[3*m+0][3*n+2] = value * B[m][2] * px;
            ytpe[3*m+1][3*n+0] = value * B[m][0] * py;
            ytpe[3*m+1][3*n+1] = value * B[m][1] * py;
            ytpe[3*m+1][3*n+2] = value * B[m][2] * py;
            ytpe[3*m+2][3*n+0] = value * B[m][0] * pz;
            ytpe[3*m+2][3*n+1] = value * B[m][1] * pz;
            ytpe[3*m+2][3*n+2] = value * B[m][2] * pz;
            // B matrix
            value = Tpspg * iN[n];
            bte[m][3*n+0] = value * B[m][0];
            bte[m][3*n+1] = value * B[m][1];
            bte[m][3*n+2] = value * B[m][2];
            // y matrix
            value = Tpspg * vdiv[n] * wt;
            ye[m][3*n+0] = value * B[m][0];
            ye[m][3*n+1] = value * B[m][1];
            ye[m][3*n+2] = value * B[m][2];
            // y+ matrix
            value = Tpspg * iN[n];
            ype[m][3*n+0] = value * (B[m][0]*vex[0] + B[m][1]*vex[1] + B[m][2]*vex[2]);
            ype[m][3*n+1] = value * (B[m][0]*vey[0] + B[m][1]*vey[1] + B[m][2]*vey[2]);
            ype[m][3*n+2] = value * (B[m][0]*vez[0] + B[m][1]*vez[1] + B[m][2]*vez[2]);
            // 0 matrix
            the[m][n] = Tpspg * ((B[m][0]*B[n][0] + B[m][1]*B[n][1] + B[m][2]*B[n][2]) * invro) * wt;
            // ep matrix
            value = Tlsic * ro * wt;
            epe[3*m+0][3*n+0] = B[m][0] * B[n][0] * value;
            epe[3*m+0][3*n+1] = B[m][0] * B[n][1] * value;
            epe[3*m+0][3*n+2] = B[m][0] * B[n][2] * value;
            epe[3*m+1][3*n+0] = B[m][1] * B[n][0] * value;
            epe[3*m+1][3*n+1] = B[m][1] * B[n][1] * value;
            epe[3*m+1][3*n+2] = B[m][1] * B[n][2] * value;
            epe[3*m+2][3*n+0] = B[m][2] * B[n][0] * value;
            epe[3*m+2][3*n+1] = B[m][2] * B[n][1] * value;
            epe[3*m+2][3*n+2] = B[m][2] * B[n][2] * value;
        }
        // m vector
        if (unsteady)
        {
            value = iN[m] * ro;
            mv[3*m+0] = value * ax[m];
            mv[3*m+1] = value * ay[m];
            mv[3*m+2] = value * az[m];
        }
        // c vector
        value = iN[m] * ro;
        cv[3*m+0] = value * (ve[0]*vex[0] + ve[1]*vey[0] + ve[2]*vez[0]);
        cv[3*m+1] = value * (ve[0]*vex[1] + ve[1]*vey[1] + ve[2]*vez[1]);
        cv[3*m+2] = value * (ve[0]*vex[2] + ve[1]*vey[2] + ve[2]*vez[2]);
        // k vector
        kv[3*m+0] = u * (  B[m][0]*vex[0] + B[m][1]*vey[0] + B[m][2]*vez[0]
                         + B[m][0]*vex[0] + B[m][1]*vex[1] + B[m][2]*vex[2]) * wt;
        kv[3*m+1] = u * (  B[m][0]*vex[1] + B[m][1]*vey[1] + B[m][2]*vez[1]
                         + B[m][0]*vey[0] + B[m][1]*vey[1] + B[m][2]*vey[2]) * wt;
        kv[3*m+2] = u * (  B[m][0]*vex[2] + B[m][1]*vey[2] + B[m][2]*vez[2]
                         + B[m][0]*vez[0] + B[m][1]*vez[1] + B[m][2]*vez[2]) * wt;
        // g vector
        value = p * wt;
        gv[3*m+0] = B[m][0] * value;
        gv[3*m+1] = B[m][1] * value;
        gv[3*m+2] = B[m][2] * value;
        // gT vector
        gvT[m] = iN[m] * (vex[0] + vey[1] + vez[2]);
        // c~ vector
        if (unsteady)
        {
            value = Tsupg * ro * vdiv[m] * wt;
            ctv[3*m+0] = value * ax[m];
            ctv[3*m+1] = value * ay[m];
            ctv[3*m+2] = value * az[m];
        }
        // k~ vector
        value = Tsupg * ro * vdiv[m] * wt;
        ktv[3*m+0] = value * (ve[0]*vex[0] + ve[1]*vey[0] + ve[2]*vez[0]);
        ktv[3*m+1] = value * (ve[0]*vex[1] + ve[1]*vey[1] + ve[2]*vez[1]);
        ktv[3*m+2] = value * (ve[0]*vex[2] + ve[1]*vey[2] + ve[2]*vez[2]);
        // y~ vector
        value = Tsupg * vdiv[m] * wt;
        ytv[3*m+0] = value * px;
        ytv[3*m+1] = value * py;
        ytv[3*m+2] = value * pz;
        // B vector
        if (unsteady)
        {
            btv[m] = Tpspg * (B[m][0]*ax[m] + B[m][1]*ay[m] + B[m][2]*az[m]) * wt;
        }
        // y vector
        yv[m] = Tpspg * (  B[m][0] * (ve[0]*vex[0] + ve[1]*vey[0] + ve[2]*vez[0])
                         + B[m][1] * (ve[0]*vex[1] + ve[1]*vey[1] + ve[2]*vez[1])
                         + B[m][2] * (ve[0]*vex[2] + ve[1]*vey[2] + ve[2]*vez[2])) * wt;
        // 0 vector
        thv[m] = Tpspg * ((B[m][0]*px + B[m][1]*py + B[m][2]*pz) * invro) * wt;
        // e vector
        value = Tlsic * ro * (vex[0] + vey[1] + vez[2]) * wt;
        ev[3*m+0] = value * B[m][0];
        ev[3*m+1] = value * B[m][1];
        ev[3*m+2] = value * B[m][2];
        // f vector
        value = ro * iN[m];
        fv[3*m+0] = value * g[0];
        fv[3*m+1] = value * g[1];
        fv[3*m+2] = value * g[2];
        // f~ vector
        value = Tsupg * ro * vdiv[m] * wt;
        ftv[3*m+0] = value * g[0];
        ftv[3*m+1] = value * g[1];
        ftv[3*m+2] = value * g[2];
        // e~ vector
        etv[m] = Tpspg * (B[m][0]*g[0] + B[m][1]*g[1] + B[m][2]*g[2]) * wt;
    }

    // Assembly element level matrixes
    for (uint m=0;m<nen*3;m++)
    {
        for (uint n=0;n<nen*3;n++)
        {
            if (unsteady)
            {
                Ae11[m][n] = me[m][n] + cte[m][n]
                           + alphaDt * (
                                          ce[m][n]    + cpe[m][n]
                                        + ctpe[m][n]  + ke[m][n]
                                        + kte[m][n]   + ktpe[m][n]
                                        + ktppe[m][n] + ytpe[m][n] )
                           + dt * epe[m][n];
            }
            else
            {
                Ae11[m][n] = ce[m][n]    + cpe[m][n]
                           + ke[m][n]
                           + kte[m][n]   + ktpe[m][n]
                           + ktppe[m][n] + ytpe[m][n]
                           + epe[m][n];
            }
        }
        for (uint n=0;n<nen;n++)
        {
            if (unsteady)
            {
                Ae12[m][n] = -dt * (ge[m][n] + yte[m][n]);
                Ae21[n][m] = bte[n][m]
                           + dt * geT[n][m]
                           + alphaDt * (ye[n][m] + ype[n][m]);
            }
            else
            {
                Ae12[m][n] = -ge[m][n] - yte[m][n];
                Ae21[n][m] =  geT[n][m] + ye[n][m] + ype[n][m];
            }
        }
        if (unsteady)
        {
            be1[m] = dt * (fv[m] + ftv[m] - (  mv[m]  + ctv[m]
                                             + cv[m]  + kv[m]
                                             - gv[m]  + ktv[m]
                                             - ytv[m] + ev[m]) );
        }
        else
        {
            be1[m] = fv[m] + ftv[m] - (  cv[m]  + kv[m]
                                       - gv[m]  + ktv[m]
                                       - ytv[m] + ev[m]);
        }

        for (uint k=0;k<nen;k++)
        {
            for (uint l=0;l<nen;l++)
            {
                if (unsteady)
                {
                    Ae22[k][l] = dt * the[k][l];
                }
                else
                {
                    Ae22[k][l] = the[k][l];
                }
            }
            if (unsteady)
            {
                be2[k] = dt * (etv[k] - (  btv[k] + gvT[k]
                                         + yv[k] + thv[k]) );
            }
            else
            {
                be2[k] = etv[k] - (gvT[k] + yv[k] + thv[k]);
            }
        }
    }

    for (uint m=0;m<nen;m++)
    {
        for (uint n=0;n<nen;n++)
        {
            Ae[4*m+0][4*n+0] += Ae11[3*m+0][3*n+0];
            Ae[4*m+0][4*n+1] += Ae11[3*m+0][3*n+1];
            Ae[4*m+0][4*n+2] += Ae11[3*m+0][3*n+2];
            Ae[4*m+1][4*n+0] += Ae11[3*m+1][3*n+0];
            Ae[4*m+1][4*n+1] += Ae11[3*m+1][3*n+1];
            Ae[4*m+1][4*n+2] += Ae11[3*m+1][3*n+2];
            Ae[4*m+2][4*n+0] += Ae11[3*m+2][3*n+0];
            Ae[4*m+2][4*n+1] += Ae11[3*m+2][3*n+1];
            Ae[4*m+2][4*n+2] += Ae11[3*m+2][3*n+2];

            Ae[4*m+0][4*n+3] += Ae12[3*m+0][n];
            Ae[4*m+1][4*n+3] += Ae12[3*m+1][n];
            Ae[4*m+2][4*n+3] += Ae12[3*m+2][n];

            Ae[4*m+3][4*n+0] += Ae21[m][3*n+0];
            Ae[4*m+3][4*n+1] += Ae21[m][3*n+1];
            Ae[4*m+3][4*n+2] += Ae21[m][3*n+2];

            Ae[4*m+3][4*n+3] += Ae22[m][n];
        }
        be[4*m+0] += be1[3*m+0];
        be[4*m+1] += be1[3*m+1];
        be[4*m+2] += be1[3*m+2];

        be[4*m+3] += be2[m];
    }

    double detJ = this->shapeDerivations[elementID]->getJacobian(0);
    Ae *= detJ;
    be *= detJ;
}

double RSolverFluid::findTimeScale() const
{
    double v = RSolverFluid::computeStreamVelocity(*this->pModel,this->nodeVelocity,true);
    double l = 1.0 / this->scales.getMetre();

    return v/l;
}

double RSolverFluid::findReScale(void) const
{
    double v = RSolverFluid::computeStreamVelocity(*this->pModel,this->nodeVelocity,true);
    double l = 1.0 / this->scales.getMetre();

    double Re = (this->avgU == 0.0) ? 1.0 : this->avgRo * v * l / this->avgU;

    return (Re == 0.0) ? 1.0 : 1.0e-2 / Re;
}

double RSolverFluid::findWeightScale(void) const
{
    double v = RSolverFluid::computeStreamVelocity(*this->pModel,this->nodeVelocity,true);
    double l = 1.0 / this->scales.getMetre();

    double ws = (this->avgU == 0.0) ? 1.0 : 1.0 *  v / (this->avgU * l * l);
    if (this->pModel->getTimeSolver().getEnabled())
    {
        ws *= 1.0e-2 / this->pModel->getTimeSolver().getCurrentTimeStepSize();
    }

    return ws;
}

void RSolverFluid::computeElementScales(void)
{
    this->elementScales.resize(this->pModel->getNElements());
    this->elementScales.fill(0.0);

    double third = 1.0/3.0;

#pragma omp parallel for default(shared)
    for (int64_t i=0;i<int64_t(this->pModel->getNElements());i++)
    {
        if (!this->computableElements[uint(i)])
        {
            continue;
        }
        const RElement &rElement = this->pModel->getElement(uint(i));
        double volume = 0.0;
        if (!rElement.findVolume(this->pModel->getNodes(),volume))
        {
            continue;
        }
        if (rElement.getType() == R_ELEMENT_TETRA1)
        {
            this->elementScales[uint(i)] = std::pow(6.0 * volume / RConstants::pi, third);
        }
        else if (rElement.getType() == R_ELEMENT_HEXA1)
        {
            this->elementScales[uint(i)] = std::pow(volume, third);
        }
        else
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,
                         "Failed to calculate element scales. Unsupported elemenent type \'%s\'.",
                         RElement::getName(rElement.getType()).toUtf8().constData());
        }
    }
}

void RSolverFluid::computeElementFreePressure(RRVector &values, RBVector &setValues)
{
    values.resize(this->pModel->getNElements());
    values.fill(0.0);
    setValues.resize(this->pModel->getNElements());
    setValues.fill(false);

#pragma omp parallel for default(shared)
    for (int64_t i=0;i<int64_t(this->pModel->getNSurfaces());i++)
    {
        const RSurface &rSurface = this->pModel->getSurface(uint(i));
        for (uint j=0;j<rSurface.getNBoundaryConditions();j++)
        {
            const RBoundaryCondition &bc = rSurface.getBoundaryCondition(j);
            if (bc.getType() != R_BOUNDARY_CONDITION_PRESSURE_IMPLICIT)
            {
                continue;
            }
            double p = 0.0;
            uint cpos = bc.findComponentPosition(R_VARIABLE_PRESSURE);
            if (cpos != RConstants::eod)
            {
                p = bc.getComponent(cpos).get(this->pModel->getTimeSolver().getCurrentTime());
            }

            for (uint k=0;k<rSurface.size();k++)
            {
                values[rSurface.get(k)] = p;
                setValues[rSurface.get(k)] = true;
            }
            break;
        }
    }
}

void RSolverFluid::assemblyMatrix(uint elementID, const RRMatrix &Ae, const RRVector &fe)
{
    const RElement &rElement = this->pModel->getElement(elementID);

    // Assembly final matrix system
    uint dims = 4;
    for (uint m=0;m<rElement.size();m++)
    {
        for (uint i=0;i<dims;i++)
        {
            uint mp = 0;

            if (this->nodeBook.getValue(dims*rElement.getNodeId(m)+i,mp))
            {
#pragma omp atomic
                this->b[mp] += fe[dims*m+i];
#pragma omp critical
                {
                    for (uint n=0;n<rElement.size();n++)
                    {
                        for (uint j=0;j<dims;j++)
                        {
                            uint np = 0;

                            if (this->nodeBook.getValue(dims*rElement.getNodeId(n)+j,np))
                            {
                                this->A.addValue(mp,np,Ae[dims*m+i][dims*n+j]);
                            }
                        }
                    }
                }
            }
        }
    }
}

void RSolverFluid::assemblyMatrix(unsigned int elementID, const RRMatrix &Ae, const RRVector &fe, RSparseMatrix &Ap, RRVector &bp)
{
    const RElement &rElement = this->pModel->getElement(elementID);

    // Assembly final matrix system
    uint dims = 4;
    for (uint m=0;m<rElement.size();m++)
    {
        for (uint i=0;i<dims;i++)
        {
            uint mp = 0;
            if (this->nodeBook.getValue(dims*rElement.getNodeId(m)+i,mp))
            {
                bp[mp] += fe[dims*m+i];
                for (uint n=0;n<rElement.size();n++)
                {
                    for (uint j=0;j<dims;j++)
                    {
                        uint np = 0;
                        if (this->nodeBook.getValue(dims*rElement.getNodeId(n)+j,np))
                        {
                            Ap.addValue(mp,np,Ae[dims*m+i][dims*n+j]);
                        }
                    }
                }
            }
        }
    }
}

void RSolverFluid::applyLocalRotations(unsigned int elementID, RRMatrix &Ae)
{
    const RElement &rElement = this->pModel->getElement(elementID);
    RRMatrix T;

    bool first = true;

    for (uint i=0;i<rElement.size();i++)
    {
        uint nodeId = rElement.getNodeId(i);
        if (this->localRotations[nodeId].isActive())
        {
            if (first)
            {
                T.setIdentity(Ae.getNRows());
                first = false;
            }
            T.setBlock(this->localRotations[nodeId].getR(),4*i,4*i);
        }
    }
    if (!first)
    {
        RRMatrix Tt(T);
        Tt.transpose();

        RRMatrix Aetmp;

        RRMatrix::mlt(Tt,Ae,Aetmp);
        RRMatrix::mlt(Aetmp,T,Ae);
    }
}

double RSolverFluid::computeStreamVelocity(const RModel &rModel, const RSolverCartesianVector<RRVector> &nodeVelocity, bool averageBased, double streamVelocity)
{
    double velocity = 1.0;

    double totalArea = 0.0;
    double totalVolurate = 0.0;

    for (uint i=0;i<rModel.getNSurfaces();i++)
    {
        const RSurface &rSurface = rModel.getSurface(i);

        bool hasVelocity = rSurface.hasBoundaryCondition(R_BOUNDARY_CONDITION_INFLOW_VELOCITY);
        bool hasVolurate = rSurface.hasBoundaryCondition(R_BOUNDARY_CONDITION_INFLOW_VOLURATE);

        double v = 0.0;
        double q = 0.0;

        if (!hasVelocity && !hasVolurate)
        {
            continue;
        }

        if (hasVelocity)
        {
            const RBoundaryCondition &rBoundaryCondition = rSurface.getBoundaryCondition(R_BOUNDARY_CONDITION_INFLOW_VELOCITY);
            uint vp = rBoundaryCondition.findComponentPosition(R_VARIABLE_VELOCITY);
            if (vp != RConstants::eod)
            {
                v = rBoundaryCondition.getComponent(vp).get(rModel.getTimeSolver().getCurrentTime());
            }
        }

        if (hasVolurate)
        {
            const RBoundaryCondition &rBoundaryCondition = rSurface.getBoundaryCondition(R_BOUNDARY_CONDITION_INFLOW_VOLURATE);
            uint vp = rBoundaryCondition.findComponentPosition(R_VARIABLE_VOLUME_FLOW_RATE);
            if (vp != RConstants::eod)
            {
                q = rBoundaryCondition.getComponent(vp).get(rModel.getTimeSolver().getCurrentTime());
            }
        }

        double area = rSurface.findArea(rModel.getNodes(),rModel.getElements());
        if (area < RConstants::eps)
        {
            continue;
        }
        if (hasVelocity)
        {
            q = v * area;
        }
        totalArea += area;
        totalVolurate += std::fabs(q);
    }

    if (totalArea < RConstants::eps)
    {
        velocity = 1.0;
    }
    else
    {
        velocity = totalVolurate / totalArea;
    }

    if (averageBased)
    {
        double vm = 0.0;

#pragma omp parallel default(shared)
        {
        double vm_local = 0.0;
#pragma omp for
            for (int64_t i=0;i<rModel.getNNodes();i++)
            {
                vm_local += std::sqrt(  std::pow(nodeVelocity.x[uint(i)],2)
                                      + std::pow(nodeVelocity.y[uint(i)],2)
                                      + std::pow(nodeVelocity.z[uint(i)],2));
            }
#pragma omp atomic
            vm += vm_local;
        }
        if (rModel.getNNodes())
        {
            vm /= double(rModel.getNNodes());
        }
        velocity = (vm + streamVelocity) / 2.0;
    }
    if (velocity < RConstants::eps)
    {
        velocity = 1.0;
    }
    return velocity;
}
