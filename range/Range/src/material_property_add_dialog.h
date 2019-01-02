/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   material_property_add_dialog.h                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   30-th August 2012                                        *
 *                                                                   *
 *  DESCRIPTION: Material property add dialog class declaration      *
 *********************************************************************/

#ifndef MATERIAL_PROPERTY_ADD_DIALOG_H
#define MATERIAL_PROPERTY_ADD_DIALOG_H

#include <QDialog>
#include <QListWidget>

#include <rmlib.h>

class MaterialPropertyAddDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Pointer to material object.
        RMaterial *pMaterial;
        //! List of available material properties.
        QListWidget *propertyList;

    public:

        //! Constructor.
        explicit MaterialPropertyAddDialog(RMaterial &material,
                                           QWidget   *parent = nullptr);

        //! Overloaded exec function.
        int exec(void);

    protected:

        //! Find available properties.
        QList<RMaterialPropertyType> findAvailableProperties(const RMaterial &material) const;
        
};

#endif /* MATERIAL_PROPERTY_ADD_DIALOG_H */
