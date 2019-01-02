/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_eigen_value_solver_conf.h                            *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   1-st July 2015                                           *
 *                                                                   *
 *  DESCRIPTION: Eigen value solver class declaration                *
 *********************************************************************/

#ifndef RML_EIGEN_VALUE_SOLVER_CONF_H
#define RML_EIGEN_VALUE_SOLVER_CONF_H

#include <QString>

class REigenValueSolverConf
{

    protected:

    public:

        //! Methods
        enum Method
        {
            None = 0,
            Lanczos,
            Arnoldi,
            Rayleigh,
            NTypes
        };

        //! Solver method.
        Method method;
        //! Number of iterations.
        unsigned int nIterations;
        //! Number of eigen values.
        unsigned int nEigenValues;
        //! Convergence value.
        double solverCvgValue;
        //! Output frequency.
        unsigned int outputFrequency;
        // FOLOWING MEMBERS ARE NOT SAVED TO FILE
        //! Output file name.
        QString outputFileName;

    private:

        //! Internal initialization function.
        void _init(const REigenValueSolverConf *pEigenValueSolverConf = nullptr);

    public:

        //! Constructor.
        REigenValueSolverConf(REigenValueSolverConf::Method method = REigenValueSolverConf::None);

        //! Copy constructor.
        REigenValueSolverConf(const REigenValueSolverConf &eigenValueSolverConf);

        //! Destructor.
        ~REigenValueSolverConf();

        //! Assignment operator.
        REigenValueSolverConf &operator =(const REigenValueSolverConf &eigenValueSolverConf);

        //! Return eigen value solver method.
        REigenValueSolverConf::Method getMethod(void) const;

        //! Set eigen value solver method.
        void setMethod(REigenValueSolverConf::Method method);

        //! Return number of iterations.
        unsigned int getNIterations(void) const;

        //! Set number of iterations.
        void setNIterations(unsigned int nIterations);

        //! Return number of eigen values.
        unsigned int getNEigenValues(void) const;

        //! Set number of eigen values.
        void setNEigenValues(unsigned int nEigenValues);

        //! Return solver convergence value.
        double getSolverCvgValue(void) const;

        //! Set solver convergence value.
        void setSolverCvgValue(double solverCvgValue);

        //! Return output frequency.
        unsigned int getOutputFrequency(void) const;

        //! Set output frequency.
        void setOutputFrequency(unsigned int outputFrequency);

        //! Return output file name.
        const QString & getOutputFileName(void) const;

        //! Set output file name.
        void setOutputFileName(const QString &outputFileName);

        //! Allow RFileIO to access private members.
        friend class RFileIO;

        //! Return solver name.
        static const QString &getName(REigenValueSolverConf::Method method);

};

#endif // RML_EIGEN_VALUE_SOLVER_CONF_H
