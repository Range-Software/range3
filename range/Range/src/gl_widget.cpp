/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_widget.cpp                                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   9-th January 2012                                        *
 *                                                                   *
 *  DESCRIPTION: OpenGL widget class definition                      *
 *********************************************************************/

#include <QApplication>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QPainter>
#include <QGuiApplication>

#include <rmlib.h>

#include "gl_arrow.h"
#include "gl_axis.h"
#include "gl_dimension.h"
#include "gl_cut_plane.h"
#include "gl_line.h"
#include "gl_point.h"
#include "gl_rotation_sphere.h"
#include "gl_widget.h"
#include "gl_functions.h"
#include "pick_list.h"
#include "session.h"
#include "main_window.h"
#include "main_settings.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

const GLsizei GLWidget::lAxisWpWidth = 100;
const GLsizei GLWidget::lAxisWpHeight = 100;
//static int GLWidget::dsktopDevicePixelRatio = 1;

GLWidget::GLWidget(uint modelID, QWidget *parent)
    : QOpenGLWidget(parent),
      modelID(modelID),
      drx(0.0),
      dry(0.0),
      dtx(0.0),
      dty(0.0),
      dtz(0.0),
      dscale(0.0),
      mscale(1.0),
      scale(1.0),
      drawStreamLinePosition(false),
      drawScaleOrigin(false),
      drawRotationOrigin(false),
      drawLocalDirections(false),
      drawCutPlane(false),
      drawMoveNodes(false),
      useGlVoidModelList(false),
      clippingPlaneEnabled(false),
      clippingPlaneDistance(0.5),
      showRotationSphere(false),
      useGlCullFace(true)
{
    this->desktopDevicePixelRatio = QApplication::desktop()->devicePixelRatio();

    this->setFocusPolicy(Qt::StrongFocus);
    this->setAutoFillBackground(false);

    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    this->setFormat(format);

    QString displayPropertiesFileName(Session::getInstance().getModel(this->modelID).buildDataFileName(RGLDisplayProperties::getDefaultFileExtension(true)));

    this->displayProperties.load(displayPropertiesFileName);

    QObject::connect(&Session::getInstance(),&Session::entityVisibilityChanged,this,&GLWidget::onEntityVisibilityChanged);
    QObject::connect(&Session::getInstance(),&Session::displayPropertiesChanged,this,&GLWidget::onDisplayPropertiesChanged);
    this->connect(&Session::getInstance(),
                  SIGNAL(variableDataChanged(uint,RVariableType)),
                  SLOT(onVariableDataChanged(uint,RVariableType)));
    this->connect(&Session::getInstance(),
                  SIGNAL(variableDataChanged(SessionEntityID,RVariableType)),
                  SLOT(onVariableDataChanged(SessionEntityID,RVariableType)));
    QObject::connect(&Session::getInstance(),&Session::modelChanged,this,&GLWidget::onModelChanged);
    QObject::connect(&Session::getInstance(),&Session::resultsChanged,this,&GLWidget::onResultsChanged);
    QObject::connect(&Session::getInstance(),&Session::beginDrawStreamLinePosition,this,&GLWidget::onBeginDrawStreamLinePosition);
    QObject::connect(&Session::getInstance(),&Session::endDrawStreamLinePosition,this,&GLWidget::onEndDrawStreamLinePosition);
    QObject::connect(&Session::getInstance(),&Session::beginDrawScaleOrigin,this,&GLWidget::onBeginDrawScaleOrigin);
    QObject::connect(&Session::getInstance(),&Session::endDrawScaleOrigin,this,&GLWidget::onEndDrawScaleOrigin);
    QObject::connect(&Session::getInstance(),&Session::beginDrawRotationOrigin,this,&GLWidget::onBeginDrawRotationOrigin);
    QObject::connect(&Session::getInstance(),&Session::endDrawRotationOrigin,this,&GLWidget::onEndDrawRotationOrigin);
    QObject::connect(&Session::getInstance(),&Session::beginDrawLocalDirections,this,&GLWidget::onBeginDrawLocalDirections);
    QObject::connect(&Session::getInstance(),&Session::endDrawLocalDirections,this,&GLWidget::onEndDrawLocalDirections);
    QObject::connect(&Session::getInstance(),&Session::beginDrawCutPlane,this,&GLWidget::onBeginDrawCutPlane);
    QObject::connect(&Session::getInstance(),&Session::endDrawCutPlane,this,&GLWidget::onEndDrawCutPlane);
    QObject::connect(&Session::getInstance(),&Session::beginDrawMoveNodes,this,&GLWidget::onBeginDrawMoveNodes);
    QObject::connect(&Session::getInstance(),&Session::endDrawMoveNodes,this,&GLWidget::onEndDrawMoveNodes);
    QObject::connect(Session::getInstance().getDrawEngine(),&DrawEngine::objectAdded,this,&GLWidget::onDrawObjectAdded);
    QObject::connect(Session::getInstance().getDrawEngine(),&DrawEngine::objectRemoved,this,&GLWidget::onDrawObjectRemoved);
    QObject::connect(Session::getInstance().getDrawEngine(),&DrawEngine::objectChanged,this,&GLWidget::onDrawObjectChanged);

    this->font = QPainter(this).font();
}

uint GLWidget::getModelID(void) const
{
    return this->modelID;
}

void GLWidget::setModelID(uint modelID)
{
    this->modelID = modelID;
}

QSize GLWidget::minimumSizeHint(void) const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint(void) const
{
    return QSize(400, 400);
}

void GLWidget::initializeGL(void)
{
    this->resetView(-45.0, 0.0, -135.0);
}

void GLWidget::resizeGL(int width, int height)
{
    GL_SAFE_CALL(glViewport(0, 0, GLsizei(width*this->desktopDevicePixelRatio), GLsizei(height*this->desktopDevicePixelRatio)));
    GL_SAFE_CALL(glMatrixMode(GL_PROJECTION));
    GL_SAFE_CALL(glLoadIdentity());

    GLdouble winRatio = GLdouble(height)/GLdouble(width);
    GLdouble winScale = this->calculateViewDepth();

    GL_SAFE_CALL(glOrtho(-1.0, 1.0, -winRatio, winRatio, -winScale, winScale));
    GL_SAFE_CALL(glMatrixMode(GL_MODELVIEW));
}

void GLWidget::paintGL(void)
{
    this->qglClearColor(this->displayProperties.getBgColor());

    GL_SAFE_CALL(glPushMatrix());
    GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    if (this->displayProperties.getBgGradient())
    {
        this->drawBackgroundGradient();
    }

    GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
    GL_SAFE_CALL(glShadeModel(GL_SMOOTH));
    GL_SAFE_CALL(glEnable(GL_MULTISAMPLE));
    GL_SAFE_CALL(glDepthFunc(GL_LEQUAL));

    this->drawModel();

    GL_SAFE_CALL(glPopMatrix());
    GL_SAFE_CALL(glFlush());

    QPainter painter(this);

    this->glTextRenderer.render(painter);
    this->glTextRenderer.clear();

    painter.setFont(this->font);

    this->drawValueRanges(painter);
    this->drawMessageBox(painter,false);
    this->drawInfoBox(painter,false);

    painter.end();

    this->makeCurrent();
}

void GLWidget::drawBackgroundGradient(void)
{


    // MAIN VIEWPORT
    GL_SAFE_CALL(glViewport(0, 0, GLsizei(this->width()*this->desktopDevicePixelRatio), GLsizei(this->height()*this->desktopDevicePixelRatio)));

    GL_SAFE_CALL(glMatrixMode(GL_PROJECTION));
    GL_SAFE_CALL(glLoadIdentity());

    GL_SAFE_CALL(glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0));

    GL_SAFE_CALL(glMatrixMode(GL_MODELVIEW));
    GL_SAFE_CALL(glLoadIdentity());

    GL_SAFE_CALL(glClear(GL_DEPTH_BUFFER_BIT));

    GL_SAFE_CALL(glDisable(GL_LIGHTING));
    GL_SAFE_CALL(glEnable(GL_BLEND));
    GL_SAFE_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_SAFE_CALL(glShadeModel(GL_SMOOTH));
    GL_SAFE_CALL(glEnable(GL_LINE_SMOOTH));
    GL_SAFE_CALL(glDepthFunc(GL_LEQUAL));

    GLFunctions::begin(GL_QUADS);

    this->qglColor(QColor(255,255,255,0));
    GL_SAFE_CALL(glVertex3d( 1.0,  0.3, 0.0));
    GL_SAFE_CALL(glVertex3d(-1.0,  0.3, 0.0));
    GL_SAFE_CALL(glColor4f(1.0f,1.0f,1.0f,0.4f));
    this->qglColor(QColor(255,255,255,100));
    GL_SAFE_CALL(glVertex3d(-1.0, -1.0, 0.0));
    GL_SAFE_CALL(glVertex3d( 1.0, -1.0, 0.0));

    GLFunctions::end();
}

