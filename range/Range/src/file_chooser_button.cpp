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

#include "file_chooser_button.h"


FileChooserButton::FileChooserButton(const QString &labelText, bool saveFile, QWidget *parent, const QString &caption, const QString &path, const QString &filter)
    : QWidget(parent)
    , saveFile(saveFile)
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

    if (this->saveFile)
    {
        fileName = QFileDialog::getSaveFileName(this,
                                                this->caption,
                                                this->getFileName(),
                                                this->filter);
    }
    else
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
    }

    if (!fileName.isEmpty())
    {
        this->setFileName(fileName);
    }
}
