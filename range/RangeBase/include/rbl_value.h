/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_value.h                                              *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   10-th February 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Value class declaration                             *
 *********************************************************************/

#ifndef RBL_VALUE_H
#define RBL_VALUE_H

#include <vector>
#include <typeinfo>
#include <limits>

//! Parameter class.
template <class T>
class RValue
{

    private:

        //! Internal initialization function.
        void _init ( const RValue *pValue = nullptr );

    protected:

        //! Actual value.
        T val;
        //! Minimum value allowed.
        T min;
        //! Maximum value allowed.
        T max;
        //! Check if min max values are relevant.
        bool hasBounds;
        //! Vector of allowed values.
        std::vector<T> allowedVals;

        //! Set min max values.
        void setBounds (void);

    public:

        //! Constructor.
        RValue ( const T &val = T() );

        //! Copy constructor.
        RValue ( const RValue &value );

        //! Destructor.
        ~RValue ();

        //! Assignment operator.
        RValue & operator = ( const RValue &value );

        //! Return value.
        const T & getValue (void) const;

        //! Set new value.
        void setValue ( const T &val );

        //! Return minimum value.
        const T & getMin ( void ) const;

        //! Return maximum value.
        const T & getMax ( void ) const;

        //! Set min and max bounds.
        void setRange ( const T &min, const T &max );

        //! Return true if value has bounds.
        bool getHasBounds ( void ) const;

        //! Return allowed values.
        const std::vector<T> & getAllowed ( void ) const;

        //! Return allowed values.
        void setAllowed ( const std::vector<T> &allowedVals );

        //! Check if value is in bounds and if it is equal
        //! to one of allowed values.
        //! If vector of allowed values is empty value is
        //! allowed.
        bool checkValue ( const T &val ) const;

};


template <class T>
RValue<T>::RValue (const T &val)
{
    this->_init();
    this->setValue(val);
} /* RValue<T>::RValue */


template <class T>
RValue<T>::RValue (const RValue<T> &value)
{
    this->_init(&value);
} /* RValue<T>::RValue (copy) */


template <class T>
RValue<T>::~RValue ()
{
} /* RValue<T>::~RValue */


template <class T>
RValue<T> & RValue<T>::operator = (const RValue<T> &value)
{
    this->_init(&value);
    return (*this);
} /* RValue<T>::operator = */


template <class T>
void RValue<T>::_init (const RValue *pValue)
{
    this->setBounds();
    if (pValue)
    {
        this->setValue(pValue->getValue());
    }
} /* RValue<T>::_init */


template <class T>
const T & RValue<T>::getValue (void) const
{
    return this->val;
} /* RValue<T>::getValue */


template <class T>
void RValue<T>::setValue (const T &val)
{
    this->val = val;
} /* RValue<T>::setValue */

template <class T>
void RValue<T>::setBounds (void)
{
    this->hasBounds = false;
    if (typeid(this->val) == typeid(int)          ||
        typeid(this->val) == typeid(unsigned int) ||
        typeid(this->val) == typeid(float)        ||
        typeid(this->val) == typeid(double)         )
    {
        this->hasBounds = true;
    }

    if (this->hasBounds)
    {
        this->min = std::numeric_limits<T>::min();
        this->min = std::numeric_limits<T>::max();
    }
} /* RValue<T>::setBounds */


template <class T>
const T & RValue<T>::getMin (void) const
{
    return this->min;
} /* RValue<T>::getMin */


template <class T>
const T & RValue<T>::getMax (void) const
{
    return this->max;
} /* RValue<T>::getMax */


template <class T>
void RValue<T>::setRange (const T &min, const T &max)
{
    R_ERROR_ASSERT(min <= max);

    this->min = min;
    this->max = max;
} /* RValue<T>::setRange */


template <class T>
bool RValue<T>::getHasBounds (void) const
{
    return this->hasBounds;
} /* RValue<T>::getHasBounds */


template <class T>
const std::vector<T> & RValue<T>::getAllowed (void) const
{
    return this->allowedVals;
} /* RValue<T>::getAllowed */


template <class T>
void RValue<T>::setAllowed(const std::vector<T> &allowedVals)
{
    this->allowedVals = allowedVals;
} /* RValue<T>::setAllowed */


template <class T>
bool RValue<T>::checkValue(const T &val) const
{
    // Check if value is in bounds.
    if (this->getHasBounds())
    {
        if (val < this->getMin())
        {
            return false;
        }
        if (val > this->getMax())
        {
            return false;
        }
    }
    // Check if value is one of allowed ones. If vector of allowed
    // values is empty value is allowed.
    typename std::vector<T>::const_iterator iter;
    for (iter = this->allowedVals.begin();
         iter != this->allowedVals.end();
         ++iter)
    {
        if (val != (*iter))
        {
            return false;
        }
    }
    return true;
} /* RValue<T>::checkValue */

#endif /* RBL_VALUE_H */
