/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   central_tab_widget.cpp                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   12-th December 2017                                      *
 *                                                                   *
 *  DESCRIPTION: Main window's central tab widget class definition   *
 *********************************************************************/

#include <QMessageBox>
#include <QApplication>
#include <QScrollBar>

#include "central_tab_widget.h"
#include "logger.h"
#include "session.h"
#include "solver_manager.h"

const QString CentralTabWidget::informationIconFile(":/icons/file/pixmaps/range-information.svg");
const QString CentralTabWidget::importantIconFile(":/icons/file/pixmaps/range-important.svg");
const QString CentralTabWidget::severeIconFile(":/icons/file/pixmaps/range-severe.svg");

const bool useTabIcons = false;

CentralTabWidget::CentralTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    this->setTabPosition(QTabWidget::South);

    this->mdiArea = new QMdiArea(this);
    this->mdiArea->setObjectName(QString::fromUtf8("mdiArea"));
    this->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    this->applicationOutputBrowser = new TextBrowser(true);
    this->processOutputBrowser = new TextBrowser(true);
    this->pickDetailsTree = new PickDetailsTree;


    this->modelTabPosition = this->addTab(this->mdiArea,QString());
    this->applicationOutputTabPosition = this->addTab(this->applicationOutputBrowser,QString());
    this->processOutputTabPosition = this->addTab(this->processOutputBrowser,QString());
    this->pickDetailsTabPosition = this->addTab(this->pickDetailsTree,QString());

    this->setTabTitle(CentralTabWidget::Model);
    this->setTabTitle(CentralTabWidget::ApplicationOutput);
    this->setTabTitle(CentralTabWidget::ProcessOutput);
    this->setTabTitle(CentralTabWidget::PickDetails);

    QObject::connect(this,&QTabWidget::currentChanged,this,&CentralTabWidget::onCurrentChanged);

    // Pick list signals
    QObject::connect(&Session::getInstance().getPickList(),&PickList::pickListChanged,this,&CentralTabWidget::onPickLostChanged);

    // Logger signals
    QObject::connect(&Logger::getInstance(),&Logger::infoPrinted,this,&CentralTabWidget::onInfoPrinted);
    QObject::connect(&Logger::getInstance(),&Logger::noticePrinted,this,&CentralTabWidget::onNoticePrinted);
    QObject::connect(&Logger::getInstance(),&Logger::warningPrinted,this,&CentralTabWidget::onWarningPrinted);
    QObject::connect(&Logger::getInstance(),&Logger::errorPrinted,this,&CentralTabWidget::onErrorPrinted);

    // Solver manager signals
    QObject::connect(&SolverManager::getInstance(),&SolverManager::readyReadStandardOutput,this,&CentralTabWidget::onProcessReadyStandardOutput);
    QObject::connect(&SolverManager::getInstance(),&SolverManager::readyReadStandardError,this,&CentralTabWidget::onProcessReadyStandardError);
}

QMdiArea *CentralTabWidget::getMdiArea(void)
{
    return this->mdiArea;
}

