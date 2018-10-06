/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_arguments_parser.cpp                                 *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   26-th August 2013                                        *
 *                                                                   *
 *  DESCRIPTION: Arguments parser class definition                   *
 *********************************************************************/

#include <QStringList>
#include <QTextDocument>

#include "rbl_arguments_parser.h"
#include "rbl_logger.h"
#include "rbl_error.h"
#include "rbl_utils.h"

void RArgumentsParser::_init(const RArgumentsParser *pArgumentsParser)
{
    if (pArgumentsParser)
    {
        this->executable = pArgumentsParser->executable;
        this->arguments = pArgumentsParser->arguments;
        this->filesEnabled = pArgumentsParser->filesEnabled;
        this->files = pArgumentsParser->files;
        this->validOptions = pArgumentsParser->validOptions;
    }
}

RArgumentsParser::RArgumentsParser(const QStringList &argumentList, const QList<RArgumentOption> &validOptions, bool filesEnabled)
    : filesEnabled(filesEnabled)
{
    this->_init();
    foreach (const RArgumentOption &option, validOptions)
    {
        this->validOptions.insert(option.getFlag(),option);
    }
    RArgumentOption helpOption(RArgumentOption::generateHelpOption());
    RArgumentOption versionOption(RArgumentOption::generateVersionOption());
    this->validOptions.insert(helpOption.getFlag(),helpOption);
    this->validOptions.insert(versionOption.getFlag(),versionOption);
    this->processArgumentList(argumentList);
}

RArgumentsParser::RArgumentsParser(const RArgumentsParser &argumentsParser)
{
    this->_init(&argumentsParser);
}

RArgumentsParser::~RArgumentsParser()
{
}

RArgumentsParser &RArgumentsParser::operator =(const RArgumentsParser &argumentsParser)
{
    this->_init(&argumentsParser);
    return (*this);
}

bool RArgumentsParser::isSet(const QString &option) const
{
    return (this->arguments.find(option) != this->arguments.end());
}

bool RArgumentsParser::isValid(const QString &option) const
{
    return (this->validOptions.find(option) != this->validOptions.end());
}

QVariant RArgumentsParser::getValue(const QString &option) const
{
    QMap<QString,RArgumentOption>::const_iterator iter = this->arguments.find(option);

    if (iter == this->arguments.end())
    {
        return QVariant();
    }
    else
    {
        return iter.value().getValue();
    }
}

const QStringList &RArgumentsParser::getFiles() const
{
    return this->files;
}

void RArgumentsParser::printHelp(void) const
{
    RLogger::info("Usage: %s%s <mandatory arguments> [optional arguments]\n",
                  this->executable.toUtf8().constData(),
                  this->filesEnabled ? " [file(s)]" : "");
    RLogger::info("\n");
    RLogger::info("  Mandatory:\n");
    foreach (const RArgumentOption &option, this->validOptions)
    {
        if (option.getMandatory())
        {
            RLogger::info("    %s\n",option.getHelpMessage(30).toUtf8().constData());
        }
    }
    RLogger::info("\n");
    RLogger::info("  Optional:\n");
    foreach (const RArgumentOption &option, this->validOptions)
    {
        if (!option.getMandatory())
        {
            RLogger::info("    %s\n",option.getHelpMessage(30).toUtf8().constData());
        }
    }
}

void RArgumentsParser::printVersion(void) const
{
    RLogger::info("VERSION=%s\n",RVendor::version.toString().toUtf8().constData());
}

void RArgumentsParser::printHeader(const QString &applicationName)
{
    RLogger::info(" ____                          ____         __ _                          \n");
    RLogger::info("|  _ \\ __ _ _ __   __ _  ___  / ___|  ___  / _| |___      ____ _ _ __ ___ \n");
    RLogger::info("| |_) / _` | \'_ \\ / _` |/ _ \\ \\___ \\ / _ \\| |_| __\\ \\ /\\ / / _` | '__/ _ \\\n");
    RLogger::info("|  _ < (_| | | | | (_| |  __/  ___) | (_) |  _| |_ \\ V  V / (_| | | |  __/\n");
    RLogger::info("|_| \\_\\__,_|_| |_|\\__, |\\___| |____/ \\___/|_|  \\__| \\_/\\_/ \\__,_|_|  \\___|\n");
    RLogger::info("                  |___/                                                   \n");
    RLogger::info("\n");
    RLogger::info("%s %s\n",RVendor::shortName.toUtf8().constData(),applicationName.toUtf8().constData());
    RLogger::info("%s\n",RVendor::title.toUtf8().constData());
    RLogger::info("Version: %s\n",RVendor::version.toString().toUtf8().constData());
    RLogger::info("--------------------------------------------------------------------------\n");
    RLogger::info("\n");
}

