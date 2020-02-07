#ifndef TST_RRVECTOR_H
#define TST_RRVECTOR_H

#include <QtTest>

class tst_RRVector : public QObject
{
    Q_OBJECT

    private slots:
        void dot();
        void norm();
};

#endif // TST_RRVECTOR_H
