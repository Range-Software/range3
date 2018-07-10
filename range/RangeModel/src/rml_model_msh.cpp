/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_model_msh.cpp                                        *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   21-st January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: MSH (old range) model class definition              *
 *********************************************************************/

#include <rblib.h>

#include "rml_model_msh.h"
#include "rml_file_io.h"
#include "rml_file.h"
#include "rml_file_manager.h"


#define FSREAD(_inFile,_data,_size,_descStr)                   \
{                                                              \
    _inFile.read((char*)_data,_size);                          \
    if (_inFile.error() != RFile::NoError)                     \
    {                                                          \
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF,_descStr);  \
    }                                                          \
}


#define FSWRITE(_outFile,_data,_size,_descStr)                 \
{                                                              \
    _outFile.write((char*)_data,_size);                        \
    if (_outFile.error() != RFile::NoError)                    \
    {                                                          \
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,_descStr); \
    }                                                          \
}


RModelMsh::RModelMsh ()
{
    this->_init();
} /* RModelMsh::RModelMsh */


RModelMsh::RModelMsh (const RModelMsh &modelMsh)
{
    this->_init(&modelMsh);
} /* RModelMsh::RModelMsh (copy) */


RModelMsh::~RModelMsh ()
{
} /* RModelMsh::~RModelMsh */


RModelMsh & RModelMsh::operator = (const RModelMsh &modelMsh)
{
    this->_init(&modelMsh);
    return (*this);
} /* RModelMsh::operator = */


void RModelMsh::_init (const RModelMsh *pModelMsh)
{
    if (pModelMsh)
    {
        this->title = pModelMsh->title;
        this->comment = pModelMsh->comment;
        this->nodes = pModelMsh->nodes;
        this->bodiesAll = pModelMsh->bodiesAll;
        this->bodies = pModelMsh->bodies;
        this->bodiesNeighbors = pModelMsh->bodiesNeighbors;
        this->facesAll = pModelMsh->facesAll;
        this->faces = pModelMsh->faces;
        this->facesNeighbors = pModelMsh->facesNeighbors;
        this->linesAll = pModelMsh->linesAll;
        this->lines = pModelMsh->lines;
        this->pointsAll = pModelMsh->pointsAll;
        this->points = pModelMsh->points;
        this->facesThickness = pModelMsh->facesThickness;
        this->linesCarea = pModelMsh->linesCarea;
        this->pointsVolume = pModelMsh->pointsVolume;
        this->bodiesNames = pModelMsh->bodiesNames;
        this->facesNames = pModelMsh->facesNames;
        this->linesNames = pModelMsh->linesNames;
        this->pointsNames = pModelMsh->pointsNames;
    }
} /* RModelMsh::_init */


void RModelMsh::clear (void)
{
    this->title.clear();
    this->comment.clear();
    this->nodes.clear();
    this->bodiesAll.clear();
    this->bodies.clear();
    this->bodiesNeighbors.clear();
    this->facesAll.clear();
    this->faces.clear();
    this->facesNeighbors.clear();
    this->linesAll.clear();
    this->lines.clear();
    this->pointsAll.clear();
    this->points.clear();
    this->facesThickness.clear();
    this->linesCarea.clear();
    this->pointsVolume.clear();
    this->bodiesNames.clear();
    this->facesNames.clear();
    this->linesNames.clear();
    this->pointsNames.clear();
    this->bodiesNeighborsComputed = false;
    this->facesNeighborsComputed = false;
} /* RModelMsh::clear */


void RModelMsh::read (const QString &fileName)
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    QString ext = RFileManager::getExtension(fileName).toLower();

    if (ext == "tmsh")
    {
        this->readAscii(fileName);
    }
    else if (ext == "bmsh")
    {
        this->readBinary(fileName);
    }
    else
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF, "Unknown extension \"" + QString(ext.toUtf8().constData()) + "\".");
    }
} /* RModelMsh::read */


void RModelMsh::write (const QString &fileName) const
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    QString ext = RFileManager::getExtension(fileName).toLower();

    if (ext == "tmsh")
    {
        this->writeAscii(fileName);
    }
    else if (ext == "bmsh")
    {
        this->writeBinary(fileName);
    }
    else
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF, "Unknown extension \"" + QString(ext.toUtf8().constData()) + "\".");
    }
} /* RModelMsh::write */


