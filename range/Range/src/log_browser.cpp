/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   log_browser.cpp                                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   9-th August 2013                                         *
 *                                                                   *
 *  DESCRIPTION: Log browser class definition                        *
 *********************************************************************/

#include <QFileSystemWatcher>
#include <QFile>
#include <QTextStream>

#include <rblib.h>

#include "log_browser.h"

LogBrowser::LogBrowser(const QString &logFileName, QWidget *parent) :
    QTextBrowser(parent),
    logFileName(logFileName),
    fileStreamEnd(0)
{
    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    this->setFont(font);

    QFileSystemWatcher *systemWatcher = new QFileSystemWatcher(QStringList(this->logFileName),this);

    this->onFileChanged(this->logFileName);

    QObject::connect(systemWatcher,&QFileSystemWatcher::fileChanged,this,&LogBrowser::onFileChanged);
}

void LogBrowser::clearLog(void)
{
    QFile file(this->logFileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        RLogger::warning("Failed to open the file \'%s\' for writing.\n", this->logFileName.toUtf8().constData());
        return;
    }
    file.close();

    this->clear();
}

void LogBrowser::onFileChanged(const QString &path)
{
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        RLogger::warning("Failed to open the file \'%s\' for reading.\n", path.toUtf8().constData());
        return;
    }

    try
    {
        qint64 fileSize = file.size();

        if (fileSize <= this->fileStreamEnd)
        {
            this->fileStreamEnd = 0;
            this->clear();
        }

        QTextStream fileStream(&file);

        fileStream.seek(this->fileStreamEnd);

        while (!fileStream.atEnd())
        {
            // add line
            this->append(fileStream.readLine());
        }
        this->fileStreamEnd = fileStream.pos();
    }
    catch (...)
    {
        RLogger::error("Unknown error while reading the log file.\n");
    }

    file.close();
}
