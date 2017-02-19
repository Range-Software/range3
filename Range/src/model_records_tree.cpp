/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   model_records_tree.cpp                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   4-th September 2013                                      *
 *                                                                   *
 *  DESCRIPTION: Model records tree class definition                 *
 *********************************************************************/

#include <QMap>

#include "model_records_tree.h"
#include "session.h"

class ModelRecordsTreeRecordID
{
    public:
        ModelRecordsTreeRecordID(uint modelID,bool isRecord,uint recordNumber)
            : modelID(modelID)
            , isRecord(isRecord)
            , recordNumber(recordNumber)
        {
        }
        bool operator < (const ModelRecordsTreeRecordID &rModelRecordsTreeRecordID) const
        {
            if (this->modelID < rModelRecordsTreeRecordID.modelID)
            {
                return true;
            }
            if (this->isRecord < rModelRecordsTreeRecordID.isRecord)
            {
                return true;
            }
            if (this->recordNumber < rModelRecordsTreeRecordID.recordNumber)
            {
                return true;
            }
            return false;
        }

    public:
        uint modelID;
        bool isRecord;
        uint recordNumber;
};

typedef enum _ModelRecordsTreeColumn
{
    MODEL_RECORDS_TREE_COLUMN_MARKED = 0,
    MODEL_RECORDS_TREE_COLUMN_MODEL_ID,
    MODEL_RECORDS_TREE_COLUMN_IS_RECORD,
    MODEL_RECORDS_TREE_COLUMN_RECORD_NUMBER,
    MODEL_RECORDS_TREE_COLUMN_RECORD_FILENAME,
    MODEL_RECORDS_TREE_COLUMN_PATH_FILENAME,
    MODEL_RECORDS_TREE_N_COLUMNS
} ModelRecordsTreeColumn;

ModelRecordsTree::ModelRecordsTree(QWidget *parent) :
    QTreeWidget(parent)
{
    this->setColumnCount(MODEL_RECORDS_TREE_N_COLUMNS);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setRootIsDecorated(false);
    this->setAutoScroll(false);
    QTreeWidgetItem* headerItem = new QTreeWidgetItem();
    headerItem->setText(MODEL_RECORDS_TREE_COLUMN_RECORD_NUMBER,QString(tr("Record")));
    headerItem->setText(MODEL_RECORDS_TREE_COLUMN_RECORD_FILENAME,QString(tr("File name")));
    headerItem->setIcon(MODEL_RECORDS_TREE_COLUMN_MARKED,QIcon(":/icons/media/pixmaps/range-play_play.svg"));
    this->setHeaderItem(headerItem);

    this->setColumnHidden(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,true);
    this->setColumnHidden(MODEL_RECORDS_TREE_COLUMN_IS_RECORD,true);
    this->setColumnHidden(MODEL_RECORDS_TREE_COLUMN_PATH_FILENAME,true);

    this->populate();

    QObject::connect(&Session::getInstance(),
                     &Session::modelSelectionChanged,
                     this,
                     &ModelRecordsTree::onModelSelectionChanged);
    QObject::connect(&Session::getInstance(),
                     &Session::modelAdded,
                     this,
                     &ModelRecordsTree::onModelChanged);
    QObject::connect(&Session::getInstance(),
                     &Session::modelChanged,
                     this,
                     &ModelRecordsTree::onModelChanged);
    QObject::connect(&Session::getInstance(),
                     &Session::modelRemoved,
                     this,
                     &ModelRecordsTree::onModelChanged);
    QObject::connect(this,
                     &ModelRecordsTree::itemChanged,
                     this,
                     &ModelRecordsTree::onItemChanged);
    QObject::connect(this,
                     &ModelRecordsTree::itemActivated,
                     this,
                     &ModelRecordsTree::onItemActivated);

    QTimer *timer = new QTimer(this);
    QObject::connect(timer,
                     &QTimer::timeout,
                     this,
                     &ModelRecordsTree::populate);
    timer->start(1000);
}

