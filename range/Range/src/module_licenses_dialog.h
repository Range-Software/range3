/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   module_licenses_dialog.h                                 *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   16-th November 2016                                      *
 *                                                                   *
 *  DESCRIPTION: Module licenses dialog class declaration            *
 *********************************************************************/

#ifndef __MODULE_LICENSES_DIALOG_H__
#define __MODULE_LICENSES_DIALOG_H__

#include <QDialog>
#include <QString>

class ModuleLicensesDialog : public QDialog
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit ModuleLicensesDialog(const QString &moduleLicenseFileName, QWidget *parent = 0);

        //! Execute dialog.
        int exec(void);

};

#endif // __MODULE_LICENSES_DIALOG_H__
