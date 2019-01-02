/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   tradiation_setup_widget.h                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   11-th May 2016                                           *
 *                                                                   *
 *  DESCRIPTION: Radiation setup widget class declaration            *
 *********************************************************************/

#ifndef RADIATION_SETUP_WIDGET_H
#define RADIATION_SETUP_WIDGET_H

#include <QWidget>

#include <rmlib.h>

#include "file_chooser_button.h"

class RadiationSetupWidget : public QWidget
{
    Q_OBJECT

    protected:

        //! Radiation setup.
        RRadiationSetup radiationSetup;
        //! Default view-factor filename.
        QString defaultViewFactorFileName;
        //! View-factor file chooser button.
        FileChooserButton *viewFactorFileButton;

    public:

        //! Constructor.
        explicit RadiationSetupWidget(const RRadiationSetup &radiationSetup, const QString &defaultViewFactorFileName, QWidget *parent = nullptr);

    signals:

        //! Radiation setup has changed.
        void changed(const RRadiationSetup &radiationSetup);

    protected slots:

        //! Radiation resolution changed.
        void onResolutionChanged(int index);

        //! Radiation custom view-factor file toggled.
        void onCustomViewFactorFileToggled(bool checked);

        //! Radiation view-factor file changed.
        void onViewFactorFileChanged(const QString &fileName);

        //! Regenerate patches.
        void regeneratePatches(void);

        //! Recalculate view-factors.
        void recalculateViewFactors(void);

        //! Clear view-factor mtrix.
        void clearViewFactorMatrix(void);

};

#endif // RADIATION_SETUP_WIDGET_H
