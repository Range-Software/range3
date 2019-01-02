/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   main_task.h                                              *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th August 2013                                        *
 *                                                                   *
 *  DESCRIPTION: Main task class declaration                         *
 *********************************************************************/

#ifndef MAIN_TASK_H
#define MAIN_TASK_H

#include <QObject>
#include <QThread>

class MainTask : public QObject
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit MainTask(QObject *parent = nullptr);
        
    protected slots:

        //! Run task.
        void run(void);
        
};

#endif // MAIN_TASK_H
