/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_element_group.cpp                                    *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   8-th December 2011                                       *
 *                                                                   *
 *  DESCRIPTION: ElementGroup class definition                       *
 *********************************************************************/

#include <rblib.h>

#include "rml_element.h"
#include "rml_element_group.h"


RElementGroup::RElementGroup ()
{
    this->_init ();
    this->setDefaultEnvironmentConditions();
} /* RElementGroup::RElementGroup */


RElementGroup::RElementGroup (const RElementGroup &elementGroup) :
    REntityGroup(elementGroup)
{
    this->_init (&elementGroup);
} /* RElementGroup::RElementGroup */


RElementGroup::~RElementGroup ()
{
} /* RElementGroup::~RElementGroup */


RElementGroup & RElementGroup::operator = (const RElementGroup &elementGroup)
{
    this->REntityGroup::operator = (elementGroup);
    this->_init (&elementGroup);
    return (*this);
} /* RElementGroup::operator = */


void RElementGroup::_init (const RElementGroup *pElementGroup)
{
    if (pElementGroup)
    {
        this->elementIDs = pElementGroup->elementIDs;
        this->bConditions = pElementGroup->bConditions;
        this->eConditions = pElementGroup->eConditions;
        this->iConditions = pElementGroup->iConditions;
        this->material = pElementGroup->material;
        this->data = pElementGroup->data;
    }
} /* RElementGroup::_init */


void RElementGroup::add (unsigned int elementID)
{
    this->elementIDs.push_back (elementID);
} /* RElementGroup::add */


unsigned int RElementGroup::get (unsigned int position) const
{
    R_ERROR_ASSERT (position < this->elementIDs.size());
    return this->elementIDs[position];
} /* RElementGroup::get */


void RElementGroup::remove (unsigned int position)
{
    std::vector<unsigned int>::iterator iter;

    R_ERROR_ASSERT (position < this->elementIDs.size());

    iter = this->elementIDs.begin();
    std::advance (iter, position);

    this->elementIDs.erase (iter);
} /* RElementGroup::remove */


void RElementGroup::set (unsigned int position,
                         unsigned int elementID)
{
    R_ERROR_ASSERT (position < this->elementIDs.size());
    this->elementIDs[position] = elementID;
} /* RElementGroup::set */


bool RElementGroup::findPosition (unsigned int  elementID,
                                  unsigned int *position) const
{
    for (unsigned int i=0;i<this->size();i++)
    {
        if (elementID == this->get(i))
        {
            (*position) = i;
            return true;
        }
    }

    return false;
} /* RElementGroup::find_position */


unsigned int RElementGroup::size (void) const
{
    return (unsigned int)this->elementIDs.size();
} /* RElementGroup::size */


void RElementGroup::resize (unsigned int nmemb)
{
    this->elementIDs.resize(nmemb);
} /* RElementGroup::size */


void RElementGroup::addBoundaryCondition
                                (const RBoundaryCondition &bCondition)
{
    this->bConditions[bCondition.getType()] = bCondition;
} /* RElementGroup::addBoundaryCondition */


bool RElementGroup::hasBoundaryCondition(RBoundaryConditionType type) const
{
    return (this->bConditions.find(type) != this->bConditions.end());
} /* RElementGroup:: */


const RBoundaryCondition & RElementGroup::getBoundaryCondition
                                   (RBoundaryConditionType type) const
{
    std::map<RBoundaryConditionType,RBoundaryCondition>::const_iterator iter;

    iter = this->bConditions.find(type);

    R_ERROR_ASSERT (iter != this->bConditions.end());

    return iter->second;
} /* RElementGroup::getBoundaryCondition */


RBoundaryCondition &RElementGroup::getBoundaryCondition(RBoundaryConditionType type)
{
    std::map<RBoundaryConditionType,RBoundaryCondition>::iterator iter;

    iter = this->bConditions.find(type);

    R_ERROR_ASSERT (iter != this->bConditions.end());

    return iter->second;
} /* RElementGroup::getBoundaryCondition */


const RBoundaryCondition & RElementGroup::getBoundaryCondition
                                         (unsigned int position) const
{
    std::map<RBoundaryConditionType,RBoundaryCondition>::const_iterator iter;

    R_ERROR_ASSERT (position < this->getNBoundaryConditions());

    iter = this->bConditions.begin();
    std::advance (iter, position);

    return iter->second;
} /* RElementGroup::getBoundaryCondition */