bool ModelRecordsTree::isFirst(void)
{
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        uint modelID = modelIDs[i];

        QTreeWidgetItemIterator it(this);
        while (*it)
        {
            if ((*it)->data(MODEL_RECORDS_TREE_COLUMN_IS_RECORD,Qt::UserRole).toBool() &&
                (*it)->data(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,Qt::UserRole).toUInt() == modelID)
            {
                if ((*it)->data(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole).toBool())
                {
                    return true;
                }
                break;
            }
            ++it;
        }
    }
    return false;
}

bool ModelRecordsTree::isLast(void)
{
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        uint modelID = modelIDs[i];
        bool marked = false;

        QTreeWidgetItemIterator it(this);
        while (*it)
        {
            if ((*it)->data(MODEL_RECORDS_TREE_COLUMN_IS_RECORD,Qt::UserRole).toBool() &&
                (*it)->data(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,Qt::UserRole).toUInt() == modelID)
            {
                if ((*it)->data(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole).toBool())
                {
                    marked = true;
                }
                else
                {
                    if (marked)
                    {
                        return false;
                    }
                }
            }
            ++it;
        }
    }
    return true;
}

void ModelRecordsTree::markCurrent(void)
{
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        uint modelID = modelIDs[i];

        QTreeWidgetItemIterator it(this);
        while (*it)
        {
            if ((*it)->data(MODEL_RECORDS_TREE_COLUMN_IS_RECORD,Qt::UserRole).toBool() &&
                (*it)->data(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,Qt::UserRole).toUInt() == modelID)
            {
                if ((*it)->data(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole).toBool())
                {
                    emit this->recordMarked((*it)->data(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,Qt::UserRole).toUInt(),
                                            (*it)->text(MODEL_RECORDS_TREE_COLUMN_PATH_FILENAME));
                }
            }
            ++it;
        }
    }
}

void ModelRecordsTree::markPrevious(void)
{
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        uint modelID = modelIDs[i];

        QTreeWidgetItem *prevItem = 0;

        QTreeWidgetItemIterator it(this);
        while (*it)
        {
            if ((*it)->data(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole).toBool() &&
                (*it)->data(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,Qt::UserRole).toUInt() == modelID)
            {
                if (prevItem)
                {
                    (*it)->setData(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole,QVariant(false));
                    prevItem->setData(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole,QVariant(true));
                }
                break;
            }
            if ((*it)->data(MODEL_RECORDS_TREE_COLUMN_IS_RECORD,Qt::UserRole).toBool() &&
                (*it)->data(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,Qt::UserRole).toUInt() == modelID)
            {
                prevItem = (*it);
            }
            ++it;
        }
    }
}

void ModelRecordsTree::markNext(void)
{
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        uint modelID = modelIDs[i];

        bool markItem = false;

        QTreeWidgetItem *prevItem = 0;

        QTreeWidgetItemIterator it(this);
        while (*it)
        {
            if (markItem)
            {
                if ((*it)->data(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,Qt::UserRole).toUInt() == modelID)
                {
                    prevItem->setData(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole,false);
                    (*it)->setData(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole,true);
                }
                break;
            }
            if ((*it)->data(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole).toBool() &&
                (*it)->data(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,Qt::UserRole).toUInt() == modelID)
            {
                modelID = (*it)->data(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,Qt::UserRole).toUInt();
                prevItem = (*it);
                markItem = true;
            }
            ++it;
        }
    }
}

void ModelRecordsTree::markFirst(void)
{
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        uint modelID = modelIDs[i];
        bool isFirst = true;

        QTreeWidgetItemIterator it(this);
        while (*it)
        {
            if ((*it)->data(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,Qt::UserRole).toUInt() == modelID &&
                (*it)->data(MODEL_RECORDS_TREE_COLUMN_IS_RECORD,Qt::UserRole).toBool())
            {
                (*it)->setData(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole,isFirst);
                isFirst = false;
            }
            ++it;
        }
    }
}

