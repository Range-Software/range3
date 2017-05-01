/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   position_widget.h                                        *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   30-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Position widget class declaration                   *
 *********************************************************************/

#ifndef __POSITION_WIDGET_H__
#define __POSITION_WIDGET_H__

#include <QGroupBox>
#include <QSlider>
#include <QPushButton>

#include <rblib.h>

#include "value_line_edit.h"

class PositionWidget : public QGroupBox
{

    Q_OBJECT

    protected:

        //! Is horizontal layout.
        bool horizontalLayout;
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
        ValueLineEdit *xLineEdit;
        //! Y coordinate.
        ValueLineEdit *yLineEdit;
        //! Z coordinate.
        ValueLineEdit *zLineEdit;
        //! Pick button.
        QPushButton *pickButton;
        //! Cancel button.
        QPushButton *cancelButton;
        //! OK button.
        QPushButton *okButton;

    public:

        //! Constructor.
        explicit PositionWidget(const QString &title, const RR3Vector &position, bool horizontalLayout = false, QWidget *parent = 0);

        //! Show sliders.
        void showSliders(void);

        //! Hide sliders.
        void hideSliders(void);

        //! Show pick button.
        void showPickButton(void);

        //! Hide pick button.
        void hidePickButton(void);

        //! Show buttons.
        void showButtons(void);

        //! Hide buttons.
        void hideButtons(void);

        //! Set X range.
        void setXRange(double xMin, double xMax, double step = 0.0);

        //! Set Y range.
        void setYRange(double yMin, double yMax, double step = 0.0);

        //! Set Z range.
        void setZRange(double zMin, double zMax, double step = 0.0);

        //! Return direction.
        RR3Vector getPosition(void) const;

        //! Set position.
        void setPosition(const RR3Vector &position);

    private:

        //! Set sliders position.
        void setSlidersPosition(const RR3Vector &position);

        //! Calculate slider positions.
        void findSliderPositions(const RR3Vector &position, int &x, int &y, int &z);

    signals:

        //! Position changed.
        void changed(const RR3Vector &position);

        //! Close signal.
        void closed(void);

    private slots:

        //! X slider value changed.
        void onXSliderValueChanged(int value);

        //! Y slider value changed.
        void onYSliderValueChanged(int value);

        //! Z slider value changed.
        void onZSliderValueChanged(int value);

        //! X line edit value changed.
        void onXLineEditValueChanged(double value);

        //! Y line edit value changed.
        void onYLineEditValueChanged(double value);

        //! Z line edit value changed.
        void onZLineEditValueChanged(double value);

        //! On pick button clicked.
        void onPickButtonClicked(void);

        //! On ok button clicked.
        void onOkButtonClicked(void);

        //! On cancel button clicked.
        void onCancelButtonClicked(void);

        //! Session pick list has changed.
        void onPickListChanged(void);
        
};

#endif // __POSITION_WIDGET_H__
