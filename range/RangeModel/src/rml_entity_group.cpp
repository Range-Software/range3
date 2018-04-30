/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_entity_group.h                                       *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   15-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Entity group class declaration                      *
 *********************************************************************/

#include "rml_entity_group.h"

void REntityGroup::_init(const REntityGroup *pEntityGroup)
{
    if (pEntityGroup)
    {
        this->name = pEntityGroup->name;
        this->data = pEntityGroup->data;
    }
}

REntityGroup::REntityGroup()
{
    this->_init();
}

REntityGroup::REntityGroup(const REntityGroup &entityGroup)
{
    this->_init(&entityGroup);
}

REntityGroup::~REntityGroup()
{
}

REntityGroup &REntityGroup::operator =(const REntityGroup &entityGroup)
{
    this->_init(&entityGroup);
    return (*this);
}

const QString &REntityGroup::getName(void) const
{
    return this->name;
} /* REntityGroup::getName */

void REntityGroup::setName(const QString &name)
{
    this->name = name;
} /* REntityGroup::setName */

std::vector<REntityGroupType> REntityGroup::getAllTypes(void)
{
    std::vector<REntityGroupType> types(R_ENTITY_GROUP_N_TYPES);
    types[0] = R_ENTITY_GROUP_POINT;
    types[1] = R_ENTITY_GROUP_LINE;
    types[2] = R_ENTITY_GROUP_SURFACE;
    types[3] = R_ENTITY_GROUP_VOLUME;
    types[4] = R_ENTITY_GROUP_VECTOR_FIELD;
    types[5] = R_ENTITY_GROUP_STREAM_LINE;
    types[6] = R_ENTITY_GROUP_CUT;
    types[7] = R_ENTITY_GROUP_ISO;
    types[9] = R_ENTITY_GROUP_SCALAR_FIELD;
    return types;
} /* REntityGroup::getAllTypes */

std::vector<REntityGroupType> REntityGroup::getMaskTypes(REntityGroupTypeMask elementGroupTypeMask)
{
    std::vector<REntityGroupType> types;
    if (elementGroupTypeMask & R_ENTITY_GROUP_POINT) types.push_back(R_ENTITY_GROUP_POINT);
    if (elementGroupTypeMask & R_ENTITY_GROUP_LINE) types.push_back(R_ENTITY_GROUP_LINE);
    if (elementGroupTypeMask & R_ENTITY_GROUP_SURFACE) types.push_back(R_ENTITY_GROUP_SURFACE);
    if (elementGroupTypeMask & R_ENTITY_GROUP_VOLUME) types.push_back(R_ENTITY_GROUP_VOLUME);
    if (elementGroupTypeMask & R_ENTITY_GROUP_VECTOR_FIELD) types.push_back(R_ENTITY_GROUP_VECTOR_FIELD);
    if (elementGroupTypeMask & R_ENTITY_GROUP_STREAM_LINE) types.push_back(R_ENTITY_GROUP_STREAM_LINE);
    if (elementGroupTypeMask & R_ENTITY_GROUP_CUT) types.push_back(R_ENTITY_GROUP_CUT);
    if (elementGroupTypeMask & R_ENTITY_GROUP_ISO) types.push_back(R_ENTITY_GROUP_ISO);
    if (elementGroupTypeMask & R_ENTITY_GROUP_SCALAR_FIELD) types.push_back(R_ENTITY_GROUP_SCALAR_FIELD);
    return types;
} /* REntityGroup::getMaskTypes */


const QString REntityGroup::getTypeName(REntityGroupType elementGroupType)
{
    switch (elementGroupType)
    {
        case R_ENTITY_GROUP_POINT:
            return "point";
        case R_ENTITY_GROUP_LINE:
            return "line";
        case R_ENTITY_GROUP_SURFACE:
            return "surface";
        case R_ENTITY_GROUP_VOLUME:
            return "volume";
        case R_ENTITY_GROUP_VECTOR_FIELD:
            return "vector field";
        case R_ENTITY_GROUP_STREAM_LINE:
            return "stream line";
        case R_ENTITY_GROUP_CUT:
            return "cut";
        case R_ENTITY_GROUP_ISO:
            return "iso";
        case R_ENTITY_GROUP_SCALAR_FIELD:
            return "scalar";
        case R_ENTITY_GROUP_NONE:
        default:
            return "none";
    }
} /* REntityGroup::getTypeName */


bool REntityGroup::typeIsElementGroup(REntityGroupType elementGroupType)
{
    switch (elementGroupType)
    {
        case R_ENTITY_GROUP_POINT:
        case R_ENTITY_GROUP_LINE:
        case R_ENTITY_GROUP_SURFACE:
        case R_ENTITY_GROUP_VOLUME:
            return true;
        default:
            return false;
    }
} /* REntityGroup::typeIsElementGroup */


bool REntityGroup::typeIsInterpolatedElementGroup(REntityGroupType elementGroupType)
{
    switch (elementGroupType)
    {
        case R_ENTITY_GROUP_CUT:
        case R_ENTITY_GROUP_ISO:
            return true;
        default:
            return false;
    }
} /* REntityGroup::typeIsInterpolatedElementGroup */
