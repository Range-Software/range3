/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rscales.h                                                *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th May 2014                                           *
 *                                                                   *
 *  DESCRIPTION: Scales class declaration                            *
 *********************************************************************/

#ifndef RSCALES_H
#define RSCALES_H

#include <rblib.h>
#include <rmlib.h>

class RScales
{

    protected:

        //! Legth.
        double metre;
        //! Mass.
        double kilogram;
        //! Time.
        double second;
        //! Electric current.
        double ampere;
        //! Temperature.
        double kelvin;
        //! Luminous intensity.
        double candela;
        //! Amount of substance.
        double mole;

    private:

        //! Internal initialization function.
        void _init(const RScales *pScales = nullptr);

    public:

        //! Constructor.
        RScales();

        //! Copy constructor.
        RScales(const RScales &scales);

        //! Destructor.
        ~RScales();

        //! Assignment operator.
        RScales & operator =(const RScales &scales);

        //! Return legth.
        inline double getMetre(void) const { return this->metre; }

        //! Set legth.
        inline void setMetre(double metre) { this->metre = metre; }

        //! Return mass.
        inline double getKilogram(void) const { return this->kilogram; }

        //! Set mass.
        inline void setKilogram(double kilogram) { this->kilogram = kilogram; }

        //! Return time.
        inline double getSecond(void) const { return this->second; }

        //! Set time.
        inline void setSecond(double second) { this->second = second; }

        //! Return electric current.
        inline double getAmpere(void) const { return this->ampere; }

        //! Set electric current.
        inline void setAmpere(double ampere) { this->ampere = ampere; }

        //! Return temperature.
        inline double getKelvin(void) const { return this->kelvin; }

        //! Set temperature.
        inline void setKelvin(double kelvin) { this->kelvin = kelvin; }

        //! Return luminous intensity.
        inline double getCandela(void) const { return this->candela; }

        //! Set luminous intensity.
        inline void setCandela(double candela) { this->candela = candela; }

        //! Return amount of substance.
        inline double getMole(void) const { return this->mole; }

        //! Set amount of substance.
        inline void setMole(double mole) { this->mole = mole; }

        //! Return scale factor for variable type.
        double findScaleFactor(RVariableType variableType) const;

        //! Return scale factor for material property type.
        double findScaleFactor(RMaterialPropertyType materialPropertyType) const;

        //! Nondimensionalize model.
        void downscale(RModel &model) const;

        //! Restore model.
        void upscale(RModel &model) const;

        //! Print scales.
        void print(bool allVariables = false) const;

    protected:

        //! Scale model.
        void convert(RModel &model, bool invert) const;

};

#endif // RSCALES_H
