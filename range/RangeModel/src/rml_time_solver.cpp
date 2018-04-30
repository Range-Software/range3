/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_time_solver.cpp                                      *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   13-th June 2012                                          *
 *                                                                   *
 *  DESCRIPTION: Time-Solver class definition                        *
 *********************************************************************/

#include <algorithm>

#include <rblib.h>

#include "rml_time_solver.h"

static QString timeApproximationNames [R_TIME_MARCH_N_TYPES] =
{
    "Central difference (accurate)",
    "Backward difference (stable)",
    "Forward difference (fast)"
};

void RTimeSolver::_init(const RTimeSolver *pTimeSolver)
{
    if (pTimeSolver)
    {
        this->enabled = pTimeSolver->enabled;
        this->timeMarchApproximation = pTimeSolver->timeMarchApproximation;
        this->times = pTimeSolver->times;
        this->inputStartTime = pTimeSolver->inputStartTime;
        this->inputTimeStepSize = pTimeSolver->inputTimeStepSize;
        this->inputNTimeSteps = pTimeSolver->inputNTimeSteps;
        this->currentTimeStep = pTimeSolver->currentTimeStep;
        this->outputFrequency = pTimeSolver->outputFrequency;
        this->computedTime = pTimeSolver->computedTime;
    }
} /* RTimeSolver::_init */

void RTimeSolver::sort(void)
{
    std::sort(this->times.begin(),this->times.end());
} /* RTimeSolver::sort */

RTimeSolver::RTimeSolver()
    : enabled(false)
    , timeMarchApproximation(R_TIME_MARCH_CENTRAL)
    , inputNTimeSteps(R_TIME_STEP_DEFAULT_NUMBER)
    , inputStartTime(0.0)
    , inputTimeStepSize(R_TIME_STEP_DEFAULT_SIZE)
    , currentTimeStep(0)
    , computedTime(0.0)
    , outputFrequency(R_TIME_DEFAULT_OUTPUT_FREQUENCY)
{
    this->times.resize(R_TIME_STEP_DEFAULT_NUMBER,R_TIME_STEP_DEFAULT_START + R_TIME_STEP_DEFAULT_SIZE);
    this->_init();
} /* RTimeSolver::RTimeSolver */

RTimeSolver::RTimeSolver(const RTimeSolver &timeSolver)
{
    this->_init(&timeSolver);
} /* RTimeSolver::RTimeSolver (copy) */

RTimeSolver::~RTimeSolver()
{
} /* RTimeSolver::~RTimeSolver */

RTimeSolver &RTimeSolver::operator =(const RTimeSolver &timeSolver)
{
    this->_init(&timeSolver);
    return (*this);
} /* RTimeSolver::operator = */

bool RTimeSolver::getEnabled(void) const
{
    return this->enabled;
} /* RTimeSolver::getEnabled */

void RTimeSolver::setEnabled(bool enabled)
{
    this->enabled = enabled;
} /* RTimeSolver::setEnabled */

RTimeMarchApproximation RTimeSolver::getTimeMarchApproximation(void) const
{
    return this->timeMarchApproximation;
} /* RTimeSolver::getTimeMarchApproximation */

double RTimeSolver::getTimeMarchApproximationCoefficient(void) const
{
    switch (this->timeMarchApproximation)
    {
        case R_TIME_MARCH_CENTRAL:
            return 0.5;
        case R_TIME_MARCH_BACKWARD:
            return 0.0;
        case R_TIME_MARCH_FORWARD:
            return 1.0;
        default:
            return 0.0;
    }
} /* RTimeSolver::getTimeMarchApproximationCoefficient */

void RTimeSolver::setTimeMarchApproximation(RTimeMarchApproximation timeMarchApproximation)
{
    R_ERROR_ASSERT(R_TIME_MARCH_APPROXIMATION_TYPE_IS_VALID(timeMarchApproximation));
    this->timeMarchApproximation = timeMarchApproximation;
} /* RTimeSolver::setTimeMarchApproximation */

const std::vector<double> &RTimeSolver::getTimes(void) const
{
    return this->times;
} /* RTimeSolver::getTimes */

std::vector<double> &RTimeSolver::getTimes(void)
{
    return this->times;
} /* RTimeSolver::getTimes */

void RTimeSolver::setTimes(const std::vector<double> &times)
{
    this->times = times;
//    this->setCurrentTimeStep(0);
    this->sort();
} /* RTimeSolver::setTimes */

void RTimeSolver::addTimes(unsigned int startTimeStep, unsigned int nTimeSteps, double timeStepSize)
{
    if (startTimeStep < this->getNTimeSteps())
    {
        this->times.erase(this->times.begin()+startTimeStep,this->times.end());
    }
    std::vector<double> newTimes = RTimeSolver::findTimesVector(nTimeSteps,this->times.back(),timeStepSize);
    this->times.insert(this->times.end(),newTimes.begin(),newTimes.end());
} /* RTimeSolver::addTimes */

unsigned int RTimeSolver::getInputNTimeSteps(void) const
{
    return this->inputNTimeSteps;
} /* RTimeSolver::getInputNTimeSteps */

void RTimeSolver::setInputNTimeSteps(unsigned int inputNTimeSteps)
{
    this->inputNTimeSteps = inputNTimeSteps;
} /* RTimeSolver::setInputNTimeSteps */

double RTimeSolver::getInputStartTime(void) const
{
    return this->inputStartTime;
} /* RTimeSolver::getInputStartTime */

