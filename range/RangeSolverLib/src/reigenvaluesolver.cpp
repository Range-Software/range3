/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   reigenvaluesolver.cpp                                    *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   1-st July 2015                                           *
 *                                                                   *
 *  DESCRIPTION: Eigen value solver class definition                 *
 *********************************************************************/

#include <cmath>

#include "reigenvaluesolver.h"

#define R_EIS_PYTHAG(a,b) (std::sqrt (a*a + b*b))
#define R_ASSERT(_condition) { if (!(_condition)) { RLogger::unindent(); R_ERROR_ASSERT(_condition); } }

void REigenValueSolver::_init(const REigenValueSolver *pEigenValueSolver)
{
    if (pEigenValueSolver)
    {
        this->eigenValueSolverConf = pEigenValueSolver->eigenValueSolverConf;
        this->matrixSolverConf = pEigenValueSolver->matrixSolverConf;
    }
}

REigenValueSolver::REigenValueSolver(const REigenValueSolverConf &eigenValueSolverConf, const RMatrixSolverConf &matrixSolverConf)
    : eigenValueSolverConf(eigenValueSolverConf)
    , matrixSolverConf(matrixSolverConf)
{
    this->_init();
}

REigenValueSolver::REigenValueSolver(const REigenValueSolver &eigenValueSolver)
{
    this->_init(&eigenValueSolver);
}

REigenValueSolver::~REigenValueSolver()
{

}

REigenValueSolver &REigenValueSolver::operator =(const REigenValueSolver &eigenValueSolver)
{
    this->_init(&eigenValueSolver);
    return (*this);
}

void REigenValueSolver::solve(const RSparseMatrix &M, const RSparseMatrix &K, RRVector &d, RRMatrix &ev)
{
    switch (this->eigenValueSolverConf.method)
    {
        case REigenValueSolverConf::Lanczos:
        {
            // Find multiple eigen values.
            try
            {
                this->solveLanczos(M,K,d);
            }
            catch (const RError &error)
            {
                throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Lanczos method failed. %s",error.getMessage().toUtf8().constData());
            }
            break;
        }
        case REigenValueSolverConf::Arnoldi:
        {
            // Find multiple eigen values.
            try
            {
                this->solveArnoldi(M,K,d,ev);
            }
            catch (const RError &error)
            {
                throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Arnoldi method failed. %s",error.getMessage().toUtf8().constData());
            }
            break;
        }
        case REigenValueSolverConf::Rayleigh:
        {
            // Find one (most dominant) eigen value.
            try
            {
                this->solveRayleigh(M,K,d,ev);
            }
            catch (const RError &error)
            {
                throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Rayleigh method failed. %s",error.getMessage().toUtf8().constData());
            }
            break;
        }
        default:
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Unknown eigen-value solver method.");
        }
    }

    if (d.getNRows() > 1)
    {
        try
        {
            for (uint i=0;i<d.getNRows();i++)
            {
                d[i] = std::fabs(1.0/d[i]);
            }
            std::vector<uint> indexes;
            std::swap(d[0],d[1]);
            RUtil::qSort(d,indexes);

            for (uint i=0;i<ev.getNRows();i++)
            {
                uint n1 = i;
                uint n2 = indexes[i];
                for (uint j=0;j<ev.getNColumns();j++)
                {
                    std::swap(ev[n1][j],ev[n2][j]);
                }
                indexes[n2] = n2;
            }
        }
        catch (const RError &error)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to reorder eigen values and vectors. %s",error.getMessage().toUtf8().constData());
        }
    }
}

