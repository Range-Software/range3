/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolvermagnetostatics.cpp                                *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   20-th November 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Magnetostatics solver class definition              *
 *********************************************************************/

#include "rsolvermagnetostatics.h"
#include "rmatrixsolver.h"

void RSolverMagnetostatics::_init(const RSolverMagnetostatics *pSolver)
{
    if (pSolver)
    {
        this->nodeCurrentDensity = pSolver->nodeCurrentDensity;
        this->nodeMagneticField = pSolver->nodeMagneticField;
    }
}

RSolverMagnetostatics::RSolverMagnetostatics(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData)
    : RSolverGeneric(pModel,modelFileName,convergenceFileName,sharedData)
{
    this->problemType = R_PROBLEM_MAGNETOSTATICS;
    this->_init();
}

RSolverMagnetostatics::RSolverMagnetostatics(const RSolverMagnetostatics &solver)
    : RSolverGeneric(solver)
{
    this->_init(&solver);
}

RSolverMagnetostatics::~RSolverMagnetostatics()
{

}

RSolverMagnetostatics &RSolverMagnetostatics::operator =(const RSolverMagnetostatics &solver)
{
    RSolverGeneric::operator =(solver);
    this->_init(&solver);
    return (*this);
}

bool RSolverMagnetostatics::hasConverged(void) const
{
    return true;
}

void RSolverMagnetostatics::updateScales(void)
{

}

void RSolverMagnetostatics::recover(void)
{
    RRVector elementCurrentDensityX(this->pModel->getNElements(),0.0);
    RRVector elementCurrentDensityY(this->pModel->getNElements(),0.0);
    RRVector elementCurrentDensityZ(this->pModel->getNElements(),0.0);

    this->recoverVariable(R_VARIABLE_CURRENT_DENSITY,R_VARIABLE_APPLY_ELEMENT,this->pModel->getNElements(),0,elementCurrentDensityX,0.0);
    this->recoverVariable(R_VARIABLE_CURRENT_DENSITY,R_VARIABLE_APPLY_ELEMENT,this->pModel->getNElements(),1,elementCurrentDensityY,0.0);
    this->recoverVariable(R_VARIABLE_CURRENT_DENSITY,R_VARIABLE_APPLY_ELEMENT,this->pModel->getNElements(),2,elementCurrentDensityZ,0.0);

    this->pModel->convertElementToNodeVector(elementCurrentDensityX,RBVector(this->pModel->getNElements(),true),this->nodeCurrentDensity.x,false);
    this->pModel->convertElementToNodeVector(elementCurrentDensityY,RBVector(this->pModel->getNElements(),true),this->nodeCurrentDensity.y,false);
    this->pModel->convertElementToNodeVector(elementCurrentDensityZ,RBVector(this->pModel->getNElements(),true),this->nodeCurrentDensity.z,false);
}

void RSolverMagnetostatics::prepare(void)
{
    this->generateNodeBook(R_PROBLEM_MAGNETOSTATICS);

    this->b.resize(3*this->nodeBook.getNEnabled());
    this->x.resize(3*this->nodeBook.getNEnabled());

    this->A.clear();
    this->b.fill(0.0);
    this->x.fill(0.0);

    // Prepare volume elements.
    for (uint i=0;i<this->pModel->getNVolumes();i++)
    {
        RVolume &volume = this->pModel->getVolume(i);

        bool abort = false;
        #pragma omp parallel for default(shared)
        for (int64_t j=0;j<int64_t(volume.size());j++)
        {
            #pragma omp flush (abort)
            if (abort)
            {
                continue;
            }
            try
            {
                uint elementID = volume.get(j);

                if (!this->computableElements[elementID])
                {
                    continue;
                }

                const RElement &element = this->pModel->getElement(elementID);
                R_ERROR_ASSERT(R_ELEMENT_TYPE_IS_VOLUME(element.getType()));
                uint nInp = RElement::getNIntegrationPoints(element.getType());
                RRMatrix Ke(element.size()*3,element.size()*3);
                RRVector fe(element.size()*3);
                RRMatrix B(element.size(),3);

                Ke.fill(0.0);
                fe.fill(0.0);

                // Conduction
                for (uint k=0;k<nInp;k++)
                {
                    const RElementShapeFunction &shapeFunc = RElement::getShapeFunction(element.getType(),k);
                    const RRVector &N = shapeFunc.getN();
                    const RRMatrix &dN = shapeFunc.getDN();
                    RRMatrix J, Rt;
                    double detJ = this->pModel->getElement(elementID).findJacobian(this->pModel->getNodes(),k,J,Rt);

                    B.fill(0.0);
                    for (uint m=0;m<dN.getNRows();m++)
                    {
                        B[m][0] += (dN[m][0]*J[0][0] + dN[m][1]*J[0][1] + dN[m][2]*J[0][2]);
                        B[m][1] += (dN[m][0]*J[1][0] + dN[m][1]*J[1][1] + dN[m][2]*J[1][2]);
                        B[m][2] += (dN[m][0]*J[2][0] + dN[m][1]*J[2][1] + dN[m][2]*J[2][2]);
                    }

                    for (unsigned m=0;m<element.size();m++)
                    {
                        uint nodeID = element.getNodeId(m);
                        for (unsigned n=0;n<element.size();n++)
                        {
                            double KeValue = (B[m][0]*B[n][0] + B[m][1]*B[n][1] + B[m][2]*B[n][2]) * detJ * shapeFunc.getW();
                            Ke[3*m+0][3*n+0] -= KeValue;
                            Ke[3*m+1][3*n+1] -= KeValue;
                            Ke[3*m+2][3*n+2] -= KeValue;
                        }
                        double feValue = N[m] * detJ * shapeFunc.getW() * RSolverGeneric::e0;

                        double jsx = - B[m][2] * this->nodeCurrentDensity.y[nodeID] + B[m][1] * this->nodeCurrentDensity.z[nodeID];
                        double jsy =   B[m][2] * this->nodeCurrentDensity.x[nodeID] - B[m][0] * this->nodeCurrentDensity.z[nodeID];
                        double jsz = - B[m][1] * this->nodeCurrentDensity.x[nodeID] + B[m][0] * this->nodeCurrentDensity.y[nodeID];

                        fe[3*m+0] += feValue * jsx;
                        fe[3*m+1] += feValue * jsy;
                        fe[3*m+2] += feValue * jsz;
                    }
                }
                #pragma omp critical
                {
                    this->assemblyMatrix(elementID,Ke,fe);
                }
            }
            catch (const RError &rError)
            {
                #pragma omp critical
                {
                    RLogger::error("%s\n",rError.getMessage().toUtf8().constData());
                    abort = true;
                }
            }
        }
        if (abort)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to prepare matrix system.");
        }
    }
}

