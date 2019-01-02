/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   help_tutorial_widget.h                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   5-th December 2017                                       *
 *                                                                   *
 *  DESCRIPTION: Help tutorial widget class declaration              *
 *********************************************************************/

#ifndef HELP_TUTORIAL_WIDGET_H
#define HELP_TUTORIAL_WIDGET_H

#include "document_widget.h"

class HelpTutorialWidget : public DocumentWidget
{

    Q_OBJECT

    public:

        //! Constructor
        explicit HelpTutorialWidget(QWidget *parent = nullptr);

};

#endif // HELP_TUTORIAL_WIDGET_H
