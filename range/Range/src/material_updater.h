/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   material_updater.h                                       *
 *  GROUP:  Range                                                    *
 *  TYPE:   header material (*.h)                                    *
 *  DATE:   17-th May 2016                                           *
 *                                                                   *
 *  DESCRIPTION: Material updater class declaration                  *
 *********************************************************************/

#ifndef MATERIAL_UPDATER_H
#define MATERIAL_UPDATER_H

#include <QString>
#include <QList>
#include <QPair>

#include "job.h"

class MaterialUpdater : public Job
{

    Q_OBJECT

    protected:

        //! List of source/destination pairs.
        QList<QString> materials;

    public:

        //! Constructor.
        explicit MaterialUpdater(QObject *parent = nullptr);

        //! Add material to be updated.
        void addMaterial(const QString &source);

    protected:

        //! Function performing actual job.
        void run(void);

};

#endif // MATERIAL_UPDATER_H
