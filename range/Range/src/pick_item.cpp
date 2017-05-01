/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   pick_item.cpp                                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   31-st October 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Pick item class definition                          *
 *********************************************************************/

#include "pick_item.h"

void PickItem::_init(const PickItem *pPickItem)
{
    if (pPickItem)
    {
        this->entityID = pPickItem->entityID;
        this->elementID = pPickItem->elementID;
        this->nodeID = pPickItem->nodeID;
    }
}

PickItem::PickItem()
    : entityID(SessionEntityID())
    , elementID(RConstants::eod)
    , nodeID(RConstants::eod)
{
    this->_init();
}

PickItem::PickItem(const SessionEntityID &entityID, uint elementID, uint nodeID)
    : entityID(entityID)
    , elementID(elementID)
    , nodeID(nodeID)
{
    this->_init();
}

PickItem::PickItem(const PickItem &pickItem)
{
    this->_init(&pickItem);
}

PickItem::~PickItem()
{
}

PickItem &PickItem::operator =(const PickItem &pickItem)
{
    this->_init(&pickItem);
    return (*this);
}

const SessionEntityID &PickItem::getEntityID(void) const
{
    return this->entityID;
}

uint PickItem::getElementID(void) const
{
    return this->elementID;
}

uint PickItem::getNodeID(void) const
{
    return this->nodeID;
}

PickItemType PickItem::getItemType(void) const
{
    if (this->entityID.getType() != R_ENTITY_GROUP_NONE)
    {
        if (this->elementID != RConstants::eod)
        {
            if (this->nodeID != RConstants::eod)
            {
                return PICK_ITEM_NODE;
            }
            return PICK_ITEM_ELEMENT;
        }
    }
    else
    {
        if (this->elementID != RConstants::eod)
        {
            return PICK_ITEM_HOLE_ELEMENT;
        }
    }
    return PICK_ITEM_NONE;
}

bool PickItem::operator ==(const PickItem &pickItem)
{
    return (this->entityID == pickItem.entityID &&
            this->elementID == pickItem.elementID &&
            this->nodeID == pickItem.nodeID);
}
