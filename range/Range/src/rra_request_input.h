/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rra_request_input.h                                      *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   14-th November 2016                                      *
 *                                                                   *
 *  DESCRIPTION: Remote Range API request input class declaration    *
 *********************************************************************/

#ifndef RRA_REQUEST_INPUT_H
#define RRA_REQUEST_INPUT_H

#include "http_request_input.h"

class RRARequestInput : public HttpRequestInput
{

    public:

        enum Type
        {
            AVAILABLE_SOFTWARE = 0,
            LOGIN,
            LOGOUT,
            SEND_USAGE_INFO,
            SEND_CRASH_REPORT
        };

    protected:

        //! RRA type.
        Type type;

    private:

        //! Constructor.
        explicit RRARequestInput();

        //! Return RRA url.
        static QString findUrl(void);

    public:

        //! Request latest software version.
        static RRARequestInput *requestLatestSoftwareVersion(void);

        //! Log in.
        static RRARequestInput *logIn(const QString &account, const QString &password);

        //! Log out.
        static RRARequestInput *logOut(const QString &account);

        //! Send usage info.
        static RRARequestInput *sendUsageInfo(const QString &usageInfo);

        //! Send crash report.
        static RRARequestInput *sendCrashReport(const QString &crashReport);

        //! Return request type.
        Type getType(void) const;

};

#endif // RRA_REQUEST_INPUT_H
