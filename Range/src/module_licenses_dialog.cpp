/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   module_licenses_dialog.cpp                               *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   16-th November 2016                                      *
 *                                                                   *
 *  DESCRIPTION: Module licenses dialog class definition             *
 *********************************************************************/

#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QTextBrowser>
#include <QSplitter>

#include <ralib.h>
#include <rblib.h>
#include <rmlib.h>

#include "module_licenses_dialog.h"
#include "module_licenses_tree.h"

ModuleLicensesDialog::ModuleLicensesDialog(const QString &moduleLicenseFileName, QWidget *parent)
    : QDialog(parent)
{
    RLicense license;
    try
    {
        license.read(moduleLicenseFileName);
    }
    catch (const RError &rError)
    {
        RLogger::warning("Failed to read module license file \'%s\'. %s\n",
                         moduleLicenseFileName.toUtf8().constData(),
                         rError.getMessage().toUtf8().constData());
    }

    QIcon closeIcon(":/icons/file/pixmaps/range-close.svg");

    this->setWindowTitle(tr("Module licenses"));
    this->resize(700,600);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    QSplitter *splitter = new QSplitter;
    splitter->setOrientation(Qt::Vertical);
    mainLayout->addWidget(splitter);

    QGroupBox *modulesBox = new QGroupBox(tr("Modules"));
    splitter->addWidget(modulesBox);

    QVBoxLayout *modulesLayout = new QVBoxLayout;
    modulesBox->setLayout(modulesLayout);

    ModuleLicensesTree *moduleLicensesTree = new ModuleLicensesTree(license);
    modulesLayout->addWidget(moduleLicensesTree);

    QGroupBox *rawLicenseBox = new QGroupBox(tr("License text"));
    splitter->addWidget(rawLicenseBox);

    QVBoxLayout *rawLicenseLayout = new QVBoxLayout;
    rawLicenseBox->setLayout(rawLicenseLayout);

    QTextBrowser *rawLicenseBrowser = new QTextBrowser;
    rawLicenseBrowser->setLineWrapMode(QTextEdit::NoWrap);
    rawLicenseBrowser->setText(license.toString());
    rawLicenseLayout->addWidget(rawLicenseBrowser);

    splitter->setStretchFactor(0,1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonsLayout);

    buttonsLayout->addStretch(1);

    QPushButton *closeButton = new QPushButton(closeIcon,tr("Close"));
    closeButton->setDefault(true);
    buttonsLayout->addWidget(closeButton);

    QObject::connect(closeButton,&QPushButton::clicked,this,&ModuleLicensesDialog::reject);
}

int ModuleLicensesDialog::exec(void)
{
    int retVal = QDialog::exec();

    return retVal;
}
