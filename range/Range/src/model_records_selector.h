/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   model_records_widget.h                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   5-th September 2013                                      *
 *                                                                   *
 *  DESCRIPTION: Model records widget class declaration              *
 *********************************************************************/

#ifndef MODEL_RECORDS_SELECTOR_H
#define MODEL_RECORDS_SELECTOR_H

#include <QWidget>
#include <QAction>

#include "model_records_tree.h"
#include "video_settings.h"

class ModelRecordsSelector : public QWidget
{

    Q_OBJECT

    protected:

        //! Play action.
        QAction *playAction;
        //! Record action.
        QAction *recordAction;
        //! Active model id (while updating).
        uint updateModelID;
        //! Mark next record.
        bool markNextIndicator;
        //! Record screenshot indicator.
        bool recordIndicator;
        //! Model records tree.
        ModelRecordsTree *tree;
        //! Video settings.
        VideoSettings videoSettings;

    public:

        //! Constructor.
        explicit ModelRecordsSelector(QWidget *parent = nullptr);

    protected:

        //! Create animation from screenshots.
        void createAnimation(bool modelID);

    protected slots:

        //! Record markeded.
        void onRecordMarked(uint modelID, const QString &recordFileName);

        //! On record has finished.
        void onRecordFinished(void);

        //! On update job has finished.
        void onUpdateJobFinished(void);

        //! Play/Pause.
        void playToggle(bool jumpToFirst = false);

        //! Load next record.
        void loadNextRecord(bool jumpToFirst = false);

        //! On record video toggled.
        void onRecordVideo(void);

    signals:

        //! Signal to load next record.
        void recordFinished(void);
    
};

#endif // MODEL_RECORDS_SELECTOR_H
