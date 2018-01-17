/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   file_chooser_button.cpp                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   28-st August 2013                                        *
 *                                                                   *
 *  DESCRIPTION: File chooser button class definition                *
 *********************************************************************/

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

#include <rblib.h>

#include "file_chooser_button.h"

FileChooserButton::FileChooserButton(const QString &labelText, Type type, QWidget *parent, const QString &caption, const QString &path, const QString &filter)
    : QWidget(parent)
    , type(type)
    , caption(caption)
    , path(path)
    , filter(filter)
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    this->setLayout(layout);

    QIcon icon(":/icons/file/pixmaps/range-open.svg");

    this->label = new QLabel(labelText);
    this->label->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
    layout->addWidget(this->label);

    this->pushButton = new QPushButton(icon,QString());
    pushButton->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum));
    layout->addWidget(pushButton);

    this->setFileName(this->path);

    QObject::connect(this->pushButton,
                     &QPushButton::clicked,
                     this,
                     &FileChooserButton::onPushButtonClicked);
}

QString FileChooserButton::getFileName(void) const
{
    return this->path;
}

void FileChooserButton::setFileName(const QString &fileName)
{
    this->path = fileName;
    QFileInfo fileInfo(this->path);
    this->pushButton->setText(fileInfo.fileName());
    emit this->fileNameChanged(this->path);
}

void FileChooserButton::setFilter(const QString &filter)
{
    this->filter = filter;
}

void FileChooserButton::setShowLabel(bool showLabel)
{
    if (showLabel)
    {
        this->label->show();
    }
    else
    {
        this->label->hide();
    }
}

void FileChooserButton::onPushButtonClicked(void)
{
    QString fileName;

    switch (this->type)
    {
        case FileChooserButton::SaveFile:
        {
            fileName = QFileDialog::getSaveFileName(this,
                                                    this->caption,
                                                    this->getFileName(),
                                                    this->filter);
            break;
        }
        case FileChooserButton::OpenFile:
        {
            fileName = QFileDialog::getOpenFileName(this,
                                                    this->caption,
                                                    this->getFileName(),
                                                    this->filter);
            if (!fileName.isEmpty())
            {
                if (!QFile::exists(fileName))
                {
                    fileName.clear();
                }
            }
            break;
        }
        case FileChooserButton::Directory:
        {
            fileName = QFileDialog::getExistingDirectory(this,
                                                         this->caption,
                                                         this->getFileName());
            if (!fileName.isEmpty())
            {
                if (!QFile::exists(fileName))
                {
                    fileName.clear();
                }
            }
            break;
        }
        default:
        {
            RLogger::error("Unknown FileChooserButton::type\n");
            break;
        }
    }

    if (!fileName.isEmpty())
    {
        this->setFileName(fileName);
    }
}
