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
}

void ModelRecordsSelector::recordScreenShot(bool modelID)
{
    Model &rModel = Session::getInstance().getModel(modelID);

    Session::getInstance().setTakeScreenShot(modelID,rModel.buildScreenShotFileName());
}

void ModelRecordsSelector::createAnimation(bool modelID)
{
    Model &rModel = Session::getInstance().getModel(modelID);

    std::vector<double> &rTimes = rModel.getTimeSolver().getTimes();

    QList<QString> imageFileNames;

    for (uint j=0;j<rTimes.size();j++)
    {
        QString recordFileName(RFileManager::getFileNameWithTimeStep(rModel.getFileName(),j+1));
        if (RFileManager::fileExists(recordFileName))
        {
            imageFileNames.append(recordFileName);
        }
    }

    QString fileName(rModel.buildAnimationFileName());

    RLogger::info("Creating animation file \'%s\'\n",fileName.toUtf8().constData());
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

void ModelRecordsSelector::onUpdateJobFinished(void)
{
    if (this->recordIndicator)
    {
        this->recordScreenShot(this->updateModelID);
    }
    if (this->markNextIndicator)
    {
        QTimer::singleShot(this->recordIndicator?0:1000, this, SLOT(loadNextRecord()));
    }
}

void ModelRecordsSelector::playToggle(bool jumpToFirst)
{
    this->markNextIndicator = !this->markNextIndicator;

    if (this->markNextIndicator)
    {
        this->playAction->setIcon(QIcon(":/icons/media/pixmaps/range-play_pause.svg"));
        this->playAction->setText("Pause");

        this->loadNextRecord(jumpToFirst);
    }
    else
    {
        this->playAction->setIcon(QIcon(":/icons/media/pixmaps/range-play_play.svg"));
        this->playAction->setText("Play");
    }
}

void ModelRecordsSelector::loadNextRecord(bool jumpToFirst)
{
    if (!jumpToFirst && (this->tree->isLast() || !this->markNextIndicator))
    {
        if (recordIndicator)
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
