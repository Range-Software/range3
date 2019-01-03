/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_modal_setup.h                                        *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   10-th May 2016                                           *
 *                                                                   *
 *  DESCRIPTION: Modal setup class declaration                       *
 *********************************************************************/

#ifndef RML_MODAL_SETUP_H
#define RML_MODAL_SETUP_H

#include <QString>

#define R_MODAL_ITERATIONS_DEFAULT_NUMBER 100
#define R_MODAL_ITERATIONS_MIN_NUMBER     1
#define R_MODAL_ITERATIONS_MAX_NUMBER     1000000
#define R_MODAL_CONVERGENCE_DEFAULT_VALUE 1.0e-9
#define R_MODAL_CONVERGENCE_MIN_VALUE     0.0
#define R_MODAL_CONVERGENCE_MAX_VALUE     1.0

#define R_MODAL_METHOD_TYPE_IS_VALID(_type) \
( \
    _type >= R_MODAL_DOMINANT_MODE && \
    _type < R_MODAL_N_TYPES \
)

//! Modal analysis method.
typedef enum _RModalMethod
{
    R_MODAL_DOMINANT_MODE = 0,
    R_MODAL_MULTIPLE_MODES,
    R_MODAL_N_TYPES
} RModalMethod;

class RModalSetup
{

    protected:

        //! Modal analysis method.
        RModalMethod method;
        //! Number of iterations.
        uint nIterations;
        //! Number of modes to extract.
        uint nModesToExtract;
        //! Convergence value.
        double convergenceValue;
        //! Current mode.
        uint mode;
        //! Current frequency.
        double frequency;

    private:

        //! Internal initialization function.
        void _init(const RModalSetup *pModalSetup = nullptr);

    public:

        //! Constructor.
        RModalSetup();

        //! Copy constructor.
        RModalSetup(const RModalSetup &modalSetup);

        //! Destructor.
        ~RModalSetup();

        //! Assignment operator.
        RModalSetup &operator =(const RModalSetup &modalSetup);

        //! Return modal analysis method.
        RModalMethod getMethod(void) const;

        //! Set modal analysis method.
        void setMethod(RModalMethod method);

        //! Return number of modal iterations.
        uint getNIterations(void) const;

        //! Set number of modal iterations.
        void setNIterations(uint nIterations);

        //! Return number of modes to extract.
        uint getNModesToExtract(void) const;

        //! Set number of modes to extract.
        void setNModesToExtract(uint nModesToExtract);

        //! Return modal analysis convergence value.
        double getConvergenceValue(void) const;

        //! Set modal analysis convergence value.
        void setConvergenceValue(double convergenceValue);

        //! Return current mode.
        uint getMode(void) const;

        //! Set current mode.
        void setMode(uint mode);

        //! Return current mode frequency.
        double getFrequency(void) const;

        //! Set current mode frequency.
        void setFrequency(double frequency);

        //! Return modal analysis method name.
        static const QString &getMethodName(RModalMethod method);

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif // RML_MODAL_SETUP_H
