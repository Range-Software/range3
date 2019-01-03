/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_results.h                                            *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   14-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Results class declaration                           *
 *********************************************************************/

#ifndef RML_RESULTS_H
#define RML_RESULTS_H

#include <string>
#include <vector>

#include "rml_variable.h"

//! Results class.
class RResults
{

    private:

        //! Internal initialization function.
        void _init ( const RResults *results = nullptr );

    protected:

        //! Number of nodes.
        unsigned int nnodes;
        //! Number of elements.
        unsigned int nelements;
//        //! Current computational time / frequency (modal analysis).
//        double compTime;
        //! Variables.
        std::vector<RVariable> variables;

    public:

        //! Constructor.
        RResults ();

        //! Copy constructor.
        RResults ( const RResults &results );

        //! Destructor.
        ~RResults ();

        //! Assignment operator.
        RResults & operator = ( const RResults &results );

        //! Clear results.
        void clearResults(void);

        //! Return current computational time / frequency.
        double getCompTime(void) const;

        //! Set current computational time / frequency.
        void setCompTime(double compTime);

        //! Return number of variables.
        unsigned int getNVariables ( void ) const;

        //! Set number of node variables.
        void setNVariables ( unsigned int nvariables );

        //! Return const reference to variable.
        const RVariable & getVariable ( unsigned int position ) const;

        //! Return reference to variable.
        RVariable & getVariable ( unsigned int position );

        //! Find variable.
        //! If such variable type can not be found RConstants::eod is returned.
        unsigned int findVariable ( RVariableType variableType ) const;

        //! Find variable.
        //! If such variable type can not be found and create is set to false
        //! RConstants::eod is returned.
        //! If create is true and variable can not be found it will be created
        //! and its possition will be returned.
//        unsigned int findVariable ( RVariableType variableType, bool create = false );

        //! Add variable.
        //! If such variable already exist it will be replaced.
        unsigned int addVariable ( const RVariable &variable );

        //! Set variable.
        void setVariable ( unsigned int     position,
                           const RVariable &variable );

        //! Remove variable.
        virtual void removeVariable(uint position);

        //! Remove all variables.
        virtual void removeAllVariables(void);

        //! Return number of nodes.
        unsigned int getNNodes ( void ) const;

        //! Set number of nodes.
        virtual void setNNodes ( unsigned int nnodes );

        //! Increase number of nodes by one.
        virtual void addNode ( double value = 0.0 );

        //! Remove node from results at give position.
        virtual void removeNode ( unsigned int position );

        //! Return number of elements.
        unsigned int getNElements ( void ) const;

        //! Set number of elements.
        virtual void setNElements ( unsigned int nelements );

        //! Increase number of elements by one.
        virtual void addElement ( double value = 0.0 );

        //! Remove element from results at give position.
        virtual void removeElement ( unsigned int position );

        //! Remove elements from results at give positions.
        //! If elementBook[i] == RConstants::eod then element will be removed.
        void removeElements(const std::vector<uint>&elementBook);

};

#endif /* RML_RESULTS_H */
