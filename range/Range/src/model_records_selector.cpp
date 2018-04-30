/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   model_records_widget.cpp                                 *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   5-th September 2013                                      *
 *                                                                   *
 *  DESCRIPTION: Model records widget class definition               *
 *********************************************************************/

#include <QAction>
#include <QIcon>
#include <QVBoxLayout>
#include <QToolBar>
#include <QCheckBox>

#include <rblib.h>

#include "job_manager.h"
#include "model_records_selector.h"
#include "model_io.h"
#include "session.h"
#include "main_window.h"
#include "video_output.h"
#include "video_settings_dialog.h"

ModelRecordsSelector::ModelRecordsSelector(QWidget *parent)
    : QWidget(parent)
    , markNextIndicator(false)
    , recordIndicator(false)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    this->setLayout(layout);

    this->tree = new ModelRecordsTree;
    layout->addWidget(this->tree);

    QObject::connect(this->tree,
                     &ModelRecordsTree::recordMarked,
                     this,
                     &ModelRecordsSelector::onRecordMarked);

    QToolBar *toolBar = new QToolBar;
    layout->addWidget(toolBar);

    QList<QAction*> actions;

    actions.append(new QAction(QIcon(":/icons/media/pixmaps/range-play_first.svg"),"First",this));
    QObject::connect(actions.last(), &QAction::triggered, this->tree, &ModelRecordsTree::markFirst);

    actions.append(new QAction(QIcon(":/icons/media/pixmaps/range-play_backward.svg"),"Backward",this));
    QObject::connect(actions.last(), &QAction::triggered, this->tree, &ModelRecordsTree::markPrevious);

    actions.append(new QAction(QIcon(":/icons/media/pixmaps/range-play_play.svg"),"Play",this));
    QObject::connect(actions.last(), &QAction::triggered, this, &ModelRecordsSelector::playToggle);
    this->playAction = actions.last();

    actions.append(new QAction(QIcon(":/icons/media/pixmaps/range-play_forward.svg"),"Forward",this));
    QObject::connect(actions.last(), &QAction::triggered, this->tree, &ModelRecordsTree::markNext);

    actions.append(new QAction(QIcon(":/icons/media/pixmaps/range-play_last.svg"),"Last",this));
    QObject::connect(actions.last(), &QAction::triggered, this->tree, &ModelRecordsTree::markLast);

    toolBar->addActions(actions);

    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    toolBar->addWidget(empty);

    actions.clear();

    actions.append(new QAction(QIcon(":/icons/media/pixmaps/range-play_record.svg"),"Record",this));
    QObject::connect(actions.last(), &QAction::triggered, this, &ModelRecordsSelector::onRecordVideo);
    this->recordAction = actions.last();

    toolBar->addActions(actions);

    QObject::connect(this,&ModelRecordsSelector::recordFinished,this,&ModelRecordsSelector::onRecordFinished);
}

