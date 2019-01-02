/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_progress.h                                           *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   13-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Progress class declaration                          *
 *********************************************************************/

#ifndef RBL_PROGRESS_H
#define RBL_PROGRESS_H

#include <QString>

//! Progress handler function prototype.
typedef void (*RProgressPrintHandler) ( double fraction );

//! Progress initialize handler function prototype.
typedef void (*RProgressInitializeHandler) ( const QString &message,
                                             bool           pulseType );

//! Progress finalize handler function prototype.
typedef void (*RProgressFinalizeHandler) ( const QString &message );

//! Progress class.
class RProgress
{

    protected:

        //! Whether the progress was initialize as a pulse type.
        bool pulseType;

        //! Last printed fraction.
        double lastFraction;

        //! Print to log file.
        bool printToLog;

        //! Custom progress handler.
        RProgressPrintHandler progressHandler;

        //! Custom progress initialize handler.
        RProgressInitializeHandler progressInitializeHandler;

        //! Custom progress finalize handler.
        RProgressFinalizeHandler progressFinalizeHandler;

    private:

        //! Internal initialization function.
        void _init ( const RProgress *pProgress = nullptr );

    public:

        //! Constructor.
        RProgress ();

        //! Copy constructor.
        RProgress ( const RProgress &progress );

        //! Destructor.
        ~RProgress ();

        //! Assignment operator.
        RProgress & operator = ( const RProgress &progress );

        //! Return static instance.
        static RProgress & getInstance ( void );

        //! Return whether the progress should be printed to a log.
        bool getPrintToLog ( void ) const;

        //! Set whether the progress should be printed to a log.
        void setPrintToLog ( bool printToLog );

        //! Return custom progress print handler.
        RProgressPrintHandler getProgressPrintHandler ( void ) const;

        //! Set custom progress print handler.
        void setProgressPrintHandler ( RProgressPrintHandler progressHandler );

        //! Return custom progress initialize handler.
        RProgressInitializeHandler getProgressInitializeHandler ( void ) const;

        //! Set custom progress initialize handler.
        void setProgressInitializeHandler ( RProgressInitializeHandler progressInitializeHandler );

        //! Return custom progress finalize handler.
        RProgressFinalizeHandler getProgressFinalizeHandler ( void ) const;

        //! Set custom progress finalize handler.
        void setProgressFinalizeHandler ( RProgressFinalizeHandler progressFinalizeHandler );

        //! Initialize progress printer.
        void initialize ( const QString &message,
                          bool               pulseType = false );

        //! Finalize progress printer.
        void finalize ( const QString &message = QString() );

        //! Print progress.
        void print ( double fraction );
};

//! Convenience functions.

void RProgressInitialize ( const QString &message,
                           bool           pulseType = false );

void RProgressFinalize ( const QString &message = QString() );

void RProgressPrint ( double fraction );

void RProgressPrint ( unsigned int step,
                      unsigned int nSteps );

void RProgressPrintToLog ( bool printToLog );

#endif /* RBL_PROGRESS_H */
