/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   variable_value_edit.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   18-th July 2012                                          *
 *                                                                   *
 *  DESCRIPTION: Variable value edit class declaration               *
 *********************************************************************/

#ifndef __VARIABLE_VALUE_EDIT_H__
#define __VARIABLE_VALUE_EDIT_H__

#include <QCheckBox>

#include <rmlib.h>

#include "value_line_edit.h"

class VariableValueEdit : public QWidget
{
    Q_OBJECT

    protected:

        //! Variable type.
        RVariableType type;
        //! Value line edit.
        ValueLineEdit *lineEdit;
        //! Boolean check box.
        QCheckBox *checkBox;

    public:

        //! Constructor.
        explicit VariableValueEdit(RVariableType variableType,
                                   QWidget *parent = 0);

        //! Set new value.
        void setValue(double value);
        
    signals:
        
        //! Value changed signal.
        void valueChanged(RVariableType variableType, double value);

    private slots:
        
        //! Catch signal value changed.
        void onValueChaged(double value);

        //! Catch signal state changed.
        void onStateChaged(int state);
};

#endif /* __VARIABLE_VALUE_EDIT_H__ */
