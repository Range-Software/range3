/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   file_updater.cpp                                         *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   17-th May 2016                                           *
 *                                                                   *
 *  DESCRIPTION: File updater class definition                       *
 *********************************************************************/

#include <QFile>

#include <rblib.h>

#include "file_updater.h"

FileUpdater::FileUpdater(QObject *parent)
    : Job(parent)
{

}

void FileUpdater::addFile(const QString &source, const QString &destination)
{
    this->files.append(QPair<QString,QString>(source,destination));
}

void FileUpdater::run(void)
{
    RLogger::info("Updating files.\n");
    RLogger::indent();
    RProgressInitialize("Updating files");
    for (int i=0;i<this->files.size();i++)
    {
        RProgressPrint(i+1,this->files.size());
        RLogger::info("Checking file \'%s\'.\n",this->files.at(i).first.toUtf8().constData());

        if (QFile::exists(this->files.at(i).second.toUtf8().constData()))
        {
            RLogger::info("Destination file exists \'%s\' and will not be updated.\n",this->files.at(i).second.toUtf8().constData());
        }
        else
        {
            RLogger::info("Copying file \'%s\' to \'%s\'\n",
                          this->files.at(i).first.toUtf8().constData(),
                          this->files.at(i).second.toUtf8().constData());
            if (!QFile::copy(this->files.at(i).first,this->files.at(i).second))
            {
                RLogger::warning("Failed to copy file \'%s\' to \'%s\'\n",
                                 this->files.at(i).first.toUtf8().constData(),
                                 this->files.at(i).second.toUtf8().constData());
            }
        }
    }
    RProgressFinalize();
    RLogger::unindent();
}
