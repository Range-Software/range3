/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   mark_entity_dialog.h                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   8-th November 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Mark entity dialog class declaration                *
 *********************************************************************/

#ifndef __MARK_ENTITY_DIALOG_H__
#define __MARK_ENTITY_DIALOG_H__

#include <QDialog>
#include <QRadioButton>
#include <QSpinBox>

#include <rmlib.h>

#include "pick_item.h"

class MarkEntityDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Separation angle.
        static double separationAngle;
        //! Entity type.
        REntityGroupType entityType;
        //! Auto-mark radio button.
        QRadioButton *autoMarkRadio;
        //! Mark picked elements radio button.
        QRadioButton *pickedMarkRadio;
        //! Separation angle spin box.
        QSpinBox *separationAngleSpin;

    public:

        //! Constructor.
        explicit MarkEntityDialog(REntityGroupType entityType, QWidget *parent = 0);

        //! Execute dialog.
        int exec(void);

    private:

        //! Automark all entities.
        bool autoMarkEntities(uint modelID);
        
        //! Mark picked entities.
        bool markEntities(uint modelID, const QVector<PickItem> &pickList);

};

#endif // __MARK_ENTITY_DIALOG_H__
