/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   application_settings_dialog.h                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   12-th July 2013                                          *
 *                                                                   *
 *  DESCRIPTION: Application settings dialog class declaration       *
 *********************************************************************/

#ifndef __APPLICATION_SETTINGS_DIALOG_H__
#define __APPLICATION_SETTINGS_DIALOG_H__

#include <QDialog>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>

#include "application_settings.h"
#include "file_chooser_button.h"

class ApplicationSettingsDialog : public QDialog
{

    Q_OBJECT

    protected:

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
        //! Send usage info allowed.
        QCheckBox *sendUsageInfoAllowed;
        //! Range API allowed checkbox.
        QGroupBox *rangeApiAllowed;
        //! Range API server edit.
        QLineEdit *rangeApiServer;
        //! Range account edit.
        QLineEdit *rangeAccountEdit;
        //! Range password edit.
        QLineEdit *rangePasswordEdit;

    public:

        //! Constructor.
        explicit ApplicationSettingsDialog(ApplicationSettings *applicationSettings, QWidget *parent = 0);

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

        //! Keyboard shortcut has changed.
        void onKeyboardShortcutChanged(ActionType actionType, const QString &shortcut);
        
};

#endif // __APPLICATION_SETTINGS_DIALOG_H__
