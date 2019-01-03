/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_matrix_solver_conf.h                                 *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   16-th August 2012                                        *
 *                                                                   *
 *  DESCRIPTION: Matrix Solver class declaration                     *
 *********************************************************************/

#ifndef RML_MATRIX_SOLVER_H
#define RML_MATRIX_SOLVER_H

#include <QString>

//! Matrix solver type.
typedef int RMatrixSolverType;

//! Matrix solver class.
class RMatrixSolverConf
{

    public:

        enum Type
        {
            None = 0,
            CG,
            GMRES,
            NTypes
        };

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
        void _init ( const RMatrixSolverConf *pMatrixSolver = nullptr );

    public:

        //! Constructor.
        RMatrixSolverConf ( RMatrixSolverType type = RMatrixSolverConf::None );

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

        //! Return solver id.
        static const QString & getId ( RMatrixSolverType type );
};

#endif /* RML_MATRIX_SOLVER_H */
