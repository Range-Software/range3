/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   main_task.h                                              *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th August 2013                                        *
 *                                                                   *
 *  DESCRIPTION: Main task class declaration                         *
 *********************************************************************/

#ifndef __MAIN_TASK_H__
#define __MAIN_TASK_H__

#include <QObject>
#include <QThread>

class MainTask : public QObject
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit MainTask(QObject *parent = 0);
        
    protected slots:

        //! Run task.
        void run(void);
        
};

#endif // __MAIN_TASK_H__
