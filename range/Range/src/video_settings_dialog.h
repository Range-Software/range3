/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   video_settings_dialog.h                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   7-th January 2018                                        *
 *                                                                   *
 *  DESCRIPTION: Video settings dialog class declaration             *
 *********************************************************************/

#ifndef VIDEO_SETTINGS_DIALOG_H
#define VIDEO_SETTINGS_DIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QComboBox>

#include "video_settings.h"

class VideoSettingsDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Pointer to video settings.
        VideoSettings *pVideoSettings;

        QComboBox *formatCombo;
        QComboBox *resolutionCombo;
        QSpinBox *fpsSpin;
        QSpinBox *fppSpin;

    public:

        //! Constructor.
        explicit VideoSettingsDialog(VideoSettings *pVideoSettings, QWidget *parent = nullptr);

        //! Execute dialog.
        int exec(void);

};

#endif // VIDEO_SETTINGS_DIALOG_H
