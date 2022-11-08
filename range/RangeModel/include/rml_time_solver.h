/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_time_solver.h                                        *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   13-th June 2012                                          *
 *                                                                   *
 *  DESCRIPTION: Time-Solver class declaration                       *
 *********************************************************************/

#ifndef RML_TIME_SOLVER_H
#define RML_TIME_SOLVER_H

#include <vector>
#include <QString>

#define R_TIME_STEP_DEFAULT_SIZE          0.001
#define R_TIME_STEP_DEFAULT_NUMBER        1
#define R_TIME_STEP_MIN_NUMBER            1
#define R_TIME_STEP_MAX_NUMBER            1000000
#define R_TIME_STEP_DEFAULT_START         1.0
#define R_TIME_MIN_OUTPUT_FREQUENCY       0
#define R_TIME_MAX_OUTPUT_FREQUENCY       1000000
#define R_TIME_DEFAULT_OUTPUT_FREQUENCY   1

#define R_TIME_MARCH_APPROXIMATION_TYPE_IS_VALID(_type) \
( \
    _type >= R_TIME_MARCH_CENTRAL && \
    _type < R_TIME_MARCH_N_TYPES \
)

//! Time marching approximation.
typedef enum _RTimeMarchApproximation
{
    R_TIME_MARCH_CENTRAL = 0,
    R_TIME_MARCH_BACKWARD,
    R_TIME_MARCH_FORWARD,
    R_TIME_MARCH_N_TYPES
} RTimeMarchApproximation;

//! Time-Solver class.
class RTimeSolver
{

    protected:

        //! Time-Solver enabled.
        bool enabled;

        // Time marching settings

        //! Time-marching approximation.
        RTimeMarchApproximation timeMarchApproximation;
        //! Vector of time values (seconds).
        std::vector<double> times;
        //! Input number of time steps.
        uint inputNTimeSteps;
        //! Input start time.
        double inputStartTime;
        //! Input time-step size.
        double inputTimeStepSize;
        //! Current time-step.
        uint currentTimeStep;
        //! Current computed time.
        double computedTime;
        //! Write output frequency.
        uint outputFrequency;

    private:

        //! Internal initialization function.
        void _init(const RTimeSolver *pTimeSolver = nullptr);

        //! Sort array of times.
        void sort();

    public:

        //! Constructor.
        RTimeSolver ();

        //! Copy constructor.
        RTimeSolver(const RTimeSolver &timeSolver);

        //! Destructor.
        ~RTimeSolver ();

        //! Assignment operator.
        RTimeSolver & operator =(const RTimeSolver &timeSolver);

        //! Return enabled.
        bool getEnabled(void) const;

        //! Set enabled.
        void setEnabled(bool enabled);

        //! Return time march approximation.
        RTimeMarchApproximation getTimeMarchApproximation() const;

        //! Return alpha coefficient for time approximation.
        double getTimeMarchApproximationCoefficient() const;

        //! Set time march approximation.
        void setTimeMarchApproximation(RTimeMarchApproximation timeMarchApproximation);

        //! Return const reference to vector containing all time step times.
        const std::vector<double> & getTimes() const;

        //! Return reference to vector containing all time step times.
        //! This function will also set the current time step to zero (first time step).
        std::vector<double> & getTimes();

        //! Set new vector containing all time step times.
        void setTimes(const std::vector<double> &times);

        //! Add new time step times.
        void addTimes( uint startTimeStep,
                       uint nTimeSteps,
                       double timeStepSize);

        //! Return input number of time-steps.
        uint getInputNTimeSteps(void) const;

        //! Set input number of time-steps.
        void setInputNTimeSteps(uint inputNTimeSteps);

        //! Return input start time.
        double getInputStartTime() const;

        //! Return input start time.
        void setInputStartTime(double inputStartTime);

        //! Return input time-step size.
        double getInputTimeStepSize() const;

        //! Set input time step size.
        void setInputTimeStepSize(double inputTimeStepSize);

        //! Return current time step.
        uint getCurrentTimeStep() const;

        //! Set current time step.
        void setCurrentTimeStep(uint currentTimeStep);

        //! Return write output frequency.
        uint getOutputFrequency() const;

        //! Set write output frequency.
        void setOutputFrequency(uint outputFrequency);

        //! Set next time step and return modified current time step.
        //! If no next time step can be set RConstants::eod is returned.
        uint setNextTimeStep();

        //! Return current time step size.
        double getCurrentTimeStepSize() const;

        //! Return computed time.
        double getComputedTime() const;

        //! Set computed time.
        void setComputedTime(double computedTime);

        //! Return current time.
        double getCurrentTime() const;

        //! Return previous time.
        double getPreviousTime() const;

        //! Return time at given time step.
        double getTime(uint timeStep) const;

        //! Return first time in the list.
        double getFirstTime() const;

        //! Return last time in the list.
        double getLastTime() const;

        //! Return number of time steps.
        uint getNTimeSteps() const;

        //! Return true if given time is in the time interval.
        bool hasTime(double timeValue) const;

        //! Harmonize times with input.
        void harmonizeTimesWithInput(bool restart);

        //! Return time-marching approximation name.
        static const QString & getTimeMarchApproximationName(RTimeMarchApproximation timeApproximation);

        //! Compute times vector.
        static std::vector<double> findTimesVector(uint nTimeSteps,
                                                   double startTime,
                                                   double timeStepSize);

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif /* RML_TIME_SOLVER_H */
