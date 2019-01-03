/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   file_updater.h                                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   17-th May 2016                                           *
 *                                                                   *
 *  DESCRIPTION: File updater class declaration                      *
 *********************************************************************/

#ifndef FILE_UPDATER_H
#define FILE_UPDATER_H

#include <QString>
#include <QList>
#include <QPair>

#include "job.h"

class FileUpdater : public Job
{

    Q_OBJECT

    protected:

        //! List of source/destination pairs.
        QList< QPair<QString,QString> > files;

    public:

        //! Constructor.
        explicit FileUpdater(QObject *parent = nullptr);

        //! Add file to be updated.
        void addFile(const QString &source, const QString &destination);

    protected:

        //! Function performing actual job.
        void run(void);

};

#endif // FILE_UPDATER_H
