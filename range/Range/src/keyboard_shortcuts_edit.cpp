/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   keyboard_shortcuts_edit.cpp                              *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   10-th September 2014                                     *
 *                                                                   *
 *  DESCRIPTION: Keyboard shortcuts editor class definition          *
 *********************************************************************/

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

#include "keyboard_shortcuts_edit.h"

KeyboardShortcutsEdit::KeyboardShortcutsEdit(const ActionDefinition *actionDefinition, QWidget *parent) :
    QWidget(parent),
    actionDefinition(actionDefinition)
{
    QVBoxLayout *topLayout = new QVBoxLayout;
    this->setLayout(topLayout);

    QGroupBox *keyboardShortcutsBox = new QGroupBox(tr("Keayboard Shortcuts"));
    topLayout->addWidget(keyboardShortcutsBox);

    QGridLayout *keyboardShortcutsLayout = new QGridLayout;
    keyboardShortcutsBox->setLayout(keyboardShortcutsLayout);

    this->populateTree();

    keyboardShortcutsLayout->addWidget(this->tree,0,0,1,2);

    QObject::connect(this->tree,
                     &QTreeWidget::itemSelectionChanged,
                     this,
                     &KeyboardShortcutsEdit::onSelectionChanged);

    QPushButton *resetAllButton = new QPushButton(tr("Reset All"));
    resetAllButton->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    keyboardShortcutsLayout->addWidget(resetAllButton,1,0,1,1);

    QObject::connect(resetAllButton,
                     &QPushButton::clicked,
                     this,
                     &KeyboardShortcutsEdit::onResetAllClicked);

    QGroupBox *shortcutBox = new QGroupBox(tr("Shortcut"));
    topLayout->addWidget(shortcutBox);

    QHBoxLayout *shortcutLayout = new QHBoxLayout;
    shortcutBox->setLayout(shortcutLayout);

    QLabel *shortcutLabel = new QLabel(tr("Target:"));
    shortcutLabel->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    shortcutLayout->addWidget(shortcutLabel);

    this->lineEdit = new ShortcutLineEdit;
    this->lineEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);
    this->lineEdit->setDisabled(true);
    shortcutLayout->addWidget(this->lineEdit);

    QObject::connect(this->lineEdit,
                     &ShortcutLineEdit::shortcutChanged,
                     this,
                     &KeyboardShortcutsEdit::onShortcutChanged);
}

void KeyboardShortcutsEdit::resetToDefault(void)
{
    QList<ActionDefinitionItem> actionDesc;

    ActionDefinition::generateActionDescList(actionDesc);

    for (int i=0;i<this->tree->topLevelItemCount();i++)
    {
        for (int j=0;j<this->tree->topLevelItem(i)->childCount();j++)
        {
            ActionType type = ActionType(this->tree->topLevelItem(i)->child(j)->data(0,Qt::UserRole).toInt());
            const QString &shortcut = actionDesc[type].getShortcut();
            if (this->tree->topLevelItem(i)->child(j)->text(1) != shortcut)
            {
                this->tree->blockSignals(true);
                this->tree->topLevelItem(i)->child(j)->setText(1,shortcut);
                this->tree->blockSignals(false);
                emit this->shortcutChanged(type,shortcut);
            }
        }
    }
}

