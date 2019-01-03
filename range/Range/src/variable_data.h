/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   variable_data.h                                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   17-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Variable data class declaration                     *
 *               Used as data object in RVariable                    *
 *********************************************************************/

#ifndef VARIABLE_DATA_H
#define VARIABLE_DATA_H

#include <rblib.h>
#include <rmlib.h>

class VariableData
{
    private:

        //! Internal initialization function.
        void _init ( const VariableData *pVariableData = nullptr );

    protected:


    public:

        //! Constructor.
        VariableData();

        //! Copy constructor.
        VariableData(const VariableData &variableData);

        //! Destructor.
        ~VariableData();

        //! Assignment operator.
        VariableData & operator = (const VariableData &variableData);

        //! Set data object to provided variable.
        void setToVariable(RVariable &variable);

        //! Get pointer to variable data from provided variable.
        static VariableData * getFromVariable(const RVariable &variable);

        //! Initialize default data callbacks.
        static void initDataCb(void);

};

#endif /* VARIABLE_DATA_H */
