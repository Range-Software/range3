/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rmatrixsolver.cpp                                        *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   23-rd January 2013                                       *
 *                                                                   *
 *  DESCRIPTION: Matrix solver class definition                      *
 *********************************************************************/

#include <string>
#include <cmath>

#include <omp.h>

#include "rmatrixsolver.h"

void RMatrixSolver::_init(const RMatrixSolver *pMatrixSolver)
{
    this->iterationInfo.setConvergenceValue(this->matrixSolverConf.getSolverCvgValue());
    this->iterationInfo.setNIterations(this->matrixSolverConf.getNOuterIterations());
    this->iterationInfo.setOutputFrequency(this->matrixSolverConf.getOutputFrequency());
    this->iterationInfo.setOutputFileName(this->matrixSolverConf.getOutputFileName());

    if (pMatrixSolver)
    {
        this->matrixSolverConf = pMatrixSolver->matrixSolverConf;
        this->iterationInfo = pMatrixSolver->iterationInfo;
    }
}

RMatrixSolver::RMatrixSolver(const RMatrixSolverConf &matrixSolverConf)
    : matrixSolverConf(matrixSolverConf)
{
    this->_init();
}

RMatrixSolver::RMatrixSolver(const RMatrixSolver &matrixSolver)
{
    this->_init(&matrixSolver);
}

RMatrixSolver::~RMatrixSolver()
{
}

RMatrixSolver & RMatrixSolver::operator =(const RMatrixSolver &matrixSolver)
{
    this->_init(&matrixSolver);
    return (*this);
}

void RMatrixSolver::solve(const RSparseMatrix &A, const RRVector &b, RRVector &x, RMatrixPreconditionerType matrixPreconditionerType, unsigned int blockSize)
{
    RMatrixPreconditioner P(A,matrixPreconditionerType,blockSize);
    RRVector y(b);

    double An = A.findNorm();
    double bn = RRVector::norm(b);
    double equationScale = 1.0;

    if (bn != 0.0 && An != 0.0)
    {
        equationScale = 1.0e9 / std::abs(bn/An);
    }

    RLogger::info("Unknowns = %u\n",b.size());
    RLogger::info("||A|| = %13e\n",An);
    RLogger::info("||b|| = %13e\n",bn);

    this->iterationInfo.printHeader(RMatrixSolverConf::getName(this->matrixSolverConf.getType()));

    this->iterationInfo.setEquationScale(equationScale);

    x.resize(b.getNRows(),0.0);

    y *= equationScale;
    x *= equationScale;

    switch (this->matrixSolverConf.getType())
    {
        case RMatrixSolverConf::CG:
            this->solveCG(A,y,x,P);
            break;
        case RMatrixSolverConf::GMRES:
            this->solveGMRES(A,y,x,P);
            break;
        default:
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Unknown matrix solver type \'%d\'",this->matrixSolverConf.getType());
    }

    x *= 1.0/equationScale;

    this->iterationInfo.printFooter();
}

void RMatrixSolver::disableConvergenceLogFile(void)
{
    this->iterationInfo.setOutputFileName(QString());
}