void ModelRecordsTree::markLast(void)
{
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        uint modelID = modelIDs[i];
        uint itemID1 = 0;
        uint itemID2 = 0;
        QTreeWidgetItem *lastItem = 0;

        QTreeWidgetItemIterator it(this);
        while (*it)
        {
            if ((*it)->data(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,Qt::UserRole).toUInt() == modelID)
            {
                lastItem = (*it);
                itemID1++;
            }
            ++it;
        }

        if (lastItem && !lastItem->data(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole).toBool())
        {
            lastItem->setData(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole,true);

            QTreeWidgetItemIterator it(this);
            while (*it)
            {
                if ((*it)->data(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,Qt::UserRole).toUInt() == modelID)
                {
                    itemID2++;
                    if (itemID1 != itemID2)
                    {
                        (*it)->setData(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole,false);
                    }
                }
                ++it;
            }
        }
    }
}

void ModelRecordsTree::populate(void)
{
    if (!Session::getInstance().trylock())
    {
        return;
    }

    this->blockSignals(true);

    QList<QTreeWidgetItem*> selectedItemList = this->selectedItems();

    QMap<ModelRecordsTreeRecordID,bool> selectedIDs;

    for (int i=0;i<selectedItemList.size();i++)
    {
        ModelRecordsTreeRecordID recordID(selectedItemList.at(i)->data(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,Qt::UserRole).toUInt(),
                                          selectedItemList.at(i)->data(MODEL_RECORDS_TREE_COLUMN_IS_RECORD,Qt::UserRole).toBool(),
                                          selectedItemList.at(i)->data(MODEL_RECORDS_TREE_COLUMN_RECORD_NUMBER,Qt::DisplayRole).toUInt());
        selectedIDs[recordID] = true;
    }

    uint nTopLevelItems = this->topLevelItemCount();

    uint nModels = Session::getInstance().getNModels();

    for (uint i=0;i<nModels;i++)
    {
        Model &rModel = Session::getInstance().getModel(i);

        QTreeWidgetItem *item = new QTreeWidgetItem(this);
        item->setFirstColumnSpanned(true);
        item->setData(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,Qt::UserRole,QVariant(i));
        item->setData(MODEL_RECORDS_TREE_COLUMN_IS_RECORD,Qt::UserRole,QVariant(false));
        item->setData(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole,QVariant(false));
        item->setText(MODEL_RECORDS_TREE_COLUMN_MARKED,rModel.getName());

        item->setSelected(selectedIDs.constFind(ModelRecordsTreeRecordID(i,false,0)) != selectedIDs.constEnd());

        QList<QString> recordFiles = rModel.getRecordFiles(false);

        for (int j=0;j<recordFiles.size();j++)
        {
            if (QFile::exists(recordFiles[j]))
            {
                QFileInfo fi(recordFiles[j]);
                QString elipsizeFileName = fi.baseName();

                QTreeWidgetItem *childItem = new QTreeWidgetItem(item);
                childItem->setData(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,Qt::UserRole,QVariant(i));
                childItem->setData(MODEL_RECORDS_TREE_COLUMN_IS_RECORD,Qt::UserRole,QVariant(true));
                childItem->setData(MODEL_RECORDS_TREE_COLUMN_RECORD_NUMBER,Qt::DisplayRole,QVariant(j+1));
                childItem->setText(MODEL_RECORDS_TREE_COLUMN_RECORD_FILENAME,elipsizeFileName);
                childItem->setText(MODEL_RECORDS_TREE_COLUMN_PATH_FILENAME,recordFiles[j]);
                uint recordNumber = 0;
                if (rModel.getTimeSolver().getEnabled())
                {
                    recordNumber = rModel.getTimeSolver().getCurrentTimeStep();
                }
                else
                {
                    if (rModel.getProblemTaskTree().getProblemTypeMask() & R_PROBLEM_STRESS_MODAL)
                    {
                        recordNumber = rModel.getProblemSetup().getModalSetup().getMode();
                    }
                }
                if (Session::getInstance().isModelSelected(i) && recordNumber == uint(j))
                {
                    childItem->setIcon(MODEL_RECORDS_TREE_COLUMN_MARKED,QIcon(":/icons/media/pixmaps/range-play_play.svg"));
                    childItem->setData(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole,QVariant(true));
                }
                else
                {
                    childItem->setData(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole,QVariant(false));
                }

                childItem->setSelected(selectedIDs.constFind(ModelRecordsTreeRecordID(i,true,j+1)) != selectedIDs.constEnd());
            }
        }
        item->setExpanded(true);
    }

    for (uint i=0;i<nTopLevelItems;i++)
    {
        this->takeTopLevelItem(0);
    }

    this->resizeColumnToContents(MODEL_RECORDS_TREE_COLUMN_RECORD_NUMBER);
    this->resizeColumnToContents(MODEL_RECORDS_TREE_COLUMN_RECORD_FILENAME);
    this->resizeColumnToContents(MODEL_RECORDS_TREE_COLUMN_MARKED);

    this->blockSignals(false);

    Session::getInstance().unlock();
}

