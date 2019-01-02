/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   progress.h                                               *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   13-1h January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Progress functions                                  *
 *********************************************************************/

#ifndef PROGRESS_H
#define PROGRESS_H

#include <QObject>

#include "main_window.h"

class Progress : public QObject
{

    Q_OBJECT

    protected:

        //! Constructor.
        inline Progress() { }

    public:

        //! Initialize RProgress static instance.
        static void initialize(void);

        //! Return reference to static object.
        static Progress & getInstance(void);

        //! Emit progress signal.
        void print(double fraction);

        //! Emit initialize signal.
        void emitProgressInitialize(const QString &message,
                                    bool           pulseType);

        //! Emit finalize signal.
        void emitProgressFinalize(const QString &message);

    signals:

        //! Signal containing progress fraction.
        void progress(double fraction);

        //! Initialize signal.
        void progressInitialize(const QString &message,
                                bool           pulseType);

        //! Finalize signal.
        void progressFinalize(const QString &message);
};

#endif /* PROGRESS_H */
