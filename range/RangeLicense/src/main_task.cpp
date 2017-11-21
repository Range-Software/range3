/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   main_task.cpp                                            *
 *  GROUP:  RangeLicense                                             *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   8-th September 2016                                      *
 *                                                                   *
 *  DESCRIPTION: Main task class definition                          *
 *********************************************************************/

#include <QCoreApplication>

#include <rblib.h>
#include <ralib.h>

#include "main_task.h"

MainTask::MainTask(QObject *parent) :
    QObject(parent)
{
}

QString MainTask::generateLicense(const QString &account, const QString &password, const QString &name, const QDate &date)
{
    RLicenseRecord licenseRecord(name,date,QString());
    licenseRecord.setCode((licenseRecord.generateAuthCode(account,password)));
    return licenseRecord.toString();
}

void MainTask::run(void)
{
    QCoreApplication *app = (QCoreApplication*)this->parent();

    try
    {
        // Process command line arguments.
        QStringList arguments = app->arguments();
        QList<QString> argumentList;
        foreach (const QString &str, arguments)
        {
            argumentList.append(str);
        }

        QList<RArgumentOption> validOptions;
        validOptions.append(RArgumentOption("account",RArgumentOption::String,QVariant(),"Account",true,false));
        validOptions.append(RArgumentOption("password",RArgumentOption::String,QVariant(),"Password",true,false));
        validOptions.append(RArgumentOption("name",RArgumentOption::String,QVariant(),"Area name",true,false));
        validOptions.append(RArgumentOption("expiry-date",RArgumentOption::Date,QVariant(),"Date of expiry",true,false));
        validOptions.append(RArgumentOption("show-key",RArgumentOption::Switch,QVariant(),"Show software key",false,false));

        RArgumentsParser argumentsParser(argumentList,validOptions,false);

        if (argumentsParser.isSet("help"))
        {
            argumentsParser.printHelp("License generator");
            app->exit(0);
            return;
        }

        if (argumentsParser.isSet("version"))
        {
            argumentsParser.printVersion();
            app->exit(0);
            return;
        }

        QString licenseRecord = this->generateLicense(argumentsParser.getValue("account").toString(),
                                                      argumentsParser.getValue("password").toString(),
                                                      argumentsParser.getValue("name").toString(),
                                                      QDate::fromString(argumentsParser.getValue("expiry-date").toString(),"dd.MM.yyyy"));

        RLogger::info("%s\n", licenseRecord.toUtf8().constData());

        if (argumentsParser.isSet("show-key"))
        {
            RLogger::info("%s\n", RLicenseRecord::generateSoftwareKey().toUtf8().constData());
        }

        app->exit(0);
    }
    catch (const RError &error)
    {
        RLogger::error("Failed to start. %s\n",error.getMessage().toUtf8().constData());
        app->exit(1);
    }
}
