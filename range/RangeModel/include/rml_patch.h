/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_patch.h                                              *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   5-th August 2014                                         *
 *                                                                   *
 *  DESCRIPTION: Patch class declaration                             *
 *********************************************************************/

#ifndef RML_PATCH_H
#define RML_PATCH_H

#include <rblib.h>

class RPatch
{

    protected:

        //! Surface ID.
        uint surfaceID;
        //! Vector of element ids.
        RUVector elementIDs;

    private:

        //! Internal initialization function.
        void _init(const RPatch *pPatch = nullptr);

    public:

        //! Constructor.
        RPatch(uint surfaceID = 0);

        //! Copy constructor.
        RPatch(const RPatch &patch);

        //! Destructor.
        ~RPatch();

        //! Assignment operator.
        RPatch &operator =(const RPatch &patch);

        //! Return patch surface ID;
        uint getSurfaceID(void) const;

        //! Return const reference to vector of element IDs;
        const RUVector & getElementIDs(void) const;

        //! Add element ID.
        //! Return true if element was added.
        bool addElementID(uint elementID);

        //! Remove element ID.
        //! Return true if element was removed.
        bool removeElementID(uint elementID);

        //! Return true if patch contains given element ID.
        bool hasElementID(uint elementID) const;

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif // RML_PATCH_H
