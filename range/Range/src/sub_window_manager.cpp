/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   sub_window_manager.cpp                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   3-rd February 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Sub window manager class definition                 *
 *********************************************************************/

#include "session.h"
#include "sub_window_manager.h"

SubWindowManager::SubWindowManager(QMdiArea *mdiArea, QObject *parent) : QObject(parent)
{
    this->mdiArea = mdiArea;
}

uint SubWindowManager::findWindow(uint modelID)
{
    for (int i=0;i<this->subWindows.size();i++)
    {
        if (this->subWindows[i]->getModelID() == modelID)
        {
            return uint(i);
        }
    }
    return uint(this->subWindows.size());
}

bool SubWindowManager::windowCreate(uint modelID)
{
    if (this->windowExists(modelID))
    {
        return false;
    }

    SubWindow *subWindow = new SubWindow(modelID,qobject_cast<QWidget*>(this->parent()));
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    this->mdiArea->addSubWindow(subWindow,Qt::SubWindow);

    if (this->subWindows.empty())
    {
        subWindow->showMaximized();
    }

    this->subWindows.append(subWindow);

    subWindow->show();

    QObject::connect(subWindow,
                     &SubWindow::closed,
                     this,
                     &SubWindowManager::onWindowClosed);

    emit this->windowCreated(modelID);

    return true;
}

bool SubWindowManager::windowClose(uint modelID)
{
    uint winPos = this->findWindow(modelID);
    if (winPos == this->getNWindows())
    {
        return false;
    }

    SubWindow *subWindow = this->subWindows[int(winPos)];
//    this->mdiArea->removeSubWindow(subWindow);
    delete subWindow;

    this->subWindows.removeAt(int(winPos));

    emit this->windowClosed(modelID);

    return true;
}

bool SubWindowManager::windowExists(uint modelID)
{
    uint winPos = this->findWindow(modelID);
    if (winPos == this->getNWindows())
    {
        return false;
    }
    return true;
}

uint SubWindowManager::getNWindows(void) const
{
    return this->subWindows.size();
}

void SubWindowManager::onModelAdded(uint modelID)
{
    for (int i=0;i<this->subWindows.size();i++)
    {
        if (this->subWindows[i]->getModelID() >= modelID)
        {
            this->subWindows[i]->setModelID(this->subWindows[i]->getModelID()+1);
        }
    }

    this->windowCreate(modelID);
}

void SubWindowManager::onModelRemoved(uint modelID)
{
    this->windowClose(modelID);

    for (int i=0;i<this->subWindows.size();i++)
    {
        if (this->subWindows[i]->getModelID() > modelID)
        {
            this->subWindows[i]->setModelID(this->subWindows[i]->getModelID()-1);
        }
    }
}

void SubWindowManager::onModelChanged(uint)
{

}

void SubWindowManager::onWindowClosed(uint modelID)
{
    Session::getInstance().setModelVisible(modelID,false);
    uint winPos = this->findWindow(modelID);
    this->subWindows.removeAt(winPos);
}
