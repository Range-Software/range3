/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_model_msh.h                                          *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   21-st January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: MSH (old range) model class declaration             *
 *********************************************************************/

#ifndef RML_MODEL_MSH_H
#define RML_MODEL_MSH_H

#include <string>
#include <vector>

#include <rblib.h>

//! MSH model class.
class RModelMsh
{

    private:

        //! Internal initialization function.
        void _init ( const RModelMsh *pModelMsh = nullptr );

        //! Read mesh from the ASCII file.
        void readAscii ( const QString &fileName );

        //! Read mesh from the binary file.
        void readBinary ( const QString &fileName );

        //! Write mesh to the ASCII file.
        void writeAscii ( const QString &fileName ) const;

        //! Write mesh to the binary file.
        void writeBinary ( const QString &fileName ) const;

    public:

        QString title;
        QString comment;

        RRMatrix nodes;
        RIMatrix bodiesAll, bodies, bodiesNeighbors;
        RIMatrix facesAll, faces, facesNeighbors;
        RIMatrix linesAll, lines;
        RIMatrix pointsAll, points;
        RRVector facesThickness;
        RRVector linesCarea;
        RRVector pointsVolume;
        std::vector<QString> bodiesNames;
        std::vector<QString> facesNames;
        std::vector<QString> linesNames;
        std::vector<QString> pointsNames;
        bool bodiesNeighborsComputed;
        bool facesNeighborsComputed;

    public:

        //! Constructor.
        RModelMsh ();

        //! Copy constructor.
        RModelMsh ( const RModelMsh &modelMsh );

        //! Destructor.
        ~RModelMsh ();

        //! Assignment operator.
        RModelMsh & operator = ( const RModelMsh &modelMsh );

        //! Clear all mesh data.
        void clear (void);

        //! Read mesh from the file.
        void read ( const QString &fileName );

        //! Write mesh to the file.
        void write ( const QString &fileName ) const;
};

#endif /* RML_MODEL_MSH_H */
