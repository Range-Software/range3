/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_statistics.h                                         *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th March 2013                                         *
 *                                                                   *
 *  DESCRIPTION: Statistics class declaration                        *
 *********************************************************************/

#ifndef RBL_STATISTICS_H
#define RBL_STATISTICS_H

#include "rbl_r3vector.h"
#include "rbl_rmatrix.h"

//! Statistics class.
class RStatistics
{

    protected:

        //! Number of values.
        unsigned int nValues;
        //! Minimum value.
        double min;
        //! Maximum value.
        double max;
        //! Average value.
        double avg;
        //! Median value.
        double med;
        //! 5 percentile value.
        double p05;
        //! 95 percentile value.
        double p95;
        //! Distribution statistics values.
        RRMatrix dsv;

    private:

        //! Internal initialization function.
        void _init(const RStatistics *pStatistics = nullptr);

    public:

        //! Constructor.
        RStatistics(const RRVector &values, unsigned int nDistValues = 100, bool sortValues = false);

        //! Copy constructor.
        RStatistics(const RStatistics &statistics);

        //! Destructor.
        ~RStatistics();

        //! Assignment operator.
        RStatistics & operator =(const RStatistics &statistics);

        //! Get number of values.
        inline unsigned int getNValues(void) const
        {
            return this->nValues;
        }

        //! Get minimum value.
        inline double getMin(void) const
        {
            return this->min;
        }

        //! Get maximum value.
        inline double getMax(void) const
        {
            return this->max;
        }

        //! Get average value.
        inline double getAvg(void) const
        {
            return this->avg;
        }

        //! Get median value.
        inline double getMed(void) const
        {
            return this->med;
        }

        //! Get 5 percentile value.
        inline double getP05(void) const
        {
            return this->p05;
        }

        //! Get 95 percentile value.
        inline double getP95(void) const
        {
            return this->p95;
        }

        //! Get distributed statistical values.
        inline const RRMatrix & getDsv(void) const
        {
            return this->dsv;
        }

        //! Print statistics.
        void print(void) const;

    protected:

        //! Calculate statistics.
        void calculate(const RRVector &values, unsigned int nDistValues = 100, bool sortValues = false);

    public:

        //! Find minimum value.
        static double findMinimumValue(const RRVector &values);

        //! Find maximum value.
        static double findMaximumValue(const RRVector &values);

        //! Find minimum absolute value.
        static double findMinimumAbsoluteValue(const RRVector &values);

        //! Find maximum absolute value.
        static double findMaximumAbsoluteValue(const RRVector &values);

        //! Find average value.
        static double findAverageValue(const RRVector &values);

        //! Find median value.
        static double findMedianValue(const RRVector &values);

        //! Find percentile value.
        static double findPercentileValue(const RRVector &values, double percentile);

        //! Find distributed statistical values.
        static RRMatrix findDistributedValues(const RRVector &values, unsigned int nDistValues = 100);

        //! Find minimum magnitude.
        static double findMinimumMagnitude(const std::vector<RR3Vector> &values);

        //! Find maximum magnitude.
        static double findMaximumMagnitude(const std::vector<RR3Vector> &values);

};

#endif // RBL_STATISTICS_H
