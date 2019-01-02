/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_problem_setup.h                                      *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   4-th July 2014                                           *
 *                                                                   *
 *  DESCRIPTION: Problem setup class declaration                     *
 *********************************************************************/

#ifndef RML_PROBLEM_SETUP_H
#define RML_PROBLEM_SETUP_H

#include "rml_mesh_setup.h"
#include "rml_modal_setup.h"
#include "rml_radiation_setup.h"

class RProblemSetup
{

    protected:

        //! Restart solver.
        bool restart;
        //! Radiation setup.
        RRadiationSetup radiationSetup;
        //! Modal setup.
        RModalSetup modalSetup;
        //! Mesh setup.
        RMeshSetup meshSetup;

    private:

        //! Internal initialization function.
        void _init(const RProblemSetup *pProblemSetup = nullptr);

    public:

        //! Constructor.
        RProblemSetup();

        //! Copy constructor.
        RProblemSetup(const RProblemSetup &problemSetup);

        //! Destructor.
        ~RProblemSetup();

        //! Assignment operator.
        RProblemSetup &operator =(const RProblemSetup &problemSetup);

        //! Return true if solver should be restarted.
        bool getRestart(void) const;

        //! Set wether the solver should be restarted.
        void setRestart(bool restart);

        //! Get const reference to radiation setup.
        const RRadiationSetup &getRadiationSetup(void) const;

        //! Get reference to radiation setup.
        RRadiationSetup &getRadiationSetup(void);

        //! Set radiation setup.
        void setRadiationSetup(const RRadiationSetup &radiationSetup);

        //! Get const reference to modal setup.
        const RModalSetup &getModalSetup(void) const;

        //! Get reference to modal setup.
        RModalSetup &getModalSetup(void);

        //! Set modal setup.
        void setModalSetup(const RModalSetup &modalSetup);

        //! Get const reference to mesh setup.
        const RMeshSetup &getMeshSetup(void) const;

        //! Get reference to mesh setup.
        RMeshSetup &getMeshSetup(void);

        //! Set mesh setup.
        void setMeshSetup(const RMeshSetup &meshSetup);

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif // RML_PROBLEM_SETUP_H
