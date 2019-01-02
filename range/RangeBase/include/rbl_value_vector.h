/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_value_vector.h                                       *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   13-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Value vector class declaration                      *
 *********************************************************************/

#ifndef RBL_VALUE_VECTOR_H
#define RBL_VALUE_VECTOR_H

#include <QString>
#include "rbl_rvector.h"

//! Value vector class.
class RValueVector
{

    private:

        //! Internal initialization function.
        void _init ( const RValueVector *valueVector = nullptr );

    protected:

        //! Value name.
        QString name;
        //! Value units.
        QString units;
        //! Values.
        RRVector values;

    public:

        //! Constructor.
        RValueVector ();

        //! Copy constructor.
        RValueVector ( const RValueVector &valueVector );

        //! Destructor.
        ~RValueVector ();

        //! Return value name.
        const QString & getName ( void ) const;

        //! Set value name.
        void setName ( const QString &name );

        //! Return value units.
        const QString & getUnits ( void ) const;

        //! Set value units.
        void setUnits ( const QString &units );

        //! Return number of values in the vector.
        unsigned int size ( void ) const;

        //! Resize value vector.
        void resize ( unsigned int nmemb );

        //! Add new item to the value vector.
        void add ( double value = 0 );

        //! Remove item at given position from the value vector.
        void remove ( unsigned int position );

        //! Remove item at given position from the value vector.
        //! If valueBook[i] == RConstants::eod then value will be removed.
        void remove(const std::vector<uint> &valueBook);

        //! Fill values with given value.
        void fill ( double value );

        //! Drop all values
        inline void clear ( void )
        {
            this->values.clear();
        }

        //! Test if value vector is empty.
        inline bool empty ( void ) const
        {
            return this->values.empty();
        }

        //! Assignment operator.
        RValueVector & operator = ( const RValueVector &valueVector );

        //! Return value at given position.
        double operator [] ( unsigned int position ) const;

        //! Set value at given position.
        double & operator [] ( unsigned int position );

        //! Return const reference to data vecor.
        inline const RRVector & getDataVector(void) const
        {
            return this->values;
        }

        //! Return reference to data vecor.
        inline RRVector & getDataVector(void)
        {
            return this->values;
        }

};

#endif /* RBL_VALUE_VECTOR_H */
