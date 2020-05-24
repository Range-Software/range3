#include <rblib.h>

#include "tst_rbl_rvector.h"

void tst_RRVector::dot()
{
    RRVector r1(5);
    r1[0] =  1.0;
    r1[1] = -2.0;
    r1[2] =  3.0;
    r1[3] = -4.0;
    r1[4] =  5.0;
    RRVector r2(5);
    r2[0] = -1.0;
    r2[1] =  2.0;
    r2[2] = -3.0;
    r2[3] =  4.0;
    r2[4] = -5.0;
    QVERIFY(R_D_ARE_SAME(RRVector::dot(r1,r2),-55.0));
}

void tst_RRVector::norm()
{
    RRVector r(10);
    r[0] =  1.0;
    r[1] = -2.0;
    r[2] =  3.0;
    r[3] = -4.0;
    r[4] =  5.0;
    r[5] = -6.0;
    r[6] =  7.0;
    r[7] = -8.0;
    r[8] =  9.0;
    r[9] = -10.0;
    QVERIFY(R_D_ARE_SAME(RRVector::norm(r),std::sqrt(385.0)));
}
