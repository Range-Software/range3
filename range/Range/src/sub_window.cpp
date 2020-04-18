/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   sub_window.cpp                                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   29-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Sub window class definition                         *
 *********************************************************************/

#include <QFileDialog>
#include <QGridLayout>

#include "sub_window.h"
#include "session.h"
#include "gl_display_properties_dialog.h"
#include "main_window.h"
#include "clipping_plane_widget.h"

SubWindow::SubWindow (uint modelID, QWidget *parent)
    : QMdiSubWindow(parent)
{
    this->modelID = modelID;
    this->createSubWindow();
}

void SubWindow::createSubWindow(void)
{
    QString windowTitle = Session::getInstance().getModel(this->modelID).getName();
    this->setWindowTitle(windowTitle);
    this->setWindowIcon(QIcon(":/icons/logos/pixmaps/range-logo-128.png"));

    QWidget *mainWidget = new QWidget(this);
    this->setWidget(mainWidget);

    QGridLayout *gridLayoutSubwindow = new QGridLayout(mainWidget);
    this->glWidget = new GLWidget(this->getModelID());
//    QSurfaceFormat format;
//    format.setDepthBufferSize(24);
//    format.setStencilBufferSize(8);
//    format.setVersion(3, 2);
//    format.setProfile(QSurfaceFormat::CoreProfile);
//    this->glWidget->setFormat(format);
    gridLayoutSubwindow->addWidget(this->glWidget, 0, 0, 1, 1);

    int toolbarIconSize = MainSettings::getInstance().getApplicationSettings()->getToolbarIconSize();
    this->toolBar = new QToolBar(QString("Sub window toolbar"));
    this->toolBar->setIconSize(QSize(toolbarIconSize,toolbarIconSize));

    gridLayoutSubwindow->addWidget(toolBar, 1, 0, 1, 1);

    QAction *actionResetO = new QAction(this);
    actionResetO->setText(tr("Reset to original view."));
    actionResetO->setShortcut(QString("O"));
    actionResetO->setIcon(QIcon(":/icons/file/pixmaps/range-resetO.svg"));
    this->toolBar->addAction(actionResetO);
    QObject::connect(actionResetO,&QAction::triggered,this,&SubWindow::onResetO);

    QAction *actionResetX = new QAction(this);
    actionResetX->setText(tr("Reset to X plane."));
    actionResetX->setShortcut(QString("X"));
    actionResetX->setIcon(QIcon(":/icons/file/pixmaps/range-resetX.svg"));
    this->toolBar->addAction(actionResetX);
    QObject::connect(actionResetX,&QAction::triggered,this,&SubWindow::onResetX);

    QAction *actionResetY = new QAction(this);
    actionResetY->setText(tr("Reset to Y plane."));
    actionResetY->setShortcut(QString("Y"));
    actionResetY->setIcon(QIcon(":/icons/file/pixmaps/range-resetY.svg"));
    this->toolBar->addAction(actionResetY);
    QObject::connect(actionResetY,&QAction::triggered,this,&SubWindow::onResetY);

    QAction *actionResetZ = new QAction(this);
    actionResetZ->setText(tr("Reset to Z plane."));
    actionResetZ->setShortcut(QString("Z"));
    actionResetZ->setIcon(QIcon(":/icons/file/pixmaps/range-resetZ.svg"));
    this->toolBar->addAction(actionResetZ);
    QObject::connect(actionResetZ,&QAction::triggered,this,&SubWindow::onResetZ);

    this->toolBar->addSeparator();

    QAction *actionScreenshot = new QAction(this);
    actionScreenshot->setText("Take screen shot.");
    actionScreenshot->setShortcut(QString("Ctrl+P"));
    actionScreenshot->setIcon(QIcon(":/icons/file/pixmaps/range-screenshot.svg"));
    this->toolBar->addAction(actionScreenshot);
    QObject::connect(actionScreenshot,&QAction::triggered,this,&SubWindow::onScreenshot);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->toolBar->addWidget(spacer);

    QCheckBox *backSideColorCheck = new QCheckBox(tr("Back side color"));
    this->toolBar->addWidget(backSideColorCheck);

    QObject::connect(backSideColorCheck,&QCheckBox::stateChanged,this,&SubWindow::onBackSideColorChanged);

    ClippingPlaneWidget *clippingPlaneWidget = new ClippingPlaneWidget;
    this->toolBar->addWidget(clippingPlaneWidget);

    QAction *actionPreferences = new QAction(this);
    actionPreferences->setText("Display preferences.");
    actionPreferences->setShortcut(QString("Ctrl+P"));
    actionPreferences->setIcon(QIcon(":/icons/application/pixmaps/range-display_preferences.svg"));
    this->toolBar->addAction(actionPreferences);
    QObject::connect(actionPreferences,&QAction::triggered,this,&SubWindow::onPreferences);

    QObject::connect(&Session::getInstance(),
                     &Session::takeScreenShot,
                     this,
                     &SubWindow::onSessionScreenshot);
    QObject::connect(&Session::getInstance(),
                     &Session::takeScreenShotBlocking,
                     this,
                     &SubWindow::onSessionScreenshot,
                     Qt::BlockingQueuedConnection);

    QObject::connect(clippingPlaneWidget,
                     &ClippingPlaneWidget::changed,
                     this->glWidget,
                     &GLWidget::setClippingPlane);

    // Tooplbar icon size changed signal
    QObject::connect(MainSettings::getInstance().getApplicationSettings(),
                     &ApplicationSettings::toolbarIconSizeChanged,
                     this,
                     &SubWindow::onToolbarIconSizeChanged);
}

