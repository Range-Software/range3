/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_variable_data.h                                      *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   18-th February 2013                                      *
 *                                                                   *
 *  DESCRIPTION: Variable data class declaration                     *
 *********************************************************************/

#ifndef RML_VARIABLE_DATA_H
#define RML_VARIABLE_DATA_H

#include <QString>

class RVariableData
{

    protected:

        //! Minimum display value.
        double minDisplayValue;
        //! Maximum display value.
        double maxDisplayValue;
        //! Display value range.
        bool displayValueRange;
        //! Name of the value range.
        QString valueRangeName;
        //! Scaling factor.
        double scale;

    private:

        //! Internal initialization function
        void _init(const RVariableData *pVariableData = nullptr);

    public:

        //! Constructor.
        RVariableData();

        //! Copy constructor.
        RVariableData(const RVariableData &variableData);

        //! Destructor.
        ~RVariableData();

        //! Assignment operator
        RVariableData & operator =(const RVariableData &variableData);

        //! Return minimum display value.
        double getMinDisplayValue(void) const;

        //! Return maximum display value.
        double getMaxDisplayValue(void) const;

        //! Set minimum and maximum display values.
        void setMinMaxDisplayValue(double minDisplayValue, double maxDisplayValue);

        //! Return whether value range should be displayed.
        bool getDisplayValueRange(void) const;

        //! Set whether value range should be displayed.
        void setDisplayValueRange(bool displayValueRange);

        //! Return const reference to value range name.
        const QString & getValueRangeName(void) const;

        //! Set value range name.
        void setValueRangeName(const QString &valueRangeName);

        //! Return scale.
        double getScale(void) const;

        //! Set scale.
        void setScale(double scale);

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif // RML_VARIABLE_DATA_H
