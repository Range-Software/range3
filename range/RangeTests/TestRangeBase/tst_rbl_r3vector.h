#ifndef TST_RR3VECTOR_H
#define TST_RR3VECTOR_H

#include <QtTest>

class tst_RR3Vector : public QObject
{
    Q_OBJECT

    private slots:
        void getOpposite();
        void findDistance();
        void findOrthogonal();
        void cross();
        void angle();

};

#endif // TST_RR3VECTOR_H
