/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   variable_selector.h                                      *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   15-th March 2014                                         *
 *                                                                   *
 *  DESCRIPTION: Variable selector class declaration                 *
 *********************************************************************/

#ifndef __VARIABLE_SELECTOR_H__
#define __VARIABLE_SELECTOR_H__

#include <QComboBox>

#include <rmlib.h>

class VariableSelector : public QComboBox
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit VariableSelector(const std::vector<RVariableType> &variableTypes,QWidget *parent = 0);

        //! Return current variable type.
        RVariableType getCurrentVariableType(void) const;

        //! Set current variable type and return true on success.
        bool setCurrentVariableType(RVariableType variableType);

    protected:

        //! Populate combo.
        void populate(const std::vector<RVariableType> &variableTypes);

    signals:

        //! Signal that results variable selection has changed.
        void resultsVariableSelected(RVariableType variableType);

    public slots:

        //! Catch QComboBox::currentIndexChanged(int) signal
        void onCurrentIndexChanged(int index);

};

#endif // __VARIABLE_SELECTOR_H__
