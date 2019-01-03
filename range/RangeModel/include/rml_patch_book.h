/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_patch_book.h                                         *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   5-th August 2014                                         *
 *                                                                   *
 *  DESCRIPTION: Patch book class declaration                        *
 *********************************************************************/

#ifndef RML_PATCH_BOOK_H
#define RML_PATCH_BOOK_H

#include <vector>
#include <map>

#include "rml_patch.h"

class RPatchBook
{

    protected:

        //! Vector of patches.
        std::vector<RPatch> patches;
        //! Element to patch map.
        std::map<uint,uint> elementPatchMap;

    private:

        //! Internal initialization function.
        void _init(const RPatchBook *pPatchBook = nullptr);

    public:

        //! Constructor.
        RPatchBook();

        //! Copy constructor.
        RPatchBook(const RPatchBook &patchBook);

        //! Destructor.
        ~RPatchBook();

        //! Assignment operator.
        RPatchBook &operator =(const RPatchBook &patchBook);

        //! Return number of patches.
        uint getNPatches(void) const;

        //! Return const reference to patch.
        const RPatch &getPatch(uint patchID) const;

        //! Return patch ID for given elemet ID.
        //! If given element ID has no patch assigned RConstants::eod is returned.
        uint findPatchID(uint elementID) const;

        //! Clear patch book.
        void clear(void);

        //! Create new patch and return its ID.
        uint createNewPatch(uint surfaceID);

        //! Register element ID for given patch.
        void registerElementID(uint patchID, uint elementID);

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif // RML_PATCH_BOOK_H
