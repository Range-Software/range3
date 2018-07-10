/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_material_list.cpp                                    *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   8-th August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: Material list class definition                      *
 *********************************************************************/

#include <sstream>

#include <QDir>
#include <QVector>

#include <rblib.h>

#include "rml_file_manager.h"
#include "rml_material_list.h"

void RMaterialList::_init(const RMaterialList *pMaterialList)
{
    if (pMaterialList)
    {
        this->materials = pMaterialList->materials;
        this->storePath = pMaterialList->storePath;
    }
} /* RMaterialList::_init */


RMaterialList::RMaterialList()
{
    this->_init();
} /* RMaterialList::RMaterialList */


RMaterialList::RMaterialList(const RMaterialList &materialList)
{
    this->_init(&materialList);
} /* RMaterialList::RMaterialList (copy) */


RMaterialList::~RMaterialList()
{
} /* RMaterialList::~RMaterialList */


RMaterialList &RMaterialList::operator =(const RMaterialList &materialList)
{
    this->_init(&materialList);
    return (*this);
} /* RMaterialList::operator = */


unsigned int RMaterialList::size(void) const
{
    return (unsigned int)this->materials.size();
} /* RMaterialList::size */


const QString &RMaterialList::getStorePath(void) const
{
    return this->storePath;
} /* RMaterialList::getStorePath */


void RMaterialList::setStorePath(const QString &storePath)
{
    this->storePath = storePath;
    this->readStore();
} /* RMaterialList::setStorePath */


void RMaterialList::add(RMaterial &material)
{
    if (this->containName(material.getName()))
    {
        RMaterial oldMaterial = this->get(material.getName());
        material.setID(oldMaterial.getID());
    }
    else
    {
        if (!this->validateID(material.getID()))
        {
            material.setID(this->findAvailableID());
        }
    }

    QString baseName = material.findMaterialFileName(true);
    this->materials[material.getName()] = baseName;
    QString fileName = RFileManager::buildPath(this->getStorePath(),baseName);
    try
    {
        material.write(fileName);
    }
    catch (const RError &error)
    {
        RLogger::error("Failed to write the material to file \'%s\' with error: %s\n",fileName.toUtf8().constData(),error.getMessage().toUtf8().constData());
    }
} /* RMaterialList::add */


unsigned int RMaterialList::remove(const QString &materialName, unsigned int *id)
{
    if (id)
    {
        RMaterial material = this->get(materialName);
        (*id) = material.getID();
    }

    QString fileName = RFileManager::buildPath(this->getStorePath(),this->materials[materialName]);
    try
    {
        RLogger::info("Removing material \'%s\'\n",fileName.toUtf8().constData());
        RFileManager::remove(fileName);
    }
    catch (const RError &error)
    {
        RLogger::error("Failed to delete the material file '%s' with error: %s\n",error.getMessage().toUtf8().constData());
    }

    return (unsigned int)this->materials.erase(materialName);
} /* RMaterialList::remove */


bool RMaterialList::rename(const QString &materialOldName, const QString &materialNewName, bool override)
{
    if (materialOldName == materialNewName)
    {
        return false;
    }

    std::map<QString,QString>::const_iterator iter = this->materials.find(materialNewName);
    if (iter == this->materials.end() || (iter != this->materials.end() && override))
    {
        if (iter != this->materials.end())
        {
            this->remove(materialNewName);
        }
        this->materials[materialNewName] = this->materials[materialOldName];
        this->materials.erase(materialOldName);
        RMaterial material = this->get(materialNewName);
        material.setName(materialNewName);
        QString fileName = RFileManager::buildPath(this->getStorePath(),this->materials[materialNewName]);
        try
        {
            RLogger::info("Writing material \'%s\'\n",fileName.toUtf8().constData());
            material.write(fileName);
        }
        catch (const RError &error)
        {
            RLogger::error("Failed to write the material to file \'%s\' with error: %s\n",fileName.toUtf8().constData(),error.getMessage().toUtf8().constData());
        }
        return true;
    }
    return false;
} /* RMaterialList::rename */


RMaterial RMaterialList::get(const QString &materialName)
{
    RMaterial material;

    if (this->containName(materialName))
    {
        QString fileName = RFileManager::buildPath(this->getStorePath(),this->materials[materialName]);
        try
        {
            material.read(fileName);
        }
        catch (const RError &error)
        {
            RLogger::error("Failed to read the material from file \'%s\' with error: %s\n",fileName.toUtf8().constData(),error.getMessage().toUtf8().constData());
            material.setName(materialName);
            try
            {
                material.write(fileName);
            }
            catch (const RError &error)
            {
                RLogger::error("Failed to write the material to file \'%s\' with error: %s\n",fileName.toUtf8().constData(),error.getMessage().toUtf8().constData());
            }
        }
    }
    else
    {
        material.setName(materialName);
        this->add(material);
    }
    return material;
} /* RMaterialList::get */


