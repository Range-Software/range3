/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_shape_generator.cpp                                  *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   17-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Shape generator class definition                    *
 *********************************************************************/

#include "rml_shape_generator.h"
#include "rml_segment.h"

void RShapeGenerator::_init(const RShapeGenerator *pShapeGenerator)
{
    if (pShapeGenerator)
    {

    }
}

RShapeGenerator::RShapeGenerator()
{
    this->_init();
}

RShapeGenerator::RShapeGenerator(const RShapeGenerator &shapeGenerator)
{
    this->_init(&shapeGenerator);
}

RShapeGenerator::~RShapeGenerator()
{
}

RShapeGenerator &RShapeGenerator::operator =(const RShapeGenerator &shapeGenerator)
{
    this->_init(&shapeGenerator);
    return (*this);
}

RModelRaw RShapeGenerator::generateArrow(const RR3Vector &v1, const RR3Vector &v2, double scale)
{
    RModelRaw arrow;

    RNode r1(  0.0,                0.0,          0.0         );
    RNode r2(  1.0/scale,          0.0,          0.0         );
    RNode r3(  1.0/scale-1.0/2.0,  1.0/6.0,      0.0         );
    RNode r4(  1.0/scale-1.0/2.0,  0.70711/6.0,  0.70711/6.0 );
    RNode r5(  1.0/scale-1.0/2.0,  0.0,          1.0/6.0     );
    RNode r6(  1.0/scale-1.0/2.0, -0.70711/6.0,  0.70711/6.0 );
    RNode r7(  1.0/scale-1.0/2.0, -1.0/6.0,      0.0         );
    RNode r8(  1.0/scale-1.0/2.0, -0.70711/6.0, -0.70711/6.0 );
    RNode r9(  1.0/scale-1.0/2.0,  0.0,         -1.0/6.0     );
    RNode r10( 1.0/scale-1.0/2.0,  0.70711/6.0, -0.70711/6.0 );
//    RNode r11( 1.0/2.0, 0.0,         0.0         );

    r1.scale(scale);
    r2.scale(scale);
    r3.scale(scale);
    r4.scale(scale);
    r5.scale(scale);
    r6.scale(scale);
    r7.scale(scale);
    r8.scale(scale);
    r9.scale(scale);
    r10.scale(scale);

    RSegment segment(RNode(v1[0],v1[1],v1[2]),RNode(v2[0],v2[1],v2[2]));

    RRMatrix R;
    segment.findRotationMatrix(R);

    R *= segment.findLength();

    r1.transform(R,v1);
    r2.transform(R,v1);
    r3.transform(R,v1);
    r4.transform(R,v1);
    r5.transform(R,v1);
    r6.transform(R,v1);
    r7.transform(R,v1);
    r8.transform(R,v1);
    r9.transform(R,v1);
    r10.transform(R,v1);
//    r11.transform(R,v1);

    arrow.addSegment(r1,r2);
    arrow.addTriangle(r2,r4,r3);
    arrow.addTriangle(r2,r5,r4);
    arrow.addTriangle(r2,r6,r5);
    arrow.addTriangle(r2,r7,r6);
    arrow.addTriangle(r2,r8,r7);
    arrow.addTriangle(r2,r9,r8);
    arrow.addTriangle(r2,r10,r9);
    arrow.addTriangle(r2,r3,r10);
//    arrow.addTriangle(r11,r3,r4);
//    arrow.addTriangle(r11,r4,r5);
//    arrow.addTriangle(r11,r5,r6);
//    arrow.addTriangle(r11,r6,r7);
//    arrow.addTriangle(r11,r7,r8);
//    arrow.addTriangle(r11,r8,r9);
//    arrow.addTriangle(r11,r9,r10);
//    arrow.addTriangle(r11,r10,r3);

    return arrow;
}
