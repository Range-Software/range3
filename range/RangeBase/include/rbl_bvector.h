/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_bvector.h                                            *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   14-th November 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Bool vector class declaration                       *
 *********************************************************************/

#ifndef __RBL_BVECTOR_H__
#define __RBL_BVECTOR_H__

#include <vector>

//! Vector class.
class RBVector : public std::vector<bool>
{

    private:

        //! Internal initialization function.
        void _init ( const RBVector *pArray = 0 );

    public:

        //! Constructor.
        RBVector ( unsigned int nRows = 0, bool value = bool() );

        //! Copy constructor.
        RBVector ( const RBVector &array );

        //! Destructor.
        ~RBVector ();

        //! Assignment operator.
        RBVector & operator = ( const RBVector &array );

        //! Return number of rows (size of the vector).
        inline unsigned int getNRows ( void ) const
        {
            return (unsigned int)this->size();
        }

//        //! Access element operator.
//        bool & operator[] ( unsigned int n );

//        //! Access element operator.
//        const bool & operator[] ( unsigned int n ) const;

        //! Fill vector with specified value.
        void fill ( bool value );

        //! Print content.
        void print ( bool oneLine = false, bool newLine = false ) const;
};

#endif /* __RBL_BVECTOR_H__ */