RBoundaryCondition &RElementGroup::getBoundaryCondition(unsigned int position)
{
    std::map<RBoundaryConditionType,RBoundaryCondition>::iterator iter;

    R_ERROR_ASSERT (position < this->getNBoundaryConditions());

    iter = this->bConditions.begin();
    std::advance (iter, position);

    return iter->second;
} /* RElementGroup::getBoundaryCondition */


void RElementGroup::removeBoundaryCondition
                                         (RBoundaryConditionType type)
{
    std::map<RBoundaryConditionType,RBoundaryCondition>::iterator iter;

    iter = this->bConditions.find(type);

    R_ERROR_ASSERT (iter != this->bConditions.end());

    this->bConditions.erase (iter);
} /* RElementGroup::removeBoundaryCondition */


void RElementGroup::removeBoundaryCondition (unsigned int position)
{
    std::map<RBoundaryConditionType,RBoundaryCondition>::iterator iter;

    R_ERROR_ASSERT (position < this->getNBoundaryConditions());

    iter = this->bConditions.begin();
    std::advance (iter, position);

    this->bConditions.erase(iter);
} /* RElementGroup::removeBoundaryCondition */


void RElementGroup::removeAllBoundaryConditions(void)
{
    this->bConditions.clear();
} /* RElementGroup::removeAllBoundaryConditions */

void RElementGroup::addEnvironmentCondition
                                (const REnvironmentCondition &eCondition)
{
    this->eConditions[eCondition.getType()] = eCondition;
} /* RElementGroup::addEnvironmentCondition */


bool RElementGroup::hasEnvironmentCondition(REnvironmentConditionType type) const
{
    return (this->eConditions.find(type) != this->eConditions.end());
} /* RElementGroup::hasEnvironmentCondition */


const REnvironmentCondition & RElementGroup::getEnvironmentCondition
                                   (REnvironmentConditionType type) const
{
    std::map<REnvironmentConditionType,REnvironmentCondition>::const_iterator iter;

    iter = this->eConditions.find(type);

    R_ERROR_ASSERT (iter != this->eConditions.end());

    return iter->second;
} /* RElementGroup::getEnvironmentCondition */


REnvironmentCondition &RElementGroup::getEnvironmentCondition(REnvironmentConditionType type)
{
    std::map<REnvironmentConditionType,REnvironmentCondition>::iterator iter;

    iter = this->eConditions.find(type);

    R_ERROR_ASSERT (iter != this->eConditions.end());

    return iter->second;
} /* RElementGroup::getEnvironmentCondition */


const REnvironmentCondition & RElementGroup::getEnvironmentCondition
                                         (unsigned int position) const
{
    std::map<REnvironmentConditionType,REnvironmentCondition>::const_iterator iter;

    R_ERROR_ASSERT (position < this->getNEnvironmentConditions());

    iter = this->eConditions.begin();
    std::advance (iter, position);

    return iter->second;
} /* RElementGroup::getEnvironmentCondition */


REnvironmentCondition &RElementGroup::getEnvironmentCondition(unsigned int position)
{
    std::map<REnvironmentConditionType,REnvironmentCondition>::iterator iter;

    R_ERROR_ASSERT (position < this->getNEnvironmentConditions());

    iter = this->eConditions.begin();
    std::advance (iter, position);

    return iter->second;
} /* RElementGroup::getEnvironmentCondition */


void RElementGroup::removeEnvironmentCondition
                                         (REnvironmentConditionType type)
{
    std::map<REnvironmentConditionType,REnvironmentCondition>::iterator iter;

    iter = this->eConditions.find(type);

    R_ERROR_ASSERT (iter != this->eConditions.end());

    this->eConditions.erase (iter);
} /* RElementGroup::removeEnvironmentCondition */


void RElementGroup::removeEnvironmentCondition (unsigned int position)
{
    std::map<REnvironmentConditionType,REnvironmentCondition>::iterator iter;

    R_ERROR_ASSERT (position < this->getNEnvironmentConditions());

    iter = this->eConditions.begin();
    std::advance (iter, position);

    this->eConditions.erase(iter);
} /* RElementGroup::removeEnvironmentCondition */


