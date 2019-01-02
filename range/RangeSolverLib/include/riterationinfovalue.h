/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   riterationinfovalue.h                                    *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th January 2016                                        *
 *                                                                   *
 *  DESCRIPTION: Iteration info value class declaration              *
 *********************************************************************/

#ifndef RITERATIONINFOVALUE_H
#define RITERATIONINFOVALUE_H

#include <QString>

class RIterationInfoValue
{

    protected:

        //! Name.
        QString name;
        //! Value.
        double value;

    private:

        //! Internal initialization function.
        void _init(const RIterationInfoValue *pIterationInfoValue = nullptr);

    public:

        //! Constructor.
        RIterationInfoValue();

        //! Constructor.
        RIterationInfoValue(const QString &name, double value);

        //! Copy constructor.
        RIterationInfoValue(const RIterationInfoValue &iterationInfoValue);

        //! Destructor.
        ~RIterationInfoValue();

        //! Assignment operator.
        RIterationInfoValue & operator =(const RIterationInfoValue &iterationInfoValue);

        //! Return name.
        const QString &getName(void) const;

        //! Return value.
        double getValue(void) const;

};

#endif // RITERATIONINFOVALUE_H
