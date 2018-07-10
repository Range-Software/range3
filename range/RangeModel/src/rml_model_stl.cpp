/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_model_stl.cpp                                        *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   10-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: STL model class definition                          *
 *********************************************************************/

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include <rblib.h>

#include "rml_model_stl.h"
#include "rml_model_raw.h"


RModelStl::RModelStl ()
{
    this->_init();
} /* RModelStl::RModelStl */


RModelStl::RModelStl (const RModelStl &modelStl) : RModelRaw(modelStl)
{
    this->_init(&modelStl);
} /* RModelStl::RModelStl (copy) */


RModelStl::~RModelStl ()
{
} /* RModelStl::~RModelStl */


RModelStl & RModelStl::operator = (const RModelStl &modelStl)
{
    this->RModelRaw::operator = (modelStl);
    this->_init(&modelStl);
    return (*this);
} /* RModelStl::operator = */


void RModelStl::_init(const RModelStl *pModelStl)
{
    if (pModelStl)
    {
        this->setName(pModelStl->getName());
        this->setDescription(pModelStl->getDescription());
    }
} /* RModelStl::_init */


QString RModelStl::getName (void) const
{
    return this->name;
} /* RModelStl::getName */


void RModelStl::setName (const QString &name)
{
    this->name = name;
} /* RModelStl::setName */


QString RModelStl::getDescription (void) const
{
    return this->description;
} /* RModelStl::getDescription */


void RModelStl::setDescription (const QString &description)
{
    this->description = description;
} /* RModelStl::setDescription */


void RModelStl::clear (void)
{
    this->RModelRaw::clear();
    this->name.clear();
    this->description.clear();
} /* RModelStl::clear */


void RModelStl::read (const QString &fileName,
                      double         tolerance)
{
    if (this->checkIfBinary(fileName))
    {
        RLogger::info("Reading binary STL file \'%s\'\n",fileName.toUtf8().constData());
        this->readBinary(fileName,tolerance);
    }
    else
    {
        RLogger::info("Reading ASCII STL file \'%s\'\n",fileName.toUtf8().constData());
        this->readAscii(fileName,tolerance);
    }
} /* RModelStl::read */


void RModelStl::write (const QString &fileName, bool binary) const
{
    if (binary)
    {
        this->writeBinary(fileName);
    }
    else
    {
        this->writeAscii(fileName);
    }
} /* RModelStl::write */


void RModelStl::readBinary(const QString &fileName, double tolerance)
{
    RNode node1;
    RNode node2;
    RNode node3;
    float xyz[3];
    double x1, y1, z1;
    double x2, y2, z2;
    double x3, y3, z3;

    RProgressInitialize("Reading STL binary file");

    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    QFile stlFile(fileName);

    if (!stlFile.open(QIODevice::ReadOnly))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    QString line(stlFile.read(80));
    if (stlFile.error() != QFile::NoError)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read from file \'%s\'.",fileName.toUtf8().constData());
    }

    this->name = QFileInfo(stlFile).baseName();

    QStringList query = line.split(QRegExp("(\\ |\\t)"));
    for (int i=0;i<query.size();i++)
    {
        this->description += query.at(i);
        if (i < query.size()-1)
        {
            this->description += " ";
        }
    }

    unsigned int nf;
    stlFile.read((char*)&nf,sizeof(unsigned int));
    if (stlFile.error() != QFile::NoError)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read from file \'%s\'.",fileName.toUtf8().constData());
    }

    char dummy[2];

    for (unsigned int i=0;i<nf;i++)
    {
        RProgressPrint(i+1,nf);

        stlFile.read((char*)xyz,3*sizeof(float));
        if (stlFile.error() != QFile::NoError)
        {
            throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read from file \'%s\'.",fileName.toUtf8().constData());
        }

        stlFile.read((char*)xyz,3*sizeof(float));
        if (stlFile.error() != QFile::NoError)
        {
            throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read from file \'%s\'.",fileName.toUtf8().constData());
        }
        x1 = (double) xyz[0];
        y1 = (double) xyz[1];
        z1 = (double) xyz[2];

        stlFile.read((char*)xyz,3*sizeof(float));
        if (stlFile.error() != QFile::NoError)
        {
            throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read from file \'%s\'.",fileName.toUtf8().constData());
        }
        x2 = (double) xyz[0];
        y2 = (double) xyz[1];
        z2 = (double) xyz[2];

        stlFile.read((char*)xyz,3*sizeof(float));
        if (stlFile.error() != QFile::NoError)
        {
            throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read from file \'%s\'.",fileName.toUtf8().constData());
        }
        x3 = (double) xyz[0];
        y3 = (double) xyz[1];
        z3 = (double) xyz[2];

        node1.set(x1,y1,z1);
        node2.set(x2,y2,z2);
        node3.set(x3,y3,z3);
        this->addTriangle(node1,node2,node3,false,tolerance);

        stlFile.read(dummy,2);
        if (stlFile.error() != QFile::NoError)
        {
            throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read from file \'%s\'.",fileName.toUtf8().constData());
        }
    }

    stlFile.close ();

    RProgressFinalize("Done");

    this->mergeNearNodes(tolerance);
} /* RModelStl::readBinary */