uint SubWindow::getModelID(void) const
{
    return this->modelID;
}

void SubWindow::setModelID(uint modelID)
{
    this->modelID = modelID;
    this->glWidget->setModelID(modelID);
}

GLWidget *SubWindow::getGlWidget(void) const
{
    return this->glWidget;
}

void SubWindow::closeEvent(QCloseEvent *closeEvent)
{
    emit closed(this->getModelID());
    closeEvent->accept();
}

void SubWindow::onResetO(void)
{
    this->glWidget->resetView(-45.0, 0.0, -135.0);
}

void SubWindow::onResetX(void)
{
    this->glWidget->resetView(-90.0, 0.0, -90.0);
}

void SubWindow::onResetY(void)
{
    this->glWidget->resetView(-90.0, 0.0, 180.0);
}

void SubWindow::onResetZ(void)
{
    this->glWidget->resetView(0.0, 0.0, 0.0);
}

void SubWindow::onScreenshot(void)
{
    this->onSessionScreenshot(this->getModelID(),QString());
}

void SubWindow::onSessionScreenshot(uint modelID, const QString &fileName)
{
    if (modelID != this->getModelID())
    {
        return;
    }

    this->glWidget->takeScreenShot(fileName);
}

void SubWindow::onShowModelEdgesToggled(bool checked)
{
    this->glWidget->getGLDisplayProperties().setShowModelEdges(checked);
    this->glWidget->getGLDisplayProperties().store();
    this->glWidget->update();
}

void SubWindow::onShowErrorsToggled(bool checked)
{
    this->glWidget->getGLDisplayProperties().setShowErrors(checked);
    this->glWidget->getGLDisplayProperties().store();
    this->glWidget->update();
}

void SubWindow::onBackSideColorChanged(int state)
{
    this->glWidget->setUseGlCullFace(state == Qt::Unchecked);
}

void SubWindow::onPreferences(void)
{
    GLDisplayPropertiesDialog preferencesDialog(this->glWidget,MainWindow::getInstance());
    if (preferencesDialog.exec() == QDialog::Accepted)
    {
        this->glWidget->update();
    }
}

void SubWindow::onToolbarIconSizeChanged(int toolbarIconSize)
{
    this->toolBar->setIconSize(QSize(toolbarIconSize,toolbarIconSize));
}
