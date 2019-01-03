/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   graph_dialog.h                                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   22-nd March 2013                                         *
 *                                                                   *
 *  DESCRIPTION: Graph dialog class declaration                      *
 *********************************************************************/

#ifndef GRAPH_DIALOG_H
#define GRAPH_DIALOG_H

#include <QDialog>

#include "graph_object.h"
#include "graph_widget.h"

class GraphDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Graph widget.
        GraphWidget *graphWidget;

    public:

        //! Constructor.
        explicit GraphDialog(GraphObject *graphObject, QWidget *parent = nullptr);

        //! Return pointer to graph widget.
        GraphWidget * getGraphWidget(void);
        
};

#endif // GRAPH_DIALOG_H
