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
    : DocumentWidget(RFileManager::buildPath(MainSettings::getInstance().getApplicationSettings()->getHelpDir(),"general_index.html"),parent)
{
    QStringList general;

    general.append("Model");
    general.append("Problem");
    general.append("Problem type");
    general.append("Boundary condition");
    general.append("Initial condition");
    general.append("Environment condition");
    general.append("Material");
    general.append("Solver");
    general.append("Results");

    foreach (const QString &tipName, general)
    {
        this->addListItem(tipName,
                          RFileManager::buildPath(MainSettings::getInstance().getApplicationSettings()->getHelpDir(),"general_" + tipName + ".html"));
    }
}
