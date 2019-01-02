/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rconvection.h                                            *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   21-st January 2013                                       *
 *                                                                   *
 *  DESCRIPTION: Convection coefficient calculator class declaration *
 *********************************************************************/

#ifndef RCONVECTION_H
#define RCONVECTION_H

#include <rmlib.h>

typedef enum _RConvectionType {
    R_CONVECTION_NATURAL_EXTERNAL_VERTICAL_PLANE = 0,
    R_CONVECTION_NATURAL_EXTERNAL_VERTICAL_CYLINDER,
    R_CONVECTION_NATURAL_EXTERNAL_HORIZONTAL_PLATES,
    R_CONVECTION_NATURAL_EXTERNAL_HORIZONTAL_CYLINDER,
    R_CONVECTION_NATURAL_EXTERNAL_SPHERES,
    R_CONVECTION_FORCED_INTERNAL_LAMINAR,
    R_CONVECTION_FORCED_INTERNAL_TURBULENT,
    R_CONVECTION_FORCED_EXTERNAL,
    R_CONVECTION_N_TYPES
} RConvectionType;

class RConvection
{

    private:

        //! Coefficient type.
        RConvectionType type;
        //! Material name.
        QString matName;
        //! Dynamic viscosity.
        double mu;
        //! Density.
        double ro;
        //! Thermal conductivity.
        double k;
        //! Heat capacity.
        double c;
        //! Thermal expansion.
        double b;
        //! Hydraulic diameter.
        double d;
        //! Mean velocity.
        double v;
        //! Acceleration due to gravity.
        double g;
        //! Surface temperature.
        double Ts;
        //! Surface temperature.
        double Tf;

    private:

        //! Internal initialization function.
        void _init(const RConvection *pConvection = nullptr);

    public:

        //! Constructor.
        RConvection();

        //! Copy constructor.
        RConvection(const RConvection &convection);

        //! Destructor.
        ~RConvection();

        //! Assignment operator.
        RConvection & operator =(const RConvection &convection);

        //! Set convection type.
        void setType(RConvectionType type);

        //! Set material.
        void setMaterial(const RMaterial &material, double temperature = 293.15);

        //! Set material properties.
        void setMaterial(const QString &matName, double mu, double ro, double k, double c, double b);

        //! Set diameter.
        void setDiameter(double d);

        //! Set velocity
        void setVelocity(double v);

        //! Set gravity.
        void setGravity(double g);

        //! Set surface temperature.
        void setSurfTemp(double Ts);

        //! Set fuid temperature.
        void setFluidTemp(double Tf);

        //! Calculate Gr number.
        double calculateGr(void) const;

        //! Calculate Ra number.
        double calculateRa(void) const;

        //! Calculate Reynolds number.
        double calculateRe(void) const;

        //! Calculate Prandtl number.
        double calculatePr(void) const;

        //! Calculate Nusslet number.
        double calculateNu(void) const;

        //! Calulate heat transfer (convection) coefficient.
        double calculateHtc(void) const;

        //! Return convection type.
        inline RConvectionType getType(void) const
        {
            return this->type;
        }

        //! Return diameter.
        inline double getDiameter(void) const
        {
            return this->d;
        }

        //! Return velocity.
        inline double getVelocity(void) const
        {
            return this->v;
        }

        //! Return gravity.
        inline double getGravity(void) const
        {
            return this->g;
        }

        //! Return surface temperature.
        inline double getSurfTemp(void) const
        {
            return this->Ts;
        }

        //! Return fluid temperature.
        inline double getFluidTemp(void) const
        {
            return this->Ts;
        }

        //! Return name (description string).
        static const QString & getName(RConvectionType type);

        //! Return true if convection is natural.
        static bool isNatural(RConvectionType type);

        //! Return true if convection is forced.
        static bool isForced(RConvectionType type);

};

#endif // RCONVECTION_H
