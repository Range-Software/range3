/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_arguments_parser.h                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th August 2013                                        *
 *                                                                   *
 *  DESCRIPTION: Arguments parser class declaration                  *
 *********************************************************************/

#ifndef RBL_ARGUMENTS_PARSER_H
#define RBL_ARGUMENTS_PARSER_H

#include <QString>
#include <QMap>
#include <QList>

#include "rbl_argument_option.h"

class RArgumentsParser
{

    protected:

        //! Application executable.
        QString executable;
        //! Commandline arguments.
        QMap<QString,RArgumentOption> arguments;
        //! Default files are enabled.
        bool filesEnabled;
        //! List of default files to open.
        QStringList files;
        //! List of valid options.
        QMap<QString,RArgumentOption> validOptions;

    private:

        //! Internal initialization function.
        void _init(const RArgumentsParser *pArgumentsParser = nullptr);

    public:

        //! Constructor.
        RArgumentsParser(const QStringList &argumentList,
                         const QList<RArgumentOption> &validOptions,
                         bool filesEnabled);

        //! Copy constructor.
        RArgumentsParser(const RArgumentsParser &argumentsParser);

        //! Destructor.
        ~RArgumentsParser();

        //! Assignment operator.
        RArgumentsParser & operator =(const RArgumentsParser &argumentsParser);

        //! Return true if given option is set.
        bool isSet(const QString &option) const;

        //! Return true if given option is valid.
        bool isValid(const QString &option) const;

        //! Return value of given option.
        //! If option is not set empty string is returned.
        QVariant getValue(const QString &option) const;

        //! Return list of files.
        const QStringList &getFiles(void) const;

        //! Print help message.
        void printHelp(void) const;

        //! Print version.
        void printVersion(void) const;

        //! Print header.
        static void printHeader(const QString &applicationName);

        //! Print footr.
        static void printFooter(void);

    protected:

        //! Process argument list.
        void processArgumentList(const QStringList &argumentList);

        //! Process argument.
        bool processArgument(const QString &argument, RArgumentOption &option);

};

#endif // RBL_ARGUMENTS_PARSER_H
