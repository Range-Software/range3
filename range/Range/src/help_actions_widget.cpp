/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   help_actions_widget.cpp                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   26-th November 2017                                      *
 *                                                                   *
 *  DESCRIPTION: Help actions widget class definition                *
 *********************************************************************/

#include <QSplitter>
#include <QVBoxLayout>
#include <QListWidget>
#include <QTextBrowser>

#include "help_actions_widget.h"

HelpActionsWidget::HelpActionsWidget(const ActionDefinition *actionDefinition, QWidget *parent)
    : DocumentWidget(parent)
{
    QList<ActionGroupType> actionGroupTypes = ActionDefinitionItem::getGroupTypes();

    for (int i=0;i<actionGroupTypes.size();i++)
    {
        QList<ActionType> actionTypes = actionDefinition->getActionTypes(actionGroupTypes[i]);
        for (int j=0;j<actionTypes.size();j++)
        {
            if (!ACTION_TYPE_IS_ACTION(actionTypes[j]))
            {
                continue;
            }
            QString iconName(actionDefinition->getIcon(actionTypes[j]));
            if (iconName.isEmpty())
            {
                iconName = ":/icons/general/pixmaps/range-generic_action.svg";
            }
            this->addListItem(QIcon(iconName),
                              ActionDefinitionItem::getGroupName(actionGroupTypes[i]) + " >> " + actionDefinition->getText(actionTypes[j]),
                              actionDefinition->getText(actionTypes[j]));
        }
    }
}
