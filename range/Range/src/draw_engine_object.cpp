/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine_object.cpp                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   1-st October 2014                                        *
 *                                                                   *
 *  DESCRIPTION: Draw engine object class definition                 *
 *********************************************************************/

#include "color.h"
#include "draw_engine_object.h"
#include "model.h"

DrawEngineObject::DrawEngineObject(QObject *parent) :
    QObject(parent),
    color(QColor(255,255,255,255))
{
    this->color = Color::random(true);
    this->color.setAlpha(100);
}

const QString &DrawEngineObject::getName() const
{
    return this->name;
}

uint DrawEngineObject::getNInputParameters() const
{
    return uint(this->inputParameters.size());
}

const DrawEngineInput &DrawEngineObject::getInputParamater(uint position) const
{
    return this->inputParameters[int(position)];
}

DrawEngineInput &DrawEngineObject::getInputParamater(uint position)
{
    return this->inputParameters[int(position)];
}

void DrawEngineObject::updateModel()
{
    this->model = Model(this->generate(),this->getName(),tr("Model to be drawn"),false);

    const bool onlyElements = true;

    for (uint i=0;i<this->model.getNEntityGroups(onlyElements);i++)
    {
        REntityGroupType entityGroupType = this->model.getEntityGroupType(i,onlyElements);
        uint entityGroupPosition = this->model.getEntityGroupPosition(i,onlyElements);
        this->model.setVisible(entityGroupType,entityGroupPosition,true);
        this->model.setColor(entityGroupType,entityGroupPosition,this->color);
        this->model.setDrawEdges(entityGroupType,entityGroupPosition,true);
    }
}

Model &DrawEngineObject::getModel()
{
    return this->model;
}

const Model &DrawEngineObject::getModel() const
{
    return this->model;
}

void DrawEngineObject::glDraw(GLWidget *glWidget) const
{
    bool useGlModelList = glWidget->getUseGLVoidModelList();
    glWidget->setUseGLVoidModelList(true);

    glWidget->getGLModelList().clear();
    this->model.glDraw(glWidget);
    glWidget->getGLModelList().clear();

    glWidget->setUseGLVoidModelList(useGlModelList);
}
