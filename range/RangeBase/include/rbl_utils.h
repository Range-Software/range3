/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_utils.h                                              *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   13-th September 2012                                     *
 *                                                                   *
 *  DESCRIPTION: Utility functions                                   *
 *********************************************************************/

#ifndef RBL_MACROS_H
#define RBL_MACROS_H

#include <QString>

#include <limits.h>
#include <vector>
#include <cmath>

#include "rbl_version.h"

namespace RVendor
{
    static const QString name("Range Software");
    static const QString shortName("Range");
    static const QString title("Finite Element Analysis");
    static const QString description("Engineering simulation software");
    static const QString author("Tomáš Šoltys");
    static const QString email("tomas.soltys@range-software.com");
    static const QString www("https://www.range-software.com");
    static const RVersion version(3,2,4,RVersion::Release);
    static const short year = 2020;
}

namespace RConstants
{
    //! Reurn machine float precision.
    float findMachineFloatEpsilon(void);
    //! Reurn machine double precision.
    double findMachineDoubleEpsilon(void);

    static const double pi = 3.141592653589793238462;
    static const float  pi_f = 3.14159265358979f;
    static const unsigned int eod = UINT_MAX;
    static const double eps = 1.0e-10;
    static const char endl = '\n';
}

#define R_RAD_TO_DEG(_radVal) (_radVal*180.0/RConstants::pi)
#define R_DEG_TO_RAD(_degVal) (_degVal*RConstants::pi/180.0)
#define R_SIGNS_EQUAL(_v1,_v2) ((_v1 < 0.0 && _v2 < 0.0) || (_v1 > 0.0 && _v2 > 0.0))
#define R_SAME_SIGN(_v1,_v2) (_v2 > 0.0 ? std::fabs(_v1) : -std::fabs(_v1))
#define R_IS_IN_CLOSED_INTERVAL(_l1,_l2,_v) (_l1 <= _v && _l2 >= _v)
#define R_IS_IN_OPEN_INTERVAL(_l1,_l2,_v) (_l1 < _v && _l2 > _v)
#define R_D_ARE_SAME(_v1,_v2) (std::fabs((_v1)-(_v2)) < RConstants::eps)

namespace RUtil
{
    //! Sort array and fill indexes array with original positions.
    template <typename T>
    struct SortRentry
    {
        T value;
        uint position;
        bool operator<(const SortRentry<T> &v) const
        {
            return (this->value < v.value);
        }
    };

    template <typename T>
    void qSort(std::vector<T> &a, std::vector<uint> &indexes)
    {
        std::vector<SortRentry<T> > arr;

        arr.resize(a.size());
        for (uint i=0;i<a.size();i++)
        {
            arr[i].value = a[i];
            arr[i].position = i;
        }

        std::sort(arr.begin(),arr.end());

        indexes.resize(arr.size());
        for (uint i=0;i<arr.size();i++)
        {
            a[i] = arr[i].value;
            indexes[i] = arr[i].position;
        }
    }

    inline double frexp10(double arg, int *exp)
    {
        *exp = (arg == 0.0) ? 0 : static_cast<int>(1 + std::log10(std::fabs(arg)));
        return arg * pow(10 , -(*exp));
    }

} // RUtil

#endif /* RBL_MACROS_H */
