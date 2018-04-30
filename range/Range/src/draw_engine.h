/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine.h                                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th September 2014                                     *
 *                                                                   *
 *  DESCRIPTION: Draw engine class declaration                       *
 *********************************************************************/

#ifndef __DRAW_ENGINE_H__
#define __DRAW_ENGINE_H__

#include <QObject>
#include <QList>

#include "draw_engine_object.h"
#include "draw_engine_point.h"
#include "draw_engine_line.h"
#include "draw_engine_triangle.h"
#include "draw_engine_quadrilateral.h"
#include "draw_engine_circle.h"
#include "draw_engine_ellipse.h"
#include "draw_engine_cylinder.h"
#include "draw_engine_sphere.h"
#include "draw_engine_tetrahedron.h"
#include "draw_engine_hexahedron.h"

class DrawEngine : public QObject
{

    Q_OBJECT

    protected:

        //! Engine is running.
        bool isRunning;
        //! Stack of drawing options.
        QList<DrawEngineObject*> objects;

    public:

        //! Constructor.
        explicit DrawEngine(QObject *parent = 0);

        //! Start engine.
        void start(void);

        //! Stop engine.
        void stop(void);

        //! Process all objects.
        void processObjects(const QList<uint> &modelIDs, bool mergeNearNodes, double tolerance = 0.0, bool findNearest = false);

        //! Add object.
        void addObject(DrawEngineObject *object);

        //! Remove object with given position.
        void removeObject(uint position);

        //! Return number of objects.
        uint getNObjects(void) const;

        //! Return const pointer to object at given position.
        const DrawEngineObject *getObject(uint position) const;

        //! Return pointer to object at given position.
        DrawEngineObject *getObject(uint position);

        //! Force to emit signal that object has changed.
        void setObjectChanged(uint position);

    signals:

        //! Object was added signal.
        void objectAdded(void);

        //! Object at given position was removed signal.
        void objectRemoved(uint position);

        //! All object were removed signal.
        void objectsRemoved(void);

        //! Object at given position has changed.
        void objectChanged(uint position);

};

#endif // __DRAW_ENGINE_H__
