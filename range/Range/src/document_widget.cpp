/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   document_widget.cpp                                      *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   26-th November 2017                                      *
 *                                                                   *
 *  DESCRIPTION: Document widget class definition                    *
 *********************************************************************/

#include <QSplitter>
#include <QVBoxLayout>

#include <rmlib.h>

#include "document_widget.h"
#include "main_settings.h"

DocumentWidget::DocumentWidget(const QString &defaultFileName, QWidget *parent)
    : QWidget(parent)
    , defaultFileName(defaultFileName)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    QSplitter *splitter = new QSplitter;
    splitter->setOrientation(Qt::Horizontal);
    mainLayout->addWidget(splitter);

    this->listWidget = new QListWidget;
    this->listWidget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    splitter->addWidget(this->listWidget);

    QObject::connect(this->listWidget,&QListWidget::itemSelectionChanged,this,&DocumentWidget::onListSelectionChanged);

    this->textBrowser = new QTextBrowser;
    this->textBrowser->setSearchPaths(QStringList() << MainSettings::getInstance().getApplicationSettings()->getHelpDir());
    this->textBrowser->setReadOnly(true);
    this->textBrowser->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Expanding);
    splitter->addWidget(this->textBrowser);

    splitter->setStretchFactor(1,1);

    this->loadFile(this->defaultFileName);
}

void DocumentWidget::addListItem(QIcon icon, const QString &text, const QString &fileName)
{
    QListWidgetItem *item = new QListWidgetItem(this->listWidget);
    item->setIcon(icon);
    item->setText(text);
    item->setData(Qt::UserRole,fileName);
}

void DocumentWidget::addListItem(const QString &text, const QString &fileName)
{
    QListWidgetItem *item = new QListWidgetItem(this->listWidget);
    item->setText(text);
    item->setData(Qt::UserRole,fileName);
}

void DocumentWidget::loadFile(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        this->textBrowser->clear();
    }
    else
    {
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            RLogger::warning("Failed to load document file \'%s\'\n",fileName.toUtf8().constData());
            this->textBrowser->setHtml(tr("No document file available."));
        }
        else
        {
            QTextStream in(&file);
            this->textBrowser->setHtml(in.readAll());
            file.close();
        }
    }
}

void DocumentWidget::onListSelectionChanged(void)
{
    QList<QListWidgetItem*> selectedItems = this->listWidget->selectedItems();

    if (selectedItems.size() == 0)
    {
        this->loadFile(this->defaultFileName);
    }
    else
    {
        this->loadFile(selectedItems.at(0)->data(Qt::UserRole).toString());
    }
}
