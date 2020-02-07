#ifndef TST_RMATRIX_H
#define TST_RMATRIX_H

#include <QtTest>

class tst_RRMatrix : public QObject
{
    Q_OBJECT

    private slots:
        void transpose() const;
        void invert() const;
        void decomposeToLU() const;
        void getDeterminant() const;
        void getSummedRows() const;
        void getBlock() const;
        void setBlock() const;
        void trace() const;
        void mlt() const;

};

#endif // TST_RMATRIX_H