void GLWidget::drawModel(void)
{
    // MAIN VIEWPORT
    GL_SAFE_CALL(glViewport(0, 0, GLsizei(this->width()*this->desktopDevicePixelRatio), GLsizei(this->height()*this->desktopDevicePixelRatio)));

    GL_SAFE_CALL(glMatrixMode(GL_PROJECTION));
    GL_SAFE_CALL(glLoadIdentity());

    GLdouble winRatio = GLdouble(this->height())/GLdouble(this->width());
    GLdouble winScale = this->calculateViewDepth();

    GL_SAFE_CALL(glOrtho(-1.0, 1.0, -winRatio, winRatio, -winScale, winScale));

    GL_SAFE_CALL(glMatrixMode(GL_MODELVIEW));
    GL_SAFE_CALL(glLoadIdentity());

    GL_SAFE_CALL(glClear(GL_DEPTH_BUFFER_BIT));

    if (this->clippingPlaneEnabled)
    {
        const GLdouble clippingPlane[4] = { 0.0, 0.0, -1.0, - GLdouble(this->scale)  + 2.0 * this->clippingPlaneDistance * GLdouble(this->scale) };
        GL_SAFE_CALL(glClipPlane(GL_CLIP_PLANE0,clippingPlane));
    }

    GL_SAFE_CALL(glEnable(GL_LIGHTING));
    for (uint i=0;i<this->displayProperties.getNLights();i++)
    {
        this->showLight(this->displayProperties.getLight(i));
    }
    GL_SAFE_CALL(glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0));

    GL_SAFE_CALL(glShadeModel(GL_SMOOTH));
    GL_SAFE_CALL(glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE));
    GL_SAFE_CALL(glEnable(GL_COLOR_MATERIAL));
    GL_SAFE_CALL(glMaterialf(GL_FRONT, GL_SHININESS, 1.0));

    this->applyTransformations();

    GL_SAFE_CALL(glMultMatrixd(this->gMatrix));
    GL_SAFE_CALL(glGetDoublev(GL_MODELVIEW_MATRIX, this->gMatrix));

    GL_SAFE_CALL(glEnable(GL_BLEND));
    GL_SAFE_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_SAFE_CALL(glShadeModel(GL_SMOOTH));

    GL_SAFE_CALL(glHint(GL_POINT_SMOOTH_HINT,GL_DONT_CARE));
    GL_SAFE_CALL(glHint(GL_LINE_SMOOTH_HINT,GL_DONT_CARE));
    GL_SAFE_CALL(glHint(GL_POLYGON_SMOOTH_HINT,GL_FASTEST));

    if (this->displayProperties.getDrawGlobalAxis())
    {
        // Draw main axis
        GLAxis gAxis(this,GL_AXIS_GLOBAL);
        gAxis.setSize(0.8f/this->scale);
        gAxis.paint();
    }

    if (this->showRotationSphere)
    {
        // Draw rotation sphere
        int lineColorValue = qGray(this->getGLDisplayProperties().getBgColor().rgb()) < 96 ? 255 : 0;
        this->qglColor(QColor(lineColorValue,lineColorValue,lineColorValue,255));

        RR3Vector p,d;
        this->calculatePickRay(QPoint(int(ceil(double(this->width())/2.0)),int(ceil(double(this->height())/2.0))),0.0,p,d,false);

        GLRotationSphere gRotationSphere(this,RR3Vector(p[0],p[1],p[2]),(0.5/double(this->scale)));
        gRotationSphere.paint();
    }

    GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
    GL_SAFE_CALL(glEnable(GL_NORMALIZE));
    GL_SAFE_CALL(glEnable(GL_POINT_SMOOTH));
    GL_SAFE_CALL(glEnable(GL_LINE_SMOOTH));
    GL_SAFE_CALL(glDisable(GL_POLYGON_SMOOTH));

    // Apply model scale
    GL_SAFE_CALL(glScaled(GLdouble(this->mscale),GLdouble(this->mscale),GLdouble(this->mscale)));

    if (this->clippingPlaneEnabled)
    {
        GL_SAFE_CALL(glEnable(GL_CLIP_PLANE0));
    }

    if (Session::getInstance().getModel(this->getModelID()).glDrawTrylock())
    {
        RStopWatch modelDrawStopWatch;
        modelDrawStopWatch.reset();

        // Draw model
        Session::getInstance().getModel(this->getModelID()).glDraw(this);
        // Draw picked elements
        Session::getInstance().getModel(this->getModelID()).glDraw(this,Session::getInstance().getPickList().getItems(this->getModelID()));
        // Unlock drawing lock
        Session::getInstance().getModel(this->getModelID()).glDrawUnlock();

        this->modelDrawTime = modelDrawStopWatch.getMiliSeconds();
    }

    if (this->clippingPlaneEnabled)
    {
        GL_SAFE_CALL(glDisable(GL_CLIP_PLANE0));
    }

    // Draw local directions
    if (this->drawLocalDirections)
    {
        int lineColorValue = qGray(this->getGLDisplayProperties().getBgColor().rgb()) < 96 ? 255 : 0;
        this->qglColor(QColor(lineColorValue,lineColorValue,lineColorValue,255));

        foreach (const RLocalDirection &localDirection, this->localDirections)
        {
            RR3Vector direction(localDirection.getDirection());
            direction *= 1.0/double(this->scale);
            GLArrow localRotationArrow(this,localDirection.getPosition(),direction,true,false,GLDimension::arrowScale/double(this->mscale));
            localRotationArrow.paint();
        }
    }

    // Draw model dimensions
    if (this->displayProperties.getShowModelDimensions())
    {
        double xMin=0.0,xMax=0.0,yMin=0.0,yMax=0.0,zMin=0.0,zMax=0.0;
        Session::getInstance().getModel(this->getModelID()).findNodeLimits(xMin,xMax,yMin,yMax,zMin,zMax);
        int lineColorValue = qGray(this->getGLDisplayProperties().getBgColor().rgb()) < 96 ? 255 : 0;
        this->qglColor(QColor(lineColorValue,lineColorValue,lineColorValue,100));
        GLDimension gDimension(this,1.0/double(this->scale),xMin,xMax,yMin,yMax,zMin,zMax);
        gDimension.paint();
    }

    // Draw draw-engine objects
    const DrawEngine *pDrawEngine = Session::getInstance().getDrawEngine();
    for (uint i=0;i<pDrawEngine->getNObjects();i++)
    {
        pDrawEngine->getObject(i)->glDraw(this);
    }

    // Draw nodes to move
    if (this->drawMoveNodes)
    {
        for (QMap<SessionNodeID,RR3Vector>::const_iterator it = this->nodesToMove.constBegin();
             it != this->nodesToMove.constEnd();
             ++it)
        {
            if (this->getModelID() != it.key().getMid())
            {
                continue;
            }
            this->qglColor(Qt::red);
            GLLine gLine(this,Session::getInstance().getModel(this->getModelID()).getNode(it.key().getNid()).toVector(),it.value(),3.0);
            gLine.paint();
            GLPoint gPoint(this,it.value(),8.0);
            gPoint.paint();
        }
    }

    // Draw stream line position
    if (this->drawStreamLinePosition && this->streamLinePosition != RR3Vector(0.0,0.0,0.0))
    {
        glPushMatrix();

        glTranslated(this->streamLinePosition[0],this->streamLinePosition[1],this->streamLinePosition[2]);

        GLAxis gAxis(this,GL_AXIS_POSITION,tr("Stream line"));
        gAxis.setSize(0.8f);
        gAxis.paint();

        glPopMatrix();
    }

    // Draw geometry scale origin
    if (this->drawScaleOrigin && this->scaleOrigin != RR3Vector(0.0,0.0,0.0))
    {
        glPushMatrix();

        glTranslated(this->scaleOrigin[0],this->scaleOrigin[1],this->scaleOrigin[2]);

        GLAxis gAxis(this,GL_AXIS_POSITION,tr("Scale"));
        gAxis.setSize(0.8f);
        gAxis.paint();

        glPopMatrix();
    }

    // Draw geometry rotation origin
    if (this->drawRotationOrigin && this->rotationOrigin != RR3Vector(0.0,0.0,0.0))
    {
        glPushMatrix();

        glTranslated(this->rotationOrigin[0],this->rotationOrigin[1],this->rotationOrigin[2]);

        GLAxis gAxis(this,GL_AXIS_POSITION,tr("Rotation"));
        gAxis.setSize(0.8f);
        gAxis.paint();

        glPopMatrix();
    }

    // Draw cut plane.
    if (this->drawCutPlane)
    {
        GLCutPlane glCutPlane(this,this->cutPlane);
        glCutPlane.setSize(1.2f/this->mscale);
        glCutPlane.paint();
    }

    // Restore original scale.
    double invMscale = 1.0/double(this->mscale);
    GL_SAFE_CALL(glScaled(invMscale,invMscale,invMscale));

    if (this->displayProperties.getDrawLocalAxis())
    {
        // LOWER-LEFT CORNER VIEWPORT
        GL_SAFE_CALL(glViewport (0,0,GLWidget::lAxisWpWidth*this->desktopDevicePixelRatio,GLWidget::lAxisWpHeight*this->desktopDevicePixelRatio));

        GL_SAFE_CALL(glMatrixMode(GL_PROJECTION));
        GL_SAFE_CALL(glLoadIdentity());

        GL_SAFE_CALL(glOrtho(-1.0, 1.0, -1.0, 1.0, -winScale, winScale));

        GL_SAFE_CALL(glMatrixMode(GL_MODELVIEW));

        GL_SAFE_CALL(glClear(GL_DEPTH_BUFFER_BIT));
        GL_SAFE_CALL(glLoadIdentity());

        if (this->drx != 0.0f) {
            GL_SAFE_CALL(glRotatef(this->drx, 1.0f, 0.0f, 0.0f));
        }
        if (this->dry != 0.0f) {
            GL_SAFE_CALL(glRotatef(this->dry, 0.0f, 1.0f, 0.0f));
        }

        GL_SAFE_CALL(glMultMatrixd(this->lMatrix));
        GL_SAFE_CALL(glGetDoublev(GL_MODELVIEW_MATRIX, this->lMatrix));

        // Draw local axis.
        GLAxis lAxis(this,GL_AXIS_LOCAL);
        lAxis.setSize(0.7f);
        lAxis.paint();
    }

    this->dtx = this->dty = this->dtz = 0.0;
    this->drx = this->dry = 0.0;
    this->dscale = 0.0;
} // drawModel()

