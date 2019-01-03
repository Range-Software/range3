/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_message.h                                            *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   21-st December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Message class declaration                           *
 *********************************************************************/

#ifndef RBL_MESSAGE_H
#define RBL_MESSAGE_H

#include <QString>

#include <vector>
#include <time.h>

#define R_MESSAGE_TYPE_IS_VALID(_type) \
( \
  _type == R_MESSAGE_INFO    || \
  _type == R_MESSAGE_NOTICE  || \
  _type == R_MESSAGE_ERROR   || \
  _type == R_MESSAGE_WARNING || \
  _type == R_MESSAGE_DEBUG   || \
  _type == R_MESSAGE_TRACE      \
)

//! Message types.
typedef enum _RMessageType
{
    R_MESSAGE_NONE    = 1 << 0,
    R_MESSAGE_INFO    = 1 << 1,
    R_MESSAGE_NOTICE  = 1 << 2,
    R_MESSAGE_ERROR   = 1 << 3,
    R_MESSAGE_WARNING = 1 << 4,
    R_MESSAGE_DEBUG   = 1 << 5,
    R_MESSAGE_TRACE   = 1 << 6
} RMessageType;

//! Message class.
class RMessage : public QString
{

    private:

        //! Internal initialization function.
        void _init ( const RMessage *pMessage = nullptr );

    protected:

        //! Message type.
        RMessageType type;
        //! Message assignment time.
        //! Value of this time variable is iset by constructors and
        //! changed whenever new value is set using assignment
        //! operator.
        time_t aTime;
        //! Indicates whether the assignment time is frozen or not.
        bool aTimeFrozen;

    public:

        //! Constructor.
        RMessage ( RMessageType type = R_MESSAGE_INFO );

        //! Copy constructor (message).
        RMessage ( const RMessage &message );

        //! Copy constructor (c++ string).
        RMessage ( const QString &cppString, RMessageType type = R_MESSAGE_INFO );

        //! Copy constructor (c string).
        RMessage ( const char *cString, RMessageType type = R_MESSAGE_INFO );

        //! Copy constructor (c char).
        RMessage ( size_t n, char cChar, RMessageType type = R_MESSAGE_INFO );

        //! Destructor.
        ~RMessage ();

        //! Get message type.
        RMessageType getType ( void ) const;

        //! Set message type
        void setType ( RMessageType type );

        //! Return message assignment time.
        time_t getAtime ( void ) const;

        //! Set message assignment time to current time.
        void setAtimeToNow ( void );

        //! Return whether the modification of assignment time is enabled.
        inline bool getAtimeFrozen ( void ) const
        {
            return this->aTimeFrozen;
        }

        //! Disable modification of assignment time.
        inline void setAtimeFrozen ( bool frozen )
        {
            this->aTimeFrozen = frozen;
        }

        //! Assignment operator (message).
        RMessage & operator = ( const RMessage &message );

        //! Assignment operator (c++ string).
        RMessage & operator = ( const QString &cppString );

        //! Assignment operator (c string).
        RMessage & operator = ( const char *cString );

        //! Assignment operator (c char).
        RMessage & operator = ( char cChar );

        //! Separate string into vector of strings.
        static std::vector<QString> explode( const QString &str,
                                             char           ch,
                                             bool           keepCh = false);

};

#endif /* RBL_MESSAGE_H */