void RModelMsh::readAscii (const QString &fileName)
{
    RProgressInitialize("Reading ASCII MSH file");
    QString flag;
    QString fVersion;

    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    bool dTitleFound = false;
    bool dCommentFound = false;
    bool dFacesNComputedFound = false;
    bool dBodiesNComputedFound = false;
    bool dNodesFound = false;
    bool dBodiesAllFound = false;
    bool dFacesAllFound = false;
    bool dLinesAllFound = false;
    bool dPointsAllFound = false;
    bool dBodiesFound = false;
    bool dFacesFound = false;
    bool dLinesFound = false;
    bool dPointsFound = false;
    bool nodesFound = false;
    bool facesNeighborsFound = false;
    bool bodiesNeighborsFound = false;
    bool bodiesNamesFound = false;
    bool facesNamesFound = false;
    bool linesNamesFound = false;
    bool pointsNamesFound = false;
    bool facesThicknessFound = false;
    bool linesCareaFound = false;
    bool pointsVolumeFound = false;
    bool bodiesAllFound = false;
    bool facesAllFound = false;
    bool linesAllFound = false;
    bool pointsAllFound = false;
    bool bodiesFound = false;
    bool facesFound = false;
    bool linesFound = false;
    bool pointsFound = false;

    RFile mshFile(fileName,RFile::ASCII);

    if (!mshFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    unsigned int nsteps = 23;
    unsigned int cstep = 0;

    RProgressPrint(cstep++,nsteps);
    while (true)
    {
        mshFile.getTextStream() >> flag;
        if (mshFile.getTextStream().status() != QTextStream::Ok)
        {
            if (mshFile.getTextStream().atEnd())
            {
                break;
            }
            throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read the file \'%s\'",fileName.toUtf8().constData());
        }

        if (flag == "!VERSION:")
        {
            RFileIO::readAscii(mshFile,fVersion);
            RLogger::info("File version %s\n",fVersion.toUtf8().constData());
        }
        else if (flag == "TITLE:")
        {
            RFileIO::readAscii(mshFile,this->title);
            dTitleFound = true;
        }
        else if (flag == "COMMENT:")
        {
            RFileIO::readAscii(mshFile,this->comment);
            dCommentFound = true;
        }
        else if (flag == "FACESNCOMPUTED:")
        {
            RFileIO::readAscii(mshFile,this->facesNeighborsComputed);
            dFacesNComputedFound = true;
        }
        else if (flag == "BODIESNCOMPUTED:")
        {
            RFileIO::readAscii(mshFile,this->bodiesNeighborsComputed);
            dBodiesNComputedFound = true;
        }
        else if (flag == "NODES:")
        {
            unsigned int nr;
            unsigned int nc;
            RFileIO::readAscii(mshFile,nr);
            RFileIO::readAscii(mshFile,nc);
            this->nodes.resize(nr,nc);
            dNodesFound = true;
        }
        else if (flag == "BODIES_A:")
        {
            unsigned int nr;
            unsigned int nc;
            RFileIO::readAscii(mshFile,nr);
            RFileIO::readAscii(mshFile,nc);
            this->bodiesAll.resize(nr,nc);
            this->bodiesNeighbors.resize(nr,4);
            dBodiesAllFound = true;
        }
        else if (flag == "FACES_A:")
        {
            unsigned int nr;
            unsigned int nc;
            RFileIO::readAscii(mshFile,nr);
            RFileIO::readAscii(mshFile,nc);
            this->facesAll.resize(nr,nc);
            this->facesNeighbors.resize(nr,3);
            dFacesAllFound = true;
        }
        else if (flag == "LINES_A:")
        {
            unsigned int nr;
            unsigned int nc;
            RFileIO::readAscii(mshFile,nr);
            RFileIO::readAscii(mshFile,nc);
            this->linesAll.resize(nr,nc);
            dLinesAllFound = true;
        }
        else if (flag == "POINTS_A:")
        {
            unsigned int nr;
            unsigned int nc;
            RFileIO::readAscii(mshFile,nr);
            RFileIO::readAscii(mshFile,nc);
            this->pointsAll.resize(nr,nc);
            dPointsAllFound = true;
        }
        else if (flag == "BODIES:")
        {
            unsigned int nr;
            unsigned int nc;
            RFileIO::readAscii(mshFile,nr);
            RFileIO::readAscii(mshFile,nc);
            this->bodies.resize(nr,nc);
            this->bodiesNames.resize(nc);
            dBodiesFound = true;
        }
        else if (flag == "FACES:")
        {
            unsigned int nr;
            unsigned int nc;
            RFileIO::readAscii(mshFile,nr);
            RFileIO::readAscii(mshFile,nc);
            this->faces.resize(nr,nc);
            this->facesNames.resize(nc);
            this->facesThickness.resize(nc);
            dFacesFound = true;
        }
        else if (flag == "LINES:")
        {
            unsigned int nr;
            unsigned int nc;
            RFileIO::readAscii(mshFile,nr);
            RFileIO::readAscii(mshFile,nc);
            this->lines.resize(nr,nc);
            this->linesNames.resize(nc);
            this->linesCarea.resize(nc);
            dLinesFound = true;
        }
        else if (flag == "POINTS:")
        {
            unsigned int nr;
            unsigned int nc;
            RFileIO::readAscii(mshFile,nr);
            RFileIO::readAscii(mshFile,nc);
            this->points.resize(nr,nc);
            this->pointsNames.resize(nc);
            this->pointsVolume.resize(nc);
            dPointsFound = true;
        }

        if (dTitleFound &&
            dCommentFound &&
            dFacesNComputedFound &&
            dBodiesNComputedFound &&
            dNodesFound &&
            dBodiesAllFound &&
            dFacesAllFound &&
            dLinesAllFound &&
            dPointsAllFound &&
            dBodiesFound &&
            dFacesFound &&
            dLinesFound &&
            dPointsFound)
        {
            break;
        }
    }
    if (!dTitleFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'TITLE\' information.");
    }
    if (!dCommentFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'COMMENT\' information.");
    }
    if (!dFacesNComputedFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'FACESNCOMPUTED\' information.");
    }
    if (!dBodiesNComputedFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'BODIESNCOMPUTED\' information.");
    }
    if (!dNodesFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'NODES\' information.");
    }
    if (!dBodiesAllFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'BODIES_A\' information.");
    }
    if (!dFacesAllFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'FACES_A\' information.");
    }
    if (!dLinesAllFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'LINES_A\' information.");
    }
    if (!dPointsAllFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'POINTS_A\' information.");
    }
    if (!dBodiesFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'BODIES\' information.");
    }
    if (!dFacesFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'FACES\' information.");
    }
    if (!dLinesFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'LINES\' information.");
    }
    if (!dPointsFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'POINTS\' information.");
    }

    mshFile.seek(0);

    while (true)
    {
        mshFile.getTextStream() >> flag;
        if (mshFile.getTextStream().status() != QTextStream::Ok)
        {
            if(mshFile.getTextStream().atEnd())
            {
                break;
            }
            throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read the file.");
        }

        if (flag == "nodes:")
        {
            RFileIO::readAscii(mshFile,this->nodes,false);
            nodesFound = true;
            RProgressPrint(cstep++,nsteps);
        }
        else if (flag == "faces_neighbors:")
        {
            RFileIO::readAscii(mshFile,this->facesNeighbors,false);
            facesNeighborsFound = true;
            RProgressPrint(cstep++,nsteps);
        }
        else if (flag == "bodies_neighbors:")
        {
            RFileIO::readAscii(mshFile,this->bodiesNeighbors,false);
            bodiesNeighborsFound = true;
            RProgressPrint(cstep++,nsteps);
        }
        else if (flag == "bodies_names:")
        {
            for (unsigned int i=0;i<this->bodiesNames.size();i++)
            {
                RFileIO::readAscii(mshFile,this->bodiesNames[i]);
            }
            bodiesNamesFound = true;
            RProgressPrint(cstep++,nsteps);
        }
        else if (flag == "faces_names:")
        {
            for (unsigned int i=0;i<this->facesNames.size();i++)
            {
                RFileIO::readAscii(mshFile,this->facesNames[i]);
            }
            facesNamesFound = true;
            RProgressPrint(cstep++,nsteps);
        }
        else if (flag == "lines_names:")
        {
            for (unsigned int i=0;i<this->linesNames.size();i++)
            {
                RFileIO::readAscii(mshFile,this->linesNames[i]);
            }
            linesNamesFound = true;
            RProgressPrint(cstep++,nsteps);
        }
        else if (flag == "points_names:")
        {
            for (unsigned int i=0;i<this->pointsNames.size();i++)
            {
                RFileIO::readAscii(mshFile,this->pointsNames[i]);
            }
            pointsNamesFound = true;
            RProgressPrint(cstep++,nsteps);
        }
        else if (flag == "faces_thickness:")
        {
            RFileIO::readAscii(mshFile,this->facesThickness,false);
            facesThicknessFound = true;
            RProgressPrint(cstep++,nsteps);
        }
        else if (flag == "lines_carea:")
        {
            RFileIO::readAscii(mshFile,this->linesCarea,false);
            linesCareaFound = true;
            RProgressPrint(cstep++,nsteps);
        }
        else if (flag == "points_volume:")
        {
            RFileIO::readAscii(mshFile,this->pointsVolume,false);
            pointsVolumeFound = true;
            RProgressPrint(cstep++,nsteps);
        }
        else if (flag == "bodies_all:")
        {
            RFileIO::readAscii(mshFile,this->bodiesAll,false);
            bodiesAllFound = true;
            RProgressPrint(cstep++,nsteps);
        }
        else if (flag == "faces_all:")
        {
            RFileIO::readAscii(mshFile,this->facesAll,false);
            facesAllFound = true;
            RProgressPrint(cstep++,nsteps);
        }
        else if (flag == "lines_all:")
        {
            RFileIO::readAscii(mshFile,this->linesAll,false);
            linesAllFound = true;
            RProgressPrint(cstep++,nsteps);
        }
        else if (flag == "points_all:")
        {
            RFileIO::readAscii(mshFile,this->pointsAll,false);
            pointsAllFound = true;
            RProgressPrint(cstep++,nsteps);
        }
        else if (flag == "bodies:")
        {
            RFileIO::readAscii(mshFile,this->bodies,false);
            bodiesFound = true;
            RProgressPrint(cstep++,nsteps);
        }
        else if (flag == "faces:")
        {
            RFileIO::readAscii(mshFile,this->faces,false);
            facesFound = true;
            RProgressPrint(cstep++,nsteps);
        }
        else if (flag == "lines:")
        {
            RFileIO::readAscii(mshFile,this->lines,false);
            linesFound = true;
            RProgressPrint(cstep++,nsteps);
        }
        else if (flag == "points:")
        {
            RFileIO::readAscii(mshFile,this->points,false);
            pointsFound = true;
            RProgressPrint(cstep++,nsteps);
        }

        if (nodesFound &&
            facesNeighborsFound &&
            bodiesNeighborsFound &&
            bodiesNamesFound &&
            facesNamesFound &&
            linesNamesFound &&
            pointsNamesFound &&
            facesThicknessFound &&
            linesCareaFound &&
            pointsVolumeFound &&
            bodiesAllFound &&
            facesAllFound &&
            linesAllFound &&
            pointsAllFound &&
            bodiesFound &&
            facesFound &&
            linesFound &&
            pointsFound)
        {
            break;
        }
    }

    if (!nodesFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'nodes\' data.");
    }
    if (!facesNeighborsFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'faces_neighbors\' data.");
    }
    if (!bodiesNeighborsFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'bodies_neighbors\' data.");
    }
    if (!bodiesNamesFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'bodies_names\' data.");
    }
    if (!facesNamesFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'faces_names\' data.");
    }
    if (!linesNamesFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'lines_names\' data.");
    }
    if (!pointsNamesFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'points_names\' data.");
    }
    if (!facesThicknessFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'faces_thickness\' data.");
    }
    if (!linesCareaFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'lines_carea\' data.");
    }
    if (!pointsVolumeFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'points_volume\' data.");
    }
    if (!bodiesAllFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'bodies_all\' data.");
    }
    if (!facesAllFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'faces_all\' data.");
    }
    if (!linesAllFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'lines_all\' data.");
    }
    if (!pointsAllFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'points_all\' data.");
    }
    if (!bodiesFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'bodies\' data.");
    }
    if (!facesFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'faces\' data.");
    }
    if (!linesFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'lines\' data.");
    }
    if (!pointsFound)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF, "File is missing \'points\' data.");
    }

    this->bodies.transpose();
    RProgressPrint(cstep++,nsteps);
    this->faces.transpose();
    RProgressPrint(cstep++,nsteps);
    this->lines.transpose();
    RProgressPrint(cstep++,nsteps);
    this->points.transpose();
    RProgressPrint(cstep++,nsteps);

    mshFile.close();

    RProgressFinalize("Done");
} /* RModelMsh::readAscii */


