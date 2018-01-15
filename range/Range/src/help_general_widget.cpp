/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   help_general_widget.cpp                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   15-th January 2018                                       *
 *                                                                   *
 *  DESCRIPTION: Help general and tricks widget class definition     *
 *********************************************************************/

#include <rmlib.h>

#include "help_general_widget.h"
#include "main_settings.h"

HelpGeneralWidget::HelpGeneralWidget(QWidget *parent)
    : DocumentWidget(parent)
{
    QStringList general;

    general.append("Problem");
    general.append("Boundary condition");
    general.append("Initial condition");
    general.append("Environment condition");
    general.append("Material");

    foreach (const QString &tipName, general)
    {
        this->addListItem(tipName,
                          RFileManager::buildPath(MainSettings::getInstance().findHelpDir(),"general_" + tipName + ".html"));
    }
}
