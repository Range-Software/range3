/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rhemicubepixel.h                                         *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   24-th July 2014                                          *
 *                                                                   *
 *  DESCRIPTION: View-factor hemi cube pixel class declaration       *
 *********************************************************************/

#ifndef RHEMICUBEPIXEL_H
#define RHEMICUBEPIXEL_H

#include <rblib.h>

class RHemiCubePixel
{

    protected:

        //! Pixel position.
        RR3Vector position;
        //! Pixel color (0 = no color).
        uint color;
        //! Pixel depth.
        double depth;
        //! Weight factor.
        double weight;

    private:

        //! Internal initialization function.
        void _init(const RHemiCubePixel *pHemiCubePixel = nullptr);

    public:

        //! Constructor.
        RHemiCubePixel();

        //! Copy constructor.
        RHemiCubePixel(const RHemiCubePixel &hemiCubePixel);

        //! Destructor.
        ~RHemiCubePixel();

        //! Assignment operator.
        RHemiCubePixel &operator =(const RHemiCubePixel &hemiCubePixel);

        //! Return const reference to pixel position.
        const RR3Vector &getPosition(void) const;

        //! Return reference to pixel position.
        RR3Vector &getPosition(void);

        //! Set pixel position.
        void setPosition(const RR3Vector &position);

        //! Return pixel color.
        uint getColor(void) const;

        //! Set pixel color.
        void setColor(uint color);

        //! Return pixel depth.
        double getDepth(void) const;

        //! Set pixel depth.
        void setDepth(double depth);

        //! Return pixel weight.
        double getWeight(void) const;

        //! Set pixel weight.
        void setWeight(double weight);

};

#endif // RHEMICUBEPIXEL_H
