/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_error.h                                              *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   11-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Error class declaration                             *
 *********************************************************************/

#ifndef RBL_ERROR_H
#define RBL_ERROR_H

#include <QString>
#include <cstdarg>

#define R_ERROR_IS_VALID(_errorType) \
    (_errorType >= R_ERROR_NONE && _errorType < R_ERROR_N_TYPES)

#define R_ERROR_REF __FILE__,__LINE__

#ifdef DEBUG
#include <assert.h>
#define R_ERROR_ASSERT(_cond) assert(_cond)
#else
#define R_ERROR_ASSERT(_cond) {if (!(_cond)) throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Condition failed \'%s\'.",#_cond);}
#endif

//! Error types.
typedef enum _RErrorType
{
    R_ERROR_NONE = 0,
    R_ERROR_OPEN_FILE,
    R_ERROR_READ_FILE,
    R_ERROR_WRITE_FILE,
    R_ERROR_REMOVE_FILE,
    R_ERROR_RENAME_FILE,
    R_ERROR_INVALID_FILE_NAME,
    R_ERROR_INVALID_FILE_FORMAT,
    R_ERROR_OPEN_DIR,
    R_ERROR_READ_DIR,
    R_ERROR_APPLICATION,
    R_ERROR_INVALID_INPUT,
    R_ERROR_UNKNOWN,
    R_ERROR_N_TYPES
} RErrorType;

//! Error class.
class RError
{

    private:

        //! Internal initialization function.
        void _init ( const RError *pError = nullptr );

    protected:

        //! Error type.
        RErrorType type;

        //! Error message.
        QString message;

    public:

        //! Constructor.
        RError ( RErrorType     type,
                 const char    *file,
                 unsigned int   line,
                 const QString &message );

        //! Constructor.
        RError ( RErrorType    type,
                 const char   *file,
                 unsigned int  line,
                 const char   *format, ... );

        //! Copy constructor.
        RError ( const RError &error );

        //! Destructor.
        ~RError ();

        //! Assignment operator.
        RError & operator = ( const RError &error );

        //! Return error type.
        RErrorType getType ( void ) const;

        //! Set error type.
        //! By setting error type an appropriate message will be
        //! set as well.
        void setType ( RErrorType type );

        //! Return error message.
        QString getMessage ( void ) const;

        //! Set error message.
        void setMessage ( QString message );

};

const QString & RErrorTypeGetMessage ( RErrorType type );

#endif /* RBL_ERROR_H */
