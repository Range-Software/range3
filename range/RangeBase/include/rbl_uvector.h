/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_uvector.h                                            *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th January 2014                                        *
 *                                                                   *
 *  DESCRIPTION: Unsigned integer vector class declaration           *
 *********************************************************************/

#ifndef RBL_UVECTOR_H
#define RBL_UVECTOR_H

#include <vector>

//! Vector class.
class RUVector : public std::vector<unsigned int>
{

    private:

        //! Internal initialization function.
        void _init ( const RUVector *pArray = nullptr );

    public:

        //! Constructor.
        RUVector ( unsigned int nRows = 0, unsigned int value = 0 );

        //! Copy constructor.
        RUVector ( const RUVector &array );

        //! Destructor.
        ~RUVector ();

        //! Assignment operator.
        RUVector & operator = ( const RUVector &array );

        //! Return number of rows (size of the vector).
        inline unsigned int getNRows ( void ) const
        {
            return (unsigned int)this->size();
        }

        //! Access element operator.
        unsigned int & operator[] ( unsigned int n );

        //! Access element operator.
        const unsigned int & operator[] ( unsigned int n ) const;

        //! Fill vector with specified value.
        void fill ( unsigned int value );

        //! Print content.
        void print ( bool oneLine = false, bool newLine = false ) const;
};

#endif /* RBL_UVECTOR_H */