void ModelRecordsTree::onModelSelectionChanged(uint modelID)
{
    this->blockSignals(true);
    QTreeWidgetItemIterator it(this);
    while (*it)
    {
        if ((*it)->data(MODEL_RECORDS_TREE_COLUMN_IS_RECORD,Qt::UserRole).toBool() &&
            (*it)->data(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole).toBool() &&
            (*it)->data(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,Qt::UserRole).toUInt() == modelID)
        {
            if (Session::getInstance().isModelSelected(modelID))
            {
                (*it)->setIcon(MODEL_RECORDS_TREE_COLUMN_MARKED,QIcon(":/icons/media/pixmaps/range-play_play.svg"));
            }
            else
            {
                (*it)->setData(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::DecorationRole,QVariant());
            }
        }
        ++it;
    }
    this->blockSignals(false);
}

void ModelRecordsTree::onModelChanged(uint modelID)
{
    this->populate();
}

void ModelRecordsTree::onItemChanged(QTreeWidgetItem *item, int column)
{
    if (column != MODEL_RECORDS_TREE_COLUMN_MARKED)
    {
        return;
    }

    bool recordMarkedIndicator = false;

    this->blockSignals(true);

    if (item->data(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole).toBool() &&
        Session::getInstance().isModelSelected(item->data(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,Qt::UserRole).toUInt()))
    {
        item->setIcon(MODEL_RECORDS_TREE_COLUMN_MARKED,QIcon(":/icons/media/pixmaps/range-play_play.svg"));
        recordMarkedIndicator = true;
    }
    else
    {
        item->setData(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::DecorationRole,QVariant());
    }

    this->blockSignals(false);

    if (recordMarkedIndicator)
    {
        emit this->recordMarked(item->data(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,Qt::UserRole).toUInt(),
                                item->text(MODEL_RECORDS_TREE_COLUMN_PATH_FILENAME));
    }
}

void ModelRecordsTree::onItemActivated(QTreeWidgetItem *item, int column)
{
    uint modelID = item->data(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,Qt::UserRole).toUInt();

    QTreeWidgetItemIterator it(this);
    while (*it)
    {
        if ((*it)->data(MODEL_RECORDS_TREE_COLUMN_IS_RECORD,Qt::UserRole).toBool() &&
            (*it)->data(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole).toBool() &&
            (*it)->data(MODEL_RECORDS_TREE_COLUMN_MODEL_ID,Qt::UserRole).toUInt() == modelID)
        {
            (*it)->setData(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole,false);
        }
        ++it;
    }
    if (!item->data(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole).toBool())
    {
        item->setData(MODEL_RECORDS_TREE_COLUMN_MARKED,Qt::UserRole,true);
    }
}
