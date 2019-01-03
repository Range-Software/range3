/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_element_shape_derivation.h                           *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   16-th December 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Element shape derivation class declaration          *
 *********************************************************************/

#ifndef RML_ELEMENT_SHAPE_DERIVATION_H
#define RML_ELEMENT_SHAPE_DERIVATION_H

#include <vector>

#include "rml_element.h"
#include "rml_problem_type.h"

class RElementShapeDerivation
{

    protected:

        //! Derivations
        std::vector<RRMatrix> B;
        //! Jacobian determinant
        std::vector<double> detJ;

    private:

        //! Internal initialization function.
        void _init(const RElementShapeDerivation *pElementShapeDerivation = nullptr);

    public:

        //! Constructor.
        RElementShapeDerivation(const RElement &rElement, const std::vector<RNode> &nodes, RProblemType problemType);

        //! Copy constructor.
        RElementShapeDerivation(const RElementShapeDerivation &elementShapeDerivation);

        //! Destructor.
        ~RElementShapeDerivation();

        //! Assignment operator.
        RElementShapeDerivation & operator =(const RElementShapeDerivation &elementShapeDerivation);

        //! Return derivative.
        const RRMatrix &getDerivative(uint integrationPoint) const;

        //! Return jacobian determinant.
        double getJacobian(uint integrationPoint) const;

    private:

        //! Generate fluid derivations.
        void generateFluid(const RElement &rElement, const std::vector<RNode> &nodes);

};

#endif // RML_ELEMENT_SHAPE_DERIVATION_H
