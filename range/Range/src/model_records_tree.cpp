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

ModelRecordsTree::ModelRecordsTree(QWidget *parent) :
    QTreeWidget(parent)
{
    this->setColumnCount(ModelRecordsTree::NColumns);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setRootIsDecorated(false);
    this->setAutoScroll(false);
    QTreeWidgetItem* headerItem = new QTreeWidgetItem();
    headerItem->setText(ModelRecordsTree::RecordNumber,QString(tr("Record")));
    headerItem->setText(ModelRecordsTree::RecordFileName,QString(tr("File name")));
    headerItem->setIcon(ModelRecordsTree::Marked,QIcon(":/icons/media/pixmaps/range-play_play.svg"));
    this->setHeaderItem(headerItem);

    this->setColumnHidden(ModelRecordsTree::ModelID,true);
    this->setColumnHidden(ModelRecordsTree::IsRecord,true);
    this->setColumnHidden(ModelRecordsTree::PathFileName,true);

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
            if ((*it)->data(ModelRecordsTree::IsRecord,Qt::UserRole).toBool() &&
                (*it)->data(ModelRecordsTree::ModelID,Qt::UserRole).toUInt() == modelID)
            {
                if ((*it)->data(ModelRecordsTree::Marked,Qt::UserRole).toBool())
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
            if ((*it)->data(ModelRecordsTree::IsRecord,Qt::UserRole).toBool() &&
                (*it)->data(ModelRecordsTree::ModelID,Qt::UserRole).toUInt() == modelID)
            {
                if ((*it)->data(ModelRecordsTree::Marked,Qt::UserRole).toBool())
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
            if ((*it)->data(ModelRecordsTree::IsRecord,Qt::UserRole).toBool() &&
                (*it)->data(ModelRecordsTree::ModelID,Qt::UserRole).toUInt() == modelID)
            {
                if ((*it)->data(ModelRecordsTree::Marked,Qt::UserRole).toBool())
                {
                    emit this->recordMarked((*it)->data(ModelRecordsTree::ModelID,Qt::UserRole).toUInt(),
                                            (*it)->text(ModelRecordsTree::PathFileName));
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

        QTreeWidgetItem *prevItem = nullptr;

        QTreeWidgetItemIterator it(this);
        while (*it)
        {
            if ((*it)->data(ModelRecordsTree::Marked,Qt::UserRole).toBool() &&
                (*it)->data(ModelRecordsTree::ModelID,Qt::UserRole).toUInt() == modelID)
            {
                if (prevItem)
                {
                    (*it)->setData(ModelRecordsTree::Marked,Qt::UserRole,QVariant(false));
                    prevItem->setData(ModelRecordsTree::Marked,Qt::UserRole,QVariant(true));
                }
                break;
            }
            if ((*it)->data(ModelRecordsTree::IsRecord,Qt::UserRole).toBool() &&
                (*it)->data(ModelRecordsTree::ModelID,Qt::UserRole).toUInt() == modelID)
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

        QTreeWidgetItem *prevItem = nullptr;

        QTreeWidgetItemIterator it(this);
        while (*it)
        {
            if (markItem)
            {
                if ((*it)->data(ModelRecordsTree::ModelID,Qt::UserRole).toUInt() == modelID)
                {
                    prevItem->setData(ModelRecordsTree::Marked,Qt::UserRole,false);
                    (*it)->setData(ModelRecordsTree::Marked,Qt::UserRole,true);
                }
                break;
            }
            if ((*it)->data(ModelRecordsTree::Marked,Qt::UserRole).toBool() &&
                (*it)->data(ModelRecordsTree::ModelID,Qt::UserRole).toUInt() == modelID)
            {
                modelID = (*it)->data(ModelRecordsTree::ModelID,Qt::UserRole).toUInt();
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
            if ((*it)->data(ModelRecordsTree::ModelID,Qt::UserRole).toUInt() == modelID &&
                (*it)->data(ModelRecordsTree::IsRecord,Qt::UserRole).toBool())
            {
                (*it)->setData(ModelRecordsTree::Marked,Qt::UserRole,isFirst);
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
        QTreeWidgetItem *lastItem = nullptr;

        QTreeWidgetItemIterator it(this);
        while (*it)
        {
            if ((*it)->data(ModelRecordsTree::ModelID,Qt::UserRole).toUInt() == modelID)
            {
                lastItem = (*it);
                itemID1++;
            }
            ++it;
        }

        if (lastItem && !lastItem->data(ModelRecordsTree::Marked,Qt::UserRole).toBool())
        {
            lastItem->setData(ModelRecordsTree::Marked,Qt::UserRole,true);

            QTreeWidgetItemIterator it(this);
            while (*it)
            {
                if ((*it)->data(ModelRecordsTree::ModelID,Qt::UserRole).toUInt() == modelID)
                {
                    itemID2++;
                    if (itemID1 != itemID2)
                    {
                        (*it)->setData(ModelRecordsTree::Marked,Qt::UserRole,false);
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
        ModelRecordsTreeRecordID recordID(selectedItemList.at(i)->data(ModelRecordsTree::ModelID,Qt::UserRole).toUInt(),
                                          selectedItemList.at(i)->data(ModelRecordsTree::IsRecord,Qt::UserRole).toBool(),
                                          selectedItemList.at(i)->data(ModelRecordsTree::RecordNumber,Qt::DisplayRole).toUInt());
        selectedIDs[recordID] = true;
    }

    uint nTopLevelItems = this->topLevelItemCount();

    uint nModels = Session::getInstance().getNModels();

    for (uint i=0;i<nModels;i++)
    {
        Model &rModel = Session::getInstance().getModel(i);

        QTreeWidgetItem *item = new QTreeWidgetItem(this);
        item->setFirstColumnSpanned(true);
        item->setData(ModelRecordsTree::ModelID,Qt::UserRole,QVariant(i));
        item->setData(ModelRecordsTree::IsRecord,Qt::UserRole,QVariant(false));
        item->setData(ModelRecordsTree::Marked,Qt::UserRole,QVariant(false));
        item->setText(ModelRecordsTree::Marked,rModel.getName());

        if (selectedIDs.constFind(ModelRecordsTreeRecordID(i,false,0)) != selectedIDs.constEnd())
        {
            this->setCurrentItem(item);
        }

        QList<QString> recordFiles = rModel.getRecordFiles(false);

        for (int j=0;j<recordFiles.size();j++)
        {
            if (QFile::exists(recordFiles[j]))
            {
                QFileInfo fi(recordFiles[j]);
                QString elipsizeFileName = fi.baseName();

                QTreeWidgetItem *childItem = new QTreeWidgetItem(item);
                childItem->setData(ModelRecordsTree::ModelID,Qt::UserRole,QVariant(i));
                childItem->setData(ModelRecordsTree::IsRecord,Qt::UserRole,QVariant(true));
                childItem->setData(ModelRecordsTree::RecordNumber,Qt::DisplayRole,QVariant(j+1));
                childItem->setText(ModelRecordsTree::RecordFileName,elipsizeFileName);
                childItem->setText(ModelRecordsTree::PathFileName,recordFiles[j]);
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
                    childItem->setIcon(ModelRecordsTree::Marked,QIcon(":/icons/media/pixmaps/range-play_play.svg"));
                    childItem->setData(ModelRecordsTree::Marked,Qt::UserRole,QVariant(true));
                }
                else
                {
                    childItem->setData(ModelRecordsTree::Marked,Qt::UserRole,QVariant(false));
                }

                if (selectedIDs.constFind(ModelRecordsTreeRecordID(i,true,j+1)) != selectedIDs.constEnd())
                {
                    this->setCurrentItem(childItem);
                }
            }
        }
        item->setExpanded(true);
    }

    for (uint i=0;i<nTopLevelItems;i++)
    {
        this->takeTopLevelItem(0);
    }

    this->resizeColumnToContents(ModelRecordsTree::RecordNumber);
    this->resizeColumnToContents(ModelRecordsTree::RecordFileName);
    this->resizeColumnToContents(ModelRecordsTree::Marked);

    this->blockSignals(false);

    Session::getInstance().unlock();
}

void ModelRecordsTree::onModelSelectionChanged(uint modelID)
{
    this->blockSignals(true);
    QTreeWidgetItemIterator it(this);
    while (*it)
    {
        if ((*it)->data(ModelRecordsTree::IsRecord,Qt::UserRole).toBool() &&
            (*it)->data(ModelRecordsTree::Marked,Qt::UserRole).toBool() &&
            (*it)->data(ModelRecordsTree::ModelID,Qt::UserRole).toUInt() == modelID)
        {
            if (Session::getInstance().isModelSelected(modelID))
            {
                (*it)->setIcon(ModelRecordsTree::Marked,QIcon(":/icons/media/pixmaps/range-play_play.svg"));
            }
            else
            {
                (*it)->setData(ModelRecordsTree::Marked,Qt::DecorationRole,QVariant());
            }
        }
        ++it;
    }
    this->blockSignals(false);
}

void ModelRecordsTree::onModelChanged(uint)
{
    this->populate();
}

void ModelRecordsTree::onItemChanged(QTreeWidgetItem *item, int column)
{
    if (column != ModelRecordsTree::Marked)
    {
        return;
    }

    bool recordMarkedIndicator = false;

    this->blockSignals(true);

    if (item->data(ModelRecordsTree::Marked,Qt::UserRole).toBool() &&
        Session::getInstance().isModelSelected(item->data(ModelRecordsTree::ModelID,Qt::UserRole).toUInt()))
    {
        item->setIcon(ModelRecordsTree::Marked,QIcon(":/icons/media/pixmaps/range-play_play.svg"));
        this->setCurrentItem(item);
        this->scrollToItem(item);
        recordMarkedIndicator = true;
    }
    else
    {
        item->setData(ModelRecordsTree::Marked,Qt::DecorationRole,QVariant());
    }

    this->blockSignals(false);

    if (recordMarkedIndicator)
    {
        emit this->recordMarked(item->data(ModelRecordsTree::ModelID,Qt::UserRole).toUInt(),
                                item->text(ModelRecordsTree::PathFileName));
    }
}

void ModelRecordsTree::onItemActivated(QTreeWidgetItem *item, int)
{
    uint modelID = item->data(ModelRecordsTree::ModelID,Qt::UserRole).toUInt();

    QTreeWidgetItemIterator it(this);
    while (*it)
    {
        if ((*it)->data(ModelRecordsTree::IsRecord,Qt::UserRole).toBool() &&
            (*it)->data(ModelRecordsTree::Marked,Qt::UserRole).toBool() &&
            (*it)->data(ModelRecordsTree::ModelID,Qt::UserRole).toUInt() == modelID)
        {
            (*it)->setData(ModelRecordsTree::Marked,Qt::UserRole,false);
        }
        ++it;
    }
    if (!item->data(ModelRecordsTree::Marked,Qt::UserRole).toBool())
    {
        item->setData(ModelRecordsTree::Marked,Qt::UserRole,true);
    }
}
