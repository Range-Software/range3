/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   direction_widget.h                                       *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   30-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Direction widget class declaration                  *
 *********************************************************************/

#ifndef DIRECTION_WIDGET_H
#define DIRECTION_WIDGET_H

#include <QGroupBox>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QPushButton>

#include <rblib.h>

class DirectionWidget : public QGroupBox
{

    Q_OBJECT

    protected:

        //! Show button box.
        //! If set to true changed signal will be emitted only if OK button is clicked.
        bool showButtonBox;
        //! X slider.
        QSlider *xSlider;
        //! Y slider.
        QSlider *ySlider;
        //! Z slider.
        QSlider *zSlider;
        //! X coordinate.
        QDoubleSpinBox *xSpin;
        //! Y coordinate.
        QDoubleSpinBox *ySpin;
        //! Z coordinate.
        QDoubleSpinBox *zSpin;
        //! Cancel button.
        QPushButton *cancelButton;
        //! OK button.
        QPushButton *okButton;

    public:

        //! Constructor.
        explicit DirectionWidget(const QString &title, const RR3Vector &direction, QWidget *parent = nullptr);

        //! Show buttons.
        void showButtons(void);

        //! Hide buttons.
        void hideButtons(void);

        //! Return direction.
        RR3Vector getDirection(void) const;

        //! Set direction.
        void setDirection(const RR3Vector &direction);

    private:

        //! Set plane normal.
        void normalize(RR3Vector &direction, int keepPosition);

        //! Calculate slider positions.
        void findSliderPositions(const RR3Vector &direction, int &x, int &y, int &z);
        
    signals:

        //! Direction changed.
        void changed(const RR3Vector &direction);

        //! Close signal.
        void closed(void);
        
    private slots:

        //! X slider value changed.
        void onXSliderValueChanged(int value);

        //! Y slider value changed.
        void onYSliderValueChanged(int value);

        //! Z slider value changed.
        void onZSliderValueChanged(int value);

        //! X spin box value changed.
        void onXSpinValueChanged(double);

        //! Y spin box value changed.
        void onYSpinValueChanged(double);

        //! Z spin box value changed.
        void onZSpinValueChanged(double);

        //! On ok button clicked.
        void onOkButtonClicked(void);

        //! On cancel button clicked.
        void onCancelButtonClicked(void);
        
};

#endif // DIRECTION_WIDGET_H