void CentralTabWidget::setTabTitle(CentralTabWidget::Type tabType, RMessageType messageType, const QString &additionalText)
{
    QString defaultText;
    int tabPosition = 0;
    QString iconFile;

    if (tabType == CentralTabWidget::Model)
    {
        defaultText = tr("Model");
        tabPosition = this->modelTabPosition;
    }
    else if (tabType == CentralTabWidget::ApplicationOutput)
    {
        defaultText = tr("Application output");
        tabPosition = this->applicationOutputTabPosition;
        iconFile = this->applicationOutputLastIcon;
    }
    else if (tabType == CentralTabWidget::ProcessOutput)
    {
        defaultText = tr("Process output");
        tabPosition = this->processOutputTabPosition;
        iconFile = this->processOutputLastIcon;
    }
    else if (tabType == CentralTabWidget::PickDetails)
    {
        defaultText = tr("Pick details");
        tabPosition = this->pickDetailsTabPosition;
    }
    else
    {
        return;
    }

    if (!additionalText.isEmpty())
    {
        defaultText += " (" + additionalText + ")";
    }

    if (this->currentIndex() == tabPosition)
    {
        iconFile = QString();
    }
    else
    {
        if (messageType == R_MESSAGE_INFO)
        {
            if (iconFile != CentralTabWidget::importantIconFile && iconFile != CentralTabWidget::severeIconFile)
            {
                iconFile = CentralTabWidget::informationIconFile;
            }
        }
        else if (messageType == R_MESSAGE_WARNING)
        {
            if (iconFile != CentralTabWidget::severeIconFile)
            {
                iconFile = CentralTabWidget::importantIconFile;
            }
        }
        else if (messageType == R_MESSAGE_ERROR)
        {
            iconFile = CentralTabWidget::severeIconFile;
        }
    }

    if (tabType == CentralTabWidget::ApplicationOutput)
    {
        this->applicationOutputLastIcon = iconFile;
    }
    else if (tabType == CentralTabWidget::ProcessOutput)
    {
        this->processOutputLastIcon = iconFile;
    }

    QColor tabTextColor = QApplication::palette().text().color();
    if (!iconFile.isEmpty())
    {
        // TODO: For unknown reason this hangs if text is printed from QDialog::exec()
        if (useTabIcons)
        {
            this->setTabIcon(tabPosition,QIcon(iconFile));
        }
        if (iconFile == CentralTabWidget::informationIconFile)
        {
            if (!useTabIcons)
            {
                defaultText.prepend("* ");
            }
            tabTextColor = Qt::darkCyan;
        }
        else if (iconFile == CentralTabWidget::importantIconFile)
        {
            if (!useTabIcons)
            {
                defaultText.prepend("! ");
            }
            tabTextColor = Qt::red;
        }
        else if (iconFile == CentralTabWidget::severeIconFile)
        {
            if (!useTabIcons)
            {
                defaultText.prepend("!!! ");
                defaultText.append(" !!!");
            }
            tabTextColor = Qt::red;
        }
    }
    this->tabBar()->setTabTextColor(tabPosition,tabTextColor);
    this->setTabText(tabPosition,defaultText);
}

void CentralTabWidget::onInfoPrinted(const QString &message)
{
    QScrollBar *sb = this->applicationOutputBrowser->verticalScrollBar();
    int scrollBarValue = sb->value();
    bool scrollBarAtMax = ((sb->maximum() - scrollBarValue) <= 10);

    this->applicationOutputBrowser->moveCursor(QTextCursor::End);
    this->applicationOutputBrowser->setTextBackgroundColor(QApplication::palette().base().color());
    this->applicationOutputBrowser->setTextColor(QApplication::palette().text().color());
    this->applicationOutputBrowser->insertPlainText(message);
    this->applicationOutputBrowser->moveCursor(QTextCursor::End);
    if (scrollBarAtMax)
    {
        scrollBarValue = sb->maximum();
    }
    sb->setValue(scrollBarValue);

    this->setTabTitle(CentralTabWidget::ApplicationOutput,R_MESSAGE_INFO);
}

void CentralTabWidget::onNoticePrinted(const QString &message)
{
    QMessageBox::information(this,tr("Notice"),QString(message).replace("NOTICE: ",""));
}

void CentralTabWidget::onWarningPrinted(const QString &message)
{
    QTextCharFormat charFormat = this->applicationOutputBrowser->currentCharFormat();

    this->applicationOutputBrowser->moveCursor(QTextCursor::End);
    this->applicationOutputBrowser->setTextBackgroundColor(QApplication::palette().base().color());
    this->applicationOutputBrowser->setTextColor(QColor(170,0,0));
    this->applicationOutputBrowser->insertPlainText(message);
    this->applicationOutputBrowser->moveCursor(QTextCursor::End);
    this->applicationOutputBrowser->setTextColor(QApplication::palette().text().color());
    this->applicationOutputBrowser->setCurrentCharFormat(charFormat);
    QScrollBar *sb = this->applicationOutputBrowser->verticalScrollBar();
    sb->setValue(sb->maximum());
    this->setTabTitle(CentralTabWidget::ApplicationOutput,R_MESSAGE_WARNING);
}

