/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   results_variable_selector.h                              *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   18-th March 2013                                         *
 *                                                                   *
 *  DESCRIPTION: Results variable selector class declaration         *
 *********************************************************************/

#ifndef RESULTS_VARIABLE_SELECTOR_H
#define RESULTS_VARIABLE_SELECTOR_H

#include <rmlib.h>

#include "variable_selector.h"

class ResultsVariableSelector : public VariableSelector
{

    Q_OBJECT

    protected:

    public:

        //! Constructor.
        explicit ResultsVariableSelector(QWidget *parent = nullptr);

    protected:

        //! Populate combo.
        void populate(RVariableType selectedVariableType = R_VARIABLE_NONE);

    protected slots:

        //! Catch Session::resultsChanged signal
        void onResultsChanged(uint);
        
};

#endif // RESULTS_VARIABLE_SELECTOR_H
