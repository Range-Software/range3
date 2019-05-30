/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   usage_info.h                                             *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   5-th December 2017                                       *
 *                                                                   *
 *  DESCRIPTION: Usage info class declaration                        *
 *********************************************************************/

#ifndef USAGE_INFO_H
#define USAGE_INFO_H

#include <QString>

class UsageInfo
{

    private:

        //! Constructor.
        UsageInfo() = default;

    public:

        //! Return reference to static instance.
        static UsageInfo &getInstance(void);

        //! Get usage report.
        QString getReport(void) const;
};

#endif // USAGE_INFO_H
