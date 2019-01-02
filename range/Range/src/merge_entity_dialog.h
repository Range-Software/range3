/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   merge_entity_dialog.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   30-th November 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Merge entity dialog class declaration               *
 *********************************************************************/

#ifndef MERGE_ENTITY_DIALOG_H
#define MERGE_ENTITY_DIALOG_H

#include <QDialog>

#include <rmlib.h>

#include "session_entity_id.h"

class MergeEntityDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Entity type.
        REntityGroupTypeMask entityTypeMask;

    public:

        //! Constructor.
        explicit MergeEntityDialog(REntityGroupTypeMask entityTypeMask, QWidget *parent = nullptr);

        //! Execute dialog.
        int exec(void);

        //! Check if there are some entities selected.
        static bool entitiesSelected(REntityGroupTypeMask entityTypeMask = R_ENTITY_GROUP_ELEMENT);

    private:

        //! Return list of entities for given model ID.
        static QList<SessionEntityID> findEntities(uint mid, REntityGroupTypeMask entityTypeMask = R_ENTITY_GROUP_ELEMENT);
        
};

#endif // MERGE_ENTITY_DIALOG_H
