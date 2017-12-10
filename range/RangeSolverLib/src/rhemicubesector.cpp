/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rhemicubesector.cpp                                      *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   24-th July 2014                                          *
 *                                                                   *
 *  DESCRIPTION: View-factor hemi-cube sector class definition       *
 *********************************************************************/

#include "rhemicubesector.h"

void RHemiCubeSector::_init(const RHemiCubeSector *pHemiCubeSector)
{
    if (pHemiCubeSector)
    {
        this->limitPolygon = pHemiCubeSector->limitPolygon;
        this->resolution = pHemiCubeSector->resolution;
        this->eyePosition = pHemiCubeSector->eyePosition;
        this->eyeDirection = pHemiCubeSector->eyeDirection;
        this->pixels = pHemiCubeSector->pixels;
    }
}

RHemiCubeSector::RHemiCubeSector(const RR3Vector &eyePosition,
                                 const RR3Vector &eyeDirection,
                                 uint resolution, uint iSubIndex, uint jSubIndex, uint nSubIndexes,
                                 double size,
                                 RHemiCubeSectorType type)
    : resolution(resolution)
    , eyePosition(eyePosition)
    , eyeDirection(eyeDirection)
{
    this->_init();
    uint sectorSize = resolution * resolution;
    this->pixels.resize(sectorSize);
    this->generate(resolution,iSubIndex,jSubIndex,nSubIndexes,size,type);
    this->limitPolygon = this->findLimitPolygon();
}

RHemiCubeSector::RHemiCubeSector(const RHemiCubeSector &hemiCubeSector)
{
    this->_init(&hemiCubeSector);
}

RHemiCubeSector::~RHemiCubeSector()
{
}

RHemiCubeSector &RHemiCubeSector::operator =(const RHemiCubeSector &hemiCubeSector)
{
    this->_init(&hemiCubeSector);
    return (*this);
}

uint RHemiCubeSector::getSize(void) const
{
    return uint(this->pixels.size());
}

const RHemiCubePixel &RHemiCubeSector::getPixel(uint position) const
{
    return this->pixels[position];
}

RHemiCubePixel &RHemiCubeSector::getPixel(uint position)
{
    return this->pixels[position];
}

void RHemiCubeSector::rayTraceTriangle(const RTriangle &triangle, uint color)
{
    if (!this->testVisibility(triangle))
    {
        return;
    }

    double shortestDistance = RSegment::findLength(RNode(this->eyePosition),triangle.getNode1());
    shortestDistance = std::min(shortestDistance,RSegment::findLength(RNode(this->eyePosition),triangle.getNode2()));
    shortestDistance = std::min(shortestDistance,RSegment::findLength(RNode(this->eyePosition),triangle.getNode3()));

    RR3Vector rayDirection;
    RR3Vector x;
    double u;

    bool iPrevFound = false;
    for (uint i=0;i<this->resolution;i++)
    {
        bool jPrevFound = false;
        bool iCurrFound = false;

        for (uint j=0;j<this->resolution;j++)
        {
            uint pixelId = i * this->resolution + j;
            if (this->pixels[pixelId].getColor() != RConstants::eod && this->pixels[i].getDepth() < shortestDistance)
            {
                continue;
            }
            rayDirection[0] = this->pixels[pixelId].getPosition()[0] - this->eyePosition[0];
            rayDirection[1] = this->pixels[pixelId].getPosition()[1] - this->eyePosition[1];
            rayDirection[2] = this->pixels[pixelId].getPosition()[2] - this->eyePosition[2];

            bool jCurrFound = false;
            if (triangle.findLineIntersection(this->eyePosition,rayDirection,x,u))
            {
                if (u > 0.0)
                {
                    iPrevFound = true;
                    iCurrFound = true;
                    jPrevFound = true;
                    jCurrFound = true;
                    if (u < this->pixels[pixelId].getDepth() || this->pixels[pixelId].getColor() == RConstants::eod)
                    {
                        this->pixels[pixelId].setDepth(u);
                        this->pixels[pixelId].setColor(color);
                    }
                }
            }

            if (jPrevFound && !jCurrFound)
            {
                // No need to continue with current row. Triangle has already ended.
                break;
            }
        }

        if (iPrevFound && !iCurrFound)
        {
            // No need to continue with current sector. Triangle has already ended.
            break;
        }
    }
}

