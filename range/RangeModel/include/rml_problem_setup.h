/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_problem_setup.h                                      *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   4-th July 2014                                           *
 *                                                                   *
 *  DESCRIPTION: Problem setup class declaration                     *
 *********************************************************************/

#ifndef __RML_PROBLEM_SETUP_H__
#define __RML_PROBLEM_SETUP_H__

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

    private:

        //! Internal initialization function.
        void _init(const RProblemSetup *pProblemSetup = 0);

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

        //! Get const reference to radiation setup.
        const RModalSetup &getModalSetup(void) const;

        //! Get reference to radiation setup.
        RModalSetup &getModalSetup(void);

        //! Set radiation setup.
        void setModalSetup(const RModalSetup &modalSetup);

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif // __RML_PROBLEM_SETUP_H__
