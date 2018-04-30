/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_statistics.cpp                                       *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   26-th March 2013                                         *
 *                                                                   *
 *  DESCRIPTION: Statistics class definition                         *
 *********************************************************************/

#include <cmath>
#include <algorithm>

#include <rblib.h>

#include "rbl_statistics.h"
#include "rbl_logger.h"

void RStatistics::_init(const RStatistics *pStatistics)
{
    if (pStatistics)
    {
        this->nValues = pStatistics->nValues;
        this->min = pStatistics->min;
        this->max = pStatistics->max;
        this->avg = pStatistics->avg;
        this->med = pStatistics->med;
        this->p05 = pStatistics->p05;
        this->p95 = pStatistics->p95;
        this->dsv = pStatistics->dsv;
    }
}

RStatistics::RStatistics(const RRVector &values, unsigned int nDistValues, bool sortValues)
    : nValues(0)
    , min(0.0)
    , max(0.0)
    , avg(0.0)
    , med(0.0)
    , p05(0.0)
    , p95(0.0)
{
    this->_init();
    this->calculate(values,nDistValues,sortValues);
}

RStatistics::RStatistics(const RStatistics &statistics)
{
    this->_init(&statistics);
}

RStatistics::~RStatistics()
{
}

RStatistics &RStatistics::operator =(const RStatistics &statistics)
{
    this->_init(&statistics);
    return (*this);
}

void RStatistics::print(void) const
{
    RLogger::info("Minimum:       % 14g\n",this->getMin());
    RLogger::info("Maximum:       % 14g\n",this->getMax());
    RLogger::info("Average:       % 14g\n",this->getAvg());
    RLogger::info("Median:        % 14g\n",this->getMed());
    RLogger::info("Percentile 05: % 14g\n",this->getP05());
    RLogger::info("Percentile 95: % 14g\n",this->getP95());
}

void RStatistics::calculate(const RRVector &values, unsigned int nDistValues, bool sortValues)
{
    this->nValues = (unsigned int)values.size();
    this->min = RStatistics::findMinimumValue(values);
    this->max = RStatistics::findMaximumValue(values);
    this->avg = RStatistics::findAverageValue(values);
    this->dsv = RStatistics::findDistributedValues(values,nDistValues);

    if (sortValues)
    {
        RRVector sortedValues(values);
        std::sort(sortedValues.begin(),sortedValues.end());
        this->med = RStatistics::findMedianValue(sortedValues);
        this->p05 = RStatistics::findPercentileValue(sortedValues,5.0);
        this->p95 = RStatistics::findPercentileValue(sortedValues,95.0);
    }
    else
    {
        this->med = RStatistics::findMedianValue(values);
        this->p05 = RStatistics::findPercentileValue(values,5.0);
        this->p95 = RStatistics::findPercentileValue(values,95.0);
    }
}

double RStatistics::findMinimumValue(const RRVector &values)
{
    double value = 0.0;
    bool firstValue = true;
    for (unsigned int i=0;i<values.size();i++)
    {
        if (firstValue)
        {
            value = values[i];
            firstValue = false;
        }
        else
        {
            value = std::min(value,values[i]);
        }
    }
    return value;
}

double RStatistics::findMaximumValue(const RRVector &values)
{
    double value = 0.0;
    bool firstValue = true;
    for (unsigned int i=0;i<values.size();i++)
    {
        if (firstValue)
        {
            value = values[i];
            firstValue = false;
        }
        else
        {
            value = std::max(value,values[i]);
        }
    }
    return value;
}

double RStatistics::findMinimumAbsoluteValue(const RRVector &values)
{
    double value = 0.0;
    bool firstValue = true;
    for (unsigned int i=0;i<values.size();i++)
    {
        if (firstValue)
        {
            value = std::fabs(values[i]);
            firstValue = false;
        }
        else
        {
            value = std::min(value,std::fabs(values[i]));
        }
    }
    return value;
}

double RStatistics::findMaximumAbsoluteValue(const RRVector &values)
{
    double value = 0.0;
    bool firstValue = true;
    for (unsigned int i=0;i<values.size();i++)
    {
        if (firstValue)
        {
            value = std::fabs(values[i]);
            firstValue = false;
        }
        else
        {
            value = std::max(value,std::fabs(values[i]));
        }
    }
    return value;
}

double RStatistics::findAverageValue(const RRVector &values)
{
    double value = 0.0;
    for (unsigned int i=0;i<values.size();i++)
    {
        value += values[i];
    }
    if (values.size() > 0)
    {
        value /= values.size();
    }
    return value;
}

double RStatistics::findMedianValue(const RRVector &values)
{
    double value = 0.0;
    unsigned int nv = (unsigned int)values.size();
    unsigned int pos = 0;

    if (nv % 2 != 0) {
        pos = nv / 2;
        value = values[pos];
    }
    else {
        value = 0.0;
        pos = (int) floor((double)nv / 2.0);
        value += values[pos];
        pos = (int) ceil((double)nv / 2.0);
        value += values[pos];
        value /= 2.0;
    }

    return value;
}

double RStatistics::findPercentileValue(const RRVector &values, double percentile)
{
    RRVector vs(values);
    double n, p, vp, vd;
    unsigned int k;
    unsigned int N;

    if (percentile < 0.0)
    {
        p = 0.0;
    }
    else if (percentile > 100.0)
    {
        p = 100.0;
    }
    else {
        p = percentile;
    }

    N = (unsigned int)vs.size();

    n = (p/100.0)*(N-1.0)+1.0;

    if (n <= 1.0)
    {
        return vs[0];
    }
    if (n >= N)
    {
        return vs[N-1];
    }

    k = (unsigned int)floor(n);
    vd = vs[k] - vs[k-1];
    vd -= floor(vd);
    vp = vs[k-1] + vd;

    return vp;
}

RRMatrix RStatistics::findDistributedValues(const RRVector &values, unsigned int nDistValues)
{
    RRMatrix dist(2,nDistValues);

    double llim,  ulim;
    double sllim, sulim;
    double step;
    int nvals;

    llim = RStatistics::findMinimumValue(values);
    ulim = RStatistics::findMaximumValue(values);
    step = ulim - llim;
    if (nDistValues > 1)
    {
        step /= (double)(nDistValues-1);
    }

    dist.fill(0.0);

    nvals = (unsigned int)values.size();

    for (unsigned int i=0;i<nDistValues;i++) {
        sllim = step*i + llim;
        sulim = sllim + step;

        dist[0][i] = sllim;

        if (i == nDistValues-1) {
            sulim = ulim + 1.0;
        }
        for (int j=0;j<nvals;j++) {
            if (sllim <= values[j] && values[j] < sulim) {
                dist[1][i]++;
            }
        }
    }

    return dist;
}

double RStatistics::findMinimumMagnitude(const std::vector<RR3Vector> &values)
{
    double minMag = 0.0;
    for (unsigned int i=0;i<values.size();i++)
    {
        if (i==0)
        {
            minMag = RR3Vector::norm(values[i]);
        }
        else
        {
            minMag = std::min(minMag,RR3Vector::norm(values[i]));
        }
    }
    return minMag;
}

double RStatistics::findMaximumMagnitude(const std::vector<RR3Vector> &values)
{
    double maxMag = 0.0;
    for (unsigned int i=0;i<values.size();i++)
    {
        if (i==0)
        {
            maxMag = RR3Vector::norm(values[i]);
        }
        else
        {
            maxMag = std::max(maxMag,RR3Vector::norm(values[i]));
        }
    }
    return maxMag;
}
