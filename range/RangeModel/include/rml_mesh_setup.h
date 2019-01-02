/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_mesh_setup.h                                         *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   12-th October 2018                                       *
 *                                                                   *
 *  DESCRIPTION: Mesh setup class declaration                        *
 *********************************************************************/

#ifndef RML_MESH_SETUP_H
#define RML_MESH_SETUP_H

#include <QSet>

#include "rml_variable.h"

class RMeshSetup
{

    protected:

        //! List of results variables for which mesh should be refined.
        QSet<RVariableType> variables;
        //! Minimum edge length.
        double minEdgeLength;
        //! Maximum edge length.
        double maxEdgeLength;

    private:

        //! Internal initialization function.
        void _init(const RMeshSetup *pMeshSetup = nullptr);

    public:

        //! Constructor.
        RMeshSetup();

        //! Copy constructor.
        RMeshSetup(const RMeshSetup &meshSetup);

        //! Destructor.
        ~RMeshSetup();

        //! Assignment operator.
        RMeshSetup &operator =(const RMeshSetup &meshSetup);

        //! Return const reference to list of results variables.
        const QSet<RVariableType> &getVariables() const;

        //! Return reference to list of results variables.
        QSet<RVariableType> &getVariables();

        //! Set list of results variables.
        void setVariables(const QSet<RVariableType> &variables);

        //! Return minimum edge length.
        double getMinEdgeLength() const;

        //! Set minimum edge length.
        void setMinEdgeLength(double minEdgeLength);

        //! Return maximum edge length.
        double getMaxEdgeLength() const;

        //! Set maximum edge length.
        void setMaxEdgeLength(double maxEdgeLength);

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif // RML_MESH_SETUP_H
