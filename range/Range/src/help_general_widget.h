/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   help_general_widget.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   15-th January 2018                                       *
 *                                                                   *
 *  DESCRIPTION: Help general and tricks widget class declaration    *
 *********************************************************************/

#ifndef HELP_GENERAL_WIDGET_H
#define HELP_GENERAL_WIDGET_H

#include "document_widget.h"

class HelpGeneralWidget : public DocumentWidget
{

    Q_OBJECT

    public:

        //! Constructor
        explicit HelpGeneralWidget(QWidget *parent = nullptr);

};

#endif // HELP_GENERAL_WIDGET_H
