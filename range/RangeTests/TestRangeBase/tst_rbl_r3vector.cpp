#include <rblib.h>

#include "tst_rbl_r3vector.h"

void tst_RR3Vector::getOpposite()
{
    RR3Vector r(1.0,1.0,1.0);
    QVERIFY(r.getOpposite() == RR3Vector(-1.0,-1.0,-1.0));
}

void tst_RR3Vector::findDistance()
{
    QVERIFY(RR3Vector::findDistance(RR3Vector(2.0,0.0,0.0),RR3Vector(2.0,0.0,0.0)) == 0.0);
    QVERIFY(RR3Vector::findDistance(RR3Vector(2.0,0.0,0.0),RR3Vector(-2.0,0.0,0.0)) == 4.0);
    QVERIFY(RR3Vector::findDistance(RR3Vector(0.0,2.0,0.0),RR3Vector(0.0,-2.0,0.0)) == 4.0);
    QVERIFY(RR3Vector::findDistance(RR3Vector(0.0,0.0,2.0),RR3Vector(0.0,0.0,-2.0)) == 4.0);
}

void tst_RR3Vector::findOrthogonal()
{
    RR3Vector r(1.0,1.0,1.0);
    QVERIFY(RRVector::dot(r,r.findOrthogonal()) == 0.0);
}

void tst_RR3Vector::cross()
{
    RR3Vector v1(1.0,0.0,0.0);
    RR3Vector v2(0.0,1.0,0.0);
    RR3Vector vx;
    RR3Vector::cross(v1,v2,vx);
    QVERIFY(vx == RR3Vector(0.0,0.0,1.0));
}

void tst_RR3Vector::angle()
{
    RR3Vector v1(1.0,0.0,0.0);
    RR3Vector v2(0.0,1.0,0.0);
    QVERIFY(R_D_ARE_SAME(RR3Vector::angle(v1,v2),RConstants::pi/2.0));
}
