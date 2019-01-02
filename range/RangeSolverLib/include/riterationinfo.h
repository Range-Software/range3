/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   riterationinfo.h                                         *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   23-rd January 2013                                       *
 *                                                                   *
 *  DESCRIPTION: Iteration info class declaration                    *
 *********************************************************************/

#ifndef RITERATIONINFO_H
#define RITERATIONINFO_H

#include <vector>
#include <QString>

#include <rblib.h>

#include "riterationinfovalue.h"

class RIterationInfo
{

    protected:

        //! Number of iterations.
        unsigned int nIterations;
        //! Convergence value.
        double convergenceValue;
        //! Iteration print frequency.
        unsigned int outputFrequency;
        //! Current iteration number.
        unsigned int iteration;
        //! Current error.
        double error;
        //! Error trend (increase/decrease in percentage).
        double trend;
        //! Equation scale.
        double equationScale;
        //! Output file name.
        QString outputFileName;

    private:

        //! Internal initialization function.
        void _init(const RIterationInfo *pIterationInfo = nullptr);

    public:

        //! Constructor.
        RIterationInfo();

        //! Copy constructor.
        RIterationInfo(const RIterationInfo &iterationInfo);

        //! Destructor.
        ~RIterationInfo();

        //! Assignment operator.
        RIterationInfo & operator =(const RIterationInfo &iterationInfo);

        //! Set number of iterations.
        void setNIterations(unsigned int nIterations);

        //! Set iteration print frequency.
        void setOutputFrequency(unsigned int outputFrequency);

        //! Set convergence value.
        void setConvergenceValue(double convergenceValue);

        //! Set iteration.
        void setIteration(unsigned int iteration);

        //! Set error.
        void setError(double error);

        //! Set equation scale.
        void setEquationScale(double equationScale);

        //! Set output file name.
        void setOutputFileName(const QString &outputFileName);

        //! Return true if iterations have converged.
        bool hasConverged(void) const;

        //! Print header.
        void printHeader(const QString &title) const;

        //! Print footer.
        void printFooter(void) const;

        //! Print iteration information.
        void printIteration(void) const;

        //! Static function to write convergence to file.
        static void writeToFile(const QString &fileName, uint iteration, const std::vector<RIterationInfoValue> &iterationValues);

    protected:

        //! Write iteration information to file.
        void writeToFile(void) const;

};

#endif // RITERATIONINFO_H
