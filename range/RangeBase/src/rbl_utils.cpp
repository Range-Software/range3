/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_utils.cpp                                            *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   13-th September 2012                                     *
 *                                                                   *
 *  DESCRIPTION: Utility functions                                   *
 *********************************************************************/

#include "rbl_utils.h"

static float storeFloat(float value)
{
    return value;
}

static double storeDouble(double value)
{
    return value;
}


float RConstants::findMachineFloatEpsilon(void)
{
    float x = 1.0f;
    while (storeFloat(1.0f + x/2.0f) != 1.0f)
    {
        x /= 2.0f;
    }
    return x;
}

double RConstants::findMachineDoubleEpsilon(void)
{
    double x = 1.0;
    while (storeDouble(1.0 + x/2.0) != 1.0)
    {
        x /= 2.0;
    }
    return x;
}
