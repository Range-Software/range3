#ifndef TST_RSPARSEVECTOR_H
#define TST_RSPARSEVECTOR_H

#include <QtTest>

class tst_RSparseVector : public QObject
{

    Q_OBJECT

    private slots:
        void addValue() const;
        void addVector() const;

};

#endif // TST_RSPARSEVECTOR_H