bool RHemiCubeSector::testVisibility(const RTriangle &triangle) const
{
    // Check if triangle's normal is opposite to eye direction.
    RR3Vector trianglePosition;
    RR3Vector triangleCenter;

    triangle.findCenter(triangleCenter);
    RR3Vector::subtract(triangleCenter,this->eyePosition,trianglePosition);
    trianglePosition.normalize();

    if (RR3Vector::angle(trianglePosition,this->eyeDirection) >= RConstants::pi/2.0 ||
        RR3Vector::angle(trianglePosition,triangle.getNormal()) <= RConstants::pi/2.0)
    {
        return false;
    }

    // Check if at least one node is visible.
    RR3Vector d1(triangle.getNode1().getX()-this->eyePosition[0],
                 triangle.getNode1().getY()-this->eyePosition[1],
                 triangle.getNode1().getZ()-this->eyePosition[2]);
    RR3Vector d2(triangle.getNode2().getX()-this->eyePosition[0],
                 triangle.getNode2().getY()-this->eyePosition[1],
                 triangle.getNode2().getZ()-this->eyePosition[2]);
    RR3Vector d3(triangle.getNode3().getX()-this->eyePosition[0],
                 triangle.getNode3().getY()-this->eyePosition[1],
                 triangle.getNode3().getZ()-this->eyePosition[2]);

    d1.normalize();
    d2.normalize();
    d3.normalize();

    if (RR3Vector::angle(d1,this->eyeDirection) > RConstants::pi/2.0 &&
        RR3Vector::angle(d2,this->eyeDirection) > RConstants::pi/2.0 &&
        RR3Vector::angle(d3,this->eyeDirection) > RConstants::pi/2.0)
    {
        return false;
    }

    // Project triangle to sector plane and check whether limit boxes of triangle and sector overlap.
    RPlane plane(this->limitPolygon[0],this->limitPolygon[1],this->limitPolygon[2]);
    double u;
    RR3Vector x;
    std::vector<RR3Vector> xNodes;
    xNodes.reserve(3);

    if (plane.findLineIntersection(this->eyePosition,d1,x,u))
    {
        if (u >= 0.0)
        {
            xNodes.push_back(x);
        }
    }
    if (plane.findLineIntersection(this->eyePosition,d2,x,u))
    {
        if (u >= 0.0)
        {
            xNodes.push_back(x);
        }
    }
    if (plane.findLineIntersection(this->eyePosition,d3,x,u))
    {
        if (u >= 0.0)
        {
            xNodes.push_back(x);
        }
    }

    if (xNodes.size() == 0)
    {
        return false;
    }

    double xl = 0.0, yl = 0.0, zl = 0.0;
    double xu = 0.0, yu = 0.0, zu = 0.0;

    for (uint i=0;i<xNodes.size();i++)
    {
        if (i == 0)
        {
            xl = xu = xNodes[i][0];
            yl = yu = xNodes[i][1];
            zl = zu = xNodes[i][2];
        }
        else
        {
            xl = std::min(xl,xNodes[i][0]);
            yl = std::min(yl,xNodes[i][1]);
            zl = std::min(zl,xNodes[i][2]);

            xu = std::max(xu,xNodes[i][0]);
            yu = std::max(yu,xNodes[i][1]);
            zu = std::max(zu,xNodes[i][2]);
        }
    }
    RLimitBox triangleLimitBox(xl,xu,yl,yu,zl,zu);
    triangleLimitBox.scale(1.0 + RConstants::eps);

    return RLimitBox::areIntersecting(this->findLimitBox(),triangleLimitBox);
}

