/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolvergeneric.h                                         *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   12-th December 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Generic solver class declaration                    *
 *********************************************************************/

#ifndef RSOLVERGENERIC_H
#define RSOLVERGENERIC_H

#include <QString>

#include <vector>

#include <rblib.h>
#include <rmlib.h>

#include "rlocalrotation.h"
#include "rscales.h"
#include "rsolvershareddata.h"

template <typename T>
struct RSolverCartesianVector
{
    public:

        T x, y, z;

};

class RSolverGeneric
{

    protected:

        //! Vacuum permittivity.
        static const double e0;
        //! Stefan–Boltzmann constant.
        static const double sigma;

    protected:

        //! Indicator whether the mesh has changed.
        bool meshChanged;

        //! Problem type.
        RProblemType problemType;
        //! Pointer to model object.
        RModel *pModel;
        //! Model file name.
        QString modelFileName;
        //! Convergence file.
        QString convergenceFileName;
        //! Matrix M (modal analysis).
        RSparseMatrix M;
        //! Matrix A.
        RSparseMatrix A;
        //! Vector x.
        RRVector x;
        //! Vector b.
        RRVector b;
        //! Node book.
        RBook nodeBook;
        //! Local rotations.
        std::vector<RLocalRotation> localRotations;
        //! Element temperature vector.
        RRVector elementTemperature;
        //! Scales.
        RScales scales;
        //! Pointer to shared data container.
        RSolverSharedData *pSharedData;
        //! First run indicator.
        bool firstRun;
        //! Task iteration counter.
        uint taskIteration;
        //! Computable elements array.
        RBVector computableElements;
        //! Includable elements array (not included in coefficient matrix).
        RBVector includableElements;
        //! Surface element inward orientation (if normal is pointing inside computable volume element).
        RBVector inwardElements;

    private:

        //! Internal initialization function.
        void _init(const RSolverGeneric *pGenericSolver = nullptr);

    public:

        //! Constructor.
        RSolverGeneric(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData);

        //! Copy constructor.
        RSolverGeneric(const RSolverGeneric &genericSolver);

        //! Destructor.
        virtual ~RSolverGeneric();

        //! Assignment operator.
        RSolverGeneric & operator =(const RSolverGeneric &genericSolver);

        //! Run solver.
        void run(bool firstRun, uint taskIteration);

        //! Get mesh changed.
        bool getMeshChanged() const;

        //! Set mesh changed.
        void setMeshChanged(bool meshChanged);

        //! Check if solver has converged.
        virtual bool hasConverged(void) const = 0;

        //! Update old records.
        static void updateOldRecords(const RTimeSolver &rTimeSolver, const QString &modelFileName);

    protected:

        //! Find element sizes.
        RRVector findElementSizes(void) const;

        //! Find element scale.
        double findElementScale(bool onlyVolumes = false) const;

        //! Find mesh scale.
        double findMeshScale(void) const;

        //! Update local rotations.
        void updateLocalRotations(void);

        //! Clear shared data.
        virtual void clearSharedData(void);

        //! Store shared data.
        virtual void storeSharedData(void);

        //! Recover shared data.
        virtual void recoverSharedData(void);

        //! Update scales.
        virtual void updateScales(void) = 0;

        //! Recover previously computed results.
        virtual void recover(void) = 0;

        //! Prepare solver.
        virtual void prepare(void) = 0;

        //! Run matrix solver.
        virtual void solve(void) = 0;

        //! Process solver results.
        virtual void process(void) = 0;

        //! Store solver results.
        virtual void store(void) = 0;

        //! Process statistics.
        virtual void statistics(void) = 0;

        //! Write results.
        void writeResults(void);

        //! Apply displacement if possible.
        void applyDisplacement(void);

        //! Remove displacement if possible.
        void removeDisplacement(void);

        //! Generate node book.
        void generateNodeBook(RProblemType problemType);

        //! Generate material element vector.
        void generateMaterialVecor(RMaterialPropertyType materialPropertyType, RRVector &materialPropertyValues) const;

        //! Generate variable element vector from applied environment/initial/boundary condition.
        void generateVariableVector(RVariableType variableType,
                                    RRVector &variableValues,
                                    RBVector &setValues,
                                    bool boundaryConditions,
                                    bool initialConditions,
                                    bool environmentConditions,
                                    bool onlyExplicitBcs = false) const;

        //! Recover variable.
        void recoverVariable(RVariableType variableType,
                             RVariableApplyType applyType,
                             uint expectedSize,
                             uint vectorPosition,
                             RRVector &variableValues,
                             double defaultValue) const;

        //! Sunc values with shared data.
        void syncShared(const QString &keyName, RRVector &values);

        //! Find computable elements based on assigned material properties or has boundary condition assigned.
        void findComputableElements(RProblemType problemType);

        //! Find includable elements (not computed in coefficient matrix) - all points are computable.
        void findIncludableElements();

        //! Find inward surface elements (normal is pointing inside computable element).
        void findInwardElements();

        //! Process monitoring points.
        void processMonitoringPoints(void) const;

        //! Print results statistics.
        void printStats(RVariableType variableType) const;

};

#endif // RSOLVERGENERIC_H
