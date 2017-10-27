/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   model_records_widget.h                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   5-th September 2013                                      *
 *                                                                   *
 *  DESCRIPTION: Model records widget class declaration              *
 *********************************************************************/

#ifndef __MODEL_RECORDS_SELECTOR_H__
#define __MODEL_RECORDS_SELECTOR_H__

#include <QWidget>
#include <QAction>

#include "model_records_tree.h"

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

    public:

        //! Constructor.
        explicit ModelRecordsSelector(QWidget *parent = 0);

    protected:

        //! Create animation from screenshots.
        void createAnimation(bool modelID);

    protected slots:

        //! Record markeded.
        void onRecordMarked(uint modelID, const QString &recordFileName);

        //! On update job has finished.
        void onUpdateJobFinished(void);

        //! Play/Pause.
        void playToggle(bool jumpToFirst = false);

        //! Load next record.
        void loadNextRecord(bool jumpToFirst = false);

        //! On record video toggled.
        void onRecordVideo(void);
    
};

#endif // __MODEL_RECORDS_SELECTOR_H__
