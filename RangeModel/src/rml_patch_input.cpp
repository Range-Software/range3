/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_patch_input.cpp                                      *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   4-th August 2014                                         *
 *                                                                   *
 *  DESCRIPTION: Patch input class definition                        *
 *********************************************************************/

#include <rblib.h>

#include "rml_patch_input.h"

void RPatchInput::_init(const RPatchInput *pPatchInput)
{
    if (pPatchInput)
    {
        this->patchArea = pPatchInput->patchArea;
        this->patchSize = pPatchInput->patchSize;
        this->separationAngle = pPatchInput->separationAngle;
        this->emitter = pPatchInput->emitter;
        this->receiver = pPatchInput->receiver;
    }
}

RPatchInput::RPatchInput()
    : patchArea(0.0)
    , patchSize(0)
    , separationAngle(RConstants::pi/4.0)
    , emitter(false)
    , receiver(false)
{
    this->_init();
}

RPatchInput::RPatchInput(const RPatchInput &patchInput)
{
    this->_init(&patchInput);
}

RPatchInput::~RPatchInput()
{

}

RPatchInput &RPatchInput::operator =(const RPatchInput &patchInput)
{
    this->_init(&patchInput);
    return (*this);
}

bool RPatchInput::operator ==(const RPatchInput &patchInput) const
{
    if (this->patchArea != patchInput.patchArea)
    {
        return false;
    }
    if (this->patchSize != patchInput.patchSize)
    {
        return false;
    }
    if (this->separationAngle != patchInput.separationAngle)
    {
        return false;
    }
    if (this->emitter != patchInput.emitter)
    {
        return false;
    }
    if (this->receiver != patchInput.receiver)
    {
        return false;
    }
    return true;
}

bool RPatchInput::operator !=(const RPatchInput &patchInput) const
{
    return !this->operator ==(patchInput);
}

double RPatchInput::getPatchArea(void) const
{
    return this->patchArea;
}

void RPatchInput::setPatchArea(double patchArea)
{
    this->patchArea = patchArea;
}

unsigned int RPatchInput::getPatchSize(void) const
{
    return this->patchSize;
}

void RPatchInput::setPatchSize(unsigned int patchSize)
{
    this->patchSize = patchSize;
}

double RPatchInput::getSeparationAngle(void) const
{
    return this->separationAngle;
}

void RPatchInput::setSeparationAngle(double separationAngle)
{
    this->separationAngle = separationAngle;
}

bool RPatchInput::getEmitter(void) const
{
    return this->emitter;
}

void RPatchInput::setEmitter(bool emitter)
{
    this->emitter = emitter;
}

bool RPatchInput::getReceiver(void) const
{
    return this->receiver;
}

void RPatchInput::setReceiver(bool receiver)
{
    this->receiver = receiver;
}