void GLWidget::drawValueRanges(QPainter &painter)
{
    const Model &rModel = Session::getInstance().getModel(this->getModelID());
    uint nDisplayVariableRanges = 0;

    for (uint i=0;i<rModel.getNVariables();i++)
    {
        const RVariable &rVariable = rModel.getVariable(i);

        if (rVariable.getVariableData().getDisplayValueRange())
        {
            std::vector<RR3Vector> pickedPositions;
            std::vector<double> pickedValues;

            const QVector<PickItem> &rPickItems = Session::getInstance().getPickList().getItems(this->getModelID());

            for (int j=0;j<rPickItems.size();j++)
            {
                QList<PickValue> vectorList = rModel.getPickedResultsValues(rPickItems[j]).values(rVariable.getType());

                for (int k=0;k<vectorList.size();k++)
                {
                    pickedPositions.push_back(vectorList.at(k).getPosition());
                    pickedValues.push_back(RRVector::norm(vectorList.at(k).getValues()));
                }
            }

            this->drawValueRange(painter,
                                 ++nDisplayVariableRanges,
                                 rVariable.getVariableData().getMinDisplayValue(),
                                 rVariable.getVariableData().getMaxDisplayValue(),
                                 pickedPositions,
                                 pickedValues,
                                 rVariable.getVariableData().getValueRangeName(),
                                 rVariable.getName(),
                                 rVariable.getUnits());
        }
    }
}

void GLWidget::drawValueRange(QPainter &painter,
                              unsigned int rangeCount,
                              double lowerValue,
                              double upperValue,
                              std::vector<RR3Vector> pickedPositions,
                              std::vector<double> pickedValues,
                              const QString &valueRangeFile,
                              const QString &valueRangeName,
                              const QString &valueRangeUnits)
{
    int lineColorValue = qGray(this->getGLDisplayProperties().getBgColor().rgb()) < 96 ? 255 : 0;

    int offset = 10;
    int padding = 2;

    int rangeWidth = 150;
    int rangeHeight = this->height() - 2*offset;
    int rangeX = this->width() - (rangeWidth + offset) * int(rangeCount);
    int rangeY = offset;

    int fontHeight = painter.fontMetrics().height();
    int fontAscent = painter.fontMetrics().ascent();
    int fontDescent = painter.fontMetrics().descent();
    int fontPixelHeight = painter.fontInfo().pixelSize();
    int fontPixelAscent = qRound(fontAscent*double(fontPixelHeight)/double(fontHeight));
    int fontPixelDescent = qRound(fontDescent*double(fontPixelHeight)/double(fontHeight));

    int headerWidth = rangeWidth;
    int headerHeight = 2*(fontPixelHeight + padding) + padding;
    int headerX = rangeX;
    int headerY = rangeY;

    int footerWidth = rangeWidth;
    int footerHeight = 2*(fontPixelHeight + padding) + padding;
    int footerX = rangeX;
    int footerY = rangeY + rangeHeight - footerHeight;

    int imageWidth = 20;
    int imageHeight = rangeHeight - headerHeight - footerHeight - 4*padding;
    int imageX = rangeX + rangeWidth - padding - imageWidth;
    int imageY = rangeY + headerHeight + 2*padding;

    painter.setRenderHint(QPainter::Antialiasing);

    // Outer box
    painter.setPen(QColor(lineColorValue,lineColorValue,lineColorValue,255));
    painter.drawRect(rangeX,rangeY,rangeWidth,rangeHeight);
    painter.fillRect(rangeX,rangeY,rangeWidth,rangeHeight,QColor(255,255,255,200));

    // Image
    QTransform t;
    t.rotate(-90.0);
    painter.drawImage(QRect(imageX,imageY,imageWidth,imageHeight),QImage(valueRangeFile).transformed(t));

    // Image box
    painter.setPen(QColor(lineColorValue,lineColorValue,lineColorValue,255));
    painter.drawRect(imageX,imageY,imageWidth,imageHeight);

    // Header box
    QLinearGradient headerGradient(headerX,headerY,headerX,headerY+headerHeight);
    headerGradient.setColorAt(0, Qt::white);
    headerGradient.setColorAt(1, Qt::lightGray);
    painter.fillRect(headerX,headerY,headerWidth,headerHeight,headerGradient);

    painter.setPen(QColor(0,0,0,255));
    painter.drawText(headerX+padding,headerY+fontPixelAscent+padding,QString(valueRangeName));
    painter.drawText(headerX+padding,headerY+fontPixelAscent+padding+fontPixelHeight+padding,"Units: [" + QString(valueRangeUnits) + "]");

    painter.setPen(QColor(lineColorValue,lineColorValue,lineColorValue,255));
    painter.drawRect(headerX,headerY,headerWidth,headerHeight);

    // Footer box
    QLinearGradient footerGradient(footerX,footerY,footerX,footerY+footerHeight);
    footerGradient.setColorAt(0, Qt::lightGray);
    footerGradient.setColorAt(1, Qt::white);
    painter.fillRect(footerX,footerY,footerWidth,footerHeight, footerGradient);

    const Model &rModel = Session::getInstance().getModel(this->getModelID());

    QString recordNumberStr;
    QString recordValueStr;
    if (rModel.getTimeSolver().getEnabled())
    {
        recordNumberStr = tr("Time-step:") + " " + QString::number(rModel.getTimeSolver().getCurrentTimeStep()+1);
        recordValueStr = tr("Time:") + " " + QString::number(rModel.getTimeSolver().getComputedTime(),'e',3) + " [" + RVariable::getUnits(R_VARIABLE_TIME) + "]";
    }
    else
    {
        if (rModel.getProblemTaskTree().getProblemTypeMask() & R_PROBLEM_STRESS_MODAL)
        {
            recordNumberStr = tr("Mode:") + " " + QString::number(rModel.getProblemSetup().getModalSetup().getMode()+1);
            recordValueStr = tr("Freq.:") + " " + QString::number(rModel.getProblemSetup().getModalSetup().getFrequency(),'e',3) + " [" + RVariable::getUnits(R_VARIABLE_FREQUENCY) + "]";
        }
        else
        {
            recordNumberStr = tr("Steady state");
            recordValueStr = "----------------------";
        }
    }

    painter.setPen(QColor(0,0,0,255));
    painter.drawText(footerX+padding,footerY+fontPixelAscent+padding,recordNumberStr);
    painter.drawText(footerX+padding,footerY+fontPixelAscent+padding+fontPixelHeight+padding,recordValueStr);

    painter.setPen(QColor(lineColorValue,lineColorValue,lineColorValue,255));
    painter.drawRect(footerX,footerY,footerWidth,footerHeight);

    // Min max values
    painter.setPen(QColor(0,0,0,255));
    painter.drawText(rangeX+padding,imageY+fontPixelAscent,QString::number(upperValue));
    painter.drawText(rangeX+padding,imageY+imageHeight-fontPixelDescent,QString::number(lowerValue));

    // Picked values
    for (uint i=0;i<pickedValues.size();i++)
    {
        int pickedWidth = imageWidth;
        int pickedX = imageX - pickedWidth;
        int pickedY = imageY + imageHeight - qRound(double(imageHeight) * (pickedValues[i] - lowerValue)/ (upperValue - lowerValue));

        painter.setPen(QColor(lineColorValue,lineColorValue,lineColorValue,255));
        painter.drawLine(pickedX,pickedY,pickedX+pickedWidth,pickedY);
        QPoint pickPosition;
        this->convertModelToScreen(pickedPositions[i],pickPosition);
        painter.drawLine(pickedX,pickedY,pickPosition.x(),pickPosition.y());

        painter.setPen(QColor(200,0,0,255));
        painter.drawText(rangeX+padding,pickedY-fontPixelDescent+qRound(double(fontPixelHeight)*(pickedValues[i] - lowerValue)/ (upperValue - lowerValue)),QString::number(pickedValues[i]));
    }
}

