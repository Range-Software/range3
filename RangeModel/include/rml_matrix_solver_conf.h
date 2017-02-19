/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_matrix_solver_conf.h                                 *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   16-th August 2012                                        *
 *                                                                   *
 *  DESCRIPTION: Matrix Solver class declaration                     *
 *********************************************************************/

#ifndef __RML_MATRIX_SOLVER_H__
#define __RML_MATRIX_SOLVER_H__

#include <QString>

//! Matrix solver type.
typedef enum _RMatrixSolverType
{
    R_MATRIX_SOLVER_NONE = 0,
    R_MATRIX_SOLVER_JACOBI,
    R_MATRIX_SOLVER_GAUSS_SEIDL,
    R_MATRIX_SOLVER_SOR,
    R_MATRIX_SOLVER_SSOR,
    R_MATRIX_SOLVER_CG,
    R_MATRIX_SOLVER_CGS,
    R_MATRIX_SOLVER_BiCG,
    R_MATRIX_SOLVER_BiCGSTAB,
    R_MATRIX_SOLVER_GMRES,
    R_MATRIX_SOLVER_QMR,
    R_MATRIX_SOLVER_CHEBYSHEV,
    R_MATRIX_SOLVER_N_TYPES
} RMatrixSolverType;


//! Matrix solver class.
class RMatrixSolverConf
{

    protected:

        //! Matrix solver type.
        RMatrixSolverType type;
        //! Number of inner iterations.
        unsigned int nInnerIterations;
        //! Number of outer iterations.
        unsigned int nOuterIterations;
        //! Convergence value.
        double solverCvgValue;
        //! Output frequency.
        unsigned int outputFrequency;
        // FOLOWING MEMBERS ARE NOT SAVED TO FILE
        //! Output file name.
        QString outputFileName;

    private:

        //! Internal initialization function.
        void _init ( const RMatrixSolverConf *pMatrixSolver = 0 );

    public:

        //! Constructor.
        RMatrixSolverConf ( RMatrixSolverType type = R_MATRIX_SOLVER_NONE );

        //! Copy constructor.
        RMatrixSolverConf ( const RMatrixSolverConf &matrixSolver );

        //! Destructor.
        ~RMatrixSolverConf ();

        //! Assignment operator.
        RMatrixSolverConf & operator = ( const RMatrixSolverConf &matrixSolver );

        //! Return matrix solver type.
        RMatrixSolverType getType ( void ) const;

        //! Set matrix solver type.
        void setType ( RMatrixSolverType type );

        //! Return number of inner iterations.
        unsigned int getNInnerIterations ( void ) const;

        //! Set number of inner iterations.
        void setNInnerIterations ( unsigned int nInnerIterations );

        //! Return number of outer iterations.
        unsigned int getNOuterIterations ( void ) const;

        //! Set number of outer iterations.
        void setNOuterIterations ( unsigned int nOuterIterations );

        //! Return solver convergence value.
        double getSolverCvgValue ( void ) const;

        //! Set solver convergence value.
        void setSolverCvgValue ( double solverCvgValue );

        //! Return output frequency.
        unsigned int getOutputFrequency ( void ) const;

        //! Set output frequency.
        void setOutputFrequency ( unsigned int outputFrequency );

        //! Return output file name.
        const QString & getOutputFileName ( void ) const;

        //! Set output file name.
        void setOutputFileName ( const QString &outputFileName );

        //! Allow RFileIO to access private members.
        friend class RFileIO;

        //! Return solver name.
        static const QString & getName ( RMatrixSolverType type );
};

#endif /* __RML_MATRIX_SOLVER_H__ */