bool RMaterialList::find(const QString &materialName, RMaterial &material) const
{
    std::map<QString,QString>::const_iterator iter = this->materials.find(materialName);

    if (iter != this->materials.end())
    {
        material.read(this->getStorePath() + iter->second);
        return true;
    }
    else
    {
        return false;
    }
} /* RMaterialList::find */

QString RMaterialList::getFileName(const QString &materialName) const
{
    std::map<QString,QString>::const_iterator iter = this->materials.find(materialName);

    if (iter != this->materials.end())
    {
        return iter->second;
    }
    else
    {
        return QString();
    }
} /* RMaterialList::getFileName */


bool RMaterialList::containName(const QString &materialName) const
{
    if (this->materials.find(materialName) == this->materials.end())
    {
        return false;
    }
    else
    {
        return true;
    }
} /* RMaterialList::containName */


std::vector<QString> RMaterialList::getMaterialNames(const std::vector<RMaterialPropertyType> &propertyTypes) const
{
    std::vector<QString> materialNames;
    std::map<QString,QString>::const_iterator iter;

    for (iter=this->materials.begin();
         iter != this->materials.end();
         iter++)
    {
        if (propertyTypes.size() == 0)
        {
            materialNames.push_back(iter->first);
            continue;
        }
        RMaterial material;
        if (this->find(iter->first,material))
        {
            if (material.hasProperties(propertyTypes))
            {
                materialNames.push_back(iter->first);
                continue;
            }
        }
    }

    return materialNames;
} /* RMaterialList::getMaterialNames */


std::vector<RMaterial> RMaterialList::getMaterials(const std::vector<RMaterialPropertyType> &propertyTypes) const
{
    std::vector<QString> materialNames = this->getMaterialNames(propertyTypes);
    std::vector<RMaterial> materialList;

    for (unsigned int i=0;i<materialNames.size();i++)
    {
        RMaterial material;
        if (this->find(materialNames[i],material))
        {
            materialList.push_back(material);
        }
    }

    return materialList;
} /* RMaterialList::getMaterials */


QString RMaterialList::findID(unsigned int id) const
{
    std::map<QString,QString>::const_iterator iter;

    for (iter=this->materials.begin();
         iter != this->materials.end();
         iter++)
    {
        RMaterial material;
        if (this->find(iter->first,material))
        {
            if (material.getID() == id)
            {
                return material.getName();
            }
        }
    }

    return QString();
} /* RMaterialList::find */


bool RMaterialList::validateID(unsigned int id) const
{
    if (id == 0)
    {
        return false;
    }
    QString materiaName = this->findID(id);
    if (materiaName.isEmpty())
    {
        return true;
    }
    else
    {
        return false;
    }
} /* RMaterialList::validateID */


unsigned int RMaterialList::findAvailableID(void) const
{
    unsigned int id = 1;

    while (!this->validateID(id))
    {
        id++;
    }

    return id;
} /* RMaterialList::findAvailableID */


void RMaterialList::readStore(void)
{
    RLogger::info("Reading material store \'%s\'\n",this->storePath.toUtf8().constData());
    RLogger::indent();

    this->materials.clear();

    QVector<QString> directoryList;
    QVector<QString> fileList;

    try
    {
        RFileManager::listDirectory(this->storePath,directoryList,fileList);
    }
    catch (const RError &error)
    {
        RLogger::error("Failed to list directory with error: %s\n", error.getMessage().toUtf8().constData());
        RLogger::unindent();
        return;
    }

    RLogger::info("Found %d files\n",fileList.size());

    for (int i=0;i<fileList.size();i++)
    {
        QString fileName = RFileManager::buildPath(this->storePath,fileList[i]);
        RMaterial material;
        try
        {
            RLogger::info("Found material file \'%s\'\n",fileName.toUtf8().constData());
            material.read(fileName);
        }
        catch (const RError &error)
        {
            RLogger::error("Failed to read material from file \'%s\' with error: %s\n", fileName.toUtf8().constData(), error.getMessage().toUtf8().constData());
            continue;
        }
        this->materials[material.getName()] = material.findMaterialFileName(true);
    }

    RLogger::unindent();
} /* RMaterialList::readStore */
