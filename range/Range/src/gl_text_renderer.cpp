/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_text_renderer.cpp                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   14-th March 2016                                         *
 *                                                                   *
 *  DESCRIPTION: OpenGL text renderer class definition               *
 *********************************************************************/

#include "gl_text_renderer.h"

void GLTextRenderer::_init(const GLTextRenderer *pGlTextRenderer)
{
    if (pGlTextRenderer)
    {
        this->list = pGlTextRenderer->list;
    }
}

GLTextRenderer::GLTextRenderer()
{
    this->_init();
}

GLTextRenderer::GLTextRenderer(const GLTextRenderer &glTextRenderer)
{
    this->_init(&glTextRenderer);
}

GLTextRenderer::~GLTextRenderer()
{

}

GLTextRenderer &GLTextRenderer::operator =(const GLTextRenderer &glTextRenderer)
{
    this->_init(&glTextRenderer);
    return (*this);
}

void GLTextRenderer::add(const GLTextRendererItem &item)
{
    this->list.append(item);
}

void GLTextRenderer::clear(void)
{
    this->list.clear();
}

void GLTextRenderer::render(QPainter &painter) const
{
    foreach (GLTextRendererItem item, this->list)
    {
        painter.setPen(item.color);
        painter.setFont(item.font);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        painter.drawText(item.position,item.text);
    }
}
