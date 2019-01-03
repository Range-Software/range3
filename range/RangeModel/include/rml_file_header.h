/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_file_header.h                                        *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   29-th August 2012                                        *
 *                                                                   *
 *  DESCRIPTION: File header class declarations                      *
 *********************************************************************/

#ifndef RML_FILE_HEADER_H
#define RML_FILE_HEADER_H

#include <rblib.h>

typedef enum _RFileType
{
    R_FILE_TYPE_NONE = 0,
    R_FILE_TYPE_MODEL,
    R_FILE_TYPE_MATERIAL,
    R_FILE_TYPE_VIEW_FACTOR_MATRIX,
    R_FILE_TYPE_DISPLAY_PROPERTIES,
    R_FILE_TYPE_LINK,
    R_FILE_N_TYPES
} RFileType;

//! File header class.
class RFileHeader
{

    private:

        //! Internal initialization function.
        void _init ( const RFileHeader *pHeader = nullptr );

    protected:

        //! File version.
        RVersion version;
        //! File type.
        RFileType type;
        //! File information.
        //! If file is a link file this field will contain path to the file.
        QString information;

    public:

        //! Constructor.
        RFileHeader( RFileType type = R_FILE_TYPE_NONE,
                     RVersion version = RVersion(),
                     const QString &information = QString());

        //! Copy constructor.
        RFileHeader ( const RFileHeader &header );

        //! Destructor.
        ~RFileHeader ();

        //! Assignment operator.
        RFileHeader & operator = ( const RFileHeader &header );

        //! Return version.
        const RVersion & getVersion ( void ) const;

        //! Set version.
        void setVersion ( const RVersion &version );

        //! Return file type.
        RFileType getType ( void ) const;

        //! Set file type.
        void setType ( RFileType type );

        //! Return file type.
        const QString & getInformation ( void ) const;

        //! Set file type.
        void setInformation ( const QString &information );

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif /* RML_FILE_HEADER_H */
