/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_scalar_field.h                                       *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   13-th February 2014                                      *
 *                                                                   *
 *  DESCRIPTION: Scalar field class declaration                      *
 *********************************************************************/

#ifndef RML_SCALAR_FIELD_H
#define RML_SCALAR_FIELD_H

#include <vector>

#include "rml_variable.h"
#include "rml_vector_field.h"

class RScalarField : public RVectorField
{

    public:

        //! Default entity name.
        const static QString defaultName;

    protected:

        //! Minimum point size.
        double minPointSize;
        //! Maximum point size.
        double maxPointSize;

    private:

        //! Internal initialization function.
        void _init ( const RScalarField *pScalarField = nullptr );

    public:

        //! Constructor.
        RScalarField();

        //! Copy constructor.
        RScalarField (const RScalarField &scalarField);

        //! Destructor.
        ~RScalarField ();

        //! Assignment operator.
        RScalarField & operator = ( const RScalarField &scalarField );

        //! Allow RFileIO to access private members.
        friend class RFileIO;

        //! Return maximum point size;
        double getMaxPointSize(void) const;

        //! Set maximum point size;
        void setMaxPointSize(double maxPointSize);

        //! Return default maximum point size.
        static double getDefaultMaxPointSize(void);

};

#endif // RML_SCALAR_FIELD_H
