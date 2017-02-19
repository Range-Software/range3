/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_view_factor_row.h                                    *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   15-th July 2014                                          *
 *                                                                   *
 *  DESCRIPTION: View-Factor row class declaration                   *
 *********************************************************************/

#ifndef __RML_VIEW_FACTOR_ROW_H__
#define __RML_VIEW_FACTOR_ROW_H__

#include <rblib.h>

#include "rml_sparse_vector.h"

class RViewFactorRow
{

    protected:

        //! View factors.
        RSparseVector<double> viewFactors;

    private:

        //! Internal initialization function.
        void _init(const RViewFactorRow *pViewFactorRow = 0);

    public:

        //! Constructor.
        RViewFactorRow();

        //! Copy constructor.
        RViewFactorRow(const RViewFactorRow &viewFactorRow);

        //! Destructor.
        ~RViewFactorRow();

        //! Assignment operator.
        RViewFactorRow &operator =(const RViewFactorRow &viewFactorRow);

        //! Return const reference to view-factor vector.
        const RSparseVector<double> &getViewFactors(void) const;

        //! Return reference to view-factor vector.
        RSparseVector<double> &getViewFactors(void);

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif // __RML_VIEW_FACTOR_ROW_H__
