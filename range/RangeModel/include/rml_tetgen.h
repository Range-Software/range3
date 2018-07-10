/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_tetgen.h                                             *
 *  GROUP:  RModel                                                   *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   4-th August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: RTetGen class declaration                           *
 *********************************************************************/

#ifndef __RTETGEN_H__
#define __RTETGEN_H__

#include <tetgen.h>

#include "rml_model.h"

class RTetGen : public tetgenio
{

    private:

        //! Point marker offset.
        static const int pointMarkerOffset;

    public:

        //! Constructor.
        explicit RTetGen(const RModel &model = RModel());

        //! Import RModel object.
        void importModel(const RModel &model, bool reconstruct = false);

        //! Export mesh to RModel.
        void exportMesh(RModel &model, bool keepResults = true) const;

};

#endif /* __RTETGEN_H__ */
