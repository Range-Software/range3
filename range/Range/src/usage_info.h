/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   usage_info.h                                             *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   5-th December 2017                                       *
 *                                                                   *
 *  DESCRIPTION: Usage info class declaration                        *
 *********************************************************************/

#ifndef __USAGE_INFO_H__
#define __USAGE_INFO_H__

#include <QString>

class UsageInfo
{

    private:

        //! Constructor.
        UsageInfo();

    public:

        //! Return reference to static instance.
        static UsageInfo &getInstance(void);

        //! Get usage report.
        QString getReport(void) const;
};

#endif // __USAGE_INFO_H__
