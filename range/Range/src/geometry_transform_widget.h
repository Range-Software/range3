/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   geometry_transform_widget.h                              *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th November 2014                                       *
 *                                                                   *
 *  DESCRIPTION: Geometry transform widget class declaration         *
 *********************************************************************/

#ifndef GEOMETRY_TRANSFORM_WIDGET_H
#define GEOMETRY_TRANSFORM_WIDGET_H

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
        QRadioButton *applyToSelectedRadio;
        QRadioButton *applyToPickedRadio;
        QRadioButton *applyToVisibleRadio;
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
        explicit GeometryTransformWidget(QWidget *parent = nullptr);

    private:

        //! Enable OK button if all conditions are met.
        void enableOkButton(void);

    signals:

        //! Finalize signal.
        void finalize(void);

    protected slots:

        //! Model selection changed.
        void onModelSelectionChanged(uint);

        //! Model visibility changed.
        void onModelVisibilityChanged(uint);

        //! Pick-list changed.
        void onPickListChanged(void);

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

        //! Apply to picked radio was clicked.
        void onApplyToPickedClicked(bool checked);

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

#endif // GEOMETRY_TRANSFORM_WIDGET_H
