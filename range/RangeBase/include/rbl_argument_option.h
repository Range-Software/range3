/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_argument_option.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   8-th September 2016                                      *
 *                                                                   *
 *  DESCRIPTION: Argument option class declaration                   *
 *********************************************************************/

#ifndef RBL_ARGUMENT_OPTION_H
#define RBL_ARGUMENT_OPTION_H

#include <QString>
#include <QVariant>

class RArgumentOption
{

    public:

        //! Option type.
        enum Type
        {
            None = 0,
            Switch,
            Char,
            String,
            Path,
            Integer,
            Real,
            Date
        };

    protected:

        //! Flag.
        QString flag;
        //! Type.
        Type type;
        //! Value.
        QVariant value;
        //! Description.
        QString description;
        //! Mandatory flag.
        bool mandatory;
        //! Exclusive flag.
        bool exclusive;

    private:

        //! Internal initialization function.
        void _init(const RArgumentOption *pArgumentOption = nullptr);

    public:

        //! Constructor.
        RArgumentOption();

        //! Constructor.
        RArgumentOption(const QString &flag, Type type, const QVariant &value, const QString &description, bool mandatory, bool exclusive);

        //! Copy constructor.
        RArgumentOption(const RArgumentOption &argumentOption);

        //! Destructor.
        ~RArgumentOption();

        //! Assignment operator.
        RArgumentOption &operator =(const RArgumentOption &argumentOption);

        //! Return const reference to flag.
        const QString &getFlag(void) const;

        //! Set flag.
        void setFlag(const QString &flag);

        //! Return type.
        Type getType(void) const;

        //! Set type.
        void setType(Type type);

        //! Return const reference to value.
        const QVariant &getValue(void) const;

        //! Set value.
        void setValue(const QVariant &value);

        //! Return const reference to description.
        const QString &getDescription(void) const;

        //! Set description.
        void setDescription(const QString &description);

        //! Return mandatory.
        bool getMandatory(void) const;

        //! Set mandatory.
        void setMandatory(bool mandatory);

        //! Return exclusive.
        bool getExclusive(void) const;

        //! Set exclusive.
        void setExclusive(bool exclusive);

        //! Generate flag message.
        QString getFlagMessage(void) const;

        //! Print help.
        QString getHelpMessage(uint width=0) const;

        //! Generate help option.
        static RArgumentOption generateHelpOption(void);

        //! Generate version option.
        static RArgumentOption generateVersionOption(void);

        //! Check if argument is option.
        static bool isOption(const QString &argument);

};

#endif // RBL_ARGUMENT_OPTION_H