void GLWidget::drawMessageBox(QPainter &painter, bool drawBox)
{
    painter.setFont(QGuiApplication::font());

    const Model &rModel = Session::getInstance().getModel(this->getModelID());

    QList<QString> messages;

    messages.push_back(rModel.getName() + " (" + rModel.getDescription() + ")");
    messages.push_back(QString());

    if (rModel.getNSlivers() > 0)
    {
        messages.push_back(QString("W: Number of sliver elements: ") + QString::number(rModel.getNSlivers()));
    }
    if (rModel.getNIntersected() > 0)
    {
        messages.push_back(QString("E: Number of intersected elements: ") + QString::number(rModel.getNIntersected()));
    }
    if (rModel.getNHoleElements() > 0)
    {
        messages.push_back(QString("E: Number of hole elements: ") + QString::number(rModel.getNHoleElements()));
    }

    const PickList &rPickList = Session::getInstance().getPickList();
    if (!rPickList.isEmpty())
    {
        messages.append("Picked entities:");

        const Model &rModel = Session::getInstance().getModel(this->modelID);
        QVector<PickItem> pickItems = rPickList.getItems(this->modelID);
        std::vector<REntityGroupType> entityTypes = REntityGroup::getAllTypes();
        for (uint i=0;i<entityTypes.size();i++)
        {
            QMap<QString,uint> pickedEntities;
            for (int j=0;j<pickItems.size();j++)
            {
                if (pickItems.at(j).getEntityID().getType() == entityTypes[i])
                {
                    if (pickItems.at(j).getItemType() == PICK_ITEM_HOLE_ELEMENT)
                    {
                        const QString eName = tr("Hole edge");
                        pickedEntities.insert(eName,pickedEntities.value(eName,0)+1);
                    }
                    else
                    {
                        if (pickItems.at(j).getEntityID().getType() != R_ENTITY_GROUP_NONE)
                        {
                            uint gid = rModel.getEntityGroupID(pickItems.at(j).getEntityID().getType(),pickItems.at(j).getEntityID().getEid());
                            const QString eName = rModel.getEntityGroupPtr(gid)->getName();
                            pickedEntities.insert(eName,pickedEntities.value(eName,0)+1);
                        }
                    }
                }
            }

            if (!pickedEntities.isEmpty())
            {
                QList<QString> pickedEntityNames = pickedEntities.uniqueKeys();

                foreach (const QString &pickedEntityName, pickedEntityNames)
                {
                    messages.append("    " + REntityGroup::getTypeName(entityTypes[i]) + ":  " + pickedEntityName + "    (" + QString::number(pickedEntities.value(pickedEntityName)) + ")");
                }
            }
        }
    }

    if (messages.size() == 0)
    {
        return;
    }

    int offset = 10;
    int padding = 2;

    int fontPixelHeight = painter.fontInfo().pixelSize();

    int sizeX = this->width() - offset - offset;
    int sizeY = offset + (fontPixelHeight+padding)*messages.size();
    int posX = offset;
    int posY = offset;

    int colorValue = qGray(this->getGLDisplayProperties().getBgColor().rgb()) < 96 ? 255 : 0;

    // Outer box
    if (drawBox)
    {
        colorValue = 255 - colorValue;
        int boxColorValue = 255 - colorValue;
        painter.setPen(QColor(boxColorValue,boxColorValue,boxColorValue,255));
        painter.drawRect(posX,posY,sizeX,sizeY);
        painter.fillRect(posX,posY,sizeX,sizeY,QColor(boxColorValue,boxColorValue,boxColorValue,200));
    }

    painter.setPen(QColor(colorValue,colorValue,colorValue,255));
    for (int i=0;i<messages.size();i++)
    {
        painter.drawText(posX+padding,posY+(fontPixelHeight+padding)*(i+1),messages[i]);
    }
}

void GLWidget::drawInfoBox(QPainter &painter, bool drawBox)
{
    painter.setFont(QGuiApplication::font());

    QList<QString> messages;

    QStringList actionMessage;
    QString keyMouseMessage(this->actionEvent.getKeyMouseCombination());
    QString eventMessage(GLActionEvent::toString(this->actionEvent.getType()));

    if (keyMouseMessage.length() != 0)
    {
        actionMessage.append(keyMouseMessage);
    }
    if (eventMessage.length() != 0)
    {
        actionMessage.append(eventMessage);
    }
    if (actionMessage.size() > 0)
    {
        messages.append(tr("Action") + ": " + actionMessage.join(" - "));
    }

    messages.append(tr("Zoom") + ": " + QString::number(double(this->scale)));
    messages.append(tr("Model draw time") + ": " + QString::number(this->modelDrawTime) + " [ms]");

    if (messages.size() == 0)
    {
        return;
    }

    int offset = 10;
    int padding = 2;

    int fontPixelHeight = painter.fontInfo().pixelSize();

    int sizeX = GLWidget::lAxisWpWidth;
    int sizeY = offset + (fontPixelHeight+padding)*messages.size();
    int posX = offset;
    int posY = this->height() - offset - GLWidget::lAxisWpHeight - sizeY;

    int colorValue = qGray(this->getGLDisplayProperties().getBgColor().rgb()) < 96 ? 255 : 0;

    // Outer box
    if (drawBox)
    {
        colorValue = 255 - colorValue;
        int boxColorValue = 255 - colorValue;
        painter.setPen(QColor(boxColorValue,boxColorValue,boxColorValue,255));
        painter.drawRect(posX,posY,sizeX,sizeY);
        painter.fillRect(posX,posY,sizeX,sizeY,QColor(boxColorValue,boxColorValue,boxColorValue,200));
    }
    painter.setPen(QColor(colorValue,colorValue,colorValue,255));
    for (int i=0;i<messages.size();i++)
    {
        painter.drawText(posX+padding,posY+(fontPixelHeight+padding)*(i+1),messages[i]);
    }
}

void GLWidget::applyTransformations(void)
{
    if (this->dtx != 0.0f || this->dty != 0.0f || this->dtz != 0.0f)
    {
        glTranslatef(this->dtx, this->dty, this->dtz);
    }

    if (this->drx != 0.0f)
    {
        glRotatef(this->drx, 1.0f, 0.0f, 0.0f);
    }
    if (this->dry != 0.0f)
    {
        glRotatef(this->dry, 0.0f, 1.0f, 0.0f);
    }

    if (this->dscale != 0.0f && this->dscale != 1.0f)
    {
        this->scale *= 1.0f+this->dscale;
        if (this->scale < 1e4f)
        {
            glScalef(1.0f+this->dscale,1.0f+this->dscale,1.0f+this->dscale);
        }
        else
        {
            this->scale /= 1.0f+this->dscale;
        }
    }
}

