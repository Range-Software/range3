/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   video_settings_dialog.cpp                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   7-th January 2018                                        *
 *                                                                   *
 *  DESCRIPTION: Video settings dialog class definition              *
 *********************************************************************/

#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

#include "main_settings.h"
#include "video_settings_dialog.h"

VideoSettingsDialog::VideoSettingsDialog(VideoSettings *pVideoSettings, QWidget *parent)
    : QDialog(parent)
    , pVideoSettings(pVideoSettings)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    this->setWindowTitle(tr("Video settings"));

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout (mainLayout);

    QLabel *formatLabel = new QLabel(tr("File format"));
    mainLayout->addWidget(formatLabel, 0, 0, 1, 1);

    this->formatCombo = new QComboBox;
    QList<QString> videoFormats = MainSettings::getSupportedVideoFormats();
    for (int i=0;i<videoFormats.size();i++)
    {
        this->formatCombo->addItem(videoFormats.at(i));
        if (videoFormats.at(i) == this->pVideoSettings->getFormat())
        {
            this->formatCombo->setCurrentIndex(i);
        }
    }
    mainLayout->addWidget(this->formatCombo, 0, 1, 1, 1);

    QLabel *resolutionLabel = new QLabel(tr("Resolution"));
    mainLayout->addWidget(resolutionLabel, 1, 0, 1, 1);

    this->resolutionCombo = new QComboBox;
    this->resolutionCombo->addItem("SD (480p)");
    this->resolutionCombo->addItem("HD (720p)");
    this->resolutionCombo->addItem("Full HD (1080p)");
    this->resolutionCombo->addItem("Ultra HD (2160p)");
    this->resolutionCombo->setCurrentIndex(1);
    mainLayout->addWidget(this->resolutionCombo, 1, 1, 1, 1);

    QLabel *fpsLabel = new QLabel(tr("Frames per second"));
    mainLayout->addWidget(fpsLabel, 2, 0, 1, 1);

    this->fpsSpin = new QSpinBox;
    this->fpsSpin->setValue(int(this->pVideoSettings->getFps()));
    this->fpsSpin->setRange(int(VideoSettings::minFps),int(VideoSettings::maxFps));
    mainLayout->addWidget(this->fpsSpin, 2, 1, 1, 1);

    QLabel *fppLabel = new QLabel(tr("Frames per record"));
    mainLayout->addWidget(fppLabel, 3, 0, 1, 1);

    this->fppSpin = new QSpinBox;
    this->fppSpin->setValue(int(this->pVideoSettings->getFpp()));
    this->fppSpin->setRange(int(VideoSettings::minFpp),int(VideoSettings::maxFpp));
    mainLayout->addWidget(this->fppSpin, 3, 1, 1, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout, 4, 0, 1, 2);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    okButton->setDefault(true);
    buttonsLayout->addWidget(okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&VideoSettingsDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&VideoSettingsDialog::accept);
}

int VideoSettingsDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        this->pVideoSettings->setFormat(MainSettings::getSupportedVideoFormats().at(this->formatCombo->currentIndex()));
        switch (this->resolutionCombo->currentIndex())
        {
            case 0:
            {
                this->pVideoSettings->setWidth(640);
                this->pVideoSettings->setHeight(480);
                break;
            }
            case 1:
            {
                this->pVideoSettings->setWidth(1280);
                this->pVideoSettings->setHeight(720);
                break;
            }
            case 2:
            {
                this->pVideoSettings->setWidth(1920);
                this->pVideoSettings->setHeight(1080);
                break;
            }
            case 3:
            {
                this->pVideoSettings->setWidth(3840);
                this->pVideoSettings->setHeight(2160);
                break;
            }
            default:
            {
                this->pVideoSettings->setWidth(1280);
                this->pVideoSettings->setHeight(720);
                break;
            }
        }
        this->pVideoSettings->setFps(uint(this->fpsSpin->value()));
        this->pVideoSettings->setFpp(uint(this->fppSpin->value()));
    }
    return retVal;
}