void RModelStl::writeBinary (const QString &fileName) const
{
    RProgressInitialize("Writing STL binary file", true);

    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    QFile stlFile(fileName);

    if (!stlFile.open(QIODevice::WriteOnly))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    char dummy[2] = { '\0', '\0' };
    char buffer[80];
    snprintf (buffer, 80, "%s %s", this->getName().toUtf8().constData(), this->getDescription().toUtf8().constData());
    stlFile.write(buffer,80);
    if (stlFile.error() != QFile::NoError)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write to file \'%s\'.",fileName.toUtf8().constData());
    }

    unsigned int nf = this->getNElements();
    stlFile.write((char*)&nf,sizeof(unsigned int));
    if (stlFile.error() != QFile::NoError)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write to file \'%s\'.",fileName.toUtf8().constData());
    }

    double dxyz[3];
    float  fxyz[3];
    for (unsigned int i=0;i<this->getNElements();i++)
    {
        if (this->getElement(i).getType() != R_ELEMENT_TRI1)
        {
            continue;
        }

        this->getNormal(i,dxyz[0],dxyz[1],dxyz[2]);
        fxyz[0] = float(dxyz[0]);
        fxyz[1] = float(dxyz[1]);
        fxyz[2] = float(dxyz[2]);

        stlFile.write((char*)fxyz,3*sizeof(float));
        if (stlFile.error() != QFile::NoError)
        {
            throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write to file \'%s\'.",fileName.toUtf8().constData());
        }
        for (unsigned int j=0;j<this->getElement(i).size();j++)
        {
            int nId = this->getElement(i).getNodeId(j);
            fxyz[0] = float(this->getNode(nId).getX());
            fxyz[1] = float(this->getNode(nId).getY());
            fxyz[2] = float(this->getNode(nId).getZ());
            stlFile.write((char*)fxyz,3*sizeof(float));
            if (stlFile.error() != QFile::NoError)
            {
                throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write to file \'%s\'.",fileName.toUtf8().constData());
            }
        }

        stlFile.write(dummy,2); // Attribute byte count UINT16
        if (stlFile.error() != QFile::NoError)
        {
            throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write to file \'%s\'.",fileName.toUtf8().constData());
        }
    }

    stlFile.close ();

    RProgressFinalize("Done");
} /* RModelStl::writeBinary */


void RModelStl::readAscii (const QString &fileName,
                           double             tolerance)
{
    RNode node1;
    RNode node2;
    RNode node3;
    double nx, ny, nz;
    double x1, y1, z1;
    double x2, y2, z2;
    double x3, y3, z3;
    QString flag;

    RProgressInitialize("Reading STL ASCII file");

    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    QTextStream in(&file);

    QString line;
    line = in.readLine();
    if (in.status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF, "Failed to read first line (1).");
    }

    this->name = QFileInfo(fileName).baseName();

    QStringList query = line.split(QRegExp("(\\ |\\t)"));
    for (int i=0;i<query.size();i++)
    {
        this->description += query.at(i);
        if (i < query.size()-1)
        {
            this->description += " ";
        }
    }

    unsigned int nf = 0;
    while (!in.atEnd() && in.status() == QTextStream::Ok)
    {
        in >> flag;
        if (flag == "normal" || flag == "NORMAL")
        {
            nf++;
        }
    }
    in.resetStatus();

    in.seek(0);
    if (in.status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF, "Failed to set position.");
    }

    line = in.readLine();
    if (in.status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF, "Failed to read first line (2).");
    }

    for (unsigned int i=0;i<nf;i++)
    {
        RProgressPrint(i+1,nf);

        in >> flag >> flag >> nx >> ny >> nz;
        if (in.status() != QTextStream::Ok)
        {
            QString errorStr("Failed to read normal (Facet: " + QString::number(i+1) + " of " + QString::number(nf) + ").");
            throw RError(R_ERROR_READ_FILE,R_ERROR_REF, errorStr);
        }
        in >> flag >> flag;
        if (in.status() != QTextStream::Ok)
        {
            QString errorStr("Failed to read outer loop (Facet: " + QString::number(i+1) + " of " + QString::number(nf) + ").");
            throw RError(R_ERROR_READ_FILE,R_ERROR_REF, errorStr);
        }
        in >> flag >> x1 >> y1 >> z1;
        if (in.status() != QTextStream::Ok)
        {
            QString errorStr("Failed to read vertex 1 (Facet: " + QString::number(i+1) + " of " + QString::number(nf) + ").");
            throw RError(R_ERROR_READ_FILE,R_ERROR_REF, errorStr);
        }
        in >> flag >> x2 >> y2 >> z2;
        if (in.status() != QTextStream::Ok)
        {
            QString errorStr("Failed to read vertex 2 (Facet: " + QString::number(i+1) + " of " + QString::number(nf) + ").");
            throw RError(R_ERROR_READ_FILE,R_ERROR_REF, errorStr);
        }
        in >> flag >> x3 >> y3 >> z3;
        if (in.status() != QTextStream::Ok)
        {
            QString errorStr("Failed to read vertex 3 (Facet: " + QString::number(i+1) + " of " + QString::number(nf) + ").");
            throw RError(R_ERROR_READ_FILE,R_ERROR_REF, errorStr);
        }
        in >> flag;
        if (in.status() != QTextStream::Ok)
        {
            QString errorStr("Failed to read endloop (Facet: " + QString::number(i+1) + " of " + QString::number(nf) + ").");
            throw RError(R_ERROR_READ_FILE,R_ERROR_REF, errorStr);
        }
        in >> flag;
        if (in.status() != QTextStream::Ok)
        {
            QString errorStr("Failed to read endfacet (Facet: " + QString::number(i+1) + " of " + QString::number(nf) + ").");
            throw RError(R_ERROR_READ_FILE,R_ERROR_REF, errorStr);
        }

        node1.set(x1,y1,z1);
        node2.set(x2,y2,z2);
        node3.set(x3,y3,z3);
        this->addTriangle(node1,node2,node3,false,tolerance);
    }

    file.close ();

    RProgressFinalize("Done");

    this->mergeNearNodes(tolerance);
} /* RModelStl::readAscii */


