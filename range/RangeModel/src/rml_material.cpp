/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_material.cpp                                         *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   16-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Material class definition                           *
 *********************************************************************/

#include <algorithm>
#include <stdexcept>

#include "rml_material.h"
#include "rml_file_io.h"
#include "rml_file_manager.h"

static const RVersion _version = RVersion(FILE_MAJOR_VERSION,FILE_MINOR_VERSION,FILE_RELEASE_VERSION);

static bool propertyTypeCompareFunc(const RMaterialProperty &p1,const RMaterialProperty &p2);

void RMaterial::_init (const RMaterial *pMaterial)
{
    this->id = 0;
    if (pMaterial)
    {
        this->id = pMaterial->id;
        this->name = pMaterial->name;
        this->state = pMaterial->state;
        this->properties = pMaterial->properties;
    }
} /* RMaterial::_init */


RMaterial::RMaterial (RMaterialState state)
{
    this->setState (state);
    this->_init ();
} /* RMaterial::RMaterial */


RMaterial::RMaterial (const RMaterial &material)
{
    this->_init (&material);
} /* RMaterial::RMaterial (copy) */


RMaterial::~RMaterial ()
{
} /* RMaterial::~RMaterial */


RMaterial & RMaterial::operator = (const RMaterial &material)
{
    this->_init (&material);
    return (*this);
} /* RMaterial::operator = */

bool RMaterial::propertiesEqual(const RMaterial &material) const
{
    if (this->properties.size() != material.properties.size())
    {
        return false;
    }
    for (unsigned int i=0;i<this->properties.size();i++)
    {
        if (this->properties[i] != material.properties[i])
        {
            return false;
        }
    }
    return true;
} /* RMaterial::propertiesEqual */


unsigned int RMaterial::getID (void) const
{
    return this->id;
} /* RMaterial::getID */


void RMaterial::setID (unsigned int id)
{
    this->id = id;
} /* RMaterial::setID */


const QString &RMaterial::getName(void) const
{
    return this->name;
} /* RMaterial::getName */


void RMaterial::setName (const QString &name)
{
    this->name = name;
} /* RMaterial::setName */


RMaterialState RMaterial::getState (void) const
{
    return this->state;
} /* RMaterial::getState */


void RMaterial::setState (RMaterialState state)
{
    this->state = state;
} /* RMaterial::setState */


unsigned int RMaterial::size (void) const
{
    return (unsigned int)this->properties.size();
} /* RMaterial::size */


bool RMaterial::add (const RMaterialProperty &property)
{
    unsigned int position = 0;

    position = this->findPosition (property.getType());

    if (position >= this->size())
    {
        this->properties.push_back(property);
        std::sort(this->properties.begin(),this->properties.end(),propertyTypeCompareFunc);
        return true;
    }
    else
    {
        this->properties[position] = property;
        return false;
    }
} /* RMaterial::add */


const RMaterialProperty & RMaterial::get (unsigned int position) const
{
    R_ERROR_ASSERT (position < this->size());
    return this->properties[position];
} /* RMaterial::get */


RMaterialProperty &RMaterial::get(unsigned int position)
{
    R_ERROR_ASSERT (position < this->size());
    return this->properties[position];
} /* RMaterial::get */


void RMaterial::remove (unsigned int position)
{
    std::vector<RMaterialProperty>::iterator iter;

    iter = this->properties.begin();
    std::advance (iter, position);

    this->properties.erase (iter);
} /* RMaterial::remove */


void RMaterial::clear(void)
{
    this->id = 0;
    this->name.clear();
    this->state = R_MATERIAL_STATE_NONE;
    this->properties.clear();
} /* RMaterial::clear */


unsigned int RMaterial::findPosition (RMaterialPropertyType type) const
{
    std::vector<RMaterialProperty>::const_iterator iter;
    unsigned int i = 0;

    for (iter = this->properties.begin();
         iter != this->properties.end();
         ++iter)
    {
        if (iter->getType() == type)
        {
           return i;
        }
        i++;
    }

    return this->size();
} /* RMaterial::find_position */


bool RMaterial::hasProperties (const std::vector<RMaterialPropertyType> &propertyTypes) const
{
    if (propertyTypes.size() == 0)
    {
        return false;
    }
    for (unsigned int i=0;i<propertyTypes.size();i++)
    {
        if (this->findPosition(propertyTypes[i]) == this->size())
        {
            return false;
        }
    }
    return true;
} /* RMaterial::hasProperties */


