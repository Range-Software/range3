/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine_tetrahedron.cpp                              *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   17-th Jun 2015                                           *
 *                                                                   *
 *  DESCRIPTION: Draw engine tetrahedron class definition            *
 *********************************************************************/

#include "draw_engine_tetrahedron.h"

DrawEngineTetrahedron::DrawEngineTetrahedron(QObject *parent)
    : DrawEngineObject(parent)
{
    this->name = tr("tetrahedron");

    this->inputParameters.append(DrawEngineInput(RR3Vector(0.0,0.0,0.0),
                                                 tr("1-st node"),
                                                 tr("Position of 1-st node in X,Y,Z coordinate system."),
                                                 "m"));
    this->inputParameters.append(DrawEngineInput(RR3Vector(1.0,0.0,0.0),
                                                 tr("2-nd node"),
                                                 tr("Position of 2-nd node in X,Y,Z coordinate system."),
                                                 "m"));
    this->inputParameters.append(DrawEngineInput(RR3Vector(0.0,1.0,0.0),
                                                 tr("3-rd node"),
                                                 tr("Position of 3-rd node in X,Y,Z coordinate system."),
                                                 "m"));
    this->inputParameters.append(DrawEngineInput(RR3Vector(0.0,0.0,1.0),
                                                 tr("4-th node"),
                                                 tr("Position of 4-th node in X,Y,Z coordinate system."),
                                                 "m"));

    this->updateModel();
}

RModelRaw DrawEngineTetrahedron::generate() const
{
    RModelRaw modelRaw;

    RNode node1(this->inputParameters.at(0).toVector());
    RNode node2(this->inputParameters.at(1).toVector());
    RNode node3(this->inputParameters.at(2).toVector());
    RNode node4(this->inputParameters.at(3).toVector());

    modelRaw.addTriangle(node1,node3,node2);
    modelRaw.addTriangle(node1,node2,node4);
    modelRaw.addTriangle(node3,node1,node4);
    modelRaw.addTriangle(node2,node3,node4);

    return modelRaw;
}

