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

DocumentWidget::DocumentWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    QSplitter *splitter = new QSplitter;
    splitter->setOrientation(Qt::Horizontal);
    mainLayout->addWidget(splitter);

    this->listWidget = new QListWidget;
    splitter->addWidget(this->listWidget);

    QObject::connect(this->listWidget,&QListWidget::itemSelectionChanged,this,&DocumentWidget::onListSelectionChanged);

    this->textBrowser = new QTextBrowser;
    splitter->addWidget(this->textBrowser);
}

void DocumentWidget::addListItem(QIcon icon, const QString &text, const QString &key)
{
    QListWidgetItem *item = new QListWidgetItem(this->listWidget);
    item->setIcon(icon);
    item->setText(text);
    item->setData(Qt::UserRole,key);
}

void DocumentWidget::addListItem(const QString &text, const QString &key)
{
    QListWidgetItem *item = new QListWidgetItem(this->listWidget);
    item->setText(text);
    item->setData(Qt::UserRole,key);
}

void DocumentWidget::onListSelectionChanged(void)
{
    QList<QListWidgetItem*> selectedItems = this->listWidget->selectedItems();

    if (selectedItems.size() == 0)
    {
        this->textBrowser->clear();
        return;
    }

    const QString &key(selectedItems.at(0)->data(Qt::UserRole).toString());

    QString helpFile(RFileManager::buildPath(MainSettings::getInstance().findHelpDir(),"action_" + key + ".html"));

    QFile file(helpFile);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        RLogger::warning("Failed to load help file \'%s\'\n",helpFile.toUtf8().constData());
        this->textBrowser->setHtml(tr("No help available."));
    }
    else
    {
        QTextStream in(&file);
        this->textBrowser->setHtml(in.readAll());
        file.close();
    }
}
