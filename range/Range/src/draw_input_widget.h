/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_input_widget.h                                      *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   14-th October 2014                                       *
 *                                                                   *
 *  DESCRIPTION: Draw input widget class declaration                 *
 *********************************************************************/

#ifndef __DRAW_INPUT_WIDGET_H__
#define __DRAW_INPUT_WIDGET_H__

#include <QPushButton>
#include <QCheckBox>
#include <QStackedLayout>

#include "draw_input_tree.h"
#include "position_widget.h"

class DrawInputWidget : public QWidget
{

    Q_OBJECT

    protected:

        //! Stacked layout.
        QStackedLayout *stackedLayout;
        //! Position widget.
        PositionWidget *positionWidget;
        //! Layout widget.
        QWidget *layoutWidget;
        //! Draw input tree.
        DrawInputTree *tree;
        //! Merge near nodes check box.
        QCheckBox *mergeNodesCheck;
        //! Remove button.
        QPushButton *removeButton;
        //! OK button.
        QPushButton *okButton;

    public:

        //! Constructor.
        explicit DrawInputWidget(QWidget *parent = 0);

    protected slots:

        //! Catch position request signal.
        void onPositionRequest(const RR3Vector &position);

        //! Position widget has changed.
        void onPositionWidgetChanged(const RR3Vector &position);

        //! Position widget has closed.
        void onPositionWidgetClosed(void);

        //! Catch tree selection changed signal.
        void onSelectionChanged(const QList<uint> &objectIDs);

        //! Catch input parameter changed.
        void onInputParameterChanged(uint objectID, uint);

        //! Remove button was clicked.
        void onRemoveClicked(void);

        //! OK button was clicked.
        void onOkClicked(void);

};

#endif // __DRAW_INPUT_WIDGET_H__
