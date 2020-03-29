/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   color.cpp                                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   17-th February 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Color class definition                              *
 *********************************************************************/

#include <QtCore>

#include "color.h"

Color::Color()
{
}

Color::Color(int r, int g, int b, int a) : QColor(r,g,b,a)
{
}

Color::Color(QRgb color) : QColor(color)
{
}

Color::Color(const QString &name) : QColor(name)
{
}

Color::Color(const char *name) : QColor(name)
{
}

Color::Color(const QColor &color) : QColor(color)
{
}

Color::Color(const Color &color) : QColor(color)
{
}

Color::Color(Qt::GlobalColor color) : QColor(color)
{
}

Color &Color::operator=(const Color &color)
{
    this->QColor::operator=(color);
    return (*this);
}

void Color::randomize(bool fromGlobal, bool excludeWhiteAndBlack)
{
    if (fromGlobal)
    {
        QList<Qt::GlobalColor> colorList = this->getGlobalColors(excludeWhiteAndBlack);
        int nColors = colorList.size();
        if (nColors == 0)
        {
            return;
        }
        int colorPos = int((nColors-1)*double(qrand())/double(RAND_MAX));
        this->operator = (colorList[colorPos]);
    }
    else
    {
        this->setRed(int(51.0*qrand()/(RAND_MAX*5.0)));
        this->setGreen(int(51.0*qrand()/(RAND_MAX*5.0)));
        this->setBlue(int(51.0*qrand()/(RAND_MAX*5.0)));
        this->setAlpha(255);
    }
}

QColor Color::random(bool fromGlobal, bool excludeWhiteAndBlack)
{
    Color color;
    color.randomize(fromGlobal,excludeWhiteAndBlack);
    return QColor(color.rgba());
}

QList<Qt::GlobalColor> Color::getGlobalColors(bool excludeWhiteAndBlack)
{
    QList<Qt::GlobalColor> colorList;
    if (!excludeWhiteAndBlack)
    {
        colorList.append(Qt::white);
        colorList.append(Qt::black);
    }
    colorList.append(Qt::red);
    colorList.append(Qt::darkRed);
    colorList.append(Qt::green);
    colorList.append(Qt::darkGreen);
    colorList.append(Qt::blue);
    colorList.append(Qt::darkBlue);
    colorList.append(Qt::cyan);
    colorList.append(Qt::darkCyan);
    colorList.append(Qt::magenta);
    colorList.append(Qt::darkMagenta);
    colorList.append(Qt::yellow);
    colorList.append(Qt::darkYellow);
    colorList.append(Qt::gray);
    colorList.append(Qt::darkGray);
    colorList.append(Qt::lightGray);
    return colorList;
}

QList<Qt::GlobalColor> Color::getPaintColors(void)
{
    QList<Qt::GlobalColor> colorList;
    colorList.append(Qt::red);
    colorList.append(Qt::green);
    colorList.append(Qt::blue);
    colorList.append(Qt::cyan);
    colorList.append(Qt::magenta);
    colorList.append(Qt::yellow);
    colorList.append(Qt::darkRed);
    colorList.append(Qt::darkGreen);
    colorList.append(Qt::darkBlue);
    colorList.append(Qt::darkCyan);
    colorList.append(Qt::darkMagenta);
    colorList.append(Qt::darkYellow);
    return colorList;
}