void RTimeSolver::setInputStartTime(double inputStartTime)
{
    this->inputStartTime = inputStartTime;
} /* RTimeSolver::setInputStartTime */

double RTimeSolver::getInputTimeStepSize(void) const
{
    return this->inputTimeStepSize;
} /* RTimeSolver::getInputTimeStepSize */

void RTimeSolver::setInputTimeStepSize(double inputTimeStepSize)
{
    this->inputTimeStepSize = inputTimeStepSize;
} /* RTimeSolver::setInputTimeStepSize */

unsigned int RTimeSolver::getCurrentTimeStep(void) const
{
    return this->currentTimeStep;
} /* RTimeSolver::getCurrentTimeStep */

void RTimeSolver::setCurrentTimeStep(unsigned int timeStep)
{
    R_ERROR_ASSERT(timeStep < this->getNTimeSteps());
    this->currentTimeStep = timeStep;
} /* RTimeSolver::setCurrentTimeStep */

unsigned int RTimeSolver::getOutputFrequency(void) const
{
    return this->outputFrequency;
} /* RTimeSolver::getOutputFrequency */

void RTimeSolver::setOutputFrequency(unsigned int outputFrequency)
{
    this->outputFrequency = outputFrequency;
} /* RTimeSolver::setOutputFrequency */

unsigned int RTimeSolver::setNextTimeStep(void)
{
    if (this->currentTimeStep + 1 >= this->getNTimeSteps())
    {
        return RConstants::eod;
    }
    return ++this->currentTimeStep;
} /* RTimeSolver::setNextTimeStep */

double RTimeSolver::getCurrentTimeStepSize(void) const
{
    if (this->enabled)
    {
        return (this->getCurrentTime() - this->getPreviousTime());
    }
    else
    {
        return 0.0;
    }
} /* RTimeSolver::getCurrentTimeStepSize */

double RTimeSolver::getComputedTime(void) const
{
    return this->computedTime;
} /* RTimeSolver::getComputedTime */

void RTimeSolver::setComputedTime(double computedTime)
{
    this->computedTime = computedTime;
} /* RTimeSolver::setComputedTime */

double RTimeSolver::getCurrentTime(void) const
{
    R_ERROR_ASSERT(this->getCurrentTimeStep() < this->getNTimeSteps());
    return this->times[this->getCurrentTimeStep()];
} /* RTimeSolver::getCurrentTime */

double RTimeSolver::getPreviousTime(void) const
{
    R_ERROR_ASSERT(this->getCurrentTimeStep() < this->getNTimeSteps());
    if (this->getCurrentTimeStep() > 0)
    {
        return this->times[this->getCurrentTimeStep() - 1];
    }
    else
    {
        return this->times[this->getCurrentTimeStep()] - this->getInputTimeStepSize();
    }
} /* RTimeSolver::getPreviousTime */

double RTimeSolver::getTime(unsigned int timeStep) const
{
    R_ERROR_ASSERT(timeStep < this->getNTimeSteps());
    return this->times[timeStep];
} /* RTimeSolver::getTime */

double RTimeSolver::getFirstTime(void) const
{
    return this->getTime(0);
} /* RTimeSolver::getFirstTime */

double RTimeSolver::getLastTime(void) const
{
    return this->getTime(this->getNTimeSteps()-1);
} /* RTimeSolver::getLastTime */

unsigned int RTimeSolver::getNTimeSteps(void) const
{
    return (unsigned int)this->times.size();
} /* RTimeSolver::getNTimeSteps */

bool RTimeSolver::hasTime(double timeValue) const
{
    return (timeValue >= this->getFirstTime() && timeValue <= this->getLastTime());
} /* RTimeSolver::hasTime */

void RTimeSolver::harmonizeTimesWithInput(bool restart)
{
    if (this->enabled)
    {
        if (restart)
        {
            if (this->getCurrentTimeStep() >= this->getNTimeSteps())
            {
                this->setCurrentTimeStep(this->getNTimeSteps()-1);
            }
            this->addTimes(this->getCurrentTimeStep()+1,
                           this->getInputNTimeSteps(),
                           this->getInputTimeStepSize());
        }
        else
        {
            this->setCurrentTimeStep(0);
            this->setComputedTime(0.0);
            this->setTimes(RTimeSolver::findTimesVector(this->getInputNTimeSteps(),
                                                        this->getInputStartTime(),
                                                        this->getInputTimeStepSize()));
        }
    }
    else
    {
        this->times.resize(R_TIME_STEP_DEFAULT_NUMBER,R_TIME_STEP_DEFAULT_START + R_TIME_STEP_DEFAULT_SIZE);
    }
} /* RTimeSolver::harmonizeTimesWithInput */

const QString &RTimeSolver::getTimeMarchApproximationName(RTimeMarchApproximation timeApproximation)
{
    R_ERROR_ASSERT(R_TIME_MARCH_APPROXIMATION_TYPE_IS_VALID(timeApproximation));
    return timeApproximationNames[timeApproximation];
} /* RTimeSolver::getTimeMarchApproximationName */

std::vector<double> RTimeSolver::findTimesVector(unsigned int nTimeSteps, double startTime, double timeStepSize)
{
    std::vector<double> timesVector;

    timesVector.resize(nTimeSteps);
    for (uint i=0;i<nTimeSteps;i++)
    {
        timesVector[i] = startTime + (i+1)*timeStepSize;
    }

    return timesVector;
} /* RTimeSolver::findTimesVector */
