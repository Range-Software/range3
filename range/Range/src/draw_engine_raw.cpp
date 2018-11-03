/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine_raw.cpp                                      *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   2-nd November 2018                                       *
 *                                                                   *
 *  DESCRIPTION: Draw engine RAW object class definition             *
 *********************************************************************/

#include "draw_engine_raw.h"

DrawEngineRaw::DrawEngineRaw(QObject *parent) :
    DrawEngineObject(parent)
{
    this->name = tr("Geometry");

    this->inputParameters.append(DrawEngineInput(QString("1 2 3"),tr("RAW model"),tr("RAW model file content."),"ASCII"));

    this->updateModel();
}

RModelRaw DrawEngineRaw::generate(const QString &rawModelContent)
{
    RModelRaw model;

    QString content(rawModelContent);
    QTextStream textStream(&content);

    try
    {
        model.readTextStream(textStream,RConstants::eps);
    }
    catch (const RError &rError)
    {
        RLogger::warning("Failed to generate object from RAW model input. %s\n",rError.getMessage().toUtf8().constData());
    }

    return model;
}

RModelRaw DrawEngineRaw::generate() const
{
    return DrawEngineRaw::generate(this->inputParameters.at(0).toText());
}
