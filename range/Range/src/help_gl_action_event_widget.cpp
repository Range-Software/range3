/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   help_gl_action_event_widget.cpp                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   31-st October 2020                                       *
 *                                                                   *
 *  DESCRIPTION: Help GL action event widget class definition        *
 *********************************************************************/

#include <QSplitter>
#include <QVBoxLayout>

#include "gl_acion_event.h"
#include "help_gl_action_event_widget.h"

HelpGLActionEventWidget::HelpGLActionEventWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    QSplitter *splitter = new QSplitter;
    splitter->setOrientation(Qt::Horizontal);
    mainLayout->addWidget(splitter);

    this->listWidget = new QListWidget;
    this->listWidget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    splitter->addWidget(this->listWidget);

    QObject::connect(this->listWidget,&QListWidget::itemSelectionChanged,this,&HelpGLActionEventWidget::onListSelectionChanged);

    this->textBrowser = new QTextBrowser;
    this->textBrowser->setReadOnly(true);
    this->textBrowser->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Expanding);
    splitter->addWidget(this->textBrowser);

    splitter->setStretchFactor(1,1);

    for (uint glActionEventType=GL_ACTION_EVENT_NONE+1;glActionEventType<GL_ACTION_N_EVENTS;glActionEventType++)
    {
        QVector<QString> keyMouseCombinationList = GLActionEvent::findKeyMouseCombinations(GLActionEventType(glActionEventType));

        QListWidgetItem *item = new QListWidgetItem(this->listWidget);
//        item->setIcon(icon);
        item->setText(GLActionEvent::toString(GLActionEventType(glActionEventType)));
        QString keyMouseCombinationText = "<ul>";
        foreach (auto keyMouseCombination,keyMouseCombinationList)
        {
            keyMouseCombinationText += "<li>" + keyMouseCombination + "</li>";
        }
        keyMouseCombinationText += "</ul>";
        item->setData(Qt::UserRole,keyMouseCombinationText);
    }
//    this->addListItem(tipName,
//                      RFileManager::buildPath(MainSettings::getInstance().getApplicationSettings()->getHelpDir(),"general_" + tipName + ".html"));

//    this->loadFile(this->defaultFileName);
}

void HelpGLActionEventWidget::onListSelectionChanged(void)
{
    QList<QListWidgetItem*> selectedItems = this->listWidget->selectedItems();

    this->textBrowser->clear();
    if (selectedItems.size() != 0)
    {
        this->textBrowser->setHtml(selectedItems.at(0)->data(Qt::UserRole).toString());
    }
}
