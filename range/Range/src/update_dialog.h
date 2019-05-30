/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   update_dialog.h                                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   22-nd August 2016                                        *
 *                                                                   *
 *  DESCRIPTION: Update dialog class declaration                     *
 *********************************************************************/

#ifndef UPDATE_DIALOG_H
#define UPDATE_DIALOG_H

#include <QDialog>

#include <rblib.h>

class UpdateDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Software version.
        RVersion softwareVersion;
        //! Software link.
        QString softwareLink;

    public:

        //! Constructor.
        explicit UpdateDialog(const RVersion &softwareVersion, const QString &softwareLink, QWidget *parent = nullptr);

        //! Execute dialog.
        int exec(void) override;

};

#endif // UPDATE_DIALOG_H
