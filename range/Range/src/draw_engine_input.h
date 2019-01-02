/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine_input.h                                      *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th October 2014                                        *
 *                                                                   *
 *  DESCRIPTION: Draw engine input class declaration                 *
 *********************************************************************/

#ifndef DRAW_ENGINE_INPUT_H
#define DRAW_ENGINE_INPUT_H

#include <cfloat>

#include <rblib.h>

typedef struct _DrawEngineInputDataType
{
    bool b;
    int i;
    uint u;
    double d;
    double v[3];
    QString text;
} DrawEngineInputDataType;

class DrawEngineInput
{

    public:

        enum Type
        {
            Bool = 0,
            Int,
            Uint,
            Double,
            Vector,
            Text
        };

    protected:

        //! Parameter data type.
        Type type;
        //! Parameter data.
        DrawEngineInputDataType value;
        DrawEngineInputDataType minValue;
        DrawEngineInputDataType maxValue;
        //! Parameter name.
        QString name;
        //! Parameter description.
        QString desc;
        //! Parameter units.
        QString units;

    private:

        //! Internal initialization function.
        void _init(const DrawEngineInput *pDrawEngineInput = 0);

    public:

        //! Constructor.
        DrawEngineInput(bool b, const QString &name = QString(), const QString &desc = QString(), const QString &units = QString());

        //! Constructor.
        DrawEngineInput(int i, const QString &name = QString(), const QString &desc = QString(), const QString &units = QString(), int minValue = INT_MIN, int maxValue = INT_MAX);

        //! Constructor.
        DrawEngineInput(uint u, const QString &name = QString(), const QString &desc = QString(), const QString &units = QString(), uint minValue = 0, uint maxValue = UINT_MAX);

        //! Constructor.
        DrawEngineInput(double d, const QString &name = QString(), const QString &desc = QString(), const QString &units = QString(), double minValue = -DBL_MAX, double maxValue = DBL_MAX);

        //! Constructor.
        DrawEngineInput(const RR3Vector &v, const QString &name = QString(), const QString &desc = QString(), const QString &units = QString());

        //! Constructor.
        DrawEngineInput(const QString &text, const QString &name = QString(), const QString &desc = QString(), const QString &units = QString());

        //! Copy constructor.
        DrawEngineInput(const DrawEngineInput &drawEngineInput);

        //! Assignment operator.
        DrawEngineInput & operator =(const DrawEngineInput &drawEngineInput);

        //! Assign bool value.
        //! Return true if value has the same type and is valid.
        bool setValue(bool b);

        //! Assign int value.
        //! Return true if value has the same type and is valid.
        bool setValue(int i);

        //! Assign uint value.
        //! Return true if value has the same type and is valid.
        bool setValue(uint u);

        //! Assign double value.
        //! Return true if value has the same type and is valid.
        bool setValue(double d);

        //! Assign vector value.
        //! Return true if value has the same type and is valid.
        bool setValue(const RR3Vector &v);

        //! Assign text value.
        //! Return true if value has the same type and valid.
        bool setValue(const QString &text);

        //! Return boolean.
        bool toBool(bool *isOk = 0) const;

        //! Return int.
        int toInt(bool *isOk = 0) const;

        //! Return uint.
        uint toUint(bool *isOk = 0) const;

        //! Return double.
        double toDouble(bool *isOk = 0) const;

        //! Return vector.
        RR3Vector toVector(bool *isOk = 0) const;

        //! Return text.
        QString toText(bool *isOk = 0) const;

        //! Return input parameter type.
        Type getType() const;

        //! Return const reference to parameter name.
        const QString &getName() const;

        //! Return const reference to parameter description.
        const QString &getDescription() const;

        //! Return const reference to parameter units.
        const QString &getUnits() const;

    protected:

        //! Validate current value.
        bool isValid(bool value) const;

        //! Validate current value.
        bool isValid(int value) const;

        //! Validate current value.
        bool isValid(uint value) const;

        //! Validate current value.
        bool isValid(double value) const;

        //! Validate current value.
        bool isValid(const RR3Vector &value) const;

        //! Validate current value.
        bool isValid(const QString &value) const;

};

#endif // DRAW_ENGINE_INPUT_H
