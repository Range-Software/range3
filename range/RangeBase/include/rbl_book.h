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

#include <QtGlobal>

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
        RBook(uint size = 0);

        //! Copy constructor.
        RBook(const RBook &book);

        //! Destructor.
        ~RBook();

        //! Assignment operator.
        RBook & operator =(const RBook &book);

        //! Resize the array.
        //! Resizing will also result in reinitializing the array.
        void resize(uint size);

        //! Return size of the node book array.
        uint size() const;

        //! Initialize the array.
        void initialize();

        //! Return book value.
        bool getValue(uint position, uint &value) const;

        //! Set book value.
        void setValue(uint position, uint value);

        //! Enable node.
        //! If consolidate is set to true all following values will be adjusted by (+1).
        void enable(uint position, bool consolidate = true);

        //! Disable node.
        //! If consolidate is set to true all following values will be adjusted by (-1).
        void disable(uint position, bool consolidate = true);

        //! Return number of enabled nodes.
        uint getNEnabled() const;

        //! Return number of disabled nodes.
        uint getNDisabled() const;

        //! Return book of enabled values.
        RUVector getEnabled() const;

        //! Print.
        void print() const;

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif // RBL_BOOK_H
