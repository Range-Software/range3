#include <rmlib.h>

#include "tst_rml_sparse_vector.h"

void tst_RSparseVector::addValue() const
{
    RSparseVector<double> v;
    v.addValue(3,-3.0);
    v.addValue(2,-2.0);

    QVERIFY(v.getIndex(0) == 2);
    QVERIFY(v.getIndex(1) == 3);

    std::vector<double> values = v.getValues(2);

    QVERIFY(R_D_ARE_SAME(values[2],-2.0));
    QVERIFY(R_D_ARE_SAME(values[3],-3.0));
}

void tst_RSparseVector::addVector() const
{
    RSparseVector<double> v1;
    v1.addValue(3,-3.0);
    v1.addValue(2,-2.0);

    RSparseVector<double> v2;
    v2.addValue(1,1.0);
    v2.addValue(2,3.0);

    v1.addVector(v2);

    std::vector<double> values = v1.getValues(3);

    QVERIFY(R_D_ARE_SAME(values[1],1.0));
    QVERIFY(R_D_ARE_SAME(values[2],1.0));
    QVERIFY(R_D_ARE_SAME(values[3],-3.0));
}
