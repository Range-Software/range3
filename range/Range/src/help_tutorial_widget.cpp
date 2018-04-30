/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   help_tutorial_widget.cpp                                 *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   5-th December 2017                                       *
 *                                                                   *
 *  DESCRIPTION: Help tutorial widget class definition               *
 *********************************************************************/

#include <rmlib.h>

#include "help_tutorial_widget.h"
#include "main_settings.h"

HelpTutorialWidget::HelpTutorialWidget(QWidget *parent)
    : DocumentWidget(RFileManager::buildPath(MainSettings::getInstance().getApplicationSettings()->getHelpDir(),"tutorial_index.html"),parent)
{
    QStringList tutorials;

    tutorials.append("Draw cube");
    tutorials.append("Transform geometry");
    tutorials.append("Stress and deformation analysis");
    tutorials.append("Multiphysics");
    tutorials.append("Contaminant dispersion in fluids");

    foreach (const QString &tutorialName, tutorials)
    {
        this->addListItem(tutorialName,
                          RFileManager::buildPath(MainSettings::getInstance().getApplicationSettings()->getHelpDir(),"tutorial_" + tutorialName + ".html"));
    }
}
