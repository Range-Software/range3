/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_imatrix.h                                            *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   21-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Integer matrix class declaration                    *
 *********************************************************************/

#ifndef RBL_IMATRIX_H
#define RBL_IMATRIX_H

#include <vector>

#include "rbl_ivector.h"

//! Integer matrix class.
class RIMatrix
{

    protected:

        //! Vector of integer vectors values.
        std::vector<RIVector> array;

    private:

        //! Internal initialization function.
        void _init ( const RIMatrix *pMatrix = nullptr );

    public:

        //! Constructor.
        RIMatrix ();

        //! Constructor.
        RIMatrix ( unsigned int nRows,
                   unsigned int nColumns,
                   int value = int() );

        //! Copy constructor.
        RIMatrix ( const RIMatrix &matrix );

        //! Destructor.
        ~RIMatrix ();

        //! Assignment operator.
        RIMatrix & operator = ( const RIMatrix &matrix );

        //! Return number of rows.
        unsigned int getNRows (void) const;

        //! Return number of coluns.
        unsigned int getNColumns (void) const;

        //! Resize matrix array.
        void resize ( unsigned int nRows,
                      unsigned int nColumns,
                      int          value = int());

        //! Return value at given position.
        int getValue ( unsigned int row,
                       unsigned int column ) const;

        //! Set value at given position.
        void setValue ( unsigned int row,
                        unsigned int column,
                        int          value );

        //! Transpose matrix.
        void transpose ( void );

        //! Access operator - return const reference to RIVector.
        const RIVector & operator [] (unsigned int row) const;

        //! Access operator - return const reference to RIVector.
        RIVector & operator [] (unsigned int row);

        //! Clear matrix.
        void clear (void);

};

#endif /* RBL_IMATRIX_H */
