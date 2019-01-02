/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   pick_value.h                                             *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   20-th April 2016                                         *
 *                                                                   *
 *  DESCRIPTION: Pick value class declaration                        *
 *********************************************************************/

#ifndef PICK_VALUE_H
#define PICK_VALUE_H

#include <rblib.h>

class PickValue
{

    private:

        //! Pick position.
        RR3Vector position;
        //! Pick values.
        RRVector values;

    private:

        //! Internal initialization function.
        void _init(const PickValue *pPickValue = nullptr);

    public:

        //! Constructor.
        PickValue();

        //! Constructor.
        PickValue(const RR3Vector &position, const RRVector &values);

        //! Copy constructor.
        PickValue(const PickValue &pickValue);

        //! Destructor.
        ~PickValue();

        //! Assignment operator.
        PickValue &operator =(const PickValue &pickValue);

        //! Return const reference to pick position.
        const RR3Vector &getPosition(void) const;

        //! Set pick position.
        void setPosition(const RR3Vector &position);

        //! Return const reference to pick values.
        const RRVector &getValues(void) const;

        //! Set pick values.
        void setValues(const RRVector &values);

};

#endif // PICK_VALUE_H
