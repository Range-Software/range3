/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   model_records_tree.h                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   4-th September 2013                                      *
 *                                                                   *
 *  DESCRIPTION: Model records tree class declaration                *
 *********************************************************************/

#ifndef MODEL_RECORDS_TREE_H
#define MODEL_RECORDS_TREE_H

#include <QTreeWidget>

class ModelRecordsTree : public QTreeWidget
{

    Q_OBJECT

    protected:

        enum ModelRecordsTreeColumn
        {
            Marked = 0,
            ModelID,
            IsRecord,
            RecordNumber,
            RecordFileName,
            PathFileName,
            NColumns
        };

    public:

        //! Constructor.
        explicit ModelRecordsTree(QWidget *parent = nullptr);

        //! Return true if marked record is first.
        bool isFirst(void);

        //! Return true if marked record is last.
        bool isLast(void);

    signals:

        //! Record marked.
        void recordMarked(uint modelID, const QString &recordFileName);

    public slots:

        //! Forced mark current record.
        void markCurrent(void);

        //! Mark previous record.
        void markPrevious(void);

        //! Mark next record.
        void markNext(void);

        //! Mark first record.
        void markFirst(void);

        //! Mark last record.
        void markLast(void);
        
    protected slots:

        //! Populate tree.
        void populate(void);

        //! Model selection has changed.
        void onModelSelectionChanged(uint modelID);

        //! Model havs changed.
        void onModelChanged(uint);

        //! On item changed.
        void onItemChanged(QTreeWidgetItem *item, int column);

        //! On item activated.
        void onItemActivated(QTreeWidgetItem *item, int);
        
};

#endif // MODEL_RECORDS_TREE_H