void CentralTabWidget::onErrorPrinted(const QString &message)
{
    QTextCharFormat charFormat = this->applicationOutputBrowser->currentCharFormat();

    this->applicationOutputBrowser->moveCursor(QTextCursor::End);
    this->applicationOutputBrowser->setTextBackgroundColor(QColor(170,0,0));
    this->applicationOutputBrowser->setTextColor(QColor(255,255,255));
    this->applicationOutputBrowser->insertPlainText(message);
    this->applicationOutputBrowser->moveCursor(QTextCursor::End);
    this->applicationOutputBrowser->setTextBackgroundColor(QApplication::palette().base().color());
    this->applicationOutputBrowser->setTextColor(QApplication::palette().text().color());
    this->applicationOutputBrowser->setCurrentCharFormat(charFormat);
    QScrollBar *sb = this->applicationOutputBrowser->verticalScrollBar();
    sb->setValue(sb->maximum());
//    this->setCurrentIndex(this->applicationOutputTabPosition);
    this->setTabTitle(CentralTabWidget::ApplicationOutput,R_MESSAGE_ERROR);
}

void CentralTabWidget::onProcessReadyStandardOutput(const QString &message)
{
    QScrollBar *sb = this->processOutputBrowser->verticalScrollBar();
    int scrollBarValue = sb->value();
    bool scrollBarAtMax = ((sb->maximum() - scrollBarValue) <= 10);

    this->processOutputBrowser->moveCursor(QTextCursor::End);
    this->processOutputBrowser->setTextBackgroundColor(QApplication::palette().base().color());
    this->processOutputBrowser->setTextColor(QApplication::palette().text().color());
    this->processOutputBrowser->insertPlainText(message);
    this->processOutputBrowser->moveCursor(QTextCursor::End);
    if (scrollBarAtMax)
    {
        scrollBarValue = sb->maximum();
    }
    sb->setValue(scrollBarValue);

    QVector<uint> runningIDs = SolverManager::getInstance().getRunningIDs();
    static uint lastRunningID = 0;
    if (runningIDs.size() > 0)
    {
        if (runningIDs[0] != lastRunningID)
        {
            this->setCurrentIndex(this->processOutputTabPosition);
            lastRunningID = runningIDs[0];
        }
    }

    this->setTabTitle(CentralTabWidget::ProcessOutput,R_MESSAGE_INFO);
}

void CentralTabWidget::onProcessReadyStandardError(const QString &message)
{
    QTextCharFormat charFormat = this->processOutputBrowser->currentCharFormat();

    this->processOutputBrowser->moveCursor(QTextCursor::End);
    this->processOutputBrowser->setTextBackgroundColor(QColor(170,0,0));
    this->processOutputBrowser->setTextColor(QColor(255,255,255));
    this->processOutputBrowser->insertPlainText(message);
    this->processOutputBrowser->moveCursor(QTextCursor::End);
    this->processOutputBrowser->setTextBackgroundColor(QApplication::palette().base().color());
    this->processOutputBrowser->setTextColor(QApplication::palette().text().color());
    this->processOutputBrowser->setCurrentCharFormat(charFormat);
    QScrollBar *sb = this->processOutputBrowser->verticalScrollBar();
    sb->setValue(sb->maximum());
    this->setCurrentIndex(this->processOutputTabPosition);
    this->setTabTitle(CentralTabWidget::ProcessOutput,R_MESSAGE_ERROR);
}

void CentralTabWidget::onCurrentChanged(int tabPosition)
{
    if (useTabIcons)
    {
        this->setTabIcon(tabPosition,QIcon());
    }
    if (tabPosition == this->applicationOutputTabPosition)
    {
        this->setTabTitle(CentralTabWidget::ApplicationOutput);
    }
    else if (tabPosition == this->processOutputTabPosition)
    {
        this->setTabTitle(CentralTabWidget::ProcessOutput);
    }
}

void CentralTabWidget::onPickLostChanged(void)
{
    int nItems = Session::getInstance().getPickList().getItems().size();
    if (nItems > 0)
    {
        this->setTabTitle(CentralTabWidget::PickDetails,R_MESSAGE_NONE,QString::number(nItems));
    }
    else
    {
        this->setTabTitle(CentralTabWidget::PickDetails);
    }
}
