/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   main_task.h                                              *
 *  GROUP:  RangeTestApp                                             *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   22-th July 2017                                          *
 *                                                                   *
 *  DESCRIPTION: Main task class declaration                         *
 *********************************************************************/

#ifndef __MAIN_TASK_H__
#define __MAIN_TASK_H__

#include <QObject>

class MainTask : public QObject
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit MainTask(QObject *parent = 0);

    private:

        //! Perform.
        void perform(void) const;
        
    protected slots:

        //! Run task.
        void run(void);
        
};

#endif // __MAIN_TASK_H__
