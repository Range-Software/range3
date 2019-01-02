/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   remove_entity_dialog.h                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   5-th December 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Remove entity dialog class declaration              *
 *********************************************************************/

#ifndef REMOVE_ENTITY_DIALOG_H
#define REMOVE_ENTITY_DIALOG_H

#include <QDialog>

#include <rmlib.h>

#include "session_entity_id.h"

class RemoveEntityDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Entity type.
        REntityGroupTypeMask entityTypeMask;

    public:

        //! Constructor.
        explicit RemoveEntityDialog(REntityGroupTypeMask entityTypeMask, QWidget *parent = nullptr);

        //! Execute dialog.
        int exec(void);

    private:

        //! Return list of entities for given model ID.
        QList<SessionEntityID> findEntities(uint mid) const;
        
};

#endif // REMOVE_ENTITY_DIALOG_H
