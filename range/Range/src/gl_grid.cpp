/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_grid.cpp                                              *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   27-th September 2020                                     *
 *                                                                   *
 *  DESCRIPTION: OpenGL grid class definition                        *
 *********************************************************************/

#include "gl_grid.h"
#include "gl_line.h"
#include "gl_node.h"

void GLGrid::_init(const GLGrid *pGlGrid)
{
    if (pGlGrid)
    {
        this->scale = pGlGrid->scale;
        this->xMin = pGlGrid->xMin;
        this->xMax = pGlGrid->xMax;
        this->yMin = pGlGrid->yMin;
        this->yMax = pGlGrid->yMax;
        this->zMin = pGlGrid->zMin;
        this->zMax = pGlGrid->zMax;
    }

    double txmin = std::min(0.0,this->xMin);
    double txmax = std::max(0.0,this->xMax);
    double tymin = std::min(0.0,this->yMin);
    double tymax = std::max(0.0,this->yMax);
    double tzmin = std::min(0.0,this->zMin);
    double tzmax = std::max(0.0,this->zMax);

    double tmin = std::min(txmin,std::min(tymin,tzmin));
    double tmax = std::max(txmax,std::max(tymax,tzmax));

    double d = std::max(std::abs(tmin),std::abs(tmax));

    int e = RUtil::findExponent(d) + 1;

    this->gdt = (std::ceil(d / std::pow(10,e)) * std::pow(10,e)) / 10.0;

    this->gMin = 0.0;
    this->gMax = 0.0;

    if (tmin > 0)
    {
        while (this->gMin + this->gdt <= tmin)
        {
            this->gMin += this->gdt;
        }
    }
    else
    {
        while (this->gMin >= tmin)
        {
            this->gMin -= this->gdt;
        }
    }

    if (tmax < 0)
    {
        while (this->gMax - this->gdt >= tmax)
        {
            this->gMax -= this->gdt;
        }
    }
    else
    {
        while (this->gMax <= tmax)
        {
            this->gMax += this->gdt;
        }
    }
}

GLGrid::GLGrid(GLWidget *glWidget,
               double scale,
               double xMin,
               double xMax,
               double yMin,
               double yMax,
               double zMin,
               double zMax)
    : GLObject(glWidget)
    , scale(scale)
    , xMin(xMin)
    , xMax(xMax)
    , yMin(yMin)
    , yMax(yMax)
    , zMin(zMin)
    , zMax(zMax)
{
    this->_init();
}

GLGrid::GLGrid(const GLGrid &glGrid)
    : GLObject(glGrid)
{
    this->_init(&glGrid);
}

GLGrid &GLGrid::operator=(const GLGrid &glGrid)
{
    this->_init(&glGrid);
    return (*this);
}

void GLGrid::initialize()
{

}

void GLGrid::finalize()
{

}

void GLGrid::draw()
{
    int e = RUtil::findExponent(this->scale);
    if (std::ceil(this->scale / std::pow(10,e)) > 2)
    {
        e++;
    }
    double sdt = this->gdt * std::pow(10,e);
    double dt = 0.0;
    uint n = 0;
    GLboolean stipple = false;
    while (this->gMin + dt < this->gMax || std::abs(this->gMin + dt - this->gMax) < RConstants::eps)
    {
        if (n % 10 != 0 && n*sdt + this->gMin < this->gMax)
        {
            GL_SAFE_CALL(glGetBooleanv(GL_LINE_STIPPLE,&stipple));

            GL_SAFE_CALL(glPushAttrib(GL_ENABLE_BIT));

            GL_SAFE_CALL(glLineStipple(6, 0xAAAA));
            GL_SAFE_CALL(glEnable(GL_LINE_STIPPLE));
        }

        // X - Y
        GLLine(this->getGLWidget(),RR3Vector(this->gMin,this->gMin+dt,0.0),RR3Vector(this->gMax,this->gMin+dt,0.0),1.0).paint();
        GLLine(this->getGLWidget(),RR3Vector(this->gMin+dt,this->gMin,0.0),RR3Vector(this->gMin+dt,this->gMax,0.0),1.0).paint();
        // X - Z
        GLLine(this->getGLWidget(),RR3Vector(this->gMin,0.0,this->gMin+dt),RR3Vector(this->gMax,0.0,this->gMin+dt),1.0).paint();
        GLLine(this->getGLWidget(),RR3Vector(this->gMin+dt,0.0,this->gMin),RR3Vector(this->gMin+dt,0.0,this->gMax),1.0).paint();
        // Y - Z
        GLLine(this->getGLWidget(),RR3Vector(0.0,this->gMin,this->gMin+dt),RR3Vector(0.0,this->gMax,this->gMin+dt),1.0).paint();
        GLLine(this->getGLWidget(),RR3Vector(0.0,this->gMin+dt,this->gMin),RR3Vector(0.0,this->gMin+dt,this->gMax),1.0).paint();

        if (n % 10 != 0 && n*sdt + this->gMin < this->gMax)
        {
            GL_SAFE_CALL(glPopAttrib());

            if (!stipple)
            {
                GL_SAFE_CALL(glDisable(GL_LINE_STIPPLE));
            }
        }

        GLNode node(this->getGLWidget());

        node.set(this->gMin+dt,0.0,0.0);
        node.paint();

        node.set(0.0,this->gMin+dt,0.0);
        node.paint();

        node.set(0.0,0.0,this->gMin+dt);
        node.paint();

        this->getGLWidget()->renderText(this->gMin+dt,0.0,0.0,QString::number(this->gMin+dt),QFont("Courier",20));
        this->getGLWidget()->renderText(0.0,this->gMin+dt,0.0,QString::number(this->gMin+dt),QFont("Courier",20));
        this->getGLWidget()->renderText(0.0,0.0,this->gMin+dt,QString::number(this->gMin+dt),QFont("Courier",20));

        dt += sdt;
        n++;
    }
}
