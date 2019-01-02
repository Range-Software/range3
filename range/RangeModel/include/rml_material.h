/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_material.h                                           *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   16-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Material class declaration                          *
 *********************************************************************/

#ifndef RML_MATERIAL_H
#define RML_MATERIAL_H

#include <string>
#include <vector>

#include <rblib.h>

#include "rml_material_property.h"

typedef enum _RMaterialState
{
    R_MATERIAL_STATE_NONE = 0,
    R_MATERIAL_STATE_GAS,
    R_MATERIAL_STATE_LIQUID,
    R_MATERIAL_STATE_SOLID,
    R_MATERIAL_STATE_N_TYPES
} RMaterialState;

//! Material class
class RMaterial
{

    private:

        //! Internal initialization function.
        void _init ( const RMaterial *pMaterial = nullptr );

    protected:

        //! Material ID.
        unsigned int id;
        //! Material name.
        QString name;
        //! Material state.
        RMaterialState state;
        //! Material properties.
        std::vector<RMaterialProperty> properties;

    public:

        //! Constructor.
        RMaterial (RMaterialState state = R_MATERIAL_STATE_NONE);

        //! Copy constructor.
        RMaterial ( const RMaterial &material );

        //! Destructor.
        ~RMaterial ();

        //! Assignment operator.
        RMaterial & operator = ( const RMaterial &material );

        //! Check whether the material properties are same.
        bool propertiesEqual( const RMaterial &material ) const;

        //! Return material ID.
        unsigned int getID ( void ) const;

        //! Set material ID.
        void setID ( unsigned int id );

        //! Return material name.
        const QString & getName ( void ) const;

        //! Set material name.
        void setName ( const QString &name );

        //! Return material state.
        RMaterialState getState ( void ) const;

        //! Set material state.
        void setState ( RMaterialState state );

        //! Return number of properties.
        unsigned int size ( void ) const;

        //! Add new material property.
        //! If a property with such type already exist an old
        //! property will be replaced.
        //! Return true if new property was added false on replace.
        bool add ( const RMaterialProperty &property );

        //! Return const reference to material property at given position.
        const RMaterialProperty & get ( unsigned int position ) const;

        //! Return reference to material property at given position.
        RMaterialProperty & get ( unsigned int position );

        //! Remove material property at given position.
        void remove ( unsigned int position );

        //! Clear all data.
        void clear ( void );

        //! Find material property position by its type.
        //! If no such property is present in material number of properties is returned.
        unsigned int findPosition
                                 ( RMaterialPropertyType type ) const;

        //! Check if material contains given list of properties.
        bool hasProperties ( const std::vector<RMaterialPropertyType> &propertyTypes ) const;

        //! Generate material with all material properties and their default values.
        static RMaterial generateDefault ( void );

        //! Allow RFileIO to access private members.
        friend class RFileIO;

        //! Read material from the file.
        void read ( const QString &fileName );

        //! Write material to the file.
        void write ( const QString &fileName ) const;

        //! Import legacy material.
        void import ( const QString &fileName );

        //! Find material file name.
        QString findMaterialFileName( bool binary = true ) const;

        //! Validate material's properties against problem type.
        bool validForProblemType( RProblemTypeMask problemTypeMask ) const;

        //! Return default file extension.
        static QString getDefaultFileExtension ( bool binary = true );

    protected:

        //! Read from the ASCII file.
        void readAscii ( const QString &fileName );

        //! Read from the binary file.
        void readBinary ( const QString &fileName );

        //! Write to the ASCII file.
        void writeAscii ( const QString &fileName ) const;

        //! Write to the binary file.
        void writeBinary ( const QString &fileName ) const;

};

#endif /* RML_MATERIAL_H */
