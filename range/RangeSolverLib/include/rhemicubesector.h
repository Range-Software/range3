/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rhemicubesector.h                                        *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   24-th July 2014                                          *
 *                                                                   *
 *  DESCRIPTION: View-factor hemi-cube sector class declaration      *
 *********************************************************************/

#ifndef RHEMICUBESECTOR_H
#define RHEMICUBESECTOR_H

#include <vector>

#include <rmlib.h>

#include "rhemicubepixel.h"

typedef enum _RHemiCubeSectorType
{
    R_HEMICUBE_SECTOR_FRONT_NORTH_EAST = 0,
    R_HEMICUBE_SECTOR_FRONT_NORTH_WEST,
    R_HEMICUBE_SECTOR_FRONT_SOUTH_EAST,
    R_HEMICUBE_SECTOR_FRONT_SOUTH_WEST,
    R_HEMICUBE_SECTOR_EAST_NORTH,
    R_HEMICUBE_SECTOR_EAST_SOUTH,
    R_HEMICUBE_SECTOR_WEST_NORTH,
    R_HEMICUBE_SECTOR_WEST_SOUTH,
    R_HEMICUBE_SECTOR_NORTH_EAST,
    R_HEMICUBE_SECTOR_NORTH_WEST,
    R_HEMICUBE_SECTOR_SOUTH_EAST,
    R_HEMICUBE_SECTOR_SOUTH_WEST,
    R_HEMICUBE_SECTOR_N_TYPES
} RHemiCubeSectorType;

class RHemiCubeSector
{

    protected:

        //! Limit polygon.
        std::vector<RR3Vector> limitPolygon;
        //! Resolution.
        uint resolution;
        //! Eye position.
        RR3Vector eyePosition;
        //! Eye direction.
        RR3Vector eyeDirection;
        //! Pixels.
        std::vector<RHemiCubePixel> pixels;

    private:

        //! Internal initialization function.
        void _init(const RHemiCubeSector *pHemiCubeSector = nullptr);

    public:

        //! Constructor.
        RHemiCubeSector(const RR3Vector &eyePosition,
                        const RR3Vector &eyeDirection,
                        uint resolution,
                        uint iSubIndex,
                        uint jSubIndex,
                        uint nSubIndexes,
                        double size,
                        RHemiCubeSectorType type);

        //! Copy constructor.
        RHemiCubeSector(const RHemiCubeSector &hemiCubeSector);

        //! Destructor.
        ~RHemiCubeSector();

        //! Assignment operator.
        RHemiCubeSector &operator =(const RHemiCubeSector &hemiCubeSector);

        //! Return size of the sector.
        uint getSize(void) const;

        //! Return const reference to pixel at given position.
        const RHemiCubePixel &getPixel(uint position) const;

        //! Return reference to pixel at given position.
        RHemiCubePixel &getPixel(uint position);

        //! Ray-trace triangle.
        //! Visible part of the triangle will be stored in the segment pixels.
        void rayTraceTriangle(const RTriangle &triangle, uint color);

    private:

        //! Test if triangle is visible in sector.
        bool testVisibility(const RTriangle &triangle) const;

        //! Generate pixel set.
        void generate(uint resolution,
                      uint iSubIndex,
                      uint jSubIndex,
                      uint nSubIndexes,
                      double size,
                      RHemiCubeSectorType type);

        //! Return limit polygon.
        std::vector<RR3Vector> findLimitPolygon(void) const;

        //! Find limit polygons min and max ranges.
        RLimitBox findLimitBox(void) const;

};

#endif // RHEMICUBESECTOR_H
