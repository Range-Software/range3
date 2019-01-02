/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   help_tips_widget.h                                       *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th November 2017                                      *
 *                                                                   *
 *  DESCRIPTION: Help tips and tricks widget class declaration       *
 *********************************************************************/

#ifndef HELP_TIPS_WIDGET_H
#define HELP_TIPS_WIDGET_H

#include "document_widget.h"

class HelpTipsWidget : public DocumentWidget
{

    Q_OBJECT

    public:

        //! Constructor
        explicit HelpTipsWidget(QWidget *parent = nullptr);

};

#endif // HELP_TIPS_WIDGET_H
