/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rhemicube.h                                              *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   24-th July 2014                                          *
 *                                                                   *
 *  DESCRIPTION: View-factor hemi cube class declaration             *
 *********************************************************************/

#ifndef RHEMICUBE_H
#define RHEMICUBE_H

#include <vector>

#include <rblib.h>
#include <rmlib.h>

#include "rhemicubesector.h"

class RHemiCube
{

    protected:

        //! Vector of hemicube sector.
        std::vector<RHemiCubeSector*> sectors;
        //! Eye direction.
        RR3Vector eyeDirection;

    private:

        //! Internal initialization function.
        void _init(const RHemiCube *pHemiCube = nullptr);

    public:

        //! Constructor.
        RHemiCube(const RR3Vector &eyePosition, const RR3Vector &eyeDirection, uint resolution, double size = 100.0);

        //! Copy constructor.
        RHemiCube(const RHemiCube &hemiCube);

        //! Destructor.
        ~RHemiCube();

        //! Assignment operator.
        RHemiCube &operator =(const RHemiCube &hemiCube);

        //! Ray-trace triangle.
        //! Visible part of the triangle will be stored in hemicube.
        void rayTraceTriangle(const RTriangle &triangle, uint color);

        //! Return view factors.
        //! Function returns map of color to view-factor value (color = 0 => empty).
        std::map<uint,double> getViewFactors(void) const;

        //! Return fill ratio.
        double getFillRatio(void) const;

        //! Calculate view factors.
        static void calculateViewFactors(const RModel &model, RViewFactorMatrix &rViewFactorMatrix);

    private:

        //! Generate hemicube.
        void generate(const RR3Vector &eyePosition, const RR3Vector &eyeDirection, uint resolution, double size);

};

#endif // RHEMICUBE_H
