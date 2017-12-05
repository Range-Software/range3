/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   application_settings_dialog.cpp                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   12-th July 2013                                          *
 *                                                                   *
 *  DESCRIPTION: Application settings dialog class definition        *
 *********************************************************************/

#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QFileDialog>
#include <QSpinBox>
#include <QLabel>
#include <QTabWidget>
#include <QTreeWidget>
#include <QMessageBox>

#include "application_settings_dialog.h"
#include "keyboard_shortcuts_edit.h"
#include "main_settings.h"

ApplicationSettingsDialog::ApplicationSettingsDialog(ApplicationSettings *applicationSettings, QWidget *parent) :
    QDialog(parent),
    applicationSettings(applicationSettings)
{
    this->setWindowTitle(tr("Application settings"));
    this->resize(500,500);

    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout(mainLayout);

    QTabWidget *tabWidget = new QTabWidget;
    mainLayout->addWidget(tabWidget, 0, 0, 1, 1);

    QWidget *generalWidget = this->createGeneralTab();
    tabWidget->addTab(generalWidget,tr("General"));

    QWidget *shortcutsWidget = this->createShortcutsTab();
    tabWidget->addTab(shortcutsWidget,tr("Keyboard shortcuts"));

    QWidget *rangeAccountWidget = this->createRangeAccountTab();
    tabWidget->addTab(rangeAccountWidget,tr("Range account"));

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout, 1, 0, 1, 1);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    okButton->setDefault(true);
    buttonsLayout->addWidget(okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&QDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&QDialog::accept);
}

int ApplicationSettingsDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        this->applicationSettings->setSolverPath(this->solverFileChooserButton->getFileName());
        this->applicationSettings->setNThreads(this->nThreadsSpin->value());
        this->applicationSettings->setNHistoryRecords(this->nHistoryRecordsSpin->value());
        this->applicationSettings->setStyle(this->styleCombo->currentText());
        this->applicationSettings->setSendUsageInfo(this->sendUsageInfoAllowed->isChecked());
        this->applicationSettings->setRangeApiAllowed(this->rangeApiAllowed->isChecked());
        this->applicationSettings->setRangeApiServer(this->rangeApiServer->text());
        this->applicationSettings->setRangeAccount(this->rangeAccountEdit->text());
        this->applicationSettings->setRangePassword(this->rangePasswordEdit->text());

        for (QMap<ActionType,QString>::iterator iter = this->changedShortcut.begin(); iter != this->changedShortcut.end(); ++iter)
        {
            this->applicationSettings->getActionDefinition()->setShortcut(iter.key(),iter.value());
        }
    }
    return retVal;
}

QWidget *ApplicationSettingsDialog::createGeneralTab(void)
{
    QWidget *widget = new QWidget;

    QGridLayout *layout = new QGridLayout;
    widget->setLayout(layout);

    this->solverFileChooserButton = new FileChooserButton("Solver executable:",
                                                          false,
                                                          this,
                                                          "Select Range solver executable",
                                                          this->applicationSettings->getSolverPath(),
                                                          "Any files (*)");
    layout->addWidget(this->solverFileChooserButton,0,0,1,2);

    QLabel *ncpusLabel = new QLabel(tr("Number of threads:"));
    layout->addWidget(ncpusLabel,1,0,1,1);

    this->nThreadsSpin = new QSpinBox;
    this->nThreadsSpin->setRange(1,ApplicationSettings::getMaxThreads());
    this->nThreadsSpin->setValue(this->applicationSettings->getNThreads());
    this->nThreadsSpin->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    layout->addWidget(this->nThreadsSpin,1,1,1,1);

    QLabel *nHistoryRecordsLabel = new QLabel(tr("Number of history (undo) records:"));
    layout->addWidget(nHistoryRecordsLabel,2,0,1,1);

    this->nHistoryRecordsSpin = new QSpinBox;
    this->nHistoryRecordsSpin->setRange(1,INT_MAX);
    this->nHistoryRecordsSpin->setValue(this->applicationSettings->getNHistoryRecords());
    this->nHistoryRecordsSpin->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    layout->addWidget(this->nHistoryRecordsSpin,2,1,1,1);

    QLabel *styleLabel = new QLabel(tr("Style:"));
    layout->addWidget(styleLabel,3,0,1,1);

    QStringList styles(ApplicationSettings::getStyles());
    this->styleCombo = new QComboBox;
    this->styleCombo->addItems(styles);
    for (int i=0;i<styles.size();i++)
    {
        if (styles.at(i) == this->applicationSettings->getStyle())
        {
            this->styleCombo->setCurrentIndex(i);
        }
    }
    layout->addWidget(this->styleCombo,3,1,1,1);

    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(spacer,4,0,1,2);

    return widget;
}