void KeyboardShortcutsEdit::populateTree(void)
{
    this->tree = new QTreeWidget;
    this->tree->setSelectionMode(QAbstractItemView::SingleSelection);
    this->tree->setSortingEnabled(true);
    this->tree->setColumnCount(2);
    this->tree->sortByColumn(0,Qt::AscendingOrder);

    QTreeWidgetItem* headerItem = new QTreeWidgetItem();
    headerItem->setText(0,QString("Action"));
    headerItem->setText(1,QString("Shortcut"));
    this->tree->setHeaderItem(headerItem);
    this->tree->setRootIsDecorated(true);

    QList<ActionGroupType> actionGroupTypes = ActionDefinitionItem::getGroupTypes();

    for (int i=0;i<actionGroupTypes.size();i++)
    {
        QTreeWidgetItem *itemGroup = new QTreeWidgetItem(this->tree);

        itemGroup->setText(0,ActionDefinitionItem::getGroupName(actionGroupTypes[i]));
        itemGroup->setData(0,Qt::UserRole,QVariant(ACTION_NONE));
        itemGroup->setForeground(0,this->palette().brush(QPalette::Active,QPalette::Text));
        QFont font(itemGroup->font(0));
        font.setBold(true);
        itemGroup->setFont(0,font);

        QList<ActionType> actionTypes = this->actionDefinition->getActionTypes(actionGroupTypes[i]);
        for (int j=0;j<actionTypes.size();j++)
        {
            if (!ACTION_TYPE_IS_ACTION(actionTypes[j]))
            {
                continue;
            }

            QTreeWidgetItem *item = new QTreeWidgetItem(itemGroup);

            item->setText(0,this->actionDefinition->getText(actionTypes[j]));
            item->setData(0,Qt::UserRole,QVariant(actionTypes[j]));
            item->setText(1,this->actionDefinition->getShortCut(actionTypes[j]));
        }
    }

    this->tree->expandAll();

    this->tree->resizeColumnToContents(0);
    this->tree->resizeColumnToContents(1);

    this->tree->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

QList<QTreeWidgetItem *> KeyboardShortcutsEdit::findItems(ActionType type)
{
    QList<QTreeWidgetItem*> items;
    for (int i=0;i<this->tree->topLevelItemCount();i++)
    {
        for (int j=0;j<this->tree->topLevelItem(i)->childCount();j++)
        {
            QTreeWidgetItem *item = this->tree->topLevelItem(i)->child(j);
            if (ActionType(item->data(0,Qt::UserRole).toInt()) == type)
            {
                items.push_back(item);
            }
        }
    }
    return items;
}

QList<QTreeWidgetItem *> KeyboardShortcutsEdit::findItems(const QString &shortcut)
{
    QList<QTreeWidgetItem*> items;
    for (int i=0;i<this->tree->topLevelItemCount();i++)
    {
        for (int j=0;j<this->tree->topLevelItem(i)->childCount();j++)
        {
            QTreeWidgetItem *item = this->tree->topLevelItem(i)->child(j);
            if (item->text(1) == shortcut)
            {
                items.push_back(item);
            }
        }
    }
    return items;
}

void KeyboardShortcutsEdit::onSelectionChanged(void)
{
    QList<QTreeWidgetItem *> items = this->tree->selectedItems();

    this->lineEdit->setDisabled(true);
    this->lineEdit->clear();

    for (int i=0;i<items.size();i++)
    {
        if (ACTION_TYPE_IS_ACTION(ActionType(items[i]->data(0,Qt::UserRole).toInt())))
        {
            QString shortcut = items[i]->text(1);
            this->lineEdit->setText(shortcut);
            this->lineEdit->setEnabled(true);
            return;
        }
    }
}

void KeyboardShortcutsEdit::onResetAllClicked(void)
{
    this->resetToDefault();
}

void KeyboardShortcutsEdit::onShortcutChanged(const QString &shortcut)
{
    QList<QTreeWidgetItem *> selectedItems = this->tree->selectedItems();

    if (!shortcut.isEmpty())
    {
        QList<QTreeWidgetItem*> previousItems = this->findItems(shortcut);

        if (previousItems.size() > 0)
        {
            QString previousActions;
            for (int j=0;j<previousItems.size();j++)
            {
                previousActions += " <b>" + previousItems[j]->text(0) + "</b>";
                if (j<previousItems.size()-1)
                {
                    previousActions += ",";
                }
            }

            QString question = tr("Shortcut ")
                             + QString("<b>") + shortcut + QString("</b> ")
                             + tr("is already assigned to action")
                             + previousActions
                             + QString(".<br/>")
                             + tr("Are you sure you want to reassign this shortcut?");
            int response = QMessageBox::question(this,
                                                 tr("Assign shortcut?"),
                                                 question,
                                                 QMessageBox::Yes | QMessageBox::No);
            if (response != QMessageBox::Yes)
            {
                return;
            }
        }

        for (int i=0;i<previousItems.size();i++)
        {
            previousItems[i]->setText(1,QString());
            emit this->shortcutChanged(ActionType(previousItems[i]->data(0,Qt::UserRole).toInt()),QString());
        }
    }

    for (int i=0;i<selectedItems.size();i++)
    {
        ActionType type = ActionType(selectedItems[i]->data(0,Qt::UserRole).toInt());

        this->lineEdit->setText(shortcut);
        selectedItems[i]->setText(1,shortcut);

        emit this->shortcutChanged(type,shortcut);
    }
}
