/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_arguments_parser.h                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th August 2013                                        *
 *                                                                   *
 *  DESCRIPTION: Arguments parser class declaration                  *
 *********************************************************************/

#ifndef __RBL_ARGUMENTS_PARSER_H__
#define __RBL_ARGUMENTS_PARSER_H__

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
        //! List of valid options.
        QMap<QString,RArgumentOption> validOptions;

    private:

        //! Internal initialization function.
        void _init(const RArgumentsParser *pArgumentsParser = 0);

    public:

        //! Constructor.
        RArgumentsParser(const QList<QString> &argumentList,
                         const QList<RArgumentOption> &validOptions);

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

        //! Print help message.
        void printHelp(const QString &name) const;

        //! Print version.
        void printVersion(void) const;

    protected:

        //! Process argument list.
        void processArgumentList(const QList<QString> &argumentList);

        //! Process argument.
        bool processArgument(const QString &argument, RArgumentOption &option);

};

#endif // __RBL_ARGUMENTS_PARSER_H__
