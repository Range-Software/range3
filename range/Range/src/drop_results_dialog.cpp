/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   drop_results_dialog.cpp                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   25-th Septamber 2013                                     *
 *                                                                   *
 *  DESCRIPTION: Drop results dialog class definition                *
 *********************************************************************/

#include <QGridLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QListWidget>
#include <QPushButton>

#include "drop_results_dialog.h"
#include "session.h"

static bool clearResults = true;
static bool deleteResults = false;

DropResultsDialog::DropResultsDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle(QString("Drop results"));
    this->resize(600,300);

    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    int mainLayoutRow = 0;

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout (mainLayout);

    this->clearResultsCheckBox = new QCheckBox(tr("Clear results from current model"));
    this->clearResultsCheckBox->setChecked(clearResults);
    mainLayout->addWidget(this->clearResultsCheckBox, mainLayoutRow++, 0, 1, 1);

    this->deleteResultsCheckBox = new QCheckBox(tr("Delete all records (model files) related to this model"));
    this->deleteResultsCheckBox->setChecked(deleteResults);
    mainLayout->addWidget(this->deleteResultsCheckBox, mainLayoutRow++, 0, 1, 1);

    QListWidget *resultsRecordsList = new QListWidget;
    resultsRecordsList->setEnabled(deleteResults);
    mainLayout->addWidget(resultsRecordsList, mainLayoutRow++, 0, 1, 1);

    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();

    uint nRecords = 0;
    for (int i=0;i<selectedModelIDs.size();i++)
    {
        QList<QString> recordFiles = Session::getInstance().getModel(selectedModelIDs[i]).getRecordFiles(true);

        for (int j=0;j<recordFiles.size();j++)
        {
            QListWidgetItem *item = new QListWidgetItem(resultsRecordsList);
            item->setText(recordFiles[j]);
            nRecords++;
        }
    }
    if (nRecords == 0)
    {
        this->deleteResultsCheckBox->setDisabled(true);
        resultsRecordsList->setDisabled(true);
    }

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout, mainLayoutRow++, 0, 1, 1);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    okButton->setDefault(true);
    buttonsLayout->addWidget(okButton);

    QObject::connect(this->deleteResultsCheckBox,
                     &QCheckBox::clicked,
                     resultsRecordsList,
                     &QListWidget::setEnabled);

    QObject::connect(cancelButton,
                     &QPushButton::clicked,
                     this,
                     &DropResultsDialog::reject);
    QObject::connect(okButton,
                     &QPushButton::clicked,
                     this,
                     &DropResultsDialog::accept);
}

int DropResultsDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        clearResults = this->clearResultsCheckBox->isChecked();
        deleteResults = this->deleteResultsCheckBox->isChecked();

        if (clearResults)
        {
            QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();

            for (int i=0;i<selectedModelIDs.size();i++)
            {
                Session::getInstance().storeCurentModelVersion(selectedModelIDs[i],tr("Drop results"));
                Session::getInstance().getModel(selectedModelIDs[i]).clearResults();
                Session::getInstance().setModelChanged(selectedModelIDs[i]);
            }
        }
        if (deleteResults)
        {
            QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();

            for (int i=0;i<selectedModelIDs.size();i++)
            {
                QList<QString> recordFiles = Session::getInstance().getModel(selectedModelIDs[i]).getRecordFiles(true);

                for (int j=0;j<recordFiles.size();j++)
                {
                    QFile file(recordFiles[j]);
                    RLogger::info("Removing file \'%s\'\n",recordFiles[j].toUtf8().constData());

                    if (!file.remove())
                    {
                        RLogger::warning("Failed to remove file \'%s\'\n",recordFiles[j].toUtf8().constData());
                    }
                }
            }
        }
    }
    return retVal;
}
