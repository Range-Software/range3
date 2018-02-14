/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   geometry_transform_widget.h                              *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th November 2014                                       *
 *                                                                   *
 *  DESCRIPTION: Geometry transform widget class declaration         *
 *********************************************************************/

#ifndef __GEOMETRY_TRANSFORM_WIDGET_H__
#define __GEOMETRY_TRANSFORM_WIDGET_H__

#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QCheckBox>
#include <QSpinBox>

#include <rblib.h>

#include "geometry_transform_input.h"
#include "geometry_rotate_widget.h"
#include "geometry_scale_widget.h"
#include "geometry_translate_widget.h"

class GeometryTransformWidget : public QWidget
{

    Q_OBJECT

    protected:

        GeometryScaleWidget *scaleWidget;
        GeometryRotateWidget *rotateWidget;
        GeometryTranslateWidget *translateWidget;
        QRadioButton *applyToAllRadio;
        QRadioButton *applyToVisibleRadio;
        QRadioButton *applyToSelectedRadio;
        QGroupBox *includeSharedNodesCheck;
        QGroupBox *splitSharedNodesCheck;
        QGroupBox *sweepNodesCheck;
        QSpinBox *sweepNStepsSpin;

        //! Transform input.
        GeometryTransformInput input;
        //! Cancel button.
        QPushButton *cancelButton;
        //! OK button.
        QPushButton *okButton;

    public:

        //! Constructor.
        explicit GeometryTransformWidget(QWidget *parent = 0);

    private:

        //! Enable OK button if all conditions are met.
        void enableOkButton(void);

    signals:

        //! Finalize signal.
        void finalize(void);

    protected slots:

        //! Scale changed.
        void onScaleChanged(const RR3Vector &center, const RR3Vector &scales);

        //! Rotation changed.
        void onRotationChanged(const RR3Vector &center, const RR3Vector &rotations);

        //! Translation changed.
        void onTranslationChanged(const RR3Vector &distances);

        //! Apply to all radio was clicked.
        void onApplyToAllClicked(bool checked);

        //! Apply to selected radio was clicked.
        void onApplyToSelectedClicked(bool checked);

        //! Apply to visible radio was clicked.
        void onApplyToVisibleClicked(bool checked);

        //! Include shared nodes was checked.
        void onIncludeSharedNodesClicked(bool checked);

        //! Split shared nodes was checked.
        void onSplitSharedNodesClicked(bool checked);

        //! Sweep shared nodes state has changed.
        void onSweepSharedNodesClicked(bool checked);

        //! Number of sweep steps spin-box has changed.
        void onSweepNStepsSpinChanged(int nSweepSteps);

        //! Cancel button was clicked.
        void onCancelClicked(void);

        //! OK button was clicked.
        void onOkClicked(void);

};

#endif // __GEOMETRY_TRANSFORM_WIDGET_H__