void RElementGroup::removeAllEnvironmentConditions(void)
{
    this->eConditions.clear();
    this->setDefaultEnvironmentConditions();
} /* RElementGroup::removeAllEnvironmentConditions */


void RElementGroup::addInitialCondition
                                (const RInitialCondition &iCondition)
{
    this->iConditions[iCondition.getType()] = iCondition;
} /* RElementGroup::addInitialCondition */


bool RElementGroup::hasInitialCondition(RInitialConditionType type) const
{
    return (this->iConditions.find(type) != this->iConditions.end());
} /* RElementGroup::hasInitialCondition */


const RInitialCondition & RElementGroup::getInitialCondition
                                   (RInitialConditionType type) const
{
    std::map<RInitialConditionType,RInitialCondition>::const_iterator iter;

    iter = this->iConditions.find(type);

    R_ERROR_ASSERT (iter != this->iConditions.end());

    return iter->second;
} /* RElementGroup::getInitialCondition */


RInitialCondition &RElementGroup::getInitialCondition(RInitialConditionType type)
{
    std::map<RInitialConditionType,RInitialCondition>::iterator iter;

    iter = this->iConditions.find(type);

    R_ERROR_ASSERT (iter != this->iConditions.end());

    return iter->second;
} /* RElementGroup::getInitialCondition */


const RInitialCondition & RElementGroup::getInitialCondition
                                         (unsigned int position) const
{
    std::map<RInitialConditionType,RInitialCondition>::const_iterator iter;

    R_ERROR_ASSERT (position < this->getNInitialConditions());

    iter = this->iConditions.begin();
    std::advance (iter, position);

    return iter->second;
} /* RElementGroup::getInitialCondition */


RInitialCondition &RElementGroup::getInitialCondition(unsigned int position)
{
    std::map<RInitialConditionType,RInitialCondition>::iterator iter;

    R_ERROR_ASSERT (position < this->getNInitialConditions());

    iter = this->iConditions.begin();
    std::advance (iter, position);

    return iter->second;
} /* RElementGroup::getInitialCondition */


void RElementGroup::removeInitialCondition
                                         (RInitialConditionType type)
{
    std::map<RInitialConditionType,RInitialCondition>::iterator iter;

    iter = this->iConditions.find(type);

    R_ERROR_ASSERT (iter != this->iConditions.end());

    this->iConditions.erase (iter);
} /* RElementGroup::removeInitialCondition */


void RElementGroup::removeInitialCondition (unsigned int position)
{
    std::map<RInitialConditionType,RInitialCondition>::iterator iter;

    R_ERROR_ASSERT (position < this->getNInitialConditions());

    iter = this->iConditions.begin();
    std::advance (iter, position);

    this->iConditions.erase(iter);
} /* RElementGroup::removeInitialCondition */


void RElementGroup::removeAllInitialConditions(void)
{
    this->iConditions.clear();
} /* RElementGroup::removeAllInitialConditions */


REntityGroupType RElementGroup::getGroupType (RElementType elementType)
{
    if (R_ELEMENT_TYPE_IS_POINT (elementType))
    {
        return R_ENTITY_GROUP_POINT;
    }

    if (R_ELEMENT_TYPE_IS_LINE (elementType))
    {
        return R_ENTITY_GROUP_LINE;
    }

    if (R_ELEMENT_TYPE_IS_SURFACE (elementType))
    {
        return R_ENTITY_GROUP_SURFACE;
    }

    if (R_ELEMENT_TYPE_IS_VOLUME (elementType))
    {
        return R_ENTITY_GROUP_VOLUME;
    }

    return R_ENTITY_GROUP_NONE;
} /* RElementGroup::getGroupType */


void RElementGroup::setDefaultEnvironmentConditions(void)
{
    this->eConditions[R_ENVIRONMENT_CONDITION_G_ACCELERATION] = REnvironmentCondition(R_ENVIRONMENT_CONDITION_G_ACCELERATION);
    this->eConditions[R_ENVIRONMENT_CONDITION_TEMPERATURE] = REnvironmentCondition(R_ENVIRONMENT_CONDITION_TEMPERATURE);
} /* RElementGroup::setDefaultEnvironmentConditions */
