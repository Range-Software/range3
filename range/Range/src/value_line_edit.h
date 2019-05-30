/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   value_line_edit.h                                        *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   18-th July 2012                                          *
 *                                                                   *
 *  DESCRIPTION: Value line edit class declaration                   *
 *********************************************************************/

#ifndef VALUE_LINE_EDIT_H
#define VALUE_LINE_EDIT_H

#include <QLineEdit>
#include <QDoubleValidator>
#include <QTimer>

class ValueLineEdit : public QLineEdit
{
    Q_OBJECT

    protected:

        //! Timer.
        QTimer *timer;

    public:

        //! Constructor.
        explicit ValueLineEdit(QWidget *parent = nullptr);

        //! Constructor.
        explicit ValueLineEdit(double min, double max, QWidget *parent = nullptr);

        //! Constructor.
        explicit ValueLineEdit(int min, int max, QWidget *parent = nullptr);

        //! Set double validator.
        void setDoubleValidator();

        //! Set int validator.
        void setIntValidator();

        //! Set range.
        void setRange(double min, double max);

        //! Set range.
        void setRange(int min, int max);

        //! Return minimum value.
        double getMinimum() const;

        //! Return maximum value.
        double getMaximum() const;

        //! Return value.
        double getValue() const;

        //! Set new value.
        void setValue(double value);

        //! Set new value.
        void setValue(int value);

        //! Set new value.
        void setValue(uint value);

        //! Return validator state.
        QValidator::State getValidatorState() const;

    private:

        //! Create timer.
        void createTimer();

        //! Paint background based on entered value.
        void paintBackground();
        
    signals:

        //! Value changed signal.
        void valueChanged(double value);


    private slots:

        //! Catch signal textChanged
        void onTextChaged(QString text);

        //! Catch timer timeout signal.
        void onTimeout();
};

#endif /* VALUE_LINE_EDIT_H */
