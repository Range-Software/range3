/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   application_settings_dialog.h                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   12-th July 2013                                          *
 *                                                                   *
 *  DESCRIPTION: Application settings dialog class declaration       *
 *********************************************************************/

#ifndef APPLICATION_SETTINGS_DIALOG_H
#define APPLICATION_SETTINGS_DIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>

#include "application_settings.h"
#include "file_chooser_button.h"
#include "keyboard_shortcuts_edit.h"

class ApplicationSettingsDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! OK button.
        QPushButton *okButton;

        //! Application settings.
        ApplicationSettings *applicationSettings;
        //! Solver path file chooser.
        FileChooserButton *solverFileChooserButton;
        //! Help directory file chooser.
        FileChooserButton *helpFileChooserButton;
        //! Number of threads spin button.
        QSpinBox *nThreadsSpin;
        //! Number of history records spin button.
        QSpinBox *nHistoryRecordsSpin;
        //! Map of changed keyboard shortcuts.
        QMap<ActionType,QString> changedShortcut;
        //! Style combo box.
        QComboBox *styleCombo;
        //! Toolbar icon size spin button.
        QSpinBox *toolbarIconSizeSpin;
        //! Keyboard shortcuts widget.
        KeyboardShortcutsEdit *keyboardShortcutsEdit;
        //! Send usage info allowed.
        QCheckBox *sendUsageInfoAllowed;
        //! Range API allowed checkbox.
        QGroupBox *rangeApiAllowed;
        //! Range API server edit.
        QLineEdit *rangeApiServer;

    public:

        //! Constructor.
        explicit ApplicationSettingsDialog(ApplicationSettings *applicationSettings, QWidget *parent = nullptr);

        //! Execute dialog.
        int exec(void);

    private:

        //! Create tab containing general settings.
        QWidget *createGeneralTab(void);

        //! Create tab short-cut settings.
        QWidget *createShortcutsTab(void);

        //! Create tab containing Range account settings.
        QWidget *createRangeAccountTab(void);

    protected slots:

        //! Range solver path changed.
        void onSolverPathChanged(const QString &);

        //! Help directory changed.
        void onHelpDirChanged(const QString &);

        //! Number of threads changed.
        void onNThreadsChanged(int);

        //! Number of history records changed.
        void onNHistoryRecordsChanged(int);

        //! Style changed.
        void onStyleChanged(int);

        //! Toolbar icon size changed.
        void onToolbarIconSizeChanged(int);

        //! Keyboard shortcut has changed.
        void onKeyboardShortcutChanged(ActionType actionType, const QString &shortcut);

        //! On Range API allowed toggled.
        void onRangeApiAllowedToggled(bool);

        //! On Send usage information allowd toggled.
        void onSendUsageInfoAllowedToggled(bool);

        //! Range API server changed.
        void onRangeApiServerChanged(const QString &);

        //! Default button was clicked.
        void onDefaultClicked(void);
        
};

#endif // APPLICATION_SETTINGS_DIALOG_H