void REigenValueSolver::solveLanczos(const RSparseMatrix &M, const RSparseMatrix &K, RRVector &d)
{
    RRVector e;
    RRVector v(M.getNRows(),0.0);
    RRVector vo(M.getNRows(),0.0);
    RRVector w(M.getNRows(),0.0);

    for (uint i=0;i<M.getNRows();i++)
    {
        v[i] = double(std::rand()) / double(RAND_MAX);
    }
    v.normalize();

    RMatrixSolver solver(this->matrixSolverConf);

    uint ne = std::min(this->eigenValueSolverConf.getNEigenValues(),K.getNRows());

    d.resize(ne,0.0);
    e.resize(ne,0.0);

    // Lanczos iteration
    for (uint i=0;i<ne;i++)
    {
        RLogger::info("Lanczos iteration %u of %u\n",i+1,ne);
        RLogger::indent();

        // w = A*v - e(i)*vo
        // w = (M/K)*v - e(i)*vo
        // K*w = M*v - K*e(i)*vo

        RRVector b;

        RSparseMatrix::mlt(M,v,b);

        if (i > 0)
        {
            RRVector bTmp;
            RSparseMatrix::mlt(K,vo,bTmp);
            bTmp *= e[i];
            for (uint j=0;j<b.size();j++)
            {
                b[j] -= bTmp[j];
            }
        }

        try
        {
            solver.solve(K,b,w,R_MATRIX_PRECONDITIONER_JACOBI);
        }
        catch (const RError &error)
        {
            RLogger::unindent();
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to solve matrix system. %s", error.getMessage().toUtf8().constData());
        }

        // d(i) = w dot v
        d[i] = RRVector::dot(w,v);

        if (i+1 < ne)
        {
            // w = w - d(i)*v
            for (uint j=0;j<w.size();j++)
            {
                w[j] -= d[i] * v[j];
            }

            // e(i+1) = || w ||
            e[i+1] = RRVector::norm(w);
            R_ASSERT(e[i+1] != 0.0);

            // vo = v
            vo = v;

            // v = w / e(i+1)
            for (uint j=0;j<v.size();j++)
            {
                v[j] = w[j] / e[i+1];
            }
        }
        RLogger::unindent();
    }

    try
    {
        REigenValueSolver::qlDecomposition(d,e);
    }
    catch (const RError &error)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"QL decomposition failed. %s",error.getMessage().toUtf8().constData());
    }
}

void REigenValueSolver::solveArnoldi(const RSparseMatrix &M, const RSparseMatrix &K, RRVector &d, RRMatrix &ev)
{
    uint ne = std::min(this->eigenValueSolverConf.getNEigenValues(),K.getNRows());
    uint n = M.getNRows();

    RRMatrix H(ne,ne,0.0);
    RRMatrix Qa(n,ne,0.0);
    RRMatrix Qq;
    RRVector q(n), qo(n);

    // generate random normalized vector
    for (uint i=0;i<n;i++)
    {
        q[i] = double(rand()) / double(RAND_MAX);
    }
    R_ERROR_ASSERT(q.normalize() != 0.0);
    for (uint i=0;i<n;i++)
    {
        Qa[i][0] = q[i];
    }

    RMatrixSolver solver(this->matrixSolverConf);

    // Arnoldi iteration
    for (uint i=1;i<ne+1;i++)
    {
        RLogger::info("Arnoldi iteration %u of %u\n",i,ne);
        RLogger::indent();

        for (uint j=0;j<n;j++)
        {
            qo[j] = Qa[j][i-1];
        }

        // K*q = M*qo
        RRVector f;
        RSparseMatrix::mlt(M,qo,f);
        try
        {
            solver.solve(K,f,q,R_MATRIX_PRECONDITIONER_JACOBI);
        }
        catch (const RError &error)
        {
            RLogger::unindent();
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to solve matrix system. %s", error.getMessage().toUtf8().constData());
        }

        for (uint j=0;j<i;j++)
        {
            for (uint k=0;k<n;k++)
            {
                qo[k] = Qa[k][j];
            }
            H[j][i-1] = RRVector::dot(qo,q);
            for (uint k=0;k<n;k++)
            {
                q[k] -= H[j][i-1] * qo[k];
            }
        }
        if (i < ne)
        {
            H[i][i-1] = RRVector::norm(q);
            R_ASSERT(H[i][i-1] != 0.0);
            for (uint j=0;j<n;j++)
            {
                Qa[j][i] = q[j] / H[i][i-1];
            }
        }
        RLogger::unindent();
    }

    try
    {
        REigenValueSolver::qrDecomposition(H,this->eigenValueSolverConf.getNIterations(),this->eigenValueSolverConf.getSolverCvgValue(),d,Qq);
    }
    catch (const RError &error)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"QR decomposition failed. %s",error.getMessage().toUtf8().constData());
    }

    RRMatrix::mlt(Qa,Qq,ev);
    ev.transpose();
}

