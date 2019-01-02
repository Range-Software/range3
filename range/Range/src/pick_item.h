/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   pick_item.h                                              *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   31-st October 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Pick item class declaration                         *
 *********************************************************************/

#ifndef PICK_ITEM_H
#define PICK_ITEM_H

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
        void _init ( const PickItem *pPickItem = nullptr );

    protected:

        //! Entity ID.
        SessionEntityID entityID;
        //! Element ID.
        //! For interpolated elements and holes element ID equals element position.
        uint elementID;
        //! Element position.
        //! For interpolated elements and holes element position equals element ID.
        uint elementPosition;
        //! Node ID.
        //! For interpolated elements and holes node ID equals node position.
        uint nodeID;
        //! Node position.
        //! For interpolated elements and holes node position equals node ID.
        uint nodePosition;

    public:

        //! Constructor.
        PickItem();

        //! Constructor.
        PickItem(const SessionEntityID &entityID, uint elementID = RConstants::eod, uint elementPosition = RConstants::eod, uint nodeID = RConstants::eod, uint nodePosition = RConstants::eod);

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

        //! Return element position.
        uint getElementPosition(void) const;

        //! Return node ID.
        uint getNodeID(void) const;

        //! Return node position.
        uint getNodePosition(void) const;

        //! Return item pick type.
        PickItemType getItemType(void) const;

        //! Operator ==.
        bool operator ==(const PickItem &pickItem);

};

#endif // PICK_ITEM_H
