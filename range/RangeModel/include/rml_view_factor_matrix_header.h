/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_view_factor_matrix_header.h                          *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   5-th August 2014                                         *
 *                                                                   *
 *  DESCRIPTION: View-Factor matrix header class declaration         *
 *********************************************************************/

#ifndef RML_VIEW_FACTOR_MATRIX_HEADER_H
#define RML_VIEW_FACTOR_MATRIX_HEADER_H

#include <vector>

#include "rml_patch_input.h"

class RViewFactorMatrixHeader
{

    protected:

        //! Patch input.
        std::vector<RPatchInput> patchInput;
        //! Hemicube resolution.
        unsigned int hemicubeResolution;
        //! Number of elements.
        unsigned int nElements;

    private:

        //! Internal initialization function.
        void _init(const RViewFactorMatrixHeader *pViewFactorMatrixHeader = nullptr);

    public:

        //! Constructor.
        RViewFactorMatrixHeader();

        //! Copy constructor.
        RViewFactorMatrixHeader(const RViewFactorMatrixHeader &viewFactorMatrixHeader);

        //! Destructor.
        ~RViewFactorMatrixHeader();

        //! Assignment operator.
        RViewFactorMatrixHeader &operator =(const RViewFactorMatrixHeader &viewFactorMatrixHeader);

        //! Equal operator.
        bool operator ==(const RViewFactorMatrixHeader &viewFactorMatrixHeader);

        //! Not equal operator.
        bool operator !=(const RViewFactorMatrixHeader &viewFactorMatrixHeader);

        //! Allow RFileIO to access private members.
        friend class RFileIO;

        //! Return const reference to patch input.
        const std::vector<RPatchInput> &getPatchInput(void) const;

        //! Return reference to patch input.
        std::vector<RPatchInput> &getPatchInput(void);

        //! Return hemicube resolution.
        unsigned int getHemicubeResolution(void) const;

        //! Set hemicube resolution.
        void setHemicubeResolution(unsigned int hemicubeResolution);

        //! Return number of elements.
        unsigned int getNElements(void) const;

        //! Set number of elements.
        void setNElements(unsigned int nElements);

        //! Clear header.
        void clear(void);

};

#endif // RML_VIEW_FACTOR_MATRIX_HEADER_H
