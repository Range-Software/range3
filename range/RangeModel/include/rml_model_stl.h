/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_model_stl.h                                          *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   10-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: STL model class declaration                         *
 *********************************************************************/

#ifndef RML_MODEL_STL_H
#define RML_MODEL_STL_H

#include <QString>

#include "rml_model_raw.h"

class RModelStl : public RModelRaw
{

    protected:

        //! Model name.
        QString name;

        //! Model description.
        QString description;

    private:

        //! Disable write fonction.
        void write ( const QString &fileName ) const;

        //! Internal initialization function.
        void _init ( const RModelStl *pModelStl = nullptr );

        //! Check if file contains binary information.
        bool checkIfBinary ( const QString &fileName ) const;

        //! Read surface mesh from binary file.
        void readBinary ( const QString &fileName,
                          double         tolerance = 0.0 );

        //! Write surface mesh to binary file.
        void writeBinary ( const QString &fileName ) const;

        //! Read surface mesh from ASCII file.
        void readAscii ( const QString &fileName,
                         double         tolerance = 0.0 );

        //! Write surface mesh to ASCII file.
        void writeAscii ( const QString &fileName ) const;

    public:

        //! Constructor.
        RModelStl ();

        //! Copy constructor.
        RModelStl ( const RModelStl &modelStl );

        //! Destructor.
        ~RModelStl();

        //! Assignment operator.
        RModelStl & operator = ( const RModelStl &modelStl );

        //! Get model name.
        QString getName ( void ) const;

        //! Set model name.
        void setName ( const QString &name );

        //! Get model description.
        QString getDescription ( void ) const;

        //! Set model description.
        void setDescription ( const QString &name );

        //! Remove all nodes and elements from the model.
        void clear (void);

        //! Read surface mesh from file.
        void read ( const QString &fileName,
                    double         tolerance = 0.0 );

        //! Write surface mesh to file.
        void write ( const QString &fileName,
                     bool           binary = false ) const;

};

#endif /* RML_MODEL_STL_H */
