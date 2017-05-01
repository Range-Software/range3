/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   drop_results_dialog.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   25-th Septamber 2013                                     *
 *                                                                   *
 *  DESCRIPTION: Drop results dialog class declaration               *
 *********************************************************************/

#ifndef __DROP_RESULTS_DIALOG_H__
#define __DROP_RESULTS_DIALOG_H__

#include <QDialog>
#include <QCheckBox>

class DropResultsDialog : public QDialog
{

    Q_OBJECT

    protected:

        QCheckBox *clearResultsCheckBox;
        QCheckBox *deleteResultsCheckBox;

    public:

        //! Constructor.
        explicit DropResultsDialog(QWidget *parent = 0);

        //! Execute dialog.
        int exec(void);
        
    signals:
        
    public slots:
        
};

#endif // __DROP_RESULTS_DIALOG_H__