void RSolverMagnetostatics::solve(void)
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

    this->nodeMagneticField.x.resize(this->pModel->getNNodes(),0.0);
    this->nodeMagneticField.y.resize(this->pModel->getNNodes(),0.0);
    this->nodeMagneticField.z.resize(this->pModel->getNNodes(),0.0);

    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        uint position;
        if (this->nodeBook.getValue(i,position))
        {
            this->nodeMagneticField.x[i] = this->x[3*position+0];
            this->nodeMagneticField.y[i] = this->x[3*position+1];
            this->nodeMagneticField.z[i] = this->x[3*position+2];
        }
    }
}

void RSolverMagnetostatics::process(void)
{

}

void RSolverMagnetostatics::store(void)
{
    RLogger::info("Storing results\n");
    RLogger::indent();

    // Displacement
    uint magneticFieldPos = this->pModel->findVariable(R_VARIABLE_MAGNETIC_FIELD);
    if (magneticFieldPos == RConstants::eod)
    {
        magneticFieldPos = this->pModel->addVariable(R_VARIABLE_MAGNETIC_FIELD);

        double umin = 0.0;
        double umax = 0.0;
        for (uint i=0;i<this->nodeMagneticField.x.size();i++)
        {
            double u = RR3Vector(this->nodeMagneticField.x[i],
                                 this->nodeMagneticField.y[i],
                                 this->nodeMagneticField.z[i]).length();
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

        this->pModel->getVariable(magneticFieldPos).getVariableData().setMinMaxDisplayValue(umin,umax);
    }
    RVariable &magneticField =  this->pModel->getVariable(magneticFieldPos);

    magneticField.setApplyType(R_VARIABLE_APPLY_NODE);
    magneticField.resize(3,this->pModel->getNNodes());
    for (uint i=0;i<this->pModel->getNNodes();i++)
    {
        magneticField.setValue(0,i,this->nodeMagneticField.x[i]);
        magneticField.setValue(1,i,this->nodeMagneticField.y[i]);
        magneticField.setValue(2,i,this->nodeMagneticField.z[i]);
    }

    RLogger::unindent();
}

void RSolverMagnetostatics::statistics(void)
{
    this->printStats(R_VARIABLE_MAGNETIC_FIELD);
    this->processMonitoringPoints();
}

void RSolverMagnetostatics::assemblyMatrix(unsigned int elementID, const RRMatrix &Ke, const RRVector &fe)
{
    const RElement &element = this->pModel->getElement(elementID);

    RRVector be(fe);

    // Apply explicit boundary conditions.
    for (uint m=0;m<element.size();m++)
    {
        uint position;
        uint nodeID = element.getNodeId(m);
        if (!this->nodeBook.getValue(nodeID,position))
        {
            for (uint n=0;n<3*element.size();n++)
            {
                be[n] -= Ke[n][3*m+0] * this->nodeMagneticField.x[nodeID];
                be[n] -= Ke[n][3*m+1] * this->nodeMagneticField.y[nodeID];
                be[n] -= Ke[n][3*m+2] * this->nodeMagneticField.z[nodeID];
            }
        }
    }

    // Assembly final matrix system
    for (uint m=0;m<element.size();m++)
    {
        uint mp;

        if (this->nodeBook.getValue(element.getNodeId(m),mp))
        {
            this->b[3*mp+0] += be[3*m+0];
            this->b[3*mp+1] += be[3*m+1];
            this->b[3*mp+2] += be[3*m+2];
            for (uint n=0;n<element.size();n++)
            {
                uint np = 0;

                if (this->nodeBook.getValue(element.getNodeId(n),np))
                {
                    this->A.addValue(3*mp+0,3*np+0,Ke[3*m+0][3*n+0]);
                    this->A.addValue(3*mp+1,3*np+0,Ke[3*m+1][3*n+0]);
                    this->A.addValue(3*mp+2,3*np+0,Ke[3*m+2][3*n+0]);

                    this->A.addValue(3*mp+0,3*np+1,Ke[3*m+0][3*n+1]);
                    this->A.addValue(3*mp+1,3*np+1,Ke[3*m+1][3*n+1]);
                    this->A.addValue(3*mp+2,3*np+1,Ke[3*m+2][3*n+1]);

                    this->A.addValue(3*mp+0,3*np+2,Ke[3*m+0][3*n+2]);
                    this->A.addValue(3*mp+1,3*np+2,Ke[3*m+1][3*n+2]);
                    this->A.addValue(3*mp+2,3*np+2,Ke[3*m+2][3*n+2]);
                }
            }
        }
    }
}
