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

ModelRecordsSelector::ModelRecordsSelector(QWidget *parent) :
    QWidget(parent),
    markNextIndicator(false),
    recordIndicator(false)
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
        QString recordFileName(RFileManager::getFileNameWithTimeStep(rModel.buildScreenShotFileName(),j+1));
        if (RFileManager::fileExists(recordFileName))
        {
            imageFileNames.append(recordFileName);
        }
    }

    if (imageFileNames.size() > 0)
    {
        QString fileName(rModel.buildAnimationFileName("mp4"));

        RLogger::info("Creating animation file \'%s\'\n",fileName.toUtf8().constData());

        int width = 640;
        int height = 480;

        width += width%2;
        height += height%2;

        VideoOutput videoOutput;
        videoOutput.setResolution(width,height);
        if (!videoOutput.openMediaFile(width,height,fileName))
        {
            RLogger::error("Failed to create animation file \'%s\'\n",fileName.toUtf8().constData());
            return;
        }

        foreach (const QString &imageFileName, imageFileNames)
        {
            QImage image;
            if (!image.load(imageFileName))
            {
                RLogger::error("Failed to load image file \'%s\'\n",imageFileName.toUtf8().constData());
                return;
            }
            QImage sImage = image.scaled(640,480,Qt::KeepAspectRatio);
            if (sImage.isNull())
            {
                RLogger::error("Failed to scale image\n");
                return;
            }
            RLogger::info("Adding frame from image file \'%s\'\n",imageFileName.toUtf8().constData());
            if (!videoOutput.newFrame(image))
            {
                RLogger::error("Failed to add frame from image file \'%s\'\n",imageFileName.toUtf8().constData());
                return;
            }
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
                this->createAnimation(selectedModelIDs.at(i));
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
    this->recordIndicator = true;
    this->recordAction->setDisabled(true);
    this->playToggle(true);
}
