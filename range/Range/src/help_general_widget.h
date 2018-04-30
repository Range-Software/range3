/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   help_general_widget.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   15-th January 2018                                       *
 *                                                                   *
 *  DESCRIPTION: Help general and tricks widget class declaration    *
 *********************************************************************/

#ifndef __HELP_GENERAL_WIDGET_H__
#define __HELP_GENERAL_WIDGET_H__

#include "document_widget.h"

class HelpGeneralWidget : public DocumentWidget
{

    Q_OBJECT

    public:

        //! Constructor
        explicit HelpGeneralWidget(QWidget *parent = nullptr);

};

#endif // __HELP_GENERAL_WIDGET_H__
