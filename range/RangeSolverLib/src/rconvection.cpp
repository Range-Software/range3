/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rconvection.cpp                                          *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   21-st January 2013                                       *
 *                                                                   *
 *  DESCRIPTION: Convection coefficient calculator class definition  *
 *********************************************************************/

#include <cmath>

#include "rconvection.h"

typedef struct _RConvectionDesc
{
    QString description;
    bool    natural;
    bool    available;
} RConvectionDesc;

static RConvectionDesc convectionDesc [] =
{
    { "Natural convection, External flow, Vertical plane",      true,  true  },
    { "Natural convection, External flow, Vertical cylinders",  true,  false },
    { "Natural convection, External flow, Horizontal plates",   true,  true  },
    { "Natural convection, External flow, Horizontal cylinder", true,  false },
    { "Natural convection, External flow, Spheres",             true,  false },
    { "Forced convection, Internal flow, Laminar flow",         false, true  },
    { "Forced convection, Internal flow, Turbulent flow",       false, true  },
    { "Forced convection, External flow",                       false, true  }
};

void RConvection::_init(const RConvection *pConvection)
{
    this->type = R_CONVECTION_FORCED_EXTERNAL;
    this->matName.clear();
    this->mu = 0.0;
    this->ro = 0.0;
    this->k = 0.0;
    this->c = 0.0;
    this->b = 0.0;
    this->d = 0.0;
    this->v = 0.0;
    this->g = 9.81;
    this->Ts = 293.15;
    this->Tf = 293.15;

    if (pConvection)
    {
        this->type = pConvection->type;
        this->matName = pConvection->matName;
        this->mu = pConvection->mu;
        this->ro = pConvection->ro;
        this->k = pConvection->k;
        this->c = pConvection->c;
        this->b = pConvection->b;
        this->d = pConvection->d;
        this->v = pConvection->v;
        this->g = pConvection->g;
        this->Ts = pConvection->Ts;
        this->Tf = pConvection->Tf;
    }
}

RConvection::RConvection()
{
    this->_init();
}

RConvection::RConvection(const RConvection &convection)
{
    this->_init(&convection);
}

RConvection::~RConvection()
{
}

RConvection &RConvection::operator =(const RConvection &convection)
{
    this->_init(&convection);
    return *this;
}

void RConvection::setType(RConvectionType type)
{
    this->type = type;
}

void RConvection::setMaterial(const RMaterial &material, double temperature)
{
    this->matName = material.getName();

    unsigned int roPos = material.findPosition(R_MATERIAL_PROPERTY_DENSITY);
    unsigned int muPos = material.findPosition(R_MATERIAL_PROPERTY_DYNAMIC_VISCOSITY);
    unsigned int cPos = material.findPosition(R_MATERIAL_PROPERTY_HEAT_CAPACITY);
    unsigned int kPos = material.findPosition(R_MATERIAL_PROPERTY_THERMAL_CONDUCTIVITY);
    unsigned int bPos = material.findPosition(R_MATERIAL_PROPERTY_THERMAL_EXPANSION_COEFFICIENT);

    if (roPos < material.size())
    {
        this->ro = material.get(roPos).get(temperature);
    }

    if (muPos < material.size())
    {
        this->mu = material.get(muPos).get(temperature);
    }

    if (cPos < material.size())
    {
        this->c = material.get(cPos).get(temperature);
    }

    if (kPos < material.size())
    {
        this->k = material.get(kPos).get(temperature);
    }

    if (bPos < material.size())
    {
        this->b = material.get(bPos).get(temperature);
    }
}

void RConvection::setMaterial(const QString &matName, double mu, double ro, double k, double c, double b)
{
    this->matName = matName;
    this->mu = mu;
    this->ro = ro;
    this->k = k;
    this->c = c;
    this->b = b;
}

void RConvection::setDiameter(double d)
{
    this->d = d;
}

void RConvection::setVelocity(double v)
{
    this->v = v;
}

void RConvection::setGravity(double g)
{
    this->g = g;
}

void RConvection::setSurfTemp(double Ts)
{
    this->Ts = Ts;
}

void RConvection::setFluidTemp(double Tf)
{
    this->Tf = Tf;
}

