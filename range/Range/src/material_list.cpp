/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   material_list.cpp                                        *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   9-th August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: Material list (database) definition                 *
 *********************************************************************/

#include "material_list.h"

MaterialList::MaterialList()
{
}

void MaterialList::initialize(void)
{
}

MaterialList &MaterialList::getInstance(void)
{
    static MaterialList materialList;
    return materialList;
}

void MaterialList::addMaterial(RMaterial &material, bool isNew)
{
    unsigned int nMaterials = this->size();
    RMaterialList::add(material);
    if (nMaterials != this->size())
    {
        emit this->materialAdded(material.getName(), isNew);
    }
    else
    {
        emit this->materialChanged(material.getName());
    }
}

void MaterialList::add(RMaterial &material)
{
    this->addMaterial(material,false);
}

unsigned int MaterialList::remove(const QString &materialName, unsigned int *id)
{
    uint removedID = 0;
    unsigned int nRemoved = RMaterialList::remove(materialName,&removedID);
    if (nRemoved > 0)
    {
        emit this->materialRemoved(materialName,removedID);
    }
    if (id)
    {
        (*id) = removedID;
    }
    return nRemoved;
}

bool MaterialList::rename(const QString &materialOldName, const QString &materialNewName, bool override)
{
    if (materialNewName.isEmpty())
    {
        return false;
    }
    if (RMaterialList::rename(materialOldName, materialNewName, override))
    {
        emit this->materialRenamed(materialOldName, materialNewName);
        return true;
    }
    return false;
}

RMaterial MaterialList::get(const QString &materialName)
{
    unsigned int nMaterials = this->size();
    RMaterial material = RMaterialList::get(materialName);
    if (nMaterials != this->size())
    {
        emit this->materialAdded(materialName,false);
    }
    return material;
}

void MaterialList::selectMaterial(const QString &materialName)
{
    this->selectedMaterialName = materialName;
    emit this->materialSelected(materialName);
}

const QString &MaterialList::getSelectedMaterialName(void) const
{
    return this->selectedMaterialName;
}

void MaterialList::signalMaterialChanged(const QString &materialName)
{
    emit this->materialChanged(materialName);
}

void MaterialList::readStore(void)
{
    this->RMaterialList::readStore();

    std::map<QString,QString>::const_iterator iter;

    for (iter=this->materials.begin();iter!=this->materials.end();++iter)
    {
        emit this->materialAdded(iter->first,false);
    }
}
