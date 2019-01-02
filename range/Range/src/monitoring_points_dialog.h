/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   monitoring_points_dialog.h                               *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   12-th March 20134                                        *
 *                                                                   *
 *  DESCRIPTION: Monitoring points class declaration                 *
 *********************************************************************/

#ifndef MONITORING_POINTS_DIALOG_H
#define MONITORING_POINTS_DIALOG_H

#include <QDialog>
#include <QTableWidget>

#include <rmlib.h>

class MonitoringPointsDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Model ID.
        uint modelID;
        //! Table widget containing monitoring points.
        QTableWidget *tableWidget;
        //! Remove button.
        QPushButton *removeButton;
        //! Use picked coordinates button.
        QPushButton *usePickedButton;

    public:

        //! Construct.
        explicit MonitoringPointsDialog(uint modelID, QWidget *parent = nullptr);

        //! Execute dialog.
        int exec(void);

    protected:

        //! Add new monitoring point.
        void addMonitoringPoint(const RMonitoringPoint &monitoringPoint);

        //! Return list of selected rows.
        QList<int> findSelectedRows(void) const;

        //! Find picked coordinates.
        RR3Vector findPickedPosition(void) const;

    protected slots:

        //! Item selection has changed.
        void onItemSelectionChanged(void);

        //! Add button was clicked.
        void onAddClicked(void);

        //! Remove button was clicked.
        void onRemoveClicked(void);

        //! Use picked button was clicked.
        void onUsePickedClicked(void);

};

#endif // MONITORING_POINTS_DIALOG_H
