/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   cut_dialog.h                                             *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   22-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Cut dialog class declaration                        *
 *********************************************************************/

#ifndef CUT_DIALOG_H
#define CUT_DIALOG_H

#include <QDialog>
#include <QDoubleSpinBox>

#include "model_tree_simple.h"

class CutDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Model tree widget.
        ModelTreeSimple *modelTree;
        //! OK button.
        QPushButton *okButton;
        //! Cut plane.
        RPlane cutPlane;
        //! Model ID.
        uint modelID;
        //! Entity ID.
        uint entityID;

    public:

        //! Constructor.
        explicit CutDialog(uint modelID, uint entityID, QWidget *parent = nullptr);
        
    private slots:

        //! Accept clicked.
        void onAccept(void);

        //! Reject clicked.
        void onReject(void);

        //! Position changed.
        void onPositionChanged(const RR3Vector &position);

        //! Direction changed.
        void onDirectionChanged(const RR3Vector &direction);

        //! Model tree selection has changed.
        void onModelTreeSelectionChanged(void);
        
};

#endif // CUT_DIALOG_H
