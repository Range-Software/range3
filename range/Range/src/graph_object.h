/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   graph_object.h                                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th March 2014                                          *
 *                                                                   *
 *  DESCRIPTION: Graph object class declaration                      *
 *********************************************************************/

#ifndef GRAPH_OBJECT_H
#define GRAPH_OBJECT_H

#include <QObject>
#include <QTimer>
#include <QString>

#include "graph_data.h"

class GraphObject : public QObject
{

    Q_OBJECT

    protected:

        //! Graph data.
        GraphData graphData;
        //! Source file read interval in miliseconds.
        //! (0 = off).
        uint sourceReadInterval;
        //! Timer.
        QTimer *sourceReadTimer;

    public:

        //! Constructor.
        explicit GraphObject(QObject *parent = nullptr);

        //! Return const reference to graph data object.
        const GraphData & getData(void) const;

        //! Return reference to graph data object.
        GraphData & getData(void);

        //! Set new graph data.
        void setData(const GraphData &graphData);

        //! Emit dataChanged signal.
        void setDataChanged(void);

        //! Return source read interval.
        uint getSourceReadInterval(void) const;

        //! Set source read interval.
        void setSourceReadInterval(uint sourceReadInterval);

    protected:

        //! Read data from source.
        virtual void readSource(void);

    signals:

        //! Graph data has changed.
        void dataChanged(void);

    public slots:

        //! Read data from source.
        void loadData(void);

};

#endif // GRAPH_OBJECT_H
