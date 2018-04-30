/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   merge_near_nodes_dialog.h                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   18-th November 2016                                      *
 *                                                                   *
 *  DESCRIPTION: Merge near nodes dialog class declaration           *
 *********************************************************************/

#ifndef __MERGE_NEAR_NODES_DIALOG_H__
#define __MERGE_NEAR_NODES_DIALOG_H__

#include <QDialog>

#include "value_line_edit.h"

class MergeNearNodesDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Model ID.
        uint modelID;
        //! Tollerance edit.
        ValueLineEdit *toleranceEdit;

    public:

        //! Constructor.
        explicit MergeNearNodesDialog(uint modelID,QWidget *parent = 0);

        //! Overloaded exec function.
        int exec(void);

};

#endif // __MERGE_NEAR_NODES_DIALOG_H__
