/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   mesh_generator.h                                         *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   7-th September 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Mesh generator class declaration                    *
 *********************************************************************/

#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H


#include <QObject>

#include "job.h"

class MeshGenerator : public Job
{
    Q_OBJECT

    protected:

        //! Model ID.
        uint modelID;

    public:

        //! Constructor.
        explicit MeshGenerator(uint modelID, QObject *parent = nullptr);

    protected:

        //! Overloaded run method.
        void run(void);
        
};

#endif // MESH_GENERATOR_H
