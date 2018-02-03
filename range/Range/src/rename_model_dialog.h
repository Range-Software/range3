/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rename_model_dialog.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   3-rd February 2018                                       *
 *                                                                   *
 *  DESCRIPTION: Rename model dialog class declaration               *
 *********************************************************************/

#ifndef __RENAME_MODEL_DIALOG_H__
#define __RENAME_MODEL_DIALOG_H__

#include <QDialog>
#include <QLineEdit>

class RenameModelDialog : public QDialog
{
    Q_OBJECT

    protected:

		//! Model ID.
        uint modelId;
        //! Name editor.
        QLineEdit *editName;
        //! Description editor.
        QLineEdit *editDesc;

    public:

        //! Constructor.
        explicit RenameModelDialog(uint modelId, QWidget *parent = 0);

        //! Execute dialog.
        int exec(void);

    private:

        //! Return model name.
        QString getName(void) const;

        //! Return model description.
        QString getDescription(void) const;
        
};

#endif /* __RENAME_MODEL_DIALOG_H__ */
