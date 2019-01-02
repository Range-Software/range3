/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_material_list.h                                      *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   8-th August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: Material list class declaration                     *
 *********************************************************************/

#ifndef RMATERIAL_LIST_H
#define RMATERIAL_LIST_H

#include <QString>

#include <string>
#include <map>
#include <vector>

#include "rml_material.h"

class RMaterialList
{

    protected:

        //! List of materials (name/file name).
        std::map<QString,QString> materials;
        //! Path to material list store directory.
        QString storePath;

    private:

        //! Internal initialization function.
        void _init ( const RMaterialList *pMaterialList = nullptr );

    public:

        //! Constructor.
        RMaterialList ();

        //! Copy constructor.
        RMaterialList ( const RMaterialList &materialList );

        //! Destructor.
        ~RMaterialList ();

        //! Assignment operator.
        RMaterialList & operator = ( const RMaterialList &materialList );

        //! Return size of the list.
        unsigned int size (void) const;

        //! Return material list store path.
        const QString & getStorePath ( void ) const;

        //! Return material list store path.
        //! Store path must contain path with directory delimiter at the end.
        void setStorePath ( const QString &storePath );

        //! Add material.
        //! If material with given name already exist it will be updated.
        virtual void add ( RMaterial &material );

        //! Delete material with given name.
        //! Returns number of removed materials.
        virtual unsigned int remove ( const QString &materialName,
                                      unsigned int  *id = nullptr);

        //! Rename material with given name to new name.
        virtual bool rename ( const QString &materialOldName,
                              const QString &materialNewName,
                              bool override = true );

        //! Return material with given name.
        //! If such material does not exist it will be created.
        RMaterial get ( const QString &materialName );

        //! Pass back material with given name.
        //! If such material does not exist false will be returned.
        bool find ( const QString &materialName, RMaterial &material ) const;

        //! Return material filename.
        QString getFileName ( const QString &materialName ) const;

        //! Return true if list already contain given material name.
        bool containName ( const QString &materialName ) const;

        //! Return list of material names.
        std::vector<QString> getMaterialNames ( const std::vector<RMaterialPropertyType> &propertyTypes = std::vector<RMaterialPropertyType>() )const;

        //! Return list of materials.
        std::vector<RMaterial> getMaterials ( const std::vector<RMaterialPropertyType> &propertyTypes = std::vector<RMaterialPropertyType>() )const;

        //! Find material with given ID and return its name.
        QString findID ( unsigned int id ) const;

        //! Check if ID is valid (vailable).
        bool validateID ( unsigned int id ) const;

        //! Find available ID.
        unsigned int findAvailableID ( void ) const;

    protected:

        //! Read (reinitialize) material list from store.
        virtual void readStore ( void );

};

#endif /* RMATERIAL_LIST_H */