void RMatrixSolver::solveCG(const RSparseMatrix &A, const RRVector &b, RRVector &x, RMatrixPreconditioner &P)
{
    unsigned int m = A.getNRows();

    RRVector r(m);
    RRVector z(m);
    RRVector p(m);
    RRVector q(m);

    r.fill(0.0);
    z.fill(0.0);
    p.fill(0.0);
    q.fill(0.0);

    double An = 0.0;
    double bn = 0.0;
    double ro[] = {0.0,0.0};
    double beta = 0.0;
    double dot = 0.0;
    std::vector< std::vector<unsigned int> > index(m);

#pragma omp parallel default(shared)
    {
        // Compute initial residual.
#pragma omp for reduction(+:An,bn)
        for (int64_t i=0;i<int64_t(m);i++)
        {
            r[i] = b[i];
            bn = bn + (b[i]*b[i]);
            index[i] = A.getRowIndexes(i);
            for (unsigned int j=0;j<index[i].size();j++)
            {
                double value = A.getValue(i,j);
                An = An + std::pow(value,2);
                if (value != 0.0)
                {
                    if (x[index[i][j]] != 0.0)
                    {
                        r[i] -= value * x[index[i][j]];
                    }
                }
            }
        }
#pragma omp barrier
#pragma omp master
        {
            An = std::sqrt(An);
            bn = std::sqrt(bn);
        }

        // Iterate and look for the solution
        for (unsigned int it=0;it<this->matrixSolverConf.getNOuterIterations();it++)
        {
#pragma omp barrier
#pragma omp master
            {
                this->iterationInfo.setIteration(it);

                P.compute(r,z);

                ro[0] = RRVector::dot(r,z);

                if (it > 0)
                {
                    beta = ro[0] / ro[1];
                    ro[1]= std::max(ro[1],RConstants::eps);
                }
            }

#pragma omp barrier
#pragma omp for
            for (int64_t i=0;i<int64_t(m);i++)
            {
                if (it == 0)
                {
                    p[i] = z[i];
                }
                else
                {
                    p[i] = p[i]*beta + z[i];
                }
            }

            // q = A*p
#pragma omp master
            dot = 0.0;
#pragma omp barrier
#pragma omp for reduction(+:dot)
            for (int64_t i=0;i<int64_t(m);i++)
            {
                q[i] = 0.0;
                for (unsigned int j=0;j<index[i].size();j++)
                {
                    double value = A.getValue(i,j);
                    if (value != 0.0)
                    {
                        q[i] += value * p[index[i][j]];
                    }
                }
                dot = dot + p[i]*q[i];
            }

#pragma omp master
            {
                if (dot > 0.0)
                {
                    dot = std::max(dot,RConstants::eps);
                }
                else if (dot < 0.0)
                {
                    dot = std::min(dot,-RConstants::eps);
                }
                else
                {
                    dot = RConstants::eps;
                }

                double alpha = ro[0] / dot;
                ro[1] = ro[0];

                // x += alpha*p
                // r -= alpha*q
                for (unsigned int i=0;i<m;i++)
                {
                    x[i] += alpha * p[i];
                    r[i] -= alpha * q[i];
                }

                double xn = RRVector::norm(x);
                double rn = RRVector::norm(r);

                double norm = An * xn + bn;
                if (std::abs(norm) < RConstants::eps)
                {
                    norm = RConstants::eps;;
                }

                this->iterationInfo.setError(rn / norm);
                this->iterationInfo.printIteration();
            }

#pragma omp barrier
            if (this->iterationInfo.hasConverged())
            {
                break;
            }
        }
    }
}