void GLWidget::processActionEvent(void)
{
    RLogger::trace("GLWidget::processActionEvent(void)\n");

    GLActionEventType glActionEventType = this->actionEvent.getType();

    if (glActionEventType == GL_ACTION_EVENT_PICK_CLEAR)
    {
        Session::getInstance().getPickList().clear();
        if (Session::getInstance().getModel(this->modelID).getProblemTaskTree().getProblemTypeMask() & R_PROBLEM_RADIATIVE_HEAT)
        {
            // If problem type is radiation surface normal list will be always regenerated if mouse pick happens.
            for (uint i=0;i<Session::getInstance().getModel(this->modelID).getNSurfaces();i++)
            {
                this->getGLModelList().getGlSurfaceList(i).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
            }
        }
        return;
    }

    if (glActionEventType != GL_ACTION_EVENT_PICK_ELEMENT &&
        glActionEventType != GL_ACTION_EVENT_PICK_NODE &&
        glActionEventType != GL_ACTION_EVENT_PICK_HOLE_ELEMENT)
    {
        return;
    }

    RR3Vector pickRayPosition;
    RR3Vector pickRayDirection;

    this->calculatePickRay(this->bpStart,this->calculateViewDepth(),pickRayPosition,pickRayDirection,true);

    PickItem pickItem;
    bool pickFound = false;
    double pickTolerance = 0.01 / double((this->mscale * this->scale));

    if (glActionEventType == GL_ACTION_EVENT_PICK_ELEMENT)
    {
        pickFound = Session::getInstance().getModel(this->modelID).findPickedElement(pickRayPosition,pickRayDirection,pickTolerance,pickItem);
    }
    else if (glActionEventType == GL_ACTION_EVENT_PICK_NODE)
    {
        pickFound = Session::getInstance().getModel(this->modelID).findPickedNode(pickRayPosition,pickRayDirection,pickTolerance,pickItem);
    }
    else if (glActionEventType == GL_ACTION_EVENT_PICK_HOLE_ELEMENT)
    {
        pickFound = Session::getInstance().getModel(this->modelID).findPickedHoleElement(pickRayPosition,pickRayDirection,pickTolerance,pickItem);
    }

    if (pickFound)
    {
        static bool itemAdded = false;
        bool actionTypeChanged = this->actionEvent.getChanged();

        if (actionTypeChanged)
        {
            itemAdded = Session::getInstance().getPickList().registerItem(pickItem);
        }
        else
        {
            if (itemAdded)
            {
                Session::getInstance().getPickList().addItem(pickItem);
            }
            else
            {
                Session::getInstance().getPickList().removeItem(pickItem);
            }
        }

        if (Session::getInstance().getModel(this->modelID).getProblemTaskTree().getProblemTypeMask() & R_PROBLEM_RADIATIVE_HEAT)
        {
            // If problem type is radiation surface normal list will be always regenerated if mouse pick happens.
            for (uint i=0;i<Session::getInstance().getModel(this->modelID).getNSurfaces();i++)
            {
                this->getGLModelList().getGlSurfaceList(i).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
            }
        }
    }
}

void GLWidget::mousePressEvent(QMouseEvent *mouseEvent)
{
    this->actionEvent.setMouseEvent(mouseEvent,false);
    this->bpStart = mouseEvent->pos();

    this->processActionEvent();

    this->update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    this->actionEvent.setMouseEvent(mouseEvent,true);
    this->bpEnd = mouseEvent->pos();
    this->showRotationSphere = false;

    if (this->actionEvent.getType() == GL_ACTION_EVENT_TRANSLATE)
    {
        this->dtx =   2 * (this->bpEnd.x() - this->bpStart.x()) / float(this->width());
        this->dty = - 2 * (this->bpEnd.y() - this->bpStart.y()) / float(this->width());
        this->dtz = 0.0;
    }

    this->bpStart = this->bpEnd;

    this->processActionEvent();
    this->update();
}

void GLWidget::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    this->actionEvent.setMouseEvent(mouseEvent,false);
    this->bpEnd = mouseEvent->pos();

    if (this->actionEvent.getType() == GL_ACTION_EVENT_TRANSLATE)
    {
        this->showRotationSphere = true;
        this->dtx =   2 * (this->bpEnd.x() - this->bpStart.x()) / float(this->width());
        this->dty = - 2 * (this->bpEnd.y() - this->bpStart.y()) / float(this->width());
        this->dtz = 0.0;
    }
    else if (this->actionEvent.getType() == GL_ACTION_EVENT_TRANSLATE_Z)
    {
        this->showRotationSphere = true;
        this->dtx = 0.0;
        this->dty = 0.0;
        this->dtz = this->scale * 2 * (this->bpEnd.y() - this->bpStart.y()) / float(this->width());
    }
    else if (this->actionEvent.getType() == GL_ACTION_EVENT_ROTATE)
    {
        this->showRotationSphere = true;
        this->drx = 500 * (this->bpEnd.y() - this->bpStart.y()) / float(this->width());
        this->dry = 500 * (this->bpEnd.x() - this->bpStart.x()) / float(this->width());
    }
    else if (this->actionEvent.getType() == GL_ACTION_EVENT_ZOOM)
    {
        this->showRotationSphere = true;
        this->dscale = (this->bpEnd.y() - this->bpStart.y()) / float(this->width());
    }
    this->bpStart = this->bpEnd;

    this->processActionEvent();
    this->update();
}

void GLWidget::enterEvent(QEvent *)
{
    this->setFocus();
}

void GLWidget::leaveEvent(QEvent *)
{
}

void GLWidget::wheelEvent(QWheelEvent *mouseEvent)
{
    this->actionEvent.setScrollPhase(Qt::ScrollUpdate);

    int numDegrees = mouseEvent->delta() / 8;
    int numSteps = numDegrees / 15;
    float x = float(mouseEvent->x());
    float y = float(mouseEvent->y());
    float w = float(this->width());
    float h = float(this->height());

    if (this->actionEvent.getType() == GL_ACTION_EVENT_TRANSLATE_Z)
    {
        this->showRotationSphere = true;
        this->dtx = 0.0;
        this->dty = 0.0;
        this->dtz = this->scale*float(numSteps)/100.0f;
    }
    else if (this->actionEvent.getType() == GL_ACTION_EVENT_ZOOM)
    {
        this->dtx = 2.0f*x/w - 1.0f;
        this->dty = 2.0f*y/h - 1.0f;
        this->dty = (0.5f - y/h)*2.0f*h/w;
        this->dtx *= numSteps/10.0f;
        this->dty *= numSteps/10.0f;
        this->dscale = -float(numSteps)/10.0f;
    }

    this->actionEvent.setScrollPhase(Qt::NoScrollPhase);

    this->processActionEvent();
    this->update();
}

void GLWidget::keyPressEvent(QKeyEvent *keyEvent)
{
    this->actionEvent.setKeyEvent(keyEvent,false);
    this->update();
}

void GLWidget::keyReleaseEvent(QKeyEvent *keyEvent)
{
    this->actionEvent.setKeyEvent(keyEvent,true);
    this->showRotationSphere = false;
    this->update();
}

void GLWidget::calculateModelScale(void)
{
    this->mscale = float(Session::getInstance().getModel(this->getModelID()).findNodeScale());
    if (this->mscale < float(RConstants::eps))
    {
        this->mscale = float(RConstants::eps);
    }
}

void GLWidget::calculatePickRay(const QPoint &screenPosition, double viewDepth, RR3Vector &position, RR3Vector &direction, bool applyModelScale) const
{
    double wx = 1.0;
    double wy = double(this->height()) / double(this->width());
    double x = 2.0*wx*(double(screenPosition.x()) / double(this->width())) - wx;
    double y = 2.0*wy*(1.0 - double(screenPosition.y()) / double(this->height())) - wy;
    double vd = viewDepth;

    RRMatrix R(4,4);
    for (uint i=0;i<4;i++)
    {
        for (uint j=0;j<4;j++)
        {
            R[i][j] = this->gMatrix[4*j+i] * (applyModelScale ? double(this->mscale) : 1.0);
        }
    }
    R.invert();

    RRVector v1(4),v2(4);

    v1[0] = x;
    v1[1] = y;
    v1[2] = vd;
    v1[3] = 1.0;

    RRMatrix::mlt(R,v1,v2);

    position[0] = v2[0];
    position[1] = v2[1];
    position[2] = v2[2];

    v1[0] = x;
    v1[1] = y;
    v1[2] = 0.0;
    v1[3] = 1.0;

    RRMatrix::mlt(R,v1,v2);

    direction[0] = v2[0]-position[0];
    direction[1] = v2[1]-position[1];
    direction[2] = v2[2]-position[2];
    direction.normalize();
}

void GLWidget::convertModelToScreen(const RR3Vector &realPosition, QPoint &screenPosition) const
{
    RRMatrix R(4,4);
    for (uint i=0;i<4;i++)
    {
        for (uint j=0;j<4;j++)
        {
            R[i][j] = this->gMatrix[4*j+i] * double(this->mscale);
        }
    }

    RRVector v1(4),v2(4);

    v1[0] = realPosition[0];
    v1[1] = realPosition[1];
    v1[2] = realPosition[2];
    v1[3] = 1.0 / double(this->mscale);

    RRMatrix::mlt(R,v1,v2);

    double wx = 1.0;
    double wy = double(this->height()) / double(this->width());

    double x = double(this->width()) * (v2[0] + wx)/(2.0*wx);
    double y = double(this->height()) * (1.0 - (v2[1] + wy)/(2.0*wy));

    screenPosition.setX(qRound(x));
    screenPosition.setY(qRound(y));
}

