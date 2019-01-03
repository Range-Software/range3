/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   model_statistics_dialog.h                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   24-th November 2016                                      *
 *                                                                   *
 *  DESCRIPTION: Model statistics dialog class declaration           *
 *********************************************************************/

#ifndef MODEL_STATISTICS_DIALOG_H
#define MODEL_STATISTICS_DIALOG_H

#include <QDialog>
#include <QTextBrowser>

class ModelStatisticsDialog : public QDialog
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit ModelStatisticsDialog(uint modelID, QWidget *parent = nullptr);

        //! Overloaded exec function.
        int exec(void);

    private:

        //! Produce statistics.
        void produceStatistics(uint modelID, QTextBrowser *textBrowser);

        //! Produce standard table row.
        static QString generateTableRow(const QString &label, const QString &value);

};

#endif // MODEL_STATISTICS_DIALOG_H