void RModelMsh::readBinary (const QString &fileName)
{
    unsigned int vMaj;
    unsigned int vMin;
    unsigned int vRel;
    int iValue;
    int iarr[18];
    char buffer[1024];

    RProgressInitialize("Reading binary MSH file");

    RFile mshFile(fileName,RFile::BINARY);

    if (!mshFile.open(QIODevice::ReadOnly))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    FSREAD(mshFile,&vMaj,sizeof(int),"Failed to read major version.");
    FSREAD(mshFile,&vMin,sizeof(int),"Failed to read minor version.");
    FSREAD(mshFile,&vRel,sizeof(int),"Failed to read release version.");
    RLogger::info("File version %u.%u.%u\n",vMaj,vMin,vRel);
    FSREAD(mshFile,&iValue,sizeof(int),"Failed to read link flag.");
    FSREAD(mshFile,buffer,256*sizeof(char),"Failed to read model title.");
    this->title = buffer;
    FSREAD(mshFile,buffer,1024*sizeof(char),"Failed to read model comment.");
    this->comment = buffer;
    FSREAD(mshFile,iarr,18*sizeof(int),"Failed to read model size definitions.");

    unsigned int nn  = iarr[0];
    unsigned int nba = iarr[2];
    unsigned int nfa = iarr[4];
    unsigned int nla = iarr[6];
    unsigned int npa = iarr[8];
    unsigned int nb  = iarr[10];
    unsigned int nbc = iarr[11];
    unsigned int nf  = iarr[12];
    unsigned int nfc = iarr[13];
    unsigned int nl  = iarr[14];
    unsigned int nlc = iarr[15];
    unsigned int np  = iarr[16];
    unsigned int npc = iarr[17];

    this->nodes.resize (nn, 3);
    this->bodiesAll.resize (nba, 4);
    this->bodiesNeighbors.resize (nba, 4);
    this->facesAll.resize (nfa, 3);
    this->facesNeighbors.resize (nfa, 3);
    this->linesAll.resize (nla, 2);
    this->pointsAll.resize (npa, 1);
    this->bodies.resize (nb, nbc);
    this->bodiesNames.resize (nb);
    this->faces.resize (nf, nfc);
    this->facesNames.resize (nf);
    this->facesThickness.resize (nf);
    this->lines.resize (nl, nlc);
    this->linesNames.resize (nl);
    this->linesCarea.resize (nl);
    this->points.resize (np, npc);
    this->pointsNames.resize (np);
    this->pointsVolume.resize (np);

    FSREAD(mshFile,&iValue,sizeof(int),"Failed to read body neighbors computed flag.");
    this->bodiesNeighborsComputed = (bool)iValue;
    FSREAD(mshFile,&iValue,sizeof(int),"Failed to read face neighbors computed flag.");
    this->facesNeighborsComputed = (bool)iValue;

    unsigned int pst = 0;
    unsigned int npst = 3*nn
     + 4*nba + 3*nfa + 2*nla + npa
     + nb*nbc + nb
     + nf*nfc + nf + nf
     + nl*nlc + nl + nl
     + np*npc + np + np;
    if (this->bodiesNeighborsComputed) {
            npst += 4*nba;
    }
    if (this->facesNeighborsComputed) {
            npst += 3*nfa;
    }

    // nodes
    for (unsigned int i=0;i<nn;i++) {
        FSREAD(mshFile,&this->nodes[i][0],sizeof(double),"Failed to read node X coordinate.");
        RProgressPrint(++pst,npst);
        FSREAD(mshFile,&this->nodes[i][1],sizeof(double),"Failed to read node Y coordinate.");
        RProgressPrint(++pst,npst);
        FSREAD(mshFile,&this->nodes[i][2],sizeof(double),"Failed to read node Z coordinate.");
        RProgressPrint(++pst,npst);
    }
    // bodies all
    for (unsigned int i=0;i<nba;i++) {
        FSREAD(mshFile,&this->bodiesAll[i][0],sizeof(int),"Failed to read body element 1.st node.");
        RProgressPrint(++pst,npst);
        FSREAD(mshFile,&this->bodiesAll[i][1],sizeof(int),"Failed to read body element 2.nd node.");
        RProgressPrint(++pst,npst);
        FSREAD(mshFile,&this->bodiesAll[i][2],sizeof(int),"Failed to read body element 3.rd node.");
        RProgressPrint(++pst,npst);
        FSREAD(mshFile,&this->bodiesAll[i][3],sizeof(int),"Failed to read body element 4.th node.");
        RProgressPrint(++pst,npst);
    }
    // faces all
    for (unsigned int i=0;i<nfa;i++) {
        FSREAD(mshFile,&this->facesAll[i][0],sizeof(int),"Failed to read face element 1.st node.");
        RProgressPrint(++pst,npst);
        FSREAD(mshFile,&this->facesAll[i][1],sizeof(int),"Failed to read face element 2.nd node.");
        RProgressPrint(++pst,npst);
        FSREAD(mshFile,&this->facesAll[i][2],sizeof(int),"Failed to read face element 3.rd node.");
        RProgressPrint(++pst,npst);
    }
    // lines all
    for (unsigned int i=0;i<nla;i++) {
        FSREAD(mshFile,&this->linesAll[i][0],sizeof(int),"Failed to read line element 1.st node.");
        RProgressPrint(++pst,npst);
        FSREAD(mshFile,&this->linesAll[i][1],sizeof(int),"Failed to read line element 2.nd node.");
        RProgressPrint(++pst,npst);
    }
    // points all
    for (unsigned int i=0;i<npa;i++) {
        FSREAD(mshFile,&this->pointsAll[i][0],sizeof(int),"Failed to read point element node.");
        RProgressPrint(++pst,npst);
    }
    // bodies
    for (unsigned int i=0;i<nb;i++) {
        for (unsigned int j=0;j<nbc;j++) {
            FSREAD(mshFile,&this->bodies[i][j],sizeof(int),"Failed to read body element.");
            RProgressPrint(++pst,npst);
        }
    }
    // faces
    for (unsigned int i=0;i<nf;i++) {
        for (unsigned int j=0;j<nfc;j++) {
            FSREAD(mshFile,&this->faces[i][j],sizeof(int),"Failed to read face element.");
            RProgressPrint(++pst,npst);
        }
    }
    // lines
    for (unsigned int i=0;i<nl;i++) {
        for (unsigned int j=0;j<nlc;j++) {
            FSREAD(mshFile,&this->lines[i][j],sizeof(int),"Failed to read line element.");
            RProgressPrint(++pst,npst);
        }
    }
    // points
    for (unsigned int i=0;i<np;i++) {
        for (unsigned int j=0;j<npc;j++) {
            FSREAD(mshFile,&this->points[i][j],sizeof(int),"Failed to read point element.");
            RProgressPrint(++pst,npst);
        }
    }
    // body names
    for (unsigned int i=0;i<nb;i++) {
        FSREAD(mshFile,buffer,256*sizeof(char),"Failed to read body name.");
        RProgressPrint(++pst,npst);
        this->bodiesNames[i] = buffer;
    }
    // face names
    for (unsigned int i=0;i<nf;i++) {
        FSREAD(mshFile,buffer,256*sizeof(char),"Failed to read face name.");
        RProgressPrint(++pst,npst);
        this->facesNames[i] = buffer;
    }
    // line names
    for (unsigned int i=0;i<nl;i++) {
        FSREAD(mshFile,buffer,256*sizeof(char),"Failed to read line name.");
        RProgressPrint(++pst,npst);
        this->linesNames[i] = buffer;
    }
    // point names
    for (unsigned int i=0;i<np;i++) {
        FSREAD(mshFile,buffer,256*sizeof(char),"Failed to read point name.");
        RProgressPrint(++pst,npst);
        this->pointsNames[i] = buffer;
    }
    // face thickness
    for (unsigned int i=0;i<nf;i++) {
        FSREAD(mshFile,&this->facesThickness[i],sizeof(double),"Failed to read face thickness.");
        RProgressPrint(++pst,npst);
    }
    // line carea
    for (unsigned int i=0;i<nl;i++) {
        FSREAD(mshFile,&this->linesCarea[i],sizeof(double),"Failed to read line cross-area.");
        RProgressPrint(++pst,npst);
    }
    // point volume
    for (unsigned int i=0;i<np;i++) {
        FSREAD(mshFile,&this->pointsVolume[i],sizeof(double),"Failed to read point volume.");
        RProgressPrint(++pst,npst);
    }
    // body neighbors
    if (this->bodiesNeighborsComputed)
    {
        for (unsigned int i=0;i<nba;i++) {
            FSREAD(mshFile,&this->bodiesNeighbors[i][0],sizeof(int),"Failed to read body element 1.st neighbor.");
            RProgressPrint(++pst,npst);
            FSREAD(mshFile,&this->bodiesNeighbors[i][1],sizeof(int),"Failed to read body element 2.nd neighbor.");
            RProgressPrint(++pst,npst);
            FSREAD(mshFile,&this->bodiesNeighbors[i][2],sizeof(int),"Failed to read body element 3.rd neighbor.");
            RProgressPrint(++pst,npst);
            FSREAD(mshFile,&this->bodiesNeighbors[i][3],sizeof(int),"Failed to read body element 4.th neighbor.");
            RProgressPrint(++pst,npst);
        }
    }
    // face neighbors
    if (this->facesNeighborsComputed)
    {
        for (unsigned int i=0;i<nfa;i++) {
            FSREAD(mshFile,&this->facesNeighbors[i][0],sizeof(int),"Failed to read face element 1.st neighbor.");
            RProgressPrint(++pst,npst);
            FSREAD(mshFile,&this->facesNeighbors[i][1],sizeof(int),"Failed to read face element 2.nd neighbor.");
            RProgressPrint(++pst,npst);
            FSREAD(mshFile,&this->facesNeighbors[i][2],sizeof(int),"Failed to read face element 3.rd neighbor.");
            RProgressPrint(++pst,npst);
        }
    }

    mshFile.close();

    RProgressFinalize("Done");
} /* RModelMsh::readBinary */