void GLWidget::convertScreenToModel(const QPoint &screenPosition, RR3Vector &modelPosition)
{
    double wx = 1.0;
    double wy = double(this->height()) / double(this->width());
    double x = 2.0*wx*(double(screenPosition.x()) / double(this->width())) - wx;
    double y = 2.0*wy*(1.0 - double(screenPosition.y()) / double(this->height())) - wy;

    RRMatrix R(4,4);
    for (uint i=0;i<4;i++)
    {
        for (uint j=0;j<4;j++)
        {
            R[i][j] = this->gMatrix[4*j+i] * double(this->mscale);
        }
    }
    R.invert();

    RRVector v1(4),v2(4);

    v1[0] = x;
    v1[1] = y;
    v1[2] = 0.0;
    v1[3] = 1.0;

    RRMatrix::mlt(R,v1,v2);

    modelPosition[0] = v2[0];
    modelPosition[1] = v2[1];
    modelPosition[2] = v2[2];
}

double GLWidget::calculateViewDepth(void) const
{
    return 1000.0 * double(this->scale);
}

void GLWidget::showLight(const RGLLight &rGlLight)
{
    GLenum lightNumber = GL_LIGHT0;

    switch (rGlLight.getLightNumber())
    {
        case 0:
        {
            lightNumber = GL_LIGHT0;
            break;
        }
        case 1:
        {
            lightNumber = GL_LIGHT1;
            break;
        }
        case 2:
        {
            lightNumber = GL_LIGHT2;
            break;
        }
        case 3:
        {
            lightNumber = GL_LIGHT3;
            break;
        }
        case 4:
        {
            lightNumber = GL_LIGHT4;
            break;
        }
        case 5:
        {
            lightNumber = GL_LIGHT5;
            break;
        }
        case 6:
        {
            lightNumber = GL_LIGHT6;
            break;
        }
        case 7:
        {
            lightNumber = GL_LIGHT7;
            break;
        }
        default:
        {
            lightNumber = GL_LIGHT0;
            break;
        }
    }

    if (!rGlLight.getEnabled())
    {
        GL_SAFE_CALL(glDisable(lightNumber));
        return;
    }

    GLfloat lightKa[4];
    GLfloat lightKs[4];
    GLfloat lightKd[4];
    GLfloat lightDr[3];
    GLfloat lightPs[4];

    lightKa[0] = GLfloat(rGlLight.getAmbient().redF());
    lightKa[1] = GLfloat(rGlLight.getAmbient().greenF());
    lightKa[2] = GLfloat(rGlLight.getAmbient().blueF());
    lightKa[3] = GLfloat(rGlLight.getAmbient().alphaF());

    lightKs[0] = GLfloat(rGlLight.getSpecular().redF());
    lightKs[1] = GLfloat(rGlLight.getSpecular().greenF());
    lightKs[2] = GLfloat(rGlLight.getSpecular().blueF());
    lightKs[3] = GLfloat(rGlLight.getSpecular().alphaF());

    lightKd[0] = GLfloat(rGlLight.getDiffuse().redF());
    lightKd[1] = GLfloat(rGlLight.getDiffuse().greenF());
    lightKd[2] = GLfloat(rGlLight.getDiffuse().blueF());
    lightKd[3] = GLfloat(rGlLight.getDiffuse().alphaF());

    lightDr[0] = GLfloat(rGlLight.getDirection()[0]);
    lightDr[1] = GLfloat(rGlLight.getDirection()[1]);
    lightDr[2] = GLfloat(rGlLight.getDirection()[2]);

    lightPs[0] = GLfloat(rGlLight.getPosition()[0]);
    lightPs[1] = GLfloat(rGlLight.getPosition()[1]);
    lightPs[2] = GLfloat(rGlLight.getPosition()[2]);
    lightPs[3] = 0.0;

    GL_SAFE_CALL(glEnable(lightNumber));

    GL_SAFE_CALL(glLightfv(lightNumber,GL_AMBIENT,lightKa));
    GL_SAFE_CALL(glLightfv(lightNumber,GL_SPECULAR,lightKs));
    GL_SAFE_CALL(glLightfv(lightNumber,GL_DIFFUSE,lightKd));
    GL_SAFE_CALL(glLightfv(lightNumber,GL_SPOT_DIRECTION,lightDr));
    GL_SAFE_CALL(glLightfv(lightNumber,GL_POSITION,lightPs));
}

GLint GLWidget::project(GLdouble objx, GLdouble objy, GLdouble objz, const GLdouble model[], const GLdouble proj[], const GLint viewport[], GLdouble *winx, GLdouble *winy, GLdouble *winz)
{
    GLdouble in[4], out[4];

    in[0] = objx;
    in[1] = objy;
    in[2] = objz;
    in[3] = 1.0;

    this->transformPoint(out, model, in);
    this->transformPoint(in, proj, out);

    if (in[3] == 0.0)
    {
        return GL_FALSE;
    }

    in[0] /= in[3];
    in[1] /= in[3];
    in[2] /= in[3];

    *winx = viewport[0] / this->desktopDevicePixelRatio + (1 + in[0]) * (viewport[2] / this->desktopDevicePixelRatio) / 2;
    *winy = viewport[1] / this->desktopDevicePixelRatio + (1 + in[1]) * (viewport[3] / this->desktopDevicePixelRatio) / 2;

    *winz = (1 + in[2]) / 2;
    return GL_TRUE;
}

void GLWidget::transformPoint(GLdouble out[], const GLdouble m[], const GLdouble in[])
{
    #define M(row,col)  m[col*4+row]
    out[0] = M(0, 0) * in[0] + M(0, 1) * in[1] + M(0, 2) * in[2] + M(0, 3) * in[3];
    out[1] = M(1, 0) * in[0] + M(1, 1) * in[1] + M(1, 2) * in[2] + M(1, 3) * in[3];
    out[2] = M(2, 0) * in[0] + M(2, 1) * in[1] + M(2, 2) * in[2] + M(2, 3) * in[3];
    out[3] = M(3, 0) * in[0] + M(3, 1) * in[1] + M(3, 2) * in[2] + M(3, 3) * in[3];
    #undef M
}

void GLWidget::resetView(float xRotation, float yRotation, float zRotation)
{
    this->scale = 1.0;

    this->calculateModelScale();

    double xPosition(0.0), yPosition(0.0), zPosition(0.0);

    Session::getInstance().getModel(this->getModelID()).findNodeCenter(xPosition,yPosition,zPosition);

    xPosition *= double(this->mscale);
    yPosition *= double(this->mscale);
    zPosition *= double(this->mscale);

    glLoadIdentity ();

    glRotatef (xRotation, 1.0f, 0.0f, 0.0f);
    glRotatef (yRotation, 0.0f, 1.0f, 0.0f);
    glRotatef (zRotation, 0.0f, 0.0f, 1.0f);

    glGetDoublev (GL_MODELVIEW_MATRIX, this->lMatrix);

    glTranslated (-xPosition,-yPosition,-zPosition);

    glGetDoublev (GL_MODELVIEW_MATRIX, this->gMatrix);

    this->update();
}

void GLWidget::setClippingPlane(bool enabled, double distance)
{
    this->clippingPlaneEnabled = enabled;
    this->clippingPlaneDistance = distance;
    this->update();
}

void GLWidget::onEntityVisibilityChanged(uint, REntityGroupType, uint, bool )
{
    this->update();
}

