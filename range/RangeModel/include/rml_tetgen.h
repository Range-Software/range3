/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_tetgen.h                                             *
 *  GROUP:  RModel                                                   *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   4-th August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: RTetGen class declaration                           *
 *********************************************************************/

#ifndef RTETGEN_H
#define RTETGEN_H

#include <tetgen.h>

#include "rml_model.h"

class RTetGen : public tetgenio
{

    public:

        //! Constructor.
        explicit RTetGen(const RModel &model = RModel());

        //! Import RModel object.
        void importModel(const RModel &model, bool reconstruct, const RRVector &nodeMeshSizeValues = RRVector());

        //! Export mesh to RModel.
        void exportMesh(RModel &model, bool keepResults = true) const;

};

#endif /* RTETGEN_H */
