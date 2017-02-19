/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   graph_dialog.h                                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   22-nd March 2013                                         *
 *                                                                   *
 *  DESCRIPTION: Graph dialog class declaration                      *
 *********************************************************************/

#ifndef __GRAPH_DIALOG_H__
#define __GRAPH_DIALOG_H__

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
        explicit GraphDialog(GraphObject *graphObject, QWidget *parent = 0);

        //! Return pointer to graph widget.
        GraphWidget * getGraphWidget(void);
        
};

#endif // __GRAPH_DIALOG_H__