void RMatrixSolver::solveGMRES(const RSparseMatrix &A, const RRVector &b, RRVector &x, RMatrixPreconditioner &P)
{
    uint mA = A.getNRows();
    uint nouter = this->matrixSolverConf.getNOuterIterations();
    uint ninner = this->matrixSolverConf.getNInnerIterations();

    RRVector ro(mA,0.0);
    RRVector w(mA,0.0);
    RRVector p(ninner+1,0.0);
    RRVector y(ninner,0.0);
    RRVector c(ninner,0.0);
    RRVector s(ninner,0.0);
    RRMatrix v(ninner+1,mA,0.0);
    RRMatrix z(ninner+1,mA,0.0);
    RRMatrix h(ninner+1,ninner,0.0);

    std::vector< std::vector<uint> > indexes;
    for (uint i=0;i<mA;i++)
    {
        indexes.push_back(A.getRowIndexes(i));
    }

    double An = A.findNorm();
    double bn = RRVector::norm(b);

    double beta = 0.0;
    double xn = 0.0;

#pragma omp parallel default(shared)
    {
        uint iti = 0;

        // Outer iteration
        for (uint ito=0;ito<nouter;ito++)
        {
#pragma omp master
            {
                this->iterationInfo.setIteration(ito);

                // Initialize matrix h, Krylov
                h.fill(0.0);

                // Compute initial residual ro and norm beta
                beta = 0.0;
                xn = 0.0;
            }
#pragma omp for reduction(+:xn,beta)
            for (int64_t i=0;i<int64_t(mA);i++)
            {
                ro[i] = b[i];
                double lxn = 0.0;
                for (uint j=0;j<indexes[i].size();j++)
                {
                    ro[i] -= A.getValue(i,j) * x[indexes[i][j]];
                    lxn += std::pow(x[indexes[i][j]],2);
                }
                xn += lxn;
                beta += std::pow(ro[i],2);
            }

#pragma omp master
            {
                xn = std::sqrt(xn);
                beta = std::sqrt(beta);

                // Check convergence
                double norm = An * xn + bn;
                if (std::fabs(norm) < RConstants::eps)
                {
                    norm = RConstants::eps;
                }
                this->iterationInfo.setError(beta / norm);
                this->iterationInfo.printIteration();
            }
#pragma omp barrier
            if (this->iterationInfo.hasConverged())
            {
                break;
            }

            // Define first krylov vector v[0]
#pragma omp for
            for (int64_t i=0;i<int64_t(mA);i++)
            {
                v[0][i] = (beta == 0.0) ? 0.0 : ro[i] / beta;
            }
#pragma omp master
            {
                // Initial rhs define p
                p.fill(0.0);
                p[0] = beta;
            }
            // Inner iteration
            for (iti=0;iti<ninner;iti++)
            {
#pragma omp barrier
#pragma omp master
                {
                    // Preconditioning
                    P.compute(v[iti],z[iti]);
//                    RLogger::warning("%u v %13g\n",iti,RRVector::norm(v[iti])-1.0);
//                    RLogger::warning("%u z %13g\n",iti,RRVector::norm(z[iti]));
                }
#pragma omp barrier
                // A multiplied by the last krylov vector at present
#pragma omp for
                for (int64_t i=0;i<int64_t(mA);i++)
                {
                    w[i] = 0.0;
                    for (uint j=0;j<indexes[i].size();j++)
                    {
                        w[i] += A.getValue(i,j) * z[iti][indexes[i][j]];
                    }
                }
#pragma omp barrier
                // of potential krylov vector with existing subtract dot prod to get orthogonal
#pragma omp master
                {
                    for (uint i=0;i<=iti;i++)
                    {
                        for (uint j=0;j<mA;j++)
                        {
                            h[i][iti] += w[j] * v[i][j];
                        }
                        for (uint j=0;j<mA;j++)
                        {
                            w[j] -= h[i][iti] * v[i][j];
                        }
                    }
                    // Finding norm of the krylov vector
                    h[iti+1][iti] = RRVector::norm(w);
//                    RLogger::warning("%u w %13g\n",iti,RRVector::norm(w));
                }
#pragma omp barrier
                // New krylov vector formed
#pragma omp for
                for (int64_t i=0;i<int64_t(mA);i++)
                {
                    v[iti+1][i] = (h[iti+1][iti] == 0.0) ? 0.0 : w[i] / h[iti+1][iti];
                }
#pragma omp barrier
#pragma omp master
                {
                    for (uint i=1;i<=iti;i++)
                    {
                        double hsave = h[i-1][iti];
                        h[i-1][iti] =  c[i-1]*hsave + s[i-1]*h[i][iti];
                        h[i]  [iti] = -s[i-1]*hsave + c[i-1]*h[i][iti];
                    }

                    double g = std::sqrt(  h[iti][iti]   * h[iti][iti]
                                         + h[iti+1][iti] * h[iti+1][iti]);
//                    RLogger::warning("%u g %13g\n",iti,g);
                    if (g == 0.0)
                    {
                        c[iti] = s[iti] = 0.0;
                    }
                    else
                    {
                        c[iti] = h[iti][iti]/g;
                        s[iti] = h[iti+1][iti]/g;
                    }
                    h[iti]  [iti] = g;
                    h[iti+1][iti] = 0.0;
                    p[iti+1]      = -s[iti]*p[iti];
                    p[iti]        =  c[iti]*p[iti];
                }
#pragma omp barrier
                if (fabs(p[iti+1]) < beta*1.0e-4)
                {
                    iti++;
                    break;
                }
            } // inner iteration

#pragma omp barrier
            // Backward substitution
#pragma omp master
            {
                y[iti-1] = (h[iti-1][iti-1] == 0.0) ? 0.0 : p[iti-1] / h[iti-1][iti-1];
                for (int i=iti-2;i>=0;i--)
                {
                    for (int j=iti-1;j>i;j--)
                    {
                        p[i] -= y[j] * h[i][j];
                    }
                    y[i] = (h[i][i] == 0.0) ? 0.0 : p[i] / h[i][i];
//                    RLogger::warning("%u %u y %13g\n",iti,i,y[i]);
                }
            }

#pragma omp barrier
            // Update prod
#pragma omp for
            for (int64_t i=0;i<int64_t(mA);i++)
            {
                for (uint j=0;j<=iti-1;j++)
                {
                    x[i] += z[j][i] * y[j];
                }
            }
        } // outer iteration
    }
}
