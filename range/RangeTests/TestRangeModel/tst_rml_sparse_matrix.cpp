#include <rmlib.h>

#include "tst_rml_sparse_matrix.h"

void tst_RSparseMatrix::addMatrix() const
{
    RSparseMatrix A1;

    A1.addValue(0,0,1.0);
    A1.addValue(1,1,1.0);

    RSparseMatrix A2;
    A2.addValue(0,1,-1.0);
    A2.addValue(1,0,-1.0);

    RSparseMatrix A3;
    A3.addValue(0,1,3.0);
    A3.addValue(1,1,3.0);

    A2.addMatrix(A3);
    A1.addMatrix(A2);

    QVERIFY(R_D_ARE_SAME(A1.findValue(0,0),1.0));
    QVERIFY(R_D_ARE_SAME(A1.findValue(0,1),2.0));
    QVERIFY(R_D_ARE_SAME(A1.findValue(1,0),-1.0));
    QVERIFY(R_D_ARE_SAME(A1.findValue(1,1),4.0));
}
