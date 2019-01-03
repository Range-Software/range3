/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   progress_bar.h                                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   23-1h November 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Progress bar class declaration                      *
 *********************************************************************/

#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include <QProgressBar>
#include <QTimer>

class ProgressBar : public QProgressBar
{

    Q_OBJECT

    protected:

        //! Pulse timer.
        QTimer *pulseTimer;
        //! Custom message.
        QString message;
        //! Autohide flag.
        bool autoHide;

    public:

        //! Constructor.
        explicit ProgressBar(QWidget *parent = nullptr);

        //! Start pulsing.
        void startPulse(void);

        //! Stop pulsing.
        void stopPulse(void);

        //! Set progress bar value.
        void setValue(int value);

        //! Set message.
        void setMessage(const QString &message);

        //! Set autohide.
        void setAutoHide(bool autoHide);

    public slots:

        //! Hide widget.
        void hide(void);

    private slots:

        //! Increase pulse value.
        void pulse(void);
        
};

#endif // PROGRESS_BAR_H
