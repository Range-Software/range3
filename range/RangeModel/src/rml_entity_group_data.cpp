/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_entity_group_data.cpp                                *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   17-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Entity group data class declaration                 *
 *********************************************************************/

#include <cstdlib>
#include <cmath>
#include <algorithm>

#include "rml_entity_group_data.h"

typedef struct _REntityGroupVariableDisplayDesc
{
    QString name;
} REntityGroupVariableDisplayDesc;

const REntityGroupVariableDisplayDesc elementGroupVariableDisplayDesc [] =
{
    { "None" },
    { "Scalar" },
    { "Displacement" }
};

REntityGroupData::REntityGroupData() :
    selected(false),
    visible(false),
    drawWire(false),
    drawEdges(false),
    drawNodes(false),
    drawElementNumbers(false),
    drawNodeNumbers(false),
    drawArrowHeads(false),
    drawEqualArrowLength(false),
    drawArrowFrom(true),
    colorByPatch(false),
    colorByViewFactor(false)
{
    int r = int(floor(255*double(rand())/double(RAND_MAX)));
    int g = int(floor(255*double(rand())/double(RAND_MAX)));
    int b = int(floor(255*double(rand())/double(RAND_MAX)));
    this->setColor(r,g,b);
    this->_init();
}

REntityGroupData::REntityGroupData(const REntityGroupData &elementGroupData)
{
    this->_init(&elementGroupData);
}

REntityGroupData::~REntityGroupData()
{
}

REntityGroupData & REntityGroupData::operator =(const REntityGroupData &elementGroupData)
{
    this->_init(&elementGroupData);
    return (*this);
}

void REntityGroupData::_init(const REntityGroupData *pElementGroupData)
{
    if (pElementGroupData)
    {
        this->selected = pElementGroupData->selected;
        this->visible = pElementGroupData->visible;
        this->rgba[0] = pElementGroupData->rgba[0];
        this->rgba[1] = pElementGroupData->rgba[1];
        this->rgba[2] = pElementGroupData->rgba[2];
        this->rgba[3] = pElementGroupData->rgba[3];
        this->drawWire = pElementGroupData->drawWire;
        this->drawEdges = pElementGroupData->drawEdges;
        this->drawNodes = pElementGroupData->drawNodes;
        this->drawElementNumbers = pElementGroupData->drawElementNumbers;
        this->drawNodeNumbers = pElementGroupData->drawNodeNumbers;
        this->drawArrowHeads = pElementGroupData->drawArrowHeads;
        this->drawEqualArrowLength = pElementGroupData->drawEqualArrowLength;
        this->drawArrowFrom = pElementGroupData->drawArrowFrom;
        this->colorByPatch = pElementGroupData->colorByPatch;
        this->colorByViewFactor = pElementGroupData->colorByViewFactor;
        this->variableData = pElementGroupData->variableData;
    }
}

bool REntityGroupData::getSelected(void) const
{
    return this->selected;
}

void REntityGroupData::setSelected(bool selected)
{
    this->selected = selected;
}

bool REntityGroupData::getVisible(void) const
{
    return this->visible;
}

void REntityGroupData::setVisible(bool visible)
{
    this->visible = visible;
}

void REntityGroupData::getColor(int &r, int &g, int &b, int &a) const
{
    r = this->rgba[0];
    g = this->rgba[1];
    b = this->rgba[2];
    a = this->rgba[3];
}

void REntityGroupData::setColor(int r, int g, int b, int a)
{
    this->rgba[0] = std::max(std::min(r,255),0);
    this->rgba[1] = std::max(std::min(g,255),0);
    this->rgba[2] = std::max(std::min(b,255),0);
    this->rgba[3] = std::max(std::min(a,255),0);
}

bool REntityGroupData::getDrawWire(void) const
{
    return this->drawWire;
}

void REntityGroupData::setDrawWire(bool drawWire)
{
    this->drawWire = drawWire;
}

bool REntityGroupData::getDrawEdges(void) const
{
    return this->drawEdges;
}

void REntityGroupData::setDrawEdges(bool drawEdges)
{
    this->drawEdges = drawEdges;
}

bool REntityGroupData::getDrawNodes(void) const
{
    return this->drawNodes;
}

void REntityGroupData::setDrawNodes(bool drawNodes)
{
    this->drawNodes = drawNodes;
}

bool REntityGroupData::getDrawElementNumbers(void) const
{
    return this->drawElementNumbers;
}

void REntityGroupData::setDrawElementNumbers(bool drawElementNumbers)
{
    this->drawElementNumbers = drawElementNumbers;
}

bool REntityGroupData::getDrawNodeNumbers(void) const
{
    return this->drawNodeNumbers;
}

void REntityGroupData::setDrawNodeNumbers(bool drawNodeNumbers)
{
    this->drawNodeNumbers = drawNodeNumbers;
}

bool REntityGroupData::getDrawArrowHeads(void) const
{
    return this->drawArrowHeads;
}

void REntityGroupData::setDrawArrowHeads(bool drawArrowHeads)
{
    this->drawArrowHeads = drawArrowHeads;
}

bool REntityGroupData::getDrawEqualArrowLength(void) const
{
    return this->drawEqualArrowLength;
}

void REntityGroupData::setDrawEqualArrowLength(bool drawEqualArrowLength)
{
    this->drawEqualArrowLength = drawEqualArrowLength;
}

bool REntityGroupData::getDrawArrowFrom(void) const
{
    return this->drawArrowFrom;
}

void REntityGroupData::setDrawArrowFrom(bool drawArrowFrom)
{
    this->drawArrowFrom = drawArrowFrom;
}

bool REntityGroupData::getColorByPatch(void) const
{
    return this->colorByPatch;
}

void REntityGroupData::setColorByPatch(bool colorByPatch)
{
    this->colorByPatch = colorByPatch;
}

bool REntityGroupData::getColorByViewFactor(void) const
{
    return this->colorByViewFactor;
}

void REntityGroupData::setColorByViewFactor(bool colorByViewFactor)
{
    this->colorByViewFactor = colorByViewFactor;
}

const std::map<RVariableType,REntityGroupVariableDisplayTypeMask> &REntityGroupData::getVariableData(void) const
{
    return this->variableData;
}

std::map<RVariableType, REntityGroupVariableDisplayTypeMask> &REntityGroupData::getVariableData(void)
{
    return this->variableData;
}

void REntityGroupData::setVariableData(std::map<RVariableType, REntityGroupVariableDisplayTypeMask> &variableData)
{
    this->variableData = variableData;
}

RVariableType REntityGroupData::findVariableByDisplayType(REntityGroupVariableDisplayTypeMask displayType) const
{
    std::map<RVariableType,REntityGroupVariableDisplayTypeMask>::const_iterator iter;
    for (iter = this->variableData.begin();
         iter != this->variableData.end();
         ++iter)
    {
        if (iter->second & displayType)
        {
            return iter->first;
        }
    }
    return R_VARIABLE_NONE;
}

const QString &REntityGroupData::getVariableDisplayName(REntityGroupVariableDisplayTypeMask type)
{
    switch (type)
    {
        case R_ENTITY_GROUP_VARIABLE_DISPLAY_SCALAR:
            return elementGroupVariableDisplayDesc[1].name;
        case R_ENTITY_GROUP_VARIABLE_DISPLAY_DISPLACEMENT:
            return elementGroupVariableDisplayDesc[2].name;
        default:
            return elementGroupVariableDisplayDesc[0].name;
    }
}

