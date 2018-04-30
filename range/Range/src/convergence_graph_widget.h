/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   convergence_graph_widget.h                               *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   21-st September 2017                                     *
 *                                                                   *
 *  DESCRIPTION: Convergence graph widget class declaration          *
 *********************************************************************/

#ifndef __CONVERGENCE_GRAPH_WIDGET_H__
#define __CONVERGENCE_GRAPH_WIDGET_H__

#include <QWidget>
#include <QComboBox>

#include "convergence_graph_object.h"

class ConvergenceGraphWidget : public QWidget
{
    Q_OBJECT

    protected:

        //! Data column combo box.
        QComboBox *dataColumnCombo;
        //! Graph object.
        ConvergenceGraphObject *graphObject;

    public:

        //! Constructor.
        explicit ConvergenceGraphWidget(const QString &convergenceFileName, QWidget *parent = 0);

    signals:

    public slots:

        //! Data column combo current index changed.
        void onCurrentIndexChanged(int index);
        //! Clear button was clicked.
        void onClearButtonClicked(void);

};

#endif // __CONVERGENCE_GRAPH_WIDGET_H__
