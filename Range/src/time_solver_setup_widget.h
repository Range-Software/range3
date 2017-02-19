/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   time_solver_setup_widget.h                               *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   15-th June 2012                                          *
 *                                                                   *
 *  DESCRIPTION: Time-solver setup widget class declaration          *
 *********************************************************************/

#ifndef __TIME_SOLVER_SETUP_WIDGET_H__
#define __TIME_SOLVER_SETUP_WIDGET_H__

#include <QWidget>

#include <rmlib.h>

class TimeSolverSetupWidget : public QWidget
{

    Q_OBJECT

    protected:

        //! Time solver.
        RTimeSolver timeSolver;

    public:

        //! Constructor.
        explicit TimeSolverSetupWidget(const RTimeSolver &timeSolver, QWidget *parent = 0);

    signals:

        //! Time solver has changed.
        void changed(const RTimeSolver &timeSolver);

    private slots:

        void onTimeSolverEnabledChanged(bool checked);

        void onTimeApproximationChanged(int index);

        void onStartTimeChanged(double startTime);

        void onTimeStepSizeChanged(double timeStepSize);

        void onNTimeStepsChanged(int nTimeSteps);

        void onOutputFrequencyChanged(int outputFrequency);
        
};

#endif /* __TIME_SOLVER_SETUP_WIDGET_H__ */
