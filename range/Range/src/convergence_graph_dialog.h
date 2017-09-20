/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   convergence_graph_dialog.h                               *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   5-th February 2014                                       *
 *                                                                   *
 *  DESCRIPTION: Convergence graph dialog class declaration          *
 *********************************************************************/

#ifndef __CONVERGENCE_GRAPH_DIALOG_H__
#define __CONVERGENCE_GRAPH_DIALOG_H__

#include <QDialog>
#include <QComboBox>

#include "convergence_graph_object.h"

class ConvergenceGraphDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Data column combo box.
        QComboBox *dataColumnCombo;
        //! Graph object.
        ConvergenceGraphObject *graphObject;

    public:

        //! Constructor.
        explicit ConvergenceGraphDialog(const QStringList &convergenceFileNames, QWidget *parent = 0);

        //! Execute dialog.
        int exec(void);

    protected slots:

        //! Data column combo current index changed.
        void onCurrentIndexChanged(int index);
        //! Clear button was clicked.
        void onClearButtonClicked(void);

};

#endif // __CONVERGENCE_GRAPH_DIALOG_H__
