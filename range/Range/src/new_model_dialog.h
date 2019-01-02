/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   new_model_dialog.h                                       *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   31-st August 2012                                        *
 *                                                                   *
 *  DESCRIPTION: New model dialog class declaration                  *
 *********************************************************************/

#ifndef NEW_MODEL_DIALOG_H
#define NEW_MODEL_DIALOG_H

#include <QDialog>
#include <QLineEdit>

class NewModelDialog : public QDialog
{
    Q_OBJECT

    protected:

        //! Name editor.
        QLineEdit *editName;
        //! Description editor.
        QLineEdit *editDesc;

    public:

        //! Constructor.
        explicit NewModelDialog(QWidget *parent = nullptr);

        //! Execute dialog.
        int exec(void);

    private:

        //! Return model name.
        QString getName(void) const;

        //! Return model description.
        QString getDescription(void) const;
        
};

#endif /* NEW_MODEL_DIALOG_H */
