/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   monitoring_point_graph_dialog.h                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   28-th March 2014                                         *
 *                                                                   *
 *  DESCRIPTION: Monitoring point graph dialog class declaration     *
 *********************************************************************/

#ifndef __MONITORING_POINT_GRAPH_DIALOG_H__
#define __MONITORING_POINT_GRAPH_DIALOG_H__

#include <QDialog>
#include <QMap>
#include <QComboBox>
#include <QTreeWidget>

#include <rmlib.h>

class MonitoringPointGraphDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Monitoring points agregated structure.
        QMap< RMonitoringPoint,QVector<RRVector> > data;
        //! Monitoring points combo box.
        QComboBox *monitoringPointsCombo;
        //! Monitoring point values tree widget.
        QTreeWidget *valuesTree;

    public:

        //! Constructor.
        explicit MonitoringPointGraphDialog(const QString &monitoringFileName, QWidget *parent = 0);

        //! Execute dialog.
        int exec(void);

    protected:

        //! Read data from file.
        void readDataFromFile(const QString &fileName);

        //! Populate values tree widget.
        void populateValuesTree(void);

        //! Dislpay graph dialog.
        void displayInGraph(void);

    protected slots:

        //! Monitoring point combo current index changed.
        void onCurrentIndexChanged(int index);

        //! Display values in graph button clicked.
        void onDisplayInGraphClicked(void);

};

#endif // __MONITORING_POINT_GRAPH_DIALOG_H__