void GLWidget::onDisplayPropertiesChanged(uint modelID, REntityGroupType elementGrpType, uint entityID)
{
    RLogger::trace("GLWidget::onDisplayPropertiesChanged(uint modelID, REntityGroupType elementGrpType, uint entityID)\n");

    if (this->modelID != modelID)
    {
        return;
    }

    switch (elementGrpType)
    {
        case R_ENTITY_GROUP_POINT:
            if (this->glModelList.getNGlPointLists() > entityID)
            {
                this->glModelList.getGlPointList(entityID).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                this->glModelList.getGlPointList(entityID).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_ELEMENT);
                this->glModelList.getGlPointList(entityID).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_NODE);
            }
            break;
        case R_ENTITY_GROUP_LINE:
            if (this->glModelList.getNGlLineLists() > entityID)
            {
                this->glModelList.getGlLineList(entityID).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                this->glModelList.getGlLineList(entityID).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_ELEMENT);
                this->glModelList.getGlLineList(entityID).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_NODE);
            }
            break;
        case R_ENTITY_GROUP_SURFACE:
            if (this->glModelList.getNGlSurfaceLists() > entityID)
            {
                this->glModelList.getGlSurfaceList(entityID).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                this->glModelList.getGlSurfaceList(entityID).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_ELEMENT);
                this->glModelList.getGlSurfaceList(entityID).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_NODE);
            }
            break;
        case R_ENTITY_GROUP_VOLUME:
            if (this->glModelList.getNGlVolumeLists() > entityID)
            {
                this->glModelList.getGlVolumeList(entityID).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                this->glModelList.getGlVolumeList(entityID).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_ELEMENT);
                this->glModelList.getGlVolumeList(entityID).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_NODE);
            }
            break;
        case R_ENTITY_GROUP_VECTOR_FIELD:
            if (this->glModelList.getNGlVectorFieldLists() > entityID)
            {
                this->glModelList.getGlVectorFieldList(entityID).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
            }
            break;
        case R_ENTITY_GROUP_SCALAR_FIELD:
            if (this->glModelList.getNGlScalarFieldLists() > entityID)
            {
                this->glModelList.getGlScalarFieldList(entityID).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
            }
            break;
        case R_ENTITY_GROUP_STREAM_LINE:
            if (this->glModelList.getNGlStreamLineLists() > entityID)
            {
                this->glModelList.getGlStreamLineList(entityID).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
            }
            break;
        case R_ENTITY_GROUP_CUT:
            if (this->glModelList.getNGlCutLists() > entityID)
            {
                this->glModelList.getGlCutList(entityID).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
            }
            break;
        case R_ENTITY_GROUP_ISO:
            if (this->glModelList.getNGlIsoLists() > entityID)
            {
                this->glModelList.getGlIsoList(entityID).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
            }
            break;
        default:
            break;
    }

    this->update();
}

void GLWidget::onVariableDataChanged(uint modelID, RVariableType variableType)
{
    RLogger::trace("GLWidget::onVariableDataChanged(uint modelID, RVariableType variableType)\n");

    if (this->modelID != modelID)
    {
        return;
    }

    Model &rModel = Session::getInstance().getModel(this->modelID);

    try
    {

        if (this->glModelList.getNGlPointLists() > 0)
        {
            for (uint j=0;j<rModel.getNPoints();j++)
            {
                const std::map<RVariableType,REntityGroupVariableDisplayTypeMask> &egVarData = rModel.getPoint(j).getData().getVariableData();
                if (egVarData.find(variableType) != egVarData.end())
                {
                    this->glModelList.getGlPointList(j).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                    this->glModelList.getGlPointList(j).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_ELEMENT);
                    this->glModelList.getGlPointList(j).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_NODE);
                }
            }
        }

        if (this->glModelList.getNGlLineLists() > 0)
        {
            for (uint j=0;j<rModel.getNLines();j++)
            {
                const std::map<RVariableType,REntityGroupVariableDisplayTypeMask> &egVarData = rModel.getLine(j).getData().getVariableData();
                if (egVarData.find(variableType) != egVarData.end())
                {
                    this->glModelList.getGlLineList(j).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                    this->glModelList.getGlLineList(j).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_ELEMENT);
                    this->glModelList.getGlLineList(j).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_NODE);
                }
            }
        }

        if (this->glModelList.getNGlSurfaceLists() > 0)
        {
            for (uint j=0;j<rModel.getNSurfaces();j++)
            {
                const std::map<RVariableType,REntityGroupVariableDisplayTypeMask> &egVarData = rModel.getSurface(j).getData().getVariableData();
                if (egVarData.find(variableType) != egVarData.end())
                {
                    this->glModelList.getGlSurfaceList(j).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                    this->glModelList.getGlSurfaceList(j).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_ELEMENT);
                    this->glModelList.getGlSurfaceList(j).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_NODE);
                }
            }
        }

        if (this->glModelList.getNGlVolumeLists() > 0)
        {
            for (uint j=0;j<rModel.getNVolumes();j++)
            {
                const std::map<RVariableType,REntityGroupVariableDisplayTypeMask> &egVarData = rModel.getVolume(j).getData().getVariableData();
                if (egVarData.find(variableType) != egVarData.end())
                {
                    this->glModelList.getGlVolumeList(j).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                    this->glModelList.getGlVolumeList(j).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_ELEMENT);
                    this->glModelList.getGlVolumeList(j).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_NODE);
                }
            }
        }

        if (this->glModelList.getNGlVectorFieldLists() > 0)
        {
            for (uint j=0;j<rModel.getNVectorFields();j++)
            {
                const std::map<RVariableType,REntityGroupVariableDisplayTypeMask> &egVarData = rModel.getVectorField(j).getData().getVariableData();
                if (egVarData.find(variableType) != egVarData.end() || rModel.getVectorField(j).getVariableType() == variableType)
                {
                    this->glModelList.getGlVectorFieldList(j).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                }
            }
        }

        if (this->glModelList.getNGlScalarFieldLists() > 0)
        {
            for (uint j=0;j<rModel.getNScalarFields();j++)
            {
                const std::map<RVariableType,REntityGroupVariableDisplayTypeMask> &egVarData = rModel.getScalarField(j).getData().getVariableData();
                if (egVarData.find(variableType) != egVarData.end() || rModel.getScalarField(j).getVariableType() == variableType)
                {
                    this->glModelList.getGlScalarFieldList(j).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                }
            }
        }

        if (this->glModelList.getNGlStreamLineLists() > 0)
        {
            for (uint j=0;j<rModel.getNStreamLines();j++)
            {
                const std::map<RVariableType,REntityGroupVariableDisplayTypeMask> &egVarData = rModel.getStreamLine(j).getData().getVariableData();
                if (egVarData.find(variableType) != egVarData.end() || rModel.getStreamLine(j).getVariableType() == variableType)
                {
                    this->glModelList.getGlStreamLineList(j).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                }
            }
        }

        if (this->glModelList.getNGlCutLists() > 0)
        {
            for (uint j=0;j<rModel.getNCuts();j++)
            {
                const std::map<RVariableType,REntityGroupVariableDisplayTypeMask> &egVarData = rModel.getCut(j).getData().getVariableData();
                if (egVarData.find(variableType) != egVarData.end())
                {
                    this->glModelList.getGlCutList(j).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                }
            }
        }

        if (this->glModelList.getNGlIsoLists() > 0)
        {
            for (uint j=0;j<rModel.getNIsos();j++)
            {
                const std::map<RVariableType,REntityGroupVariableDisplayTypeMask> &egVarData = rModel.getIso(j).getData().getVariableData();
                if (egVarData.find(variableType) != egVarData.end() || rModel.getIso(j).getVariableType() == variableType)
                {
                    this->glModelList.getGlIsoList(j).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                }
            }
        }
    }
    catch (const RError &rError)
    {
        RLogger::error("Failed to handle event. %s",rError.getMessage().toUtf8().constData());
    }

    this->update();
}

void GLWidget::onVariableDataChanged(const SessionEntityID &entityID, RVariableType)
{
    RLogger::trace("GLWidget::onVariableDataChanged(const SessionEntityID &entityID, RVariableType)\n");

    if (this->modelID != entityID.getMid())
    {
        return;
    }

    try
    {
        switch (entityID.getType())
        {
            case R_ENTITY_GROUP_POINT:
                if (this->glModelList.getNGlPointLists() > entityID.getEid())
                {
                    this->glModelList.getGlPointList(entityID.getEid()).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                    this->glModelList.getGlPointList(entityID.getEid()).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_ELEMENT);
                    this->glModelList.getGlPointList(entityID.getEid()).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_NODE);
                }
                break;
            case R_ENTITY_GROUP_LINE:
                if (this->glModelList.getNGlLineLists() > entityID.getEid())
                {
                    this->glModelList.getGlLineList(entityID.getEid()).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                    this->glModelList.getGlLineList(entityID.getEid()).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_ELEMENT);
                    this->glModelList.getGlLineList(entityID.getEid()).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_NODE);
                }
                break;
            case R_ENTITY_GROUP_SURFACE:
                if (this->glModelList.getNGlSurfaceLists() > entityID.getEid())
                {
                    this->glModelList.getGlSurfaceList(entityID.getEid()).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                    this->glModelList.getGlSurfaceList(entityID.getEid()).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_ELEMENT);
                    this->glModelList.getGlSurfaceList(entityID.getEid()).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_NODE);
                }
                break;
            case R_ENTITY_GROUP_VOLUME:
                if (this->glModelList.getNGlVolumeLists() > entityID.getEid())
                {
                    this->glModelList.getGlVolumeList(entityID.getEid()).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                    this->glModelList.getGlVolumeList(entityID.getEid()).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_ELEMENT);
                    this->glModelList.getGlVolumeList(entityID.getEid()).setListInvalid(GL_ENTITY_LIST_ITEM_PICK_NODE);
                }
                break;
            case R_ENTITY_GROUP_VECTOR_FIELD:
                if (this->glModelList.getNGlVectorFieldLists() > entityID.getEid())
                {
                    this->glModelList.getGlVectorFieldList(entityID.getEid()).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                }
                break;
            case R_ENTITY_GROUP_SCALAR_FIELD:
                if (this->glModelList.getNGlScalarFieldLists() > entityID.getEid())
                {
                    this->glModelList.getGlScalarFieldList(entityID.getEid()).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                }
                break;
            case R_ENTITY_GROUP_STREAM_LINE:
                if (this->glModelList.getNGlStreamLineLists() > entityID.getEid())
                {
                    this->glModelList.getGlStreamLineList(entityID.getEid()).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                }
                break;
            case R_ENTITY_GROUP_CUT:
                if (this->glModelList.getNGlCutLists() > entityID.getEid())
                {
                    this->glModelList.getGlCutList(entityID.getEid()).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                }
                break;
            case R_ENTITY_GROUP_ISO:
                if (this->glModelList.getNGlIsoLists() > entityID.getEid())
                {
                    this->glModelList.getGlIsoList(entityID.getEid()).setListInvalid(GL_ENTITY_LIST_ITEM_NORMAL);
                }
                break;
            default:
                break;
        }
    }
    catch (const RError &rError)
    {
        RLogger::error("Failed to handle event. %s",rError.getMessage().toUtf8().constData());
    }

    this->update();
}

