/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   help_actions_widget.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th November 2017                                      *
 *                                                                   *
 *  DESCRIPTION: Help actions widget class declaration               *
 *********************************************************************/

#ifndef __HELP_ACTIONS_WIDGET_H__
#define __HELP_ACTIONS_WIDGET_H__

#include "action_definition.h"
#include "document_widget.h"

class HelpActionsWidget : public DocumentWidget
{

    Q_OBJECT

    public:

        //! Constructor
        explicit HelpActionsWidget(const ActionDefinition *actionDefinition, QWidget *parent = nullptr);

};

#endif // __HELP_ACTIONS_WIDGET_H__
