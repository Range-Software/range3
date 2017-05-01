/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   main_task.h                                              *
 *  GROUP:  RangeLicense                                             *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   8-th September 2016                                      *
 *                                                                   *
 *  DESCRIPTION: Main task class declaration                         *
 *********************************************************************/

#ifndef __MAIN_TASK_H__
#define __MAIN_TASK_H__

#include <QObject>
#include <QString>
#include <QDate>

class MainTask : public QObject
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit MainTask(QObject *parent = 0);

    private:

        //! Generate license.
        QString generateLicense(const QString &account,const QString &password,const QString &name,const QDate &date);

    protected slots:

        //! Run task.
        void run(void);

};

#endif // __MAIN_TASK_H__