void RArgumentsParser::printFooter(void)
{
    RLogger::info("\n");
    RLogger::info("--------------------------------------------------------------------------\n");
    RLogger::info("%s (c) %d\n",RVendor::author.toUtf8().constData(),RVendor::year);
    RLogger::info("%s\n",RVendor::email.toUtf8().constData());
    RLogger::info("%s\n",RVendor::www.toUtf8().constData());
}

void RArgumentsParser::processArgumentList(const QStringList &argumentList)
{
    // Process executable.
    if (argumentList.size() > 0)
    {
        this->executable = argumentList.at(0);
    }

    bool exclusiveSet = false;

    // Process arguments
    for (int i=1;i<argumentList.size();i++)
    {
        RArgumentOption option;
        if (this->processArgument(argumentList[i],option))
        {
            this->arguments.insert(option.getFlag(),option);
            if (option.getExclusive())
            {
                exclusiveSet = true;
            }
        }
        else
        {
            if (this->filesEnabled && !RArgumentOption::isOption(argumentList[i]))
            {
                this->files.append(argumentList[i]);
            }
            else
            {
                throw RError(R_ERROR_INVALID_INPUT,R_ERROR_REF,"Invalid command line argument \'%s\'",argumentList[i].toUtf8().constData());
            }
        }
    }

    if (!exclusiveSet)
    {
        // Check for mandatory options
        foreach (const RArgumentOption &rArgument, this->validOptions)
        {
            if (rArgument.getMandatory())
            {
                if (!this->isSet(rArgument.getFlag()))
                {
                    throw RError(R_ERROR_INVALID_INPUT,
                                 R_ERROR_REF,
                                 "Missing mandatory command line argument \'%s\'",
                                 rArgument.getFlagMessage().toUtf8().constData());
                }
            }
        }
    }
}

bool RArgumentsParser::processArgument(const QString &argument, RArgumentOption &option)
{
    QString flag;
    QString value;

    if (argument.size() < 2)
    {
        return false;
    }
    if (argument[0] != '-')
    {
        return false;
    }

    if (argument.size() == 2)
    {
        flag = argument[1];
    }
    else
    {
        if (argument[1] != '-')
        {
            return false;
        }

        QStringList query = argument.split(QRegExp("(\\=)"));

        flag = query.at(0);
        flag.remove(QRegExp("^[-]*"));
        if (query.size() > 1)
        {
            value = query.at(1);
        }
    }

    QMap<QString,RArgumentOption>::const_iterator iter = this->validOptions.find(flag);
    if (iter == this->validOptions.end())
    {
        return false;
    }

    option = iter.value();
    switch (option.getType())
    {
        case RArgumentOption::Switch:
        {
            option.setValue(QVariant(true));
            break;
        }
        case RArgumentOption::Char:
        {
            QChar c = value.isEmpty() ? QChar('\0') : value[0];
            option.setValue(QVariant(c));
            break;
        }
        case RArgumentOption::Integer:
        {
            bool isOk = false;
            option.setValue(QVariant(value.toInt(&isOk)));
            if (!isOk)
            {
                RLogger::warning("Invalid number \'%s\'\n",value.toUtf8().constData());
                return false;
            }
            break;
        }
        case RArgumentOption::Real:
        {
            bool isOk = false;
            option.setValue(QVariant(value.toDouble(&isOk)));
            if (!isOk)
            {
                RLogger::warning("Invalid value \'%s\'\n",value.toUtf8().constData());
                return false;
            }
            break;
        }
        case RArgumentOption::Date:
        {
            QDate date(QDate::fromString(value,"dd.MM.yyyy"));
            if (!date.isValid())
            {
                RLogger::warning("Invalid date \'%s\'\n",value.toUtf8().constData());
                return false;
            }
            option.setValue(QVariant(value));
            break;
        }
        case RArgumentOption::String:
        case RArgumentOption::Path:
        default:
        {
            option.setValue(QVariant(value));
            break;
        }
    }

    return true;
}
