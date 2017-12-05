/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rra_request_input.h                                      *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   14-th November 2016                                      *
 *                                                                   *
 *  DESCRIPTION: Remote Range API request input class declaration    *
 *********************************************************************/

#ifndef __RRA_REQUEST_INPUT_H__
#define __RRA_REQUEST_INPUT_H__

#include "http_request_input.h"

class RRARequestInput : public HttpRequestInput
{

    public:

        enum Type
        {
            AVAILABLE_SOFTWARE = 0,
            LOGIN,
            LOGOUT,
            CLIENT_LICENSE,
            SEND_USAGE_INFO
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

        //! Request latest software version.
        static RRARequestInput *requestClientLicense(void);

        //! Send usage info.
        static RRARequestInput *sendUsageInfo(const QString &usageInfo);

        //! Return request type.
        Type getType(void) const;

};

#endif // __RRA_REQUEST_INPUT_H__
