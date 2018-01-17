/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   report_dialog.cpp                                        *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   28-st August 2013                                        *
 *                                                                   *
 *  DESCRIPTION: Report dialog class definition                      *
 *********************************************************************/

#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>

#include <rblib.h>

#include "session.h"
#include "main_settings.h"
#include "job_manager.h"
#include "report_dialog.h"
#include "report_generator.h"
#include "file_chooser_button.h"

bool ReportDialog::htmlFileEnabled = true;
bool ReportDialog::pdfFileEnabled = true;
bool ReportDialog::odfFileEnabled = true;

ReportDialog::ReportDialog(uint modelID, QWidget *parent)
    : QDialog(parent)
    , modelID(modelID)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    this->setWindowTitle(tr("Create report"));

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout (mainLayout);

    uint rowCount = 0;

    QLabel *titleLabel = new QLabel(tr("Generate report for model") + " <b>" + Session::getInstance().getModel(this->modelID).getName() + "</b>");
    mainLayout->addWidget(titleLabel, rowCount++, 0, 1, 1);

    QGroupBox *fileGroupBox = new QGroupBox(tr("File"));
    mainLayout->addWidget(fileGroupBox, rowCount++, 0, 1, 1);

    QVBoxLayout *fileLayout = new QVBoxLayout;
    fileGroupBox->setLayout(fileLayout);

    QString htmlFileName = Session::getInstance().getModel(this->modelID).buildDocFileName("html");
    QString odfFileName = Session::getInstance().getModel(this->modelID).buildDocFileName("odf");
    QString pdfFileName = Session::getInstance().getModel(this->modelID).buildDocFileName("pdf");

    this->htmlGroupBox = new QGroupBox("HyperText Markup Language (HTML)");
    this->htmlGroupBox->setCheckable(true);
    this->htmlGroupBox->setChecked(ReportDialog::htmlFileEnabled);
    fileLayout->addWidget(this->htmlGroupBox);

    QVBoxLayout *htmlFileLayout = new QVBoxLayout;
    this->htmlGroupBox->setLayout(htmlFileLayout);

    this->htmlFileChooserButton = new FileChooserButton("Report file:",
                                                        FileChooserButton::SaveFile,
                                                        this,
                                                        "Select file",
                                                        htmlFileName,
                                                        "HTML files (*.html *.htm);;Any files (*)");
    htmlFileLayout->addWidget(this->htmlFileChooserButton);

    this->odfGroupBox = new QGroupBox("Portable Document Format (ODF)");
    this->odfGroupBox->setCheckable(true);
    this->odfGroupBox->setChecked(ReportDialog::odfFileEnabled);
    fileLayout->addWidget(this->odfGroupBox);

    QVBoxLayout *odfFileLayout = new QVBoxLayout;
    this->odfGroupBox->setLayout(odfFileLayout);

    this->odfFileChooserButton = new FileChooserButton("Report file:",
                                                       FileChooserButton::SaveFile,
                                                       this,"Select file",
                                                       odfFileName,
                                                       "ODF files (*.odf);;Any files (*)");
    odfFileLayout->addWidget(this->odfFileChooserButton);

    this->pdfGroupBox = new QGroupBox("Portable Document Format (PDF)");
    this->pdfGroupBox->setCheckable(true);
    this->pdfGroupBox->setChecked(ReportDialog::pdfFileEnabled);
    fileLayout->addWidget(this->pdfGroupBox);

    QVBoxLayout *pdfFileLayout = new QVBoxLayout;
    this->pdfGroupBox->setLayout(pdfFileLayout);

    this->pdfFileChooserButton = new FileChooserButton("Report file:",
                                                       FileChooserButton::SaveFile,
                                                       this,
                                                       "Select file",
                                                       pdfFileName,
                                                       "PDF files (*.pdf);;Any files (*)");
    pdfFileLayout->addWidget(this->pdfFileChooserButton);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout, rowCount++, 0, 1, 1);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    okButton->setDefault(true);
    buttonsLayout->addWidget(okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&ReportDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&ReportDialog::accept);
}

int ReportDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        ReportDialog::htmlFileEnabled = this->htmlGroupBox->isChecked();
        ReportDialog::odfFileEnabled = this->odfGroupBox->isChecked();
        ReportDialog::pdfFileEnabled = this->pdfGroupBox->isChecked();

        ReportGenerator *reportGenerator = new ReportGenerator(this->modelID);

        if (ReportDialog::htmlFileEnabled)
        {
            reportGenerator->addExportType(ReportGenerator::HTML,this->htmlFileChooserButton->getFileName());
        }
        if (ReportDialog::odfFileEnabled)
        {
            reportGenerator->addExportType(ReportGenerator::ODF,this->odfFileChooserButton->getFileName());
        }
        if (ReportDialog::pdfFileEnabled)
        {
            reportGenerator->addExportType(ReportGenerator::PDF,this->pdfFileChooserButton->getFileName());
        }

        JobManager::getInstance().submit(reportGenerator);
    }

    return retVal;
}
