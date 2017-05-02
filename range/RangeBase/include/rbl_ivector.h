/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_ivector.h                                            *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   21-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Integer vector class declaration                    *
 *********************************************************************/

#ifndef __RBL_IVECTOR_H__
#define __RBL_IVECTOR_H__

#include <vector>

//! Vector class.
class RIVector : public std::vector<int>
{

    private:

        //! Internal initialization function.
        void _init ( const RIVector *pArray = 0 );

    public:

        //! Constructor.
        RIVector ( unsigned int nRows = 0, int value = int() );

        //! Copy constructor.
        RIVector ( const RIVector &array );

        //! Destructor.
        ~RIVector ();

        //! Assignment operator.
        RIVector & operator = ( const RIVector &array );

        //! Return number of rows (size of the vector).
        inline unsigned int getNRows ( void ) const
        {
            return this->size();
        }

        //! Access element operator.
        int & operator[] ( unsigned int n );

        //! Access element operator.
        const int & operator[] ( unsigned int n ) const;

        //! Fill vector with specified value.
        void fill ( int value );

        //! Print content.
        void print ( bool oneLine = false, bool newLine = false ) const;
};

#endif /* __RBL_IVECTOR_H__ */