void ModelRecordsSelector::createAnimation(bool modelID)
{
    Model &rModel = Session::getInstance().getModel(modelID);

    std::vector<double> &rTimes = rModel.getTimeSolver().getTimes();

    QList<QString> imageFileNames;

    for (uint j=0;j<rTimes.size();j++)
    {
        QString recordFileName(RFileManager::getFileNameWithTimeStep(rModel.getFileName(),j+1));
        QString imageFileName(RFileManager::getFileNameWithTimeStep(rModel.buildScreenShotFileName(),j+1));
        if (RFileManager::fileExists(recordFileName) && RFileManager::fileExists(imageFileName))
        {
            imageFileNames.append(imageFileName);
        }
    }

    if (imageFileNames.size() > 0)
    {
        QString fileName(rModel.buildAnimationFileName(this->videoSettings.getFormat()));

        RLogger::info("Creating animation file \'%s\'\n",fileName.toUtf8().constData());

        VideoOutput videoOutput;
        videoOutput.setStreamRate(this->videoSettings.getFps());
        videoOutput.setResolution(this->videoSettings.getWidth(),this->videoSettings.getHeight());
        if (!videoOutput.openMediaFile(this->videoSettings.getWidth(),this->videoSettings.getHeight(),fileName))
        {
            RLogger::error("Failed to create animation file \'%s\'\n",fileName.toUtf8().constData());
            return;
        }

        uint cnt=0;
        foreach (const QString &imageFileName, imageFileNames)
        {
            QImage image;
            if (!image.load(imageFileName))
            {
                RLogger::error("Failed to load image file \'%s\'\n",imageFileName.toUtf8().constData());
                return;
            }
            QImage sImage = image.scaled(this->videoSettings.getWidth(),this->videoSettings.getHeight(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
            if (sImage.isNull())
            {
                RLogger::error("Failed to scale image\n");
                return;
            }
            if (sImage.height() > int(this->videoSettings.getHeight()))
            {
                sImage = image.scaled(this->videoSettings.getWidth(),this->videoSettings.getHeight(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
                if (sImage.isNull())
                {
                    RLogger::error("Failed to scale image\n");
                    return;
                }
            }

            QImage fImage(this->videoSettings.getWidth(),this->videoSettings.getHeight(),sImage.format());
            QPainter painter;
            painter.begin(&fImage);
            painter.setBackground(QBrush(Qt::black));
            painter.setBrush(QBrush(Qt::black));
            painter.setPen(Qt::black);
            painter.drawRect(0,0,this->videoSettings.getWidth(),this->videoSettings.getHeight());
            painter.drawImage((this->videoSettings.getWidth()-sImage.width())/2,
                              (this->videoSettings.getHeight()-sImage.height())/2,
                              sImage);
            painter.end();

            RLogger::info("Adding frame from image file \'%s\'\n",imageFileName.toUtf8().constData());
            for (uint i=0;i<this->videoSettings.getFpp();i++)
            {
                if (!videoOutput.newFrame(fImage))
                {
                    RLogger::error("Failed to add frame from image file \'%s\'\n",imageFileName.toUtf8().constData());
                    return;
                }
            }
            RProgressPrint(cnt++,imageFileNames.size());
        }

        if (!videoOutput.closeMediaFile())
        {
            RLogger::error("Failed to create animation file \'%s\'\n",fileName.toUtf8().constData());
            return;
        }
        RLogger::info("Animation file \'%s\' created\n",fileName.toUtf8().constData());
    }
}

void ModelRecordsSelector::onRecordMarked(uint modelID, const QString &recordFileName)
{
    RLogger::info("Selected record: \'%s\'\n", recordFileName.toUtf8().constData());

    Job *updateJob = new ModelIO(MODEL_IO_UPDATE, recordFileName, &Session::getInstance().getModel(modelID));
    this->updateModelID = modelID;

    QObject::connect(updateJob,
                     &Job::finished,
                     this,
                     &ModelRecordsSelector::onUpdateJobFinished);

    JobManager::getInstance().submit(updateJob);
}

void ModelRecordsSelector::onRecordFinished(void)
{
    QTimer::singleShot(this->recordIndicator?0:1000, this, SLOT(loadNextRecord()));
}

void ModelRecordsSelector::onUpdateJobFinished(void)
{
    if (this->recordIndicator)
    {
        // Take screen-shot
        Session::getInstance().setTakeScreenShot(this->updateModelID,Session::getInstance().getModel(this->updateModelID).buildScreenShotFileName(),false);
    }
    if (this->markNextIndicator)
    {
        emit this->recordFinished();
    }
}

void ModelRecordsSelector::playToggle(bool jumpToFirst)
{
    this->markNextIndicator = !this->markNextIndicator;

    if (this->markNextIndicator)
    {
        MainWindow::getInstance()->progressAutoHideDisable();
        this->playAction->setIcon(QIcon(":/icons/media/pixmaps/range-play_pause.svg"));
        this->playAction->setText("Pause");

        this->loadNextRecord(jumpToFirst);
    }
    else
    {
        this->playAction->setIcon(QIcon(":/icons/media/pixmaps/range-play_play.svg"));
        this->playAction->setText("Play");
        this->recordAction->setEnabled(true);
        MainWindow::getInstance()->progressAutoHideEnable();
    }
}

void ModelRecordsSelector::loadNextRecord(bool jumpToFirst)
{
    if (!jumpToFirst && (this->tree->isLast() || !this->markNextIndicator))
    {
        if (this->recordIndicator)
        {
            QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();
            for (int i=0;i<selectedModelIDs.size();i++)
            {
                RProgressInitialize("Creating a video file");
                this->createAnimation(selectedModelIDs.at(i));
                RProgressFinalize();
            }
        }
        this->markNextIndicator = false;
        this->recordIndicator = false;
        this->playAction->setIcon(QIcon(":/icons/media/pixmaps/range-play_play.svg"));
        this->playAction->setText("Play");
        this->recordAction->setEnabled(true);
        MainWindow::getInstance()->progressAutoHideEnable();
    }
    else
    {
        if (jumpToFirst)
        {
            if (this->tree->isFirst())
            {
                this->tree->markCurrent();
            }
            else
            {
                this->tree->markFirst();
            }
        }
        else
        {
            this->tree->markNext();
        }
    }
}

void ModelRecordsSelector::onRecordVideo(void)
{
    VideoSettingsDialog videoSettingsDialog(&this->videoSettings);
    if (videoSettingsDialog.exec() == QDialog::Accepted)
    {
        this->recordIndicator = true;
        this->recordAction->setDisabled(true);
        this->playToggle(true);
    }
}