RMaterial RMaterial::generateDefault(void)
{
    RMaterial material;

    material.setName("Default");
    for (RMaterialPropertyType type=RMaterialPropertyType(R_MATERIAL_PROPERTY_NONE+1);
         type<RMaterialPropertyType(R_MATERIAL_PROPERTY_N_TYPES-1);
         type++)
    {
        RMaterialProperty property(type);
        material.add(property);
    }

    return material;
} /* RMaterial::generateDefault */


void RMaterial::read(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    QString ext = RFileManager::getExtension(fileName);

    try
    {
        if (ext == RMaterial::getDefaultFileExtension(false))
        {
            this->readAscii(fileName);
        }
        else if (ext == RMaterial::getDefaultFileExtension(true))
        {
            this->readBinary(fileName);
        }
        else
        {
            throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF, "Unknown extension \"" + ext + "\".");
        }
    }
    catch (RError &error)
    {
        throw error;
    }
    catch (const std::exception &error)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "%s", error.what());
    }
    catch (...)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "Unknown exception.");
    }
} /* RMaterial::read */


void RMaterial::write(const QString &fileName) const
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file was name provided.");
    }

    QString ext = RFileManager::getExtension(fileName);

    try
    {
        if (ext == RMaterial::getDefaultFileExtension(false))
        {
            this->writeAscii(fileName);
        }
        else if (ext == RMaterial::getDefaultFileExtension(true))
        {
            this->writeBinary(fileName);
        }
        else
        {
            throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF, "Unknown extension \"" + ext + "\".");
        }
    }
    catch (RError &error)
    {
        throw error;
    }
    catch (const std::exception &error)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "%s", error.what());
    }
    catch (...)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "Unknown exception.");
    }
} /* RMaterial::write */


void RMaterial::import(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file was name provided.");
    }

    try
    {
        RFile file(fileName,RFile::ASCII);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
        }

        QString flag;
        QString fVersion;
        uint nProperties = 0;
        QVector<QString> tableFlags;
        QVector<uint> tableSizes;

        while (true)
        {

            file.getTextStream() >> flag;
            if (file.getTextStream().status() != QTextStream::Ok)
            {
                if (file.getTextStream().atEnd())
                {
                    break;
                }
                throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read the file \'%s\'",fileName.toUtf8().constData());
            }

            if (flag == "!VERSION")
            {
                RFileIO::readAscii(file,fVersion);
            }
            else if (flag == "NAME:")
            {
                RFileIO::readAscii(file,this->name);
            }
            else if (flag == "NPROP:")
            {
                RFileIO::readAscii(file,nProperties);
            }
            else if (flag == "properties:")
            {
                QString propertyName;
                QString propertyDimension;
                double propertyValue;
                uint propertyNTableValues;
                for (uint i=0;i<nProperties;i++)
                {
                    RFileIO::readAscii(file,propertyName);
                    RFileIO::readAscii(file,propertyValue);
                    RFileIO::readAscii(file,propertyDimension);
                    RFileIO::readAscii(file,propertyNTableValues);

                    tableFlags.push_back(QString(propertyName + "/temperature:").replace(' ','_'));
                    tableSizes.push_back(std::max(propertyNTableValues,uint(0)));

                    RMaterialProperty property;

                    propertyName = propertyName.toLower();
                    if (propertyName == "density")
                    {
                        property.setType(R_MATERIAL_PROPERTY_DENSITY);
                    }
                    else if (propertyName == "dynamic viscosity")
                    {
                        property.setType(R_MATERIAL_PROPERTY_DYNAMIC_VISCOSITY);
                    }
                    else if (propertyName == "electrical conductivity")
                    {
                        property.setType(R_MATERIAL_PROPERTY_ELECTRICAL_CONDUCTIVITY);
                    }
                    else if (propertyName == "emissivity")
                    {
                        property.setType(R_MATERIAL_PROPERTY_EMISSIVITY);
                    }
                    else if (propertyName == "heat capacity")
                    {
                        property.setType(R_MATERIAL_PROPERTY_HEAT_CAPACITY);
                    }
                    else if (propertyName == "modulus of elasticity")
                    {
                        property.setType(R_MATERIAL_PROPERTY_MODULUS_OF_ELASTICITY);
                    }
                    else if (propertyName == "permeability")
                    {
                        property.setType(R_MATERIAL_PROPERTY_PERMEABILITY_TO_FLUIDS);
                    }
                    else if (propertyName == "poissons ratio")
                    {
                        property.setType(R_MATERIAL_PROPERTY_POISSON_RATIO);
                    }
                    else if (propertyName == "relative permittivity")
                    {
                        property.setType(R_MATERIAL_PROPERTY_RELATIVE_PERMITTIVITY);
                    }
                    else if (propertyName == "speed of sound")
                    {
                        property.setType(R_MATERIAL_PROPERTY_SOUND_SPEED);
                    }
                    else if (propertyName == "thermal conductivity")
                    {
                        property.setType(R_MATERIAL_PROPERTY_THERMAL_CONDUCTIVITY);
                    }
                    else if (propertyName == "thermal expansion coefficient")
                    {
                        property.setType(R_MATERIAL_PROPERTY_THERMAL_EXPANSION_COEFFICIENT);
                    }
                    if (propertyNTableValues < 1)
                    {
                        property.add(293.15,propertyValue);
                    }

                    this->add(property);
                }
            }
            else
            {
                for (int i=0;i<tableFlags.size();i++)
                {
                    if (flag == tableFlags[i])
                    {
                        for (uint j=0;j<tableSizes[i];j++)
                        {
                            double key, value;
                            RFileIO::readAscii(file,key);
                            RFileIO::readAscii(file,value);
                            this->get(i).add(key,value);
                        }
                    }
                }
            }
        }
    }
    catch (RError &error)
    {
        throw error;
    }
    catch (const std::exception &error)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "%s", error.what());
    }
    catch (...)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "Unknown exception.");
    }
} /* RMaterial::import */


