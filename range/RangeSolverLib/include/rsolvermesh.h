/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolvermesh.h                                            *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   11-th October 2018                                       *
 *                                                                   *
 *  DESCRIPTION: Mesh solver class declaration                       *
 *********************************************************************/

#ifndef RSOLVERMESH_H
#define RSOLVERMESH_H

#include "rsolvergeneric.h"

class RSolverMesh : public RSolverGeneric
{

    protected:

        //! Mesh input.
        RMeshInput meshInput;

    private:

        //! Internal initialization function.
        void _init(const RSolverMesh *pSolver = nullptr);

    public:

        //! Constructor.
        explicit RSolverMesh(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData);

        //! Copy constructor.
        RSolverMesh(const RSolverMesh &solver);

        //! Destructor.
        ~RSolverMesh();

        //! Assignment operator.
        RSolverMesh & operator =(const RSolverMesh &solver);

        //! Check if solver has converged.
        bool hasConverged(void) const;

    protected:

        //! Update scales.
        void updateScales(void);

        //! Recover previously computed results.
        void recover(void);

        //! Prepare solver.
        void prepare(void);

        //! Run matrix solver.
        void solve(void);

        //! Process solver results.
        void process(void);

        //! Store solver results.
        void store(void);

        //! Process statistics.
        void statistics(void);

};

#endif // RSOLVERMESH_H
