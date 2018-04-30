/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_limit_box.cpp                                        *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   10-th March 2015                                         *
 *                                                                   *
 *  DESCRIPTION: Limit box class definition                          *
 *********************************************************************/

#include "rbl_limit_box.h"
#include "rbl_utils.h"
#include "rbl_logger.h"

void RLimitBox::_init(const RLimitBox *pBox)
{
    if (pBox)
    {
        this->xl = pBox->xl;
        this->xu = pBox->xu;
        this->yl = pBox->yl;
        this->yu = pBox->yu;
        this->zl = pBox->zl;
        this->zu = pBox->zu;
    }
    this->fixLimits();
}

RLimitBox::RLimitBox()
    : xl(0.0)
    , xu(0.0)
    , yl(0.0)
    , yu(0.0)
    , zl(0.0)
    , zu(0.0)
{
    this->_init();
}

RLimitBox::RLimitBox(double xl, double xu, double yl, double yu, double zl, double zu)
    : xl(xl)
    , xu(xu)
    , yl(yl)
    , yu(yu)
    , zl(zl)
    , zu(zu)
{
    this->_init();
}

RLimitBox::RLimitBox(const RLimitBox &box)
{
    this->_init(&box);
}

RLimitBox::~RLimitBox()
{

}

RLimitBox &RLimitBox::operator =(const RLimitBox &box)
{
    this->_init(&box);
    return (*this);
}

void RLimitBox::setLimits(double xl, double xu, double yl, double yu, double zl, double zu)
{
    this->xl = xl;
    this->xu = xu;
    this->yl = yl;
    this->yu = yu;
    this->zl = zl;
    this->zu = zu;
    this->fixLimits();
}

void RLimitBox::scale(double scaleFactor)
{
    double dx = (this->xu - this->xl) * (scaleFactor - 1.0) / 2.0;
    double dy = (this->yu - this->yl) * (scaleFactor - 1.0) / 2.0;
    double dz = (this->zu - this->zl) * (scaleFactor - 1.0) / 2.0;

    this->xl -= dx;
    this->xu += dx;

    this->yl -= dy;
    this->yu += dy;

    this->zl -= dz;
    this->zu += dz;
}

void RLimitBox::print(void) const
{
    RLogger::info("Limit box:\n");
    RLogger::info("  lower: %15.6e %15.6e %15.6e\n", this->xl, this->yl, this->zl);
    RLogger::info("  upper: %15.6e %15.6e %15.6e\n", this->xu, this->yu, this->zu);
}

bool RLimitBox::areIntersecting(const RLimitBox &b1, const RLimitBox &b2)
{
    if (b1.xl - b2.xu > RConstants::eps)
    {
        return false;
    }
    if (b2.xl - b1.xu > RConstants::eps)
    {
        return false;
    }

    if (b1.yl - b2.yu > RConstants::eps)
    {
        return false;
    }
    if (b2.yl - b1.yu > RConstants::eps)
    {
        return false;
    }

    if (b1.zl - b2.zu > RConstants::eps)
    {
        return false;
    }
    if (b2.zl - b1.zu > RConstants::eps)
    {
        return false;
    }

    return true;
}

void RLimitBox::fixLimits(void)
{
    if (this->xl > this->xu)
    {
        std::swap(this->xl,this->xu);
    }
    if (this->yl > this->yu)
    {
        std::swap(this->yl,this->yu);
    }
    if (this->zl > this->zu)
    {
        std::swap(this->zl,this->zu);
    }
}
