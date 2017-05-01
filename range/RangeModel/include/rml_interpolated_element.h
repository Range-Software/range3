/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_interpolated_element.h                               *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   31-st May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Interpolated element class declaration              *
 *********************************************************************/

#ifndef __RML_INTERPOLATED_ELEMENT_H__
#define __RML_INTERPOLATED_ELEMENT_H__

#include <vector>

#include "rml_interpolated_node.h"
#include "rml_node.h"
#include "rml_element.h"
#include "rml_variable.h"

class RInterpolatedElement : public std::vector<RInterpolatedNode>
{

    private:

        //! Internal initialization function.
        void _init ( const RInterpolatedElement *pInterpolatedElement = 0 );

    public:

        //! Constructor.
        RInterpolatedElement();

        //! Copy constructor.
        RInterpolatedElement ( const RInterpolatedElement &interpolatedElement );

        //! Destructor.
        ~RInterpolatedElement ();

        //! Assignment operator.
        RInterpolatedElement & operator = ( const RInterpolatedElement &interpolatedElement );

        //! Sort element nodes.
        void sortNodes ( void );

        //! Remove duplicate element nodes.
        void removeDuplicateNodes ( void );

        //! Compute and return element's center.
        void findCenter ( double &cx,
                          double &cy,
                          double &cz ) const;

        //! Check if picking ray is intersecting given element and return intersection distance from the position.
        bool findPickDistance(const RR3Vector &position,
                              const RR3Vector &direction,
                              double tolerance,
                              double &distance) const;

        //! Get scalar vector.
        void findScalarNodeValues(const std::vector<RNode> &rNodes,
                                  const std::vector<RElement> &rElements,
                                  const RVariable &rVariable,
                                  std::vector<double> &values) const;

        //! Get displacement vector.
        void findDisplacementNodeValues(const std::vector<RNode> &rNodes,
                                        const std::vector<RElement> &rElements,
                                        const RVariable &rVariable,
                                        std::vector<RR3Vector> &values) const;

};

#endif /* __RML_INTERPOLATED_ELEMENT_H__ */
