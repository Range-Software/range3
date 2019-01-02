/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_book.h                                               *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   4-th August 2014                                         *
 *                                                                   *
 *  DESCRIPTION: Book class declaration                              *
 *********************************************************************/

#ifndef RBL_BOOK_H
#define RBL_BOOK_H

#include "rbl_uvector.h"

class RBook
{

    protected:

        //! Node book array.
        RUVector book;

    private:

        //! Internal initialization function.
        void _init(const RBook *pBook = nullptr);

    public:

        //! Constructor.
        RBook(unsigned int size = 0);

        //! Copy constructor.
        RBook(const RBook &book);

        //! Destructor.
        ~RBook();

        //! Assignment operator.
        RBook & operator =(const RBook &book);

        //! Resize the array.
        //! Resizing will also result in reinitializing the array.
        void resize(unsigned int size);

        //! Return size of the node book array.
        unsigned int size(void) const;

        //! Initialize the array.
        void initialize(void);

        //! Return book value.
        bool getValue(unsigned int position, unsigned int &value) const;

        //! Set book value.
        void setValue(unsigned int position, unsigned int value);

        //! Enable node.
        //! If consolidate is set to true all following values will be adjusted by (+1).
        void enable(unsigned int position, bool consolidate = true);

        //! Disable node.
        //! If consolidate is set to true all following values will be adjusted by (-1).
        void disable(unsigned int position, bool consolidate = true);

        //! Return number of enabled nodes.
        unsigned int getNEnabled(void) const;

        //! Return number of disabled nodes.
        unsigned int getNDisabled(void) const;

        //! Return book of enabled values.
        RUVector getEnabled(void) const;

        //! Print.
        void print(void) const;

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif // RBL_BOOK_H