double RConvection::calculateGr(void) const
{
    if (this->mu == 0.0)
    {
        return 0.0;
    }
    return std::abs(pow(this->ro,2.0) * this->g * this->b * (this->Ts - this->Tf) * pow(this->d,3.0) / pow(this->mu,2.0));
}

double RConvection::calculateRa(void) const
{
    return this->calculateGr() * this->calculatePr();
}

double RConvection::calculateRe(void) const
{
    if (this->mu == 0.0)
    {
        return 0.0;
    }
    return this->ro * this->v * this->d / this->mu;
}

double RConvection::calculatePr(void) const
{
    if (this->k == 0.0)
    {
        return 0.0;
    }
    return this->c * this->mu / this->k;
}

double RConvection::calculateNu(void) const
{
    double Nu = 0.0;
    double Ra = 0.0;
    double Re = 0.0;
    double Pr = 0.0;

    if (RConvection::isNatural(this->type))
    {
        Ra = this->calculateRa();
        if (Ra <= 0.0)
        {
            return 1.0;
        }
    }
    if (RConvection::isForced(this->type))
    {
        Re = this->calculateRe();
    }
    Pr = this->calculatePr();

    if (Pr == 0.0)
    {
        return 0.0;
    }

    if (RConvection::isForced(this->type))
    {
        if (Re == 0.0)
        {
            return 0.0;
        }
    }

    switch (this->type)
    {
        case R_CONVECTION_NATURAL_EXTERNAL_VERTICAL_PLANE:
        case R_CONVECTION_NATURAL_EXTERNAL_VERTICAL_CYLINDER:
            if (Ra <= 1.0e-9)
            {
                Nu = 0.68 + (0.67*pow(Ra,1.0/4.0))/pow(1.0 + pow(0.492/Pr, 9.0/16.0),4.0/9.0);
            }
            else
            {
                Nu = pow (0.825 + (0.387*pow(Ra,1.0/6.0))/pow(1.0 + pow(0.492/Pr, 9.0/16.0),8.0/27.0), 2.0);
            }
            break;
        case R_CONVECTION_NATURAL_EXTERNAL_HORIZONTAL_PLATES:
            if (Ra < 1.0e5)
            {
                /* Not really correct */
                Nu = 0.54*pow(Ra,1.0/4.0);
            }
            else if (1.0e5 <= Ra && Ra < 2.0e7)
            {
                Nu = 0.54*pow(Ra,1.0/4.0);
            }
            else if (2.0e7 <= Ra && Ra < 3.0e10)
            {
                Nu = 0.14*pow(Ra,1.0/3.0);
            }
            else if (3.0e5 <= Ra && Ra < 1.0e10)
            {
                Nu = 0.27*pow(Ra,1.0/3.0);
            }
            else
            {
                /* Not really correct */
                Nu = 0.27*pow(Ra,1.0/3.0);
            }
            break;
        case R_CONVECTION_NATURAL_EXTERNAL_HORIZONTAL_CYLINDER:
            Nu = pow (0.6 + (0.387*pow(Ra,1.0/6.0))/pow(1.0 + pow(0.559/Pr, 9.0/16.0),8.0/27.0), 2.0);
            break;
        case R_CONVECTION_NATURAL_EXTERNAL_SPHERES:
            Nu = 2.0 + 0.43*pow(Ra,1.0/4.0);
            break;
        case R_CONVECTION_FORCED_INTERNAL_LAMINAR:
            Nu = 1.86 / pow(Re*Pr,2.0/3.0);
            break;
        case R_CONVECTION_FORCED_INTERNAL_TURBULENT:
            Nu = 0.023 * pow(Re,0.8) * pow(Pr,0.4);
            break;
        case R_CONVECTION_FORCED_EXTERNAL:
            Nu = 0.036 * pow(Re,4.0/5.0) * pow(Pr,1.0/3.0);
            break;
        default:
            Nu = 0.0;
            break;
    }

    return Nu;
}

double RConvection::calculateHtc(void) const
{
    if (this->d == 0.0)
    {
        return 0.0;
    }
    return this->calculateNu() * this->k / this->d;
}

const QString &RConvection::getName(RConvectionType type)
{
    return convectionDesc[type].description;
}

bool RConvection::isNatural(RConvectionType type)
{
    return convectionDesc[type].natural;
}

bool RConvection::isForced(RConvectionType type)
{
    return !convectionDesc[type].natural;
}
