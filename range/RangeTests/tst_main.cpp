#include <QTest>

#include "TestRangeBase/tst_rbl_rvector.h"
#include "TestRangeBase/tst_rbl_r3vector.h"
#include "TestRangeBase/tst_rbl_rmatrix.h"
#include "TestRangeModel/tst_rml_sparse_vector.h"
#include "TestRangeModel/tst_rml_sparse_matrix.h"

int main(int argc, char *argv[])
{
   int status = 0;

   {
       tst_RRVector tc;
       status |= QTest::qExec(&tc, argc, argv);
   }

   {
       tst_RR3Vector tc;
       status |= QTest::qExec(&tc, argc, argv);
   }

   {
       tst_RRMatrix tc;
       status |= QTest::qExec(&tc, argc, argv);
   }

   {
       tst_RSparseVector tc;
       status |= QTest::qExec(&tc, argc, argv);
   }

   {
       tst_RSparseMatrix tc;
       status |= QTest::qExec(&tc, argc, argv);
   }

   return status;
}