void RModelMsh::writeAscii (const QString &fileName) const
{
    RProgressInitialize("Writing ASCII MSH file");

    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    RSaveFile mshFile(fileName,RSaveFile::ASCII);

    if (!mshFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    RIMatrix tmpMat;
    unsigned int cstep = 0;
    unsigned int nsteps = 20;
    RProgressPrint(cstep++,nsteps);
    RFileIO::writeAscii(mshFile,"!VERSION: 2.3.0");
    RFileIO::writeAscii(mshFile,"# --== Range Software - Mesh File ==--");
    RFileIO::writeNewLineAscii(mshFile);
    RFileIO::writeAscii(mshFile,"# MESH DESCRIPTION");
    RFileIO::writeAscii(mshFile,"TITLE: \"" + this->title + "\"");
    RFileIO::writeAscii(mshFile,"COMMENT: \"" + this->comment + "\"");
    RFileIO::writeNewLineAscii(mshFile);
    RFileIO::writeAscii(mshFile,"# MESH SIZES");
    RFileIO::writeNewLineAscii(mshFile);
    RFileIO::writeAscii(mshFile,"FACESNCOMPUTED: ",false);
    RFileIO::writeAscii(mshFile,this->facesNeighborsComputed);
    RFileIO::writeAscii(mshFile,"BODIESNCOMPUTED: ",false);
    RFileIO::writeAscii(mshFile,this->bodiesNeighborsComputed);
    RFileIO::writeNewLineAscii(mshFile);
    RFileIO::writeAscii(mshFile,"NODES: ",false);
    RFileIO::writeAscii(mshFile,this->nodes.getNRows(), false);
    RFileIO::writeAscii(mshFile," ",false);
    RFileIO::writeAscii(mshFile,this->nodes.getNColumns());
    RFileIO::writeAscii(mshFile,"BODIES_A: ",false);
    RFileIO::writeAscii(mshFile,this->bodiesAll.getNRows(), false);
    RFileIO::writeAscii(mshFile," ",false);
    RFileIO::writeAscii(mshFile,this->bodiesAll.getNColumns());
    RFileIO::writeAscii(mshFile,"FACES_A: ",false);
    RFileIO::writeAscii(mshFile,this->facesAll.getNRows(), false);
    RFileIO::writeAscii(mshFile," ",false);
    RFileIO::writeAscii(mshFile,this->facesAll.getNColumns());
    RFileIO::writeAscii(mshFile,"LINES_A: ",false);
    RFileIO::writeAscii(mshFile,this->linesAll.getNRows(), false);
    RFileIO::writeAscii(mshFile," ",false);
    RFileIO::writeAscii(mshFile,this->linesAll.getNColumns());
    RFileIO::writeAscii(mshFile,"POINTS_A: ",false);
    RFileIO::writeAscii(mshFile,this->pointsAll.getNRows(), false);
    RFileIO::writeAscii(mshFile," ",false);
    RFileIO::writeAscii(mshFile,this->pointsAll.getNColumns());
    RFileIO::writeAscii(mshFile,"BODIES: ",false);
    RFileIO::writeAscii(mshFile,this->bodies.getNColumns(), false);
    RFileIO::writeAscii(mshFile," ",false);
    RFileIO::writeAscii(mshFile,this->bodies.getNRows());
    RFileIO::writeAscii(mshFile,"FACES: ",false);
    RFileIO::writeAscii(mshFile,this->faces.getNColumns(), false);
    RFileIO::writeAscii(mshFile," ",false);
    RFileIO::writeAscii(mshFile,this->faces.getNRows());
    RFileIO::writeAscii(mshFile,"LINES: ",false);
    RFileIO::writeAscii(mshFile,this->lines.getNColumns(), false);
    RFileIO::writeAscii(mshFile," ",false);
    RFileIO::writeAscii(mshFile,this->lines.getNRows());
    RFileIO::writeAscii(mshFile,"POINTS: ",false);
    RFileIO::writeAscii(mshFile,this->points.getNColumns(), false);
    RFileIO::writeAscii(mshFile," ",false);
    RFileIO::writeAscii(mshFile,this->points.getNRows());
    RFileIO::writeNewLineAscii(mshFile);
    RFileIO::writeAscii(mshFile,"# MESH VALUES");
    RFileIO::writeNewLineAscii(mshFile);
    RProgressPrint(cstep++,nsteps);
    RFileIO::writeAscii(mshFile,"nodes:");
    RFileIO::writeAscii(mshFile,this->nodes,false);
    RFileIO::writeNewLineAscii(mshFile);
    RProgressPrint(cstep++,nsteps);
    RFileIO::writeAscii(mshFile,"faces_neighbors:");
    RFileIO::writeAscii(mshFile,this->facesNeighbors,false);
    RFileIO::writeNewLineAscii(mshFile);
    RProgressPrint(cstep++,nsteps);
    RFileIO::writeAscii(mshFile,"bodies_neighbors:");
    RFileIO::writeAscii(mshFile,this->bodiesNeighbors,false);
    RFileIO::writeNewLineAscii(mshFile);
    RProgressPrint(cstep++,nsteps);
    RFileIO::writeAscii(mshFile,"bodies_names:");
    for (unsigned int i=0;i<this->bodiesNames.size();i++)
    {
        RFileIO::writeAscii(mshFile,this->bodiesNames[i]);
    }
    RFileIO::writeNewLineAscii(mshFile);
    RProgressPrint(cstep++,nsteps);
    RFileIO::writeAscii(mshFile,"faces_names:");
    for (unsigned int i=0;i<this->facesNames.size();i++)
    {
        RFileIO::writeAscii(mshFile,this->facesNames[i]);
    }
    RFileIO::writeNewLineAscii(mshFile);
    RProgressPrint(cstep++,nsteps);
    RFileIO::writeAscii(mshFile,"lines_names:");
    for (unsigned int i=0;i<this->linesNames.size();i++)
    {
        RFileIO::writeAscii(mshFile,this->linesNames[i]);
    }
    RFileIO::writeNewLineAscii(mshFile);
    RProgressPrint(cstep++,nsteps);
    RFileIO::writeAscii(mshFile,"points_names:");
    for (unsigned int i=0;i<this->pointsNames.size();i++)
    {
        RFileIO::writeAscii(mshFile,this->pointsNames[i]);
    }
    RFileIO::writeNewLineAscii(mshFile);
    RProgressPrint(cstep++,nsteps);
    RFileIO::writeAscii(mshFile,"faces_thickness:");
    RFileIO::writeAscii(mshFile,this->facesThickness,false);
    RFileIO::writeNewLineAscii(mshFile);
    RProgressPrint(cstep++,nsteps);
    RFileIO::writeAscii(mshFile,"lines_carea:");
    RFileIO::writeAscii(mshFile,this->linesCarea,false);
    RFileIO::writeNewLineAscii(mshFile);
    RProgressPrint(cstep++,nsteps);
    RFileIO::writeAscii(mshFile,"points_volume:");
    RFileIO::writeAscii(mshFile,this->pointsVolume,false);
    RFileIO::writeNewLineAscii(mshFile);
    RProgressPrint(cstep++,nsteps);
    RFileIO::writeAscii(mshFile,"bodies_all:");
    RFileIO::writeAscii(mshFile,this->bodiesAll,false);
    RFileIO::writeNewLineAscii(mshFile);
    RProgressPrint(cstep++,nsteps);
    RFileIO::writeAscii(mshFile,"faces_all:");
    RFileIO::writeAscii(mshFile,this->facesAll,false);
    RFileIO::writeNewLineAscii(mshFile);
    RProgressPrint(cstep++,nsteps);
    RFileIO::writeAscii(mshFile,"lines_all:");
    RFileIO::writeAscii(mshFile,this->linesAll,false);
    RFileIO::writeNewLineAscii(mshFile);
    RProgressPrint(cstep++,nsteps);
    RFileIO::writeAscii(mshFile,"points_all:");
    RFileIO::writeAscii(mshFile,this->pointsAll,false);
    RFileIO::writeNewLineAscii(mshFile);
    RProgressPrint(cstep++,nsteps);
    RFileIO::writeAscii(mshFile,"bodies:");
    tmpMat = this->bodies;
    tmpMat.transpose();
    RFileIO::writeAscii(mshFile,tmpMat,false);
    RFileIO::writeNewLineAscii(mshFile);
    RProgressPrint(cstep++,nsteps);
    RFileIO::writeAscii(mshFile,"faces:");
    tmpMat = this->faces;
    tmpMat.transpose();
    RFileIO::writeAscii(mshFile,tmpMat,false);
    RFileIO::writeNewLineAscii(mshFile);
    RProgressPrint(cstep++,nsteps);
    RFileIO::writeAscii(mshFile,"lines:");
    tmpMat = this->lines;
    tmpMat.transpose();
    RFileIO::writeAscii(mshFile,tmpMat,false);
    RFileIO::writeNewLineAscii(mshFile);
    RProgressPrint(cstep++,nsteps);
    RFileIO::writeAscii(mshFile,"points:");
    tmpMat = this->points;
    tmpMat.transpose();
    RFileIO::writeAscii(mshFile,tmpMat,false);
    RProgressPrint(cstep++,nsteps);

    mshFile.commit();

    RProgressFinalize("Done");
} /* RModelMsh::writeAscii */


void RModelMsh::writeBinary (const QString &fileName) const
{
    unsigned int vMaj = 2;
    unsigned int vMin = 3;
    unsigned int vRel = 0;
    int iValue = 0;
    int iarr[18];
    char buffer[256];

    RProgressInitialize("Writing binary MSH file");

    RFile mshFile(fileName,RFile::BINARY);

    if (!mshFile.open(QIODevice::WriteOnly))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    FSWRITE(mshFile,&vMaj,sizeof(int),"Failed to write major version.");
    FSWRITE(mshFile,&vMin,sizeof(int),"Failed to write minor version.");
    FSWRITE(mshFile,&vRel,sizeof(int),"Failed to write release version.");
    FSWRITE(mshFile,&iValue,sizeof(int),"Failed to write link flag.");
    snprintf(buffer,256,"%s",this->title.toUtf8().constData());
    FSWRITE(mshFile,buffer,256*sizeof(char),"Failed to write model title.");
    snprintf(buffer,256,"%s",this->comment.toUtf8().constData());
    FSWRITE(mshFile,buffer,256*sizeof(char),"Failed to write model comment.");

    iarr[0] = this->nodes.getNRows();
    iarr[1] = this->nodes.getNColumns();
    iarr[2] = this->bodiesAll.getNRows();
    iarr[3] = this->bodiesAll.getNColumns();
    iarr[4] = this->facesAll.getNRows();
    iarr[5] = this->facesAll.getNColumns();
    iarr[6] = this->linesAll.getNRows();
    iarr[7] = this->linesAll.getNColumns();
    iarr[8] = this->pointsAll.getNRows();
    iarr[9] = this->pointsAll.getNColumns();
    iarr[10] = this->bodies.getNRows();
    iarr[11] = this->bodies.getNColumns();
    iarr[12] = this->faces.getNRows();
    iarr[13] = this->faces.getNColumns();
    iarr[14] = this->lines.getNRows();
    iarr[15] = this->lines.getNColumns();
    iarr[16] = this->points.getNRows();
    iarr[17] = this->points.getNColumns();

    unsigned int nn  = iarr[0];
    unsigned int nba = iarr[2];
    unsigned int nfa = iarr[4];
    unsigned int nla = iarr[6];
    unsigned int npa = iarr[8];
    unsigned int nb  = iarr[10];
    unsigned int nbc = iarr[11];
    unsigned int nf  = iarr[12];
    unsigned int nfc = iarr[13];
    unsigned int nl  = iarr[14];
    unsigned int nlc = iarr[15];
    unsigned int np  = iarr[16];
    unsigned int npc = iarr[17];

    FSWRITE(mshFile,iarr,18*sizeof(int),"Failed to write model size definitions.");

    iValue = (int) this->bodiesNeighborsComputed;
    FSWRITE(mshFile,&iValue,sizeof(int),"Failed to write body neighbors computed flag.");
    iValue = (int) this->facesNeighborsComputed;
    FSWRITE(mshFile,&iValue,sizeof(int),"Failed to write face neighbors computed flag.");

    unsigned int pst = 0;
    unsigned int npst = 3*nn
     + 4*nba + 3*nfa + 2*nla + npa
     + nb*nbc + nb
     + nf*nfc + nf + nf
     + nl*nlc + nl + nl
     + np*npc + np + np;
    if (this->bodiesNeighborsComputed) {
            npst += 4*nba;
    }
    if (this->facesNeighborsComputed) {
            npst += 3*nfa;
    }

    // nodes
    for (unsigned int i=0;i<nn;i++) {
        FSWRITE(mshFile,&this->nodes[i][0],sizeof(double),"Failed to write node X coordinate.");
        RProgressPrint(++pst,npst);
        FSWRITE(mshFile,&this->nodes[i][1],sizeof(double),"Failed to write node Y coordinate.");
        RProgressPrint(++pst,npst);
        FSWRITE(mshFile,&this->nodes[i][2],sizeof(double),"Failed to write node Z coordinate.");
        RProgressPrint(++pst,npst);
    }
    // bodies all
    for (unsigned int i=0;i<nba;i++) {
        FSWRITE(mshFile,&this->bodiesAll[i][0],sizeof(int),"Failed to write body element 1.st node.");
        RProgressPrint(++pst,npst);
        FSWRITE(mshFile,&this->bodiesAll[i][1],sizeof(int),"Failed to write body element 2.nd node.");
        RProgressPrint(++pst,npst);
        FSWRITE(mshFile,&this->bodiesAll[i][2],sizeof(int),"Failed to write body element 3.rd node.");
        RProgressPrint(++pst,npst);
        FSWRITE(mshFile,&this->bodiesAll[i][3],sizeof(int),"Failed to write body element 4.th node.");
        RProgressPrint(++pst,npst);
    }
    // faces all
    for (unsigned int i=0;i<nfa;i++) {
        FSWRITE(mshFile,&this->facesAll[i][0],sizeof(int),"Failed to write face element 1.st node.");
        RProgressPrint(++pst,npst);
        FSWRITE(mshFile,&this->facesAll[i][1],sizeof(int),"Failed to write face element 2.nd node.");
        RProgressPrint(++pst,npst);
        FSWRITE(mshFile,&this->facesAll[i][2],sizeof(int),"Failed to write face element 3.rd node.");
        RProgressPrint(++pst,npst);
    }
    // lines all
    for (unsigned int i=0;i<nla;i++) {
        FSWRITE(mshFile,&this->linesAll[i][0],sizeof(int),"Failed to write line element 1.st node.");
        RProgressPrint(++pst,npst);
        FSWRITE(mshFile,&this->linesAll[i][1],sizeof(int),"Failed to write line element 2.nd node.");
        RProgressPrint(++pst,npst);
    }
    // points all
    for (unsigned int i=0;i<npa;i++) {
        FSWRITE(mshFile,&this->pointsAll[i][0],sizeof(int),"Failed to write point element node.");
        RProgressPrint(++pst,npst);
    }
    // bodies
    for (unsigned int i=0;i<nb;i++) {
        for (unsigned int j=0;j<nbc;j++) {
            FSWRITE(mshFile,&this->bodies[i][j],sizeof(int),"Failed to write body element.");
            RProgressPrint(++pst,npst);
        }
    }
    // faces
    for (unsigned int i=0;i<nf;i++) {
        for (unsigned int j=0;j<nfc;j++) {
            FSWRITE(mshFile,&this->faces[i][j],sizeof(int),"Failed to write face element.");
            RProgressPrint(++pst,npst);
        }
    }
    // lines
    for (unsigned int i=0;i<nl;i++) {
        for (unsigned int j=0;j<nlc;j++) {
            FSWRITE(mshFile,&this->lines[i][j],sizeof(int),"Failed to write line element.");
            RProgressPrint(++pst,npst);
        }
    }
    // points
    for (unsigned int i=0;i<np;i++) {
        for (unsigned int j=0;j<npc;j++) {
            FSWRITE(mshFile,&this->points[i][j],sizeof(int),"Failed to write point element.");
            RProgressPrint(++pst,npst);
        }
    }
    // body names
    for (unsigned int i=0;i<nb;i++) {
        snprintf(buffer,256,"%s",this->bodiesNames[i].toUtf8().constData());
        FSWRITE(mshFile,buffer,256*sizeof(char),"Failed to write body name.");
        RProgressPrint(++pst,npst);
    }
    // face names
    for (unsigned int i=0;i<nf;i++) {
        snprintf(buffer,256,"%s",this->facesNames[i].toUtf8().constData());
        FSWRITE(mshFile,buffer,256*sizeof(char),"Failed to write face name.");
        RProgressPrint(++pst,npst);
    }
    // line names
    for (unsigned int i=0;i<nl;i++) {
        snprintf(buffer,256,"%s",this->linesNames[i].toUtf8().constData());
        FSWRITE(mshFile,buffer,256*sizeof(char),"Failed to write line name.");
        RProgressPrint(++pst,npst);
    }
    // point names
    for (unsigned int i=0;i<np;i++) {
        snprintf(buffer,256,"%s",this->pointsNames[i].toUtf8().constData());
        FSWRITE(mshFile,buffer,256*sizeof(char),"Failed to write point name.");
        RProgressPrint(++pst,npst);
    }
    // face thickness
    for (unsigned int i=0;i<nf;i++) {
        FSWRITE(mshFile,&this->facesThickness[i],sizeof(double),"Failed to write face thickness.");
        RProgressPrint(++pst,npst);
    }
    // line carea
    for (unsigned int i=0;i<nl;i++) {
        FSWRITE(mshFile,&this->linesCarea[i],sizeof(double),"Failed to write line cross-area.");
        RProgressPrint(++pst,npst);
    }
    // point volume
    for (unsigned int i=0;i<np;i++) {
        FSWRITE(mshFile,&this->pointsVolume[i],sizeof(double),"Failed to write point volume.");
        RProgressPrint(++pst,npst);
    }
    // body neighbors
    if (this->bodiesNeighborsComputed)
    {
        for (unsigned int i=0;i<nba;i++) {
            FSWRITE(mshFile,&this->bodiesNeighbors[i][0],sizeof(int),"Failed to write body element 1.st neighbor.");
            RProgressPrint(++pst,npst);
            FSWRITE(mshFile,&this->bodiesNeighbors[i][1],sizeof(int),"Failed to write body element 2.nd neighbor.");
            RProgressPrint(++pst,npst);
            FSWRITE(mshFile,&this->bodiesNeighbors[i][2],sizeof(int),"Failed to write body element 3.rd neighbor.");
            RProgressPrint(++pst,npst);
            FSWRITE(mshFile,&this->bodiesNeighbors[i][3],sizeof(int),"Failed to write body element 4.th neighbor.");
            RProgressPrint(++pst,npst);
        }
    }
    // face neighbors
    if (this->facesNeighborsComputed)
    {
        for (unsigned int i=0;i<nfa;i++) {
            FSWRITE(mshFile,&this->facesNeighbors[i][0],sizeof(int),"Failed to write face element 1.st neighbor.");
            RProgressPrint(++pst,npst);
            FSWRITE(mshFile,&this->facesNeighbors[i][1],sizeof(int),"Failed to write face element 2.nd neighbor.");
            RProgressPrint(++pst,npst);
            FSWRITE(mshFile,&this->facesNeighbors[i][2],sizeof(int),"Failed to write face element 3.rd neighbor.");
            RProgressPrint(++pst,npst);
        }
    }

    mshFile.close();

    RProgressFinalize("Done");
} /* RModelMsh::writeBinary */
