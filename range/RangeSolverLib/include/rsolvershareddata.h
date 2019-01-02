/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolvershareddata.h                                      *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   7-th November 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Range solver shared data class declaration          *
 *********************************************************************/

#ifndef RSOLVERSHAREDDATA_H
#define RSOLVERSHAREDDATA_H

#include <QMap>

#include <rmlib.h>

class RSolverSharedData
{

    protected:

        //! Variables.
        QMap<QString,RRVector> data;

    private:

        //! Internal initialization function.
        void _init(const RSolverSharedData *pSolverSharedData = nullptr);

    public:

        //! Constructor.
        RSolverSharedData();

        //! Copy constructor.
        RSolverSharedData(const RSolverSharedData &solverSharedData);

        //! Destructor.
        ~RSolverSharedData();

        //! Assignment operator.
        RSolverSharedData & operator =(const RSolverSharedData &solverSharedData);

        //! Return true if given data is present in data container and has correct size.
        bool hasData(const QString &name, uint dataSize) const;

        //! Add vector into data container.
        //! If given vector already exist it will be overwritten.
        void addData(const QString &name, const RRVector &data);

        //! Return reference to vector.
        //! If given vector does not exist it will be created.
        RRVector &findData(const QString &name);

        //! Clear shared data.
        void clearData(void);

};

#endif // RSOLVERSHAREDDATA_H