void RHemiCubeSector::generate(uint resolution, uint iSubIndex, uint jSubIndex, uint nSubIndexes, double size, RHemiCubeSectorType type)
{
    double si,sj,sk;
    double x(0.0),y(0.0),z(0.0);

    RRMatrix R;
    this->eyeDirection.findRotationMatrix(R);

    RR3Vector v;

    uint totalResolution = resolution * nSubIndexes;

    for (uint i=0;i<resolution;i++)
    {
        for (uint j=0;j<resolution;j++)
        {
            si = double(i+iSubIndex*resolution) * size / double(totalResolution);
            sj = double(j+jSubIndex*resolution) * size / double(totalResolution);
            sk = size;

            if (type == R_HEMICUBE_SECTOR_FRONT_NORTH_EAST)
            {
                x = sk;
                y = si;
                z = sj;
            }
            else if (type == R_HEMICUBE_SECTOR_FRONT_NORTH_WEST)
            {
                x = sk;
                y = -si;
                z = sj;
            }
            else if (type == R_HEMICUBE_SECTOR_FRONT_SOUTH_EAST)
            {
                x = sk;
                y = si;
                z = -sj;
            }
            else if (type == R_HEMICUBE_SECTOR_FRONT_SOUTH_WEST)
            {
                x = sk;
                y = -si;
                z = -sj;
            }
            else if (type == R_HEMICUBE_SECTOR_EAST_NORTH)
            {
                x = si;
                y = sk;
                z = sj;
            }
            else if (type == R_HEMICUBE_SECTOR_EAST_SOUTH)
            {
                x = si;
                y = sk;
                z = -sj;
            }
            else if (type == R_HEMICUBE_SECTOR_WEST_NORTH)
            {
                x = si;
                y = -sk;
                z = sj;
            }
            else if (type == R_HEMICUBE_SECTOR_WEST_SOUTH)
            {
                x = si;
                y = -sk;
                z = -sj;
            }
            else if (type == R_HEMICUBE_SECTOR_NORTH_EAST)
            {
                x = si;
                y = sj;
                z = sk;
            }
            else if (type == R_HEMICUBE_SECTOR_NORTH_WEST)
            {
                x = si;
                y = -sj;
                z = sk;
            }
            else if (type == R_HEMICUBE_SECTOR_SOUTH_EAST)
            {
                x = si;
                y = sj;
                z = -sk;
            }
            else if (type == R_HEMICUBE_SECTOR_SOUTH_WEST)
            {
                x = si;
                y = -sj;
                z = -sk;
            }

            double weight = x / (RConstants::pi * std::pow(x*x+y*y+z*z,2));
            RRMatrix::mlt(R,RR3Vector(x,y,z),v);
            v[0] += this->eyePosition[0];
            v[1] += this->eyePosition[1];
            v[2] += this->eyePosition[2];
            this->pixels[i*resolution+j].setPosition(v);
            this->pixels[i*resolution+j].setWeight(weight);
        }
    }
}

std::vector<RR3Vector> RHemiCubeSector::findLimitPolygon(void) const
{
    std::vector<RR3Vector> polygon;

    polygon.push_back(this->pixels[0].getPosition());
    polygon.push_back(this->pixels[this->resolution-1].getPosition());
    polygon.push_back(this->pixels[this->resolution * this->resolution - 1].getPosition());
    polygon.push_back(this->pixels[this->resolution * (this->resolution - 1)].getPosition());

    return polygon;
}

RLimitBox RHemiCubeSector::findLimitBox(void) const
{
    double xl = 0.0, yl = 0.0, zl = 0.0;
    double xu = 0.0, yu = 0.0, zu = 0.0;

    for (uint i=0;i<this->limitPolygon.size();i++)
    {
        if (i == 0)
        {
            xl = xu = this->limitPolygon[i][0];
            yl = yu = this->limitPolygon[i][1];
            zl = zu = this->limitPolygon[i][2];
        }
        else
        {
            xl = std::min(xl,this->limitPolygon[i][0]);
            yl = std::min(yl,this->limitPolygon[i][1]);
            zl = std::min(zl,this->limitPolygon[i][2]);

            xu = std::max(xu,this->limitPolygon[i][0]);
            yu = std::max(yu,this->limitPolygon[i][1]);
            zu = std::max(zu,this->limitPolygon[i][2]);
        }
    }

    return RLimitBox(xl,xu,yl,yu,zl,zu);
}
