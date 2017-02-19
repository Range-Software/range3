/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   pick_item.h                                              *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   31-st October 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Pick item class declaration                         *
 *********************************************************************/

#ifndef __PICK_ITEM_H__
#define __PICK_ITEM_H__

#include "session_entity_id.h"

#define PICK_ITEM_TYPE_IS_VALID(_type) \
( \
    _type >= 0 && _type < PICK_ITEM_N_TYPES \
)

typedef enum _PickItemType
{
    PICK_ITEM_NONE = 0,
    PICK_ITEM_ELEMENT,
    PICK_ITEM_NODE,
    PICK_ITEM_HOLE_ELEMENT,
    PICK_ITEM_N_TYPES
} PickItemType;

class PickItem
{

    private:

        //! Internal initialization function.
        void _init ( const PickItem *pPickItem = 0 );

    protected:

        //! Entity ID.
        SessionEntityID entityID;
        //! Element ID.
        uint elementID;
        //! Node ID.
        uint nodeID;

    public:

        //! Constructor.
        PickItem();

        //! Constructor.
        PickItem(const SessionEntityID &entityID, uint elementID = RConstants::eod, uint nodeID = RConstants::eod);

        //! Copy constructor.
        PickItem(const PickItem &pickItem);

        //! Destructor.
        ~PickItem();

        //! Assignment operator.
        PickItem & operator =(const PickItem &pickItem);

        //! Return const reference to entity ID.
        const SessionEntityID & getEntityID(void) const;

        //! Return element ID.
        uint getElementID(void) const;

        //! Return element ID.
        uint getNodeID(void) const;

        //! Return item pick type.
        PickItemType getItemType(void) const;

        //! Operator ==.
        bool operator ==(const PickItem &pickItem);

};

#endif // __PICK_ITEM_H__