QWidget *ApplicationSettingsDialog::createShortcutsTab(void)
{
    KeyboardShortcutsEdit *widget = new KeyboardShortcutsEdit(this->applicationSettings->getActionDefinition());

    QObject::connect(widget,
                     &KeyboardShortcutsEdit::shortcutChanged,
                     this,
                     &ApplicationSettingsDialog::onKeyboardShortcutChanged);

    return widget;
}

QWidget *ApplicationSettingsDialog::createRangeAccountTab(void)
{
    QWidget *widget = new QWidget;

    QGridLayout *layout = new QGridLayout;
    widget->setLayout(layout);

    this->rangeApiAllowed = new QGroupBox(tr("Allow connection to Range API server"));
    this->rangeApiAllowed->setCheckable(true);
    this->rangeApiAllowed->setChecked(this->applicationSettings->getRangeApiAllowed());
    layout->addWidget(this->rangeApiAllowed,0,0,1,1);

    QGridLayout *rraLayout = new QGridLayout;
    this->rangeApiAllowed->setLayout(rraLayout);

    uint rowCount = 0;

    this->sendUsageInfoAllowed = new QCheckBox(tr("Send usage info"));
    this->sendUsageInfoAllowed->setCheckState(this->applicationSettings->getSendUsageInfo()?Qt::Checked:Qt::Unchecked);
    rraLayout->addWidget(this->sendUsageInfoAllowed,rowCount,0,1,2);

    QLabel *rangeApiServerLabel = new QLabel(tr("Range API server:"));
    rangeApiServerLabel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Minimum);
    rraLayout->addWidget(rangeApiServerLabel,++rowCount,0,1,1);

    this->rangeApiServer = new QLineEdit(this->applicationSettings->getRangeApiServer());
    this->rangeApiServer->setPlaceholderText(tr("http://"));
    this->rangeApiServer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    rraLayout->addWidget(this->rangeApiServer,rowCount,1,1,1);

    QLabel *rangeAccountLabel = new QLabel(tr("Username:"));
    rangeAccountLabel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Minimum);
    rraLayout->addWidget(rangeAccountLabel,++rowCount,0,1,1);

    this->rangeAccountEdit = new QLineEdit(this->applicationSettings->getRangeAccount());
    this->rangeAccountEdit->setPlaceholderText(tr("username"));
    this->rangeAccountEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    rraLayout->addWidget(this->rangeAccountEdit,rowCount,1,1,1);

    QLabel *rangePasswordLabel = new QLabel(tr("Password:"));
    rangePasswordLabel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Minimum);
    rraLayout->addWidget(rangePasswordLabel,++rowCount,0,1,1);

    this->rangePasswordEdit = new QLineEdit(this->applicationSettings->getRangePassword());
    this->rangePasswordEdit->setPlaceholderText(tr("password"));
    this->rangePasswordEdit->setEchoMode(QLineEdit::Password);
    this->rangePasswordEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    rraLayout->addWidget(this->rangePasswordEdit,rowCount,1,1,1);

    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(spacer,++rowCount,0,1,2);

    return widget;
}

void ApplicationSettingsDialog::onKeyboardShortcutChanged(ActionType actionType, const QString &shortcut)
{
    this->changedShortcut.insert(actionType,shortcut);
}
