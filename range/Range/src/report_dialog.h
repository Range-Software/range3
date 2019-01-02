/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   report_dialog.h                                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   28-st August 2013                                        *
 *                                                                   *
 *  DESCRIPTION: Report dialog class declaration                     *
 *********************************************************************/

#ifndef REPORT_DIALOG_H
#define REPORT_DIALOG_H

#include <QDialog>
#include <QGroupBox>
#include <QRadioButton>

#include "file_chooser_button.h"

class ReportDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Model ID for which report will be generated.
        uint modelID;
        //! HTML file enabled.
        static bool htmlFileEnabled;
        //! HTML group box.
        QGroupBox *htmlGroupBox;
        //! HTML file chooser.
        FileChooserButton *htmlFileChooserButton;
        //! ODF file enabled.
        static bool odfFileEnabled;
        //! ODF group box.
        QGroupBox *odfGroupBox;
        //! ODF file chooser.
        FileChooserButton *odfFileChooserButton;
        //! PDF file enabled.
        static bool pdfFileEnabled;
        //! PDF group box.
        QGroupBox *pdfGroupBox;
        //! PDF file chooser.
        FileChooserButton *pdfFileChooserButton;

    public:

        //! Constructor.
        explicit ReportDialog(uint modelID, QWidget *parent = nullptr);

        //! Execute dialog.
        int exec(void);
        
};

#endif // REPORT_DIALOG_H
