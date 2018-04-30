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
        this->elementPosition = pPickItem->elementPosition;
        this->nodeID = pPickItem->nodeID;
        this->nodePosition = pPickItem->nodePosition;
    }
}

PickItem::PickItem()
    : entityID(SessionEntityID())
    , elementID(RConstants::eod)
    , elementPosition(RConstants::eod)
    , nodeID(RConstants::eod)
    , nodePosition(RConstants::eod)
{
    this->_init();
}

PickItem::PickItem(const SessionEntityID &entityID, uint elementID, uint elementPosition, uint nodeID, uint nodePosition)
    : entityID(entityID)
    , elementID(elementID)
    , elementPosition(elementPosition)
    , nodeID(nodeID)
    , nodePosition(nodePosition)
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

uint PickItem::getElementPosition(void) const
{
    return this->elementPosition;
}

uint PickItem::getNodeID(void) const
{
    return this->nodeID;
}

uint PickItem::getNodePosition(void) const
{
    return this->nodePosition;
}

PickItemType PickItem::getItemType(void) const
{
    if (this->entityID.getType() != R_ENTITY_GROUP_NONE)
    {
        if (this->elementPosition != RConstants::eod && this->elementID != RConstants::eod)
        {
            if (this->nodePosition != RConstants::eod && this->nodeID != RConstants::eod)
            {
                return PICK_ITEM_NODE;
            }
            return PICK_ITEM_ELEMENT;
        }
    }
    else
    {
        if (this->elementPosition != RConstants::eod && this->elementID != RConstants::eod)
        {
            return PICK_ITEM_HOLE_ELEMENT;
        }
    }
    return PICK_ITEM_NONE;
}

bool PickItem::operator ==(const PickItem &pickItem)
{
    return (this->entityID == pickItem.entityID &&
            this->elementPosition == pickItem.elementPosition &&
            this->elementID == pickItem.elementID &&
            this->nodePosition == pickItem.nodePosition &&
            this->nodeID == pickItem.nodeID);
}