QString RMaterial::findMaterialFileName(bool binary) const
{
    return QString::number(this->getID()) + QString(".") + RMaterial::getDefaultFileExtension(binary);
} /* RMaterial::findMaterialFileName */


bool RMaterial::validForProblemType(RProblemTypeMask problemTypeMask) const
{
    return this->hasProperties(RMaterialProperty::getTypes(problemTypeMask));
} /* RMaterial::findMaterialFileName */


QString RMaterial::getDefaultFileExtension(bool binary)
{
    if (binary)
    {
        return "rbmt";
    }
    else
    {
        return "ramt";
    }
} /* RMaterial::getDefaultFileExtension */


void RMaterial::readAscii(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided");
    }

    RFile materialFile(fileName,RFile::ASCII);

    if (!materialFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the material file \'%s\'.",fileName.toUtf8().constData());
    }

    RFileHeader fileHeader;

    RFileIO::readAscii(materialFile,fileHeader);
    if (fileHeader.getType() != R_FILE_TYPE_MATERIAL)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF,"File type of the file \'" + fileName + "\' is not MATERIAL.");
    }

    // Set file version
    materialFile.setVersion(fileHeader.getVersion());

    RFileIO::readAscii(materialFile,*this);

    materialFile.close();
} /* RMaterial::readAscii */


void RMaterial::readBinary(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided");
    }

    RFile materialFile(fileName,RFile::BINARY);

    if (!materialFile.open(QIODevice::ReadOnly))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the material file \'%s\'.",fileName.toUtf8().constData());
    }

    RFileHeader fileHeader;

    RFileIO::readBinary(materialFile,fileHeader);
    if (fileHeader.getType() != R_FILE_TYPE_MATERIAL)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF,"Filed type of the file \'" + fileName + "\' is not MATERIAL.");
    }

    // Set file version
    materialFile.setVersion(fileHeader.getVersion());

    RFileIO::readBinary(materialFile,*this);

    materialFile.close();
} /* RMaterial::readBinary */


void RMaterial::writeAscii(const QString &fileName) const
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    RSaveFile materialFile(fileName,RSaveFile::ASCII);

    if (!materialFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the material file \'%s\'.",fileName.toUtf8().constData());
    }

    RFileIO::writeAscii(materialFile,RFileHeader(R_FILE_TYPE_MATERIAL,_version));
    RFileIO::writeAscii(materialFile,*this);

    materialFile.commit();
} /* RMaterial::writeAscii */


void RMaterial::writeBinary(const QString &fileName) const
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    RSaveFile materialFile(fileName,RSaveFile::BINARY);

    if (!materialFile.open(QIODevice::WriteOnly))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the material file \'%s\'.",fileName.toUtf8().constData());
    }

    RFileIO::writeBinary(materialFile,RFileHeader(R_FILE_TYPE_MATERIAL,_version));
    RFileIO::writeBinary(materialFile,*this);

    materialFile.commit();
} /* RMaterial::writeBinary */


static bool propertyTypeCompareFunc(const RMaterialProperty &p1,const RMaterialProperty &p2)
{
    return (p1.getType()<p2.getType());
}
