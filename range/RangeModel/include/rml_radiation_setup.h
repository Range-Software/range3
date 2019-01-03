/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_radiation_setup.h                                    *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   4-th July 2014                                           *
 *                                                                   *
 *  DESCRIPTION: Radiation setup class declaration                   *
 *********************************************************************/

#ifndef RML_RADIATION_SETUP_H
#define RML_RADIATION_SETUP_H

#include <QString>

typedef enum RRadiationResolution
{
    R_RADIATION_RESOLUTION_LOW    = 100,
    R_RADIATION_RESOLUTION_MEDIUM = 200,
    R_RADIATION_RESOLUTION_HIGH   = 300
} RRadiationResolution;

class RRadiationSetup
{

    protected:

        //! Radiation view factor resolution.
        RRadiationResolution resolution;
        //! View-factor matrix file.
        QString viewFactorMatrixFile;

    private:

        //! Internal initialization function.
        void _init(const RRadiationSetup *pRadiationSetup = nullptr);

    public:

        //! Constructor.
        RRadiationSetup();

        //! Copy constructor.
        RRadiationSetup(const RRadiationSetup &radiationSetup);

        //! Destructor.
        ~RRadiationSetup();

        //! Assignment operator.
        RRadiationSetup &operator =(const RRadiationSetup &radiationSetup);

        //! Return resolution.
        RRadiationResolution getResolution(void) const;

        //! Set resolution.
        void setResolution(const RRadiationResolution &value);

        //! Return view-factor matrix file.
        const QString &getViewFactorMatrixFile(void) const;

        //! Set view-factor matrix file.
        void setViewFactorMatrixFile(const QString &viewFactorMatrixFile);

        //! Find most recent view factor matrix file.
        static QString findRecentViewFactorMatrixFile(const QString &viewFactorMatrixFile, uint timeStep = 0);

        //! Get radiation resolution text representation.
        static QString getResolutionText(RRadiationResolution resolution);

        //! Allow RFileIO to access private members.
        friend class RFileIO;
};

#endif // RML_RADIATION_SETUP_H
