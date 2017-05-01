/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   material_updater.cpp                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   source material (*.cpp)                                  *
 *  DATE:   17-th May 2016                                           *
 *                                                                   *
 *  DESCRIPTION: Material updater class definition                   *
 *********************************************************************/

#include <rmlib.h>

#include "material_updater.h"
#include "material_list.h"

MaterialUpdater::MaterialUpdater(QObject *parent)
    : Job(parent)
{

}

void MaterialUpdater::addMaterial(const QString &source)
{
    this->materials.append(source);
}

void MaterialUpdater::run(void)
{
    RLogger::info("Updating material database.\n");
    RLogger::indent();
    RProgressInitialize("Updating material database.");
    for (int i=0;i<this->materials.size();i++)
    {
        RProgressPrint(i+1,this->materials.size());

        RLogger::info("Reading material file \'%s\'.\n",this->materials.at(i).toUtf8().constData());

        RMaterial material;
        try
        {
            material.read(this->materials.at(i));
            if (MaterialList::getInstance().containName(material.getName()))
            {
                RLogger::info("Material database already contains material with name \'%s\'. Material will not be imported.\n",material.getName().toUtf8().constData());
            }
            else
            {
                RLogger::info("Inserting material \'%s\' into database.\n",material.getName().toUtf8().constData());
                MaterialList::getInstance().add(material);
            }
        }
        catch (const RError &rError)
        {
            RLogger::warning("Failed to update material from file \'%s\'. %s\n",
                             this->materials.at(i).toUtf8().constData(),
                             rError.getMessage().toUtf8().constData());
        }
    }
    RProgressFinalize();
    RLogger::unindent();
}