void REigenValueSolver::solveRayleigh(const RSparseMatrix &M, const RSparseMatrix &K, RRVector &e, RRMatrix &ev)
{
    RSparseMatrix M2(M);
    uint n = M2.getNRows();

    e.resize(1,0.0);
    ev.resize(1,n,0.0);

    RRVector b(n);
    RRVector f(n);
    RRVector c(n);
    RRVector d(n);

    for (uint i=0;i<n;i++)
    {
        b[i] = double(rand()) / double(RAND_MAX);
    }
    b.normalize();

    double eps = this->eigenValueSolverConf.getSolverCvgValue();
    double mu = std::max(d[0], 1.0e9 * double(rand()) / double(RAND_MAX));
    double muo = mu + 2 * eps;
    uint nIterations = this->eigenValueSolverConf.getNIterations();

    std::vector< std::vector<uint> > mIndexes;
    mIndexes.resize(n);
    for (uint i=0;i<n;i++)
    {
        mIndexes[i] = M2.getRowIndexes(i);
    }

    std::vector< std::vector<uint> > kIndexes;
    kIndexes.resize(n);
    for (uint i=0;i<n;i++)
    {
        kIndexes[i] = K.getRowIndexes(i);
    }

    RMatrixSolver solver(this->matrixSolverConf);

    for (uint it=0;it<nIterations;it++)
    {
        RLogger::info("Rayleigh quotient iteration %u of %u\n",it+1,nIterations);
        RLogger::indent();

        muo = mu;

        // Add K*mu to M
        for (uint i=0;i<n;i++)
        {
            f[i] = 0.0;
            for (uint j=0;j<kIndexes[i].size();j++)
            {
                double value = K.getValue(i,j) * mu;
                if (value != 0.0)
                {
                    M2.addValue(i,kIndexes[i][j],value);
                }
                // K*bi
                f[i] += value * b[kIndexes[i][j]];
            }
        }

        // (M + K*ui)*ci = K*bi
        solver.solve(M2,f,c,R_MATRIX_PRECONDITIONER_JACOBI);

        // K*bi/||ci||
        double norm = RRVector::norm(c);
        R_ASSERT(norm != 0.0);
        f *= 1.0 / norm;

        // (M + K*ui)*b(i+1) = K*bi/||ci||
        solver.solve(M,f,b,R_MATRIX_PRECONDITIONER_JACOBI);

        // Remove K*mu from M
        for (uint i=0;i<n;i++)
        {
            for (uint j=0;j<kIndexes[i].size();j++)
            {
                double value = -1.0 * K.getValue(i,j) * mu;
                if (value != 0.0)
                {
                    M2.addValue(i,kIndexes[i][j],value);
                }
            }
        }

        // M*bi
        for (uint i=0;i<n;i++)
        {
            f[i] = 0.0;
            for (uint j=0;j<mIndexes[i].size();j++)
            {
                double value = M2.getValue(i,j);
                f[i] += value * b[mIndexes[i][j]];
            }
        }

        // K*di = M*bi
        solver.solve(K,f,d,R_MATRIX_PRECONDITIONER_JACOBI);

        // mui = bi dot di / ( bi * bi)
        double bDot = RRVector::dot(b,b);
        R_ASSERT(bDot != 0.0);
        mu = RRVector::dot(d,b) / bDot;

        double cvgVal = std::fabs(muo - mu);
        RLogger::info("Convergence rate = %g", cvgVal);

        RLogger::unindent();

        if (it > 0)
        {
            if (cvgVal < this->eigenValueSolverConf.getSolverCvgValue())
            {
                break;
            }
        }
    }

    e[0] = mu;
    for (uint i=0;i<n;i++)
    {
        ev[0][i] = b[i];
    }
}

