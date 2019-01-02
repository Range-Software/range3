/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_entity_group.h                                       *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   15-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Entity group class declaration                      *
 *********************************************************************/

#ifndef RML_ENTITY_GROUP_H
#define RML_ENTITY_GROUP_H

#include "rml_entity_group_data.h"

//! Entity group types.
typedef enum _REntityGroupType
{
    R_ENTITY_GROUP_NONE           = 0,
    R_ENTITY_GROUP_POINT          = 1 << 0,
    R_ENTITY_GROUP_LINE           = 1 << 1,
    R_ENTITY_GROUP_SURFACE        = 1 << 2,
    R_ENTITY_GROUP_VOLUME         = 1 << 3,
    R_ENTITY_GROUP_VECTOR_FIELD   = 1 << 4,
    R_ENTITY_GROUP_STREAM_LINE    = 1 << 5,
    R_ENTITY_GROUP_CUT            = 1 << 6,
    R_ENTITY_GROUP_ISO            = 1 << 7,
    R_ENTITY_GROUP_SCALAR_FIELD = 1 << 8
} REntityGroupType;

#define R_ENTITY_GROUP_N_TYPES 9

#define R_ENTITY_GROUP_ALL                   \
    (                                        \
        R_ENTITY_GROUP_POINT               | \
        R_ENTITY_GROUP_LINE                | \
        R_ENTITY_GROUP_SURFACE             | \
        R_ENTITY_GROUP_VOLUME              | \
        R_ENTITY_GROUP_VECTOR_FIELD        | \
        R_ENTITY_GROUP_STREAM_LINE         | \
        R_ENTITY_GROUP_CUT                 | \
        R_ENTITY_GROUP_ISO                 | \
        R_ENTITY_GROUP_SCALAR_FIELD   \
    )

//! Macro to validate element group type.
#define R_ENTITY_GROUP_TYPE_IS_VALID(_entityGroupType)            \
    (                                                             \
        _entityGroupType == R_ENTITY_GROUP_POINT               || \
        _entityGroupType == R_ENTITY_GROUP_LINE                || \
        _entityGroupType == R_ENTITY_GROUP_SURFACE             || \
        _entityGroupType == R_ENTITY_GROUP_VOLUME              || \
        _entityGroupType == R_ENTITY_GROUP_VECTOR_FIELD        || \
        _entityGroupType == R_ENTITY_GROUP_STREAM_LINE         || \
        _entityGroupType == R_ENTITY_GROUP_CUT                 || \
        _entityGroupType == R_ENTITY_GROUP_ISO                 || \
        _entityGroupType == R_ENTITY_GROUP_SCALAR_FIELD    \
    )

//! Macro to validate element group type.
#define R_ENTITY_GROUP_TYPE_IS_ELEMENT(_entityGroupType)   \
    (                                                      \
        _entityGroupType == R_ENTITY_GROUP_POINT        || \
        _entityGroupType == R_ENTITY_GROUP_LINE         || \
        _entityGroupType == R_ENTITY_GROUP_SURFACE      || \
        _entityGroupType == R_ENTITY_GROUP_VOLUME          \
    )

#define R_ENTITY_GROUP_ELEMENT   \
    (                            \
        R_ENTITY_GROUP_POINT   | \
        R_ENTITY_GROUP_LINE    | \
        R_ENTITY_GROUP_SURFACE | \
        R_ENTITY_GROUP_VOLUME    \
    )

typedef int REntityGroupTypeMask;

class REntityGroup
{

    private:

        //! Internal initialization function.
        void _init ( const REntityGroup *pEntityGroup = nullptr );

    protected:

        //! Group name.
        QString name;
        //! Data object.
        REntityGroupData data;

    public:

        //! Constructor.
        REntityGroup();

        //! Copy constructor.
        REntityGroup(const REntityGroup &entityGroup);

        //! Destructor.
        ~REntityGroup();

        //! Assignment operator.
        REntityGroup & operator = (const REntityGroup &entityGroup);

        //! Get group name.
        const QString & getName ( void ) const;

        //! Set group name.
        void setName ( const QString &name );

        //! Return const reference to data object.
        inline const REntityGroupData & getData ( void ) const
        {
            return this->data;
        }

        //! Return reference to data object.
        inline REntityGroupData & getData ( void )
        {
            return this->data;
        }

        //! Set data object.
        inline void setData ( const REntityGroupData &data )
        {
            this->data = data;
        }

        //! Return vector of entity group types.
        static std::vector<REntityGroupType> getAllTypes ( void );

        //! Return vector of entity group types.
        static std::vector<REntityGroupType> getMaskTypes ( REntityGroupTypeMask elementGroupTypeMask );

        //! Return entity group type name.
        static const QString getTypeName ( REntityGroupType elementGroupType );

        //! Check if provided entity group type is element group type.
        static bool typeIsElementGroup ( REntityGroupType elementGroupType );

        //! Check if provided entity group type is interpolated element group type.
        static bool typeIsInterpolatedElementGroup ( REntityGroupType elementGroupType );

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif /* RML_ENTITY_GROUP_H */
