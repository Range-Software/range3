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

class ConvergenceGraphDialog : public QDialog
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit ConvergenceGraphDialog(const QStringList &convergenceFileNames, QWidget *parent = 0);

        //! Execute dialog.
        int exec(void);

    private:

        //! Find problem ID or matrix solver ID in convergence log filename and convert it to name.
        static QString getNameFromID(const QString &fileName, const QString &fallbackName);

};

#endif // __CONVERGENCE_GRAPH_DIALOG_H__