void GLWidget::onModelChanged(uint modelID)
{
    if (this->modelID != modelID)
    {
        return;
    }
    this->calculateModelScale();
    this->glModelList.clear();
    this->update();
}

void GLWidget::onResultsChanged(uint modelID)
{
    if (this->modelID != modelID)
    {
        return;
    }
    this->glModelList.clear();
    this->update();
}

void GLWidget::onBeginDrawStreamLinePosition(const RR3Vector &streamLinePosition)
{
    this->drawStreamLinePosition = true;
    this->streamLinePosition = streamLinePosition;
    this->update();
}

void GLWidget::onEndDrawStreamLinePosition(void)
{
    this->drawStreamLinePosition = false;
    this->update();
}

void GLWidget::onBeginDrawScaleOrigin(const RR3Vector &scaleOrigin)
{
    this->drawScaleOrigin = true;
    this->scaleOrigin = scaleOrigin;
    this->update();
}

void GLWidget::onEndDrawScaleOrigin(void)
{
    this->drawScaleOrigin = false;
    this->update();
}

void GLWidget::onBeginDrawRotationOrigin(const RR3Vector &rotationOrigin)
{
    this->drawRotationOrigin = true;
    this->rotationOrigin = rotationOrigin;
    this->update();
}

void GLWidget::onEndDrawRotationOrigin(void)
{
    this->drawRotationOrigin = false;
    this->update();
}

void GLWidget::onBeginDrawLocalDirections(const QList<RLocalDirection> &localDirections)
{
    this->drawLocalDirections = true;
    this->localDirections = localDirections;
    this->update();
}

void GLWidget::onEndDrawLocalDirections()
{
    this->drawLocalDirections = false;
    this->update();
}

void GLWidget::onBeginDrawCutPlane(const RPlane &plane)
{
    this->drawCutPlane = true;
    this->cutPlane = plane;
    this->update();
}

void GLWidget::onEndDrawCutPlane(void)
{
    this->drawCutPlane = false;
    this->update();
}

void GLWidget::onBeginDrawMoveNodes(const QMap<SessionNodeID, RR3Vector> &nodesToMove)
{
    this->nodesToMove = nodesToMove;
    this->drawMoveNodes = true;
    this->update();
}

void GLWidget::onEndDrawMoveNodes(void)
{
    this->drawMoveNodes = false;
    this->update();
}

void GLWidget::onDrawObjectAdded(void)
{
    this->update();
}

void GLWidget::onDrawObjectRemoved(void)
{
    this->update();
}

void GLWidget::onDrawObjectChanged(uint)
{
    this->update();
}

const GLModelList &GLWidget::getGLModelList(void) const
{
    return this->useGlVoidModelList ? this->glVoidModelList : this->glModelList;
}

GLModelList &GLWidget::getGLModelList(void)
{
    return this->useGlVoidModelList ? this->glVoidModelList : this->glModelList;
}

const GLActionEvent &GLWidget::getGLActionEvent(void) const
{
    return this->actionEvent;
}

const GLDisplayProperties &GLWidget::getGLDisplayProperties(void) const
{
    return this->displayProperties;
}

GLDisplayProperties &GLWidget::getGLDisplayProperties(void)
{
    return this->displayProperties;
}

bool GLWidget::getUseGLVoidModelList(void) const
{
    return this->useGlVoidModelList;
}

void GLWidget::setUseGLVoidModelList(bool useGlVoidModelList)
{
    this->useGlVoidModelList = useGlVoidModelList;
}

bool GLWidget::getUseGlCullFace(void) const
{
    return this->useGlCullFace;
}

void GLWidget::setUseGlCullFace(bool useGlCullFace)
{
    bool isDifferent = this->useGlCullFace != useGlCullFace;
    this->useGlCullFace = useGlCullFace;
    if (isDifferent)
    {
        Session::getInstance().setModelChanged(this->modelID);
    }
}

void GLWidget::takeScreenShot(const QString &fileName)
{
    QPixmap screenShot(QPixmap::fromImage(this->grabFramebuffer()));
    QString format = "PNG";
    QString saveFileName(fileName);

    QList<QString> supportedFormats = MainSettings::getSupportedImageFormats();

    if (saveFileName.isEmpty())
    {
        QString filter;

        if (supportedFormats.size() > 0)
        {
            filter = QString("Image files (");

            for (int i=0;i<supportedFormats.size();i++)
            {
                filter += QString("*.%1 ").arg(supportedFormats[i].toLower());
            }

            filter += QString(");;");

            for (int i=0;i<supportedFormats.size();i++)
            {
                filter += QString("%1 (*.%2);;").arg(supportedFormats[i].toUpper(),supportedFormats[i].toLower());
            }
        }
        filter += "All Files (*)";

        saveFileName = QFileDialog::getSaveFileName(MainWindow::getInstance(),
                                                    tr("Save screenshot as"),
                                                    Session::getInstance().getModel(this->modelID).buildScreenShotFileName(format.toLower()),
                                                    filter);
    }

    if (!saveFileName.isEmpty())
    {
        QFileInfo fi(saveFileName);
        QString suffix(fi.suffix().toUpper());

        for (int i=0;i<supportedFormats.size();i++)
        {
            if (suffix == supportedFormats[i].toUpper())
            {
                format = suffix;
            }
        }

        RLogger::info("Saving screen-shot file \'%s\' in format \'%s\'.\n",saveFileName.toUtf8().constData(), format.toUtf8().constData());
        screenShot.save(saveFileName, format.toUtf8().constData());
    }
}

void GLWidget::qglColor(const QColor &color)
{
    GL_SAFE_CALL(glColor4d(color.redF(),color.greenF(),color.blueF(),color.alphaF()));
}

void GLWidget::qglClearColor(const QColor &clearColor)
{
    GL_SAFE_CALL(glClearColor(GLclampf(clearColor.redF()),
                              GLclampf(clearColor.greenF()),
                              GLclampf(clearColor.blueF()),
                              GLclampf(clearColor.alphaF())));
}

void GLWidget::renderText(double x, double y, double z, const QString &str, const QFont &font)
{
    // Identify x and y locations to render text within widget
    int height = this->height();
    GLdouble model[4][4], proj[4][4];
    GLint view[4];
    GL_SAFE_CALL(glGetDoublev(GL_MODELVIEW_MATRIX, &model[0][0]));
    GL_SAFE_CALL(glGetDoublev(GL_PROJECTION_MATRIX, &proj[0][0]));
    GL_SAFE_CALL(glGetIntegerv(GL_VIEWPORT, &view[0]));
    GLdouble textPosX = 0, textPosY = 0, textPosZ = 0;
    this->project(x, y, z,
                  &model[0][0], &proj[0][0], &view[0],
                  &textPosX, &textPosY, &textPosZ);
    textPosY = height - textPosY; // y is inverted

    // Retrieve last OpenGL color to use as a font color
    GLdouble glColor[4];
    GL_SAFE_CALL(glGetDoublev(GL_CURRENT_COLOR, glColor));
    QColor fontColor;
    fontColor.setRedF(qreal(glColor[0]));
    fontColor.setGreenF(qreal(glColor[1]));
    fontColor.setBlueF(qreal(glColor[2]));
    fontColor.setAlphaF(qreal(glColor[3]));

    this->glTextRenderer.add(GLTextRendererItem(fontColor,font,QPointF(textPosX, textPosY)," " + str));
}
