/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_interpolated_entity.h                                *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   31-st May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Interpolated entity class declaration               *
 *********************************************************************/

#ifndef __RML_INTERPOLATED_ENTITY_H__
#define __RML_INTERPOLATED_ENTITY_H__

#include <vector>

#include "rml_interpolated_element.h"
#include "rml_entity_group.h"

class RInterpolatedEntity : public REntityGroup, public std::vector<RInterpolatedElement>
{

    private:

        //! Internal initialization function.
        void _init ( const RInterpolatedEntity *pInterpolatedEntity = 0 );

    public:

        //! Constructor.
        RInterpolatedEntity();

        //! Copy constructor.
        RInterpolatedEntity ( const RInterpolatedEntity &interpolatedEntity );

        //! Destructor.
        ~RInterpolatedEntity ();

        //! Assignment operator.
        RInterpolatedEntity & operator = ( const RInterpolatedEntity &interpolatedEntity );

};

#endif /* __RML_INTERPOLATED_ENTITY_H__ */