void REigenValueSolver::qlDecomposition(RRVector &d, RRVector &e)
{
    uint n = d.getNRows();

    // Convenient to renumber the elements of e
    for (uint i=1;i<n;i++)
    {
        e[i-1] = e[i];
    }
    e[n-1] = 0.0;

    for (uint l=0;l<n;l++)
    {
        RLogger::info("QL decomposition %u of %u\n",l+1,n);
        RLogger::indent();

        uint iter = 0;
        uint m = 0;
        while (true)
        {
            // Look for a single small subdiagonal element to split the matrix
            for (m=l;m<n-1;m++)
            {
                double dd = std::fabs(d[m]) + std::fabs(d[m+1]);
                if (std::fabs(e[m]+dd) == dd)
                {
                    break;
                }
            }
            if (m >= l)
            {
                break;
            }
            if (iter ++ >= 30)
            {
                RLogger::warning("Too many iterations %u.\n",iter);
            }
            // Form shift
            double g = (d[l+1] - d[l]) / (2.0*e[l]);
            double r = R_EIS_PYTHAG(g,1.0);
            // This is dm - ks
            g = d[m] - d[l] + e[l] / (g+R_SAME_SIGN(r,g));

            double s = 1.0, c = 1.0, p = 0.0;
            // A plane rotation as in the original QL, followed by Givens rotations to restore tridiagonal form
            uint i;
            for (i=m-1;i>=l;i++)
            {
                double f = s * e[i];
                double b = c * e[i];
                e[i+1] = (r = R_EIS_PYTHAG(f,g));

                // Recover from underflow
                if (r == 0.0)
                {
                    d[i+1] -= p;
                    e[m] = 0.0;
                    break;
                }

                s = f/r;
                c = g/r;
                g = d[i+1] - p;
                r = (d[i] - g) * s + 2.0 * c * b;
                d[i+1] = g + (p = s * r);
                g = c * r - b;
            }
            if (r == 0.0 && i >= l)
            {
                continue;
            }
            d[l] -= p;
            e[l] = g;
            e[m] = 0.0;
        }
        RLogger::unindent();
    }
}

void REigenValueSolver::qrDecomposition(const RRMatrix &H, uint nIterations, double convergenceValue, RRVector &d, RRMatrix &V)
{
    uint n = H.getNRows();

    d.resize(n);
    V.resize(n,n,0.0);

    RRVector a(n);
    RRVector e(n);
    RRMatrix A(H);
    RRMatrix Q(n,n,0.0);
    RRMatrix R(n,n,0.0);

    for (uint it=0;it<nIterations;it++)
    {
        RLogger::info("QR / Gram-Schmidt %u of %u\n",it+1,nIterations);
        RLogger::indent();

        for (uint i=0;i<n;i++)
        {
            for (uint j=0;j<n;j++)
            {
                a[j] = A[j][i];
            }
            RRVector u(a);
            for (uint j=0;j<i;j++)
            {
                for (uint k=0;k<n;k++)
                {
                    e[k] = Q[k][j];
                }
                double dot = RRVector::dot(a,e);
                for (uint k=0;k<n;k++)
                {
                    u[k] -= dot * e[k];
                }
            }
            double norm = RRVector::norm(u);
            R_ASSERT(norm != 0.0);
            e = u;
            e *= 1.0 / norm;
            for (uint j=0;j<n;j++)
            {
                Q[j][i] = e[j];
            }
        }
        for (uint i=0;i<n;i++)
        {
            for (uint j=0;j<n;j++)
            {
                e[j] = Q[j][i];
            }
            for (uint j=i;j<n;j++)
            {
                for (uint k=0;k<n;k++)
                {
                    a[k] = A[k][j];
                }
                R[i][j] = RRVector::dot(a,e);
            }
        }

        RRMatrix::mlt(R,Q,A);

        if (it == 0)
        {
            V = Q;
        }
        else
        {
            RRMatrix Vo(V);
            RRMatrix::mlt(Vo,Q,V);
        }

        bool converged = true;

        for (uint i=1;i<n;i++)
        {
            if (std::fabs(A[i][i-1]) > convergenceValue)
            {
                converged = false;
            }
        }
        if (converged)
        {
            break;
        }
        RLogger::unindent();
    }
    for (uint i=0;i<n;i++)
    {
        d[i] = A[i][i];
    }
}
