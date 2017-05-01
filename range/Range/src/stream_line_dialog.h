/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   stream_line_dialog.h                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   22-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Stream line dialog class declaration                *
 *********************************************************************/

#ifndef __STREAM_LINE_DIALOG_H__
#define __STREAM_LINE_DIALOG_H__

#include <QDialog>
#include <QListWidget>

#include <rmlib.h>

#include "position_widget.h"

class StreamLineDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Vector variables list widget.
        QListWidget *variableList;
        //! Stream line position.
        PositionWidget *positionWidget;
        //! OK button.
        QPushButton *okButton;
        //! Model ID.
        uint modelID;
        //! Entity ID.
        uint entityID;

    public:

        //! Constructor.
        explicit StreamLineDialog(uint modelID, QWidget *parent = 0);

        //! Constructor.
        explicit StreamLineDialog(uint modelID, uint entityID, QWidget *parent = 0);

        //! Execute dialog.
        int exec(void);

    private:

        //! Create dialog.
        void createDialog(void);

        //! Return selected variable type.
        RVariableType getVariableType(void) const;

    private slots:

        //! Position has changed.
        void onPositionChanged(const RR3Vector &position);

        //! Variable tree selection has changed.
        void onVariableListSelectionChanged(void);
        
};

#endif // __STREAM_LINE_DIALOG_H__
