/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   material_list.h                                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   9-th August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: Material list (database) declaration                *
 *********************************************************************/

#ifndef MATERIAL_LIST_H
#define MATERIAL_LIST_H

#include <QObject>

#include <rmlib.h>

class MaterialList : public QObject, public RMaterialList
{
    Q_OBJECT

    private:

        //! Constructor.
        MaterialList();

        //! Selected material name.
        QString selectedMaterialName;

    public:

        //! Initialize material list.
        static void initialize(void);

        //! Return reference to static instance.
        static MaterialList & getInstance(void);

        //! Add new material.
        //! If material with given name already exist it will be updated.
        void addMaterial(RMaterial &material, bool isNew);

        //! Add material.
        //! If material with given name already exist it will be updated.
        void add(RMaterial &material);

        //! Delete material with given name.
        //! Returns number of removed materials.
        unsigned int remove(const QString &materialName,
                            unsigned int      *id = nullptr);

        //! Rename material with given name to new name.
        bool rename(const QString &materialOldName,
                    const QString &materialNewName,
                    bool override = true);

        //! Return material with given name.
        //! If such material does not exist it will be created.
        RMaterial get(const QString &materialName);

        //! Set material selected.
        void selectMaterial(const QString &materialName);

        //! Return const reference to selected material name.
        const QString &getSelectedMaterialName(void) const;

        //! Force to emit materialChanged signal.
        void signalMaterialChanged(const QString &materialName);

    protected:

        //! Read (reinitialize) material list from store.
        void readStore(void);

    signals:

        //! Material has been added to the list.
        void materialAdded(const QString &materialName, bool isNew);

        //! Material has been removed from the list.
        void materialRemoved(const QString &materialName, uint id);

        //! Material has been removed from the list.
        void materialRenamed(const QString &materialOldName,
                             const QString &materialNewName);

        //! Material has been changed.
        void materialChanged(const QString &materialName);

        //! Material has been selected.
        void materialSelected(const QString &materialName);
    
};

#endif /* MATERIAL_LIST_H */