void RModelStl::writeAscii (const QString &fileName) const
{
    RProgressInitialize("Writing STL ASCII file", true);

    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    QTextStream out(&file);

    out.setRealNumberNotation(QTextStream::ScientificNotation);
    out.setFieldAlignment(QTextStream::AlignRight);

    out << "solid " << this->getName().toUtf8().constData() << " " << this->getDescription().toUtf8().constData();
    endl(out);
    if (out.status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write to file \'%s\'.",fileName.toUtf8().constData());
    }

    double xyz[3];
    for (unsigned int i=0;i<this->getNElements();i++)
    {
        if (this->getElement(i).getType() != R_ELEMENT_TRI1)
        {
            continue;
        }

        this->getNormal(i,xyz[0],xyz[1],xyz[2]);

        out << "  facet normal " << xyz[0] << " " << xyz[1] << " " << xyz[2];
        endl(out);
        if (out.status() != QTextStream::Ok)
        {
            throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write to file \'%s\'.",fileName.toUtf8().constData());
        }
        out << "    outer loop ";
        endl(out);
        if (out.status() != QTextStream::Ok)
        {
            throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write to file \'%s\'.",fileName.toUtf8().constData());
        }
        for (unsigned int j=0;j<this->getElement(i).size();j++)
        {
            int nId = this->getElement(i).getNodeId(j);
            out << "      vertex "
                    << this->getNode(nId).getX() << " "
                    << this->getNode(nId).getY() << " "
                    << this->getNode(nId).getZ();
            endl(out);
            if (out.status() != QTextStream::Ok)
            {
                throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write to file \'%s\'.",fileName.toUtf8().constData());
            }
        }
        out << "    endloop";
        endl(out);
        if (out.status() != QTextStream::Ok)
        {
            throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write to file \'%s\'.",fileName.toUtf8().constData());
        }
        out << "  endfacet";
        endl(out);
        if (out.status() != QTextStream::Ok)
        {
            throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write to file \'%s\'.",fileName.toUtf8().constData());
        }
    }

    out << "endsolid";
    endl(out);
    if (out.status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write to file \'%s\'.",fileName.toUtf8().constData());
    }

    file.close ();

    RProgressFinalize("Done");
} /* RModelStl::writeAscii */


bool RModelStl::checkIfBinary (const QString &fileName) const
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    QTextStream in(&file);

    bool isBinary = true;
    bool solidFound = false;
    bool faceFound = false;
    bool normalFound = false;
    bool outerFound = false;
    bool loopFound = false;
    qint64 maxPos = 10240; // Max. 10 Kb
    QString strVal;

    while (!in.atEnd())
    {
        in >> strVal;
        if (strVal == "solid")
        {
            solidFound = true;
        }
        else if (strVal == "facet")
        {
            faceFound = true;
        }
        else if (strVal == "normal")
        {
            normalFound = true;
        }
        else if (strVal == "outer")
        {
            outerFound = true;
        }
        else if (strVal == "loop")
        {
            loopFound = true;
        }
        if (solidFound && faceFound && normalFound && outerFound && loopFound)
        {
            isBinary = false;
            break;
        }
        if (in.pos() > maxPos)
        {
            break;
        }
    }

    file.close();

    return isBinary;
} /* RModelStl::checkIfBinary */
