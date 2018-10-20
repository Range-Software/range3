/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rhemicube.cpp                                            *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   24-th July 2014                                          *
 *                                                                   *
 *  DESCRIPTION: View-factor hemi cube class definition              *
 *********************************************************************/

#include <omp.h>

#include "rhemicube.h"

class RHemicubeTriangle
{
    protected:

        RTriangle triangle;
        uint color;

    public:

        RHemicubeTriangle(const RTriangle &triangle, uint color) : triangle(triangle), color(color) {}

        const RTriangle &getTriangle(void) const { return this->triangle; }

        uint getColor(void) const { return this->color; }
};

class RHemicubeTriangleComp
{
    protected:

        RR3Vector eye;

    public:

        RHemicubeTriangleComp(const RR3Vector &eye) : eye(eye) {}

        bool operator() (const RHemicubeTriangle &t1,const RHemicubeTriangle &t2)
        {
            double d1 = RSegment::findLength(RNode(this->eye),t1.getTriangle().getNode1());
            d1 = std::min(d1,RSegment::findLength(RNode(this->eye),t1.getTriangle().getNode2()));
            d1 = std::min(d1,RSegment::findLength(RNode(this->eye),t1.getTriangle().getNode3()));

            double d2 = RSegment::findLength(RNode(this->eye),t2.getTriangle().getNode1());
            d2 = std::min(d2,RSegment::findLength(RNode(this->eye),t2.getTriangle().getNode2()));
            d2 = std::min(d2,RSegment::findLength(RNode(this->eye),t2.getTriangle().getNode3()));

            return (d1 < d2);
        }
};

void RHemiCube::_init(const RHemiCube *pHemiCube)
{
    if (pHemiCube)
    {
        this->eyeDirection = pHemiCube->eyeDirection;
        this->sectors.resize(pHemiCube->sectors.size());
        for (uint i=0;i<pHemiCube->sectors.size();i++)
        {
            this->sectors[i] = new RHemiCubeSector(*pHemiCube->sectors[i]);
        }
    }
}

RHemiCube::RHemiCube(const RR3Vector &eyePosition, const RR3Vector &eyeDirection, uint resolution, double size)
    : eyeDirection(eyeDirection)
{
    this->_init();
    this->generate(eyePosition,eyeDirection,resolution,size);
}

RHemiCube::RHemiCube(const RHemiCube &hemiCube)
{
    this->_init(&hemiCube);
}

RHemiCube::~RHemiCube()
{
    for (uint i=0;i<this->sectors.size();i++)
    {
        delete this->sectors[i];
    }
}

RHemiCube &RHemiCube::operator =(const RHemiCube &hemiCube)
{
    this->_init(&hemiCube);
    return (*this);
}

void RHemiCube::rayTraceTriangle(const RTriangle &triangle, uint color)
{
//    #pragma omp parallel for default(shared)
    for (uint i=0;i<this->sectors.size();i++)
    {
        this->sectors[i]->rayTraceTriangle(triangle,color);
    }
}

std::map<uint, double> RHemiCube::getViewFactors(void) const
{

    std::map<uint, double> viewFactors;

    for (uint i=0;i<this->sectors.size();i++)
    {
        for (uint j=0;j<this->sectors[i]->getSize();j++)
        {
            const RHemiCubePixel &rPixel = this->sectors[i]->getPixel(j);
            uint color = rPixel.getColor();
            if (color == RConstants::eod)
            {
                continue;
            }
            if (viewFactors.find(color) == viewFactors.end())
            {
                viewFactors[color] = rPixel.getWeight();
            }
            else
            {
                viewFactors[color] += rPixel.getWeight();
            }
        }
    }

    return viewFactors;
}

double RHemiCube::getFillRatio(void) const
{
    uint totalSize = 0;
    uint totalFill = 0;

    for (uint i=0;i<this->sectors.size();i++)
    {
        for (uint j=0;j<this->sectors[i]->getSize();j++)
        {
            if (this->sectors[i]->getPixel(j).getColor() != RConstants::eod)
            {
                totalFill++;
            }
            totalSize++;
        }
    }

    return double(totalFill)/double(totalSize);
}

void RHemiCube::calculateViewFactors(const RModel &model, RViewFactorMatrix &rViewFactorMatrix)
{
    RPatchBook &rPatchBook = rViewFactorMatrix.getPatchBook();
    std::vector<RPatchInput> &rPatchInput = rViewFactorMatrix.getHeader().getPatchInput();


    rViewFactorMatrix.getHeader().setNElements(model.getNElements());
    rViewFactorMatrix.getHeader().setHemicubeResolution(model.getProblemSetup().getRadiationSetup().getResolution());

    rViewFactorMatrix.resize(rPatchBook.getNPatches());

    RLogger::info("Calculating view-factors.\n");
    RLogger::indent();

    uint nPatchesProcessed = 0;

    RProgressInitialize("Calculating view-factors");
    #pragma omp parallel for default(shared)
    for (int64_t eyePatchID=0;eyePatchID<int64_t(rPatchBook.getNPatches());eyePatchID++)
    {
        RViewFactorRow &rViewFactorRow = rViewFactorMatrix.getRow(eyePatchID);
        rViewFactorRow.getViewFactors().clear();

        const RPatch &rPatch = rPatchBook.getPatch(eyePatchID);
        uint surfaceID = rPatch.getSurfaceID();
        if (rPatchInput[surfaceID].getEmitter())
        {
            RR3Vector eyePosition;
            RR3Vector eyeDirection;

            model.findPatchCenter(rPatch,eyePosition[0],eyePosition[1],eyePosition[2]);
            model.findPatchNormal(rPatch,eyeDirection[0],eyeDirection[1],eyeDirection[2]);

            RHemiCube hemiCube(eyePosition,eyeDirection,rViewFactorMatrix.getHeader().getHemicubeResolution(),100);

            RHemicubeTriangleComp hemiCubeTriangleComp(eyePosition);

            std::vector<RHemicubeTriangle> hemiCubeTriangles;
            hemiCubeTriangles.reserve(rPatchBook.getNPatches()*2);

            // Fill vector of triangles to be rendered / ray-traced.
            for (uint patchID=0;patchID<rPatchBook.getNPatches();patchID++)
            {
                if (eyePatchID == patchID)
                {
                    continue;
                }
                if (!rPatchInput[rPatchBook.getPatch(patchID).getSurfaceID()].getReceiver())
                {
                    continue;
                }

                const RUVector &rElementIDs = rPatchBook.getPatch(patchID).getElementIDs();
                for (uint j=0;j<rElementIDs.size();j++)
                {
                    const RElement &rElement = model.getElement(rElementIDs[j]);
                    std::vector<RTriangle> triangles = rElement.triangulate(model.getNodes());
                    for (uint k=0;k<triangles.size();k++)
                    {
                        hemiCubeTriangles.push_back(RHemicubeTriangle(triangles[k],patchID));
                    }
                }
            }

            // Sort vector of triangles by distance.
            // This way closest triangles will be rendered first and those which are at the back
            // will not waste computatuional time.
            std::sort(hemiCubeTriangles.begin(),hemiCubeTriangles.end(),hemiCubeTriangleComp);

            // Raytrace triangles.
            for (uint i=0;i<hemiCubeTriangles.size();i++)
            {
                hemiCube.rayTraceTriangle(hemiCubeTriangles[i].getTriangle(),hemiCubeTriangles[i].getColor());
            }

            // Transfer view-factors from hemi-cube to view-factor row storage.
            std::map<uint,double> viewFactorMap = hemiCube.getViewFactors();
            std::map<uint,double>::const_iterator iter;
            for (iter = viewFactorMap.begin(); iter != viewFactorMap.end(); ++iter)
            {
                uint patchID = uint(iter->first);
                double viewFactor = double(iter->second);
                rViewFactorRow.getViewFactors().addValue(patchID,viewFactor);
            }
        }

        // Calculate view-factor row sum.
        const RSparseVector<double> &viewFactors = rViewFactorRow.getViewFactors();
        double vfRowSum = 0.0;
        for (uint i=0;i<uint(viewFactors.size());i++)
        {
            vfRowSum += viewFactors.getValue(i);
        }

#pragma omp critical
        {
            RProgressPrint(++nPatchesProcessed,rPatchBook.getNPatches());
            RLogger::info("[%9u of %-9u] Patch %9u: row sum = %g\n",nPatchesProcessed,rPatchBook.getNPatches(),eyePatchID+1,vfRowSum);
        }
    }
    RProgressFinalize("Done");
    RLogger::unindent();
}

void RHemiCube::generate(const RR3Vector &eyePosition, const RR3Vector &eyeDirection, uint resolution, double size)
{
    uint nSubSectors = 5;
    uint subSectorResolution = uint(std::ceil(double(resolution)/double(nSubSectors)));

    this->sectors.resize(R_HEMICUBE_SECTOR_N_TYPES * nSubSectors * nSubSectors);

    uint idx = 0;
    double totalWeight = 0.0;
    for (uint i=0;i<R_HEMICUBE_SECTOR_N_TYPES;i++)
    {
        for (uint k=0;k<nSubSectors;k++)
        {
            for (uint l=0;l<nSubSectors;l++)
            {
                this->sectors[idx] = new RHemiCubeSector(eyePosition,eyeDirection,subSectorResolution,k,l,nSubSectors,size,RHemiCubeSectorType(i));
                for (uint j=0;j<this->sectors[idx]->getSize();j++)
                {
                    totalWeight += this->sectors[idx]->getPixel(j).getWeight();
                }
                idx ++;
            }
        }
    }
    if (totalWeight == 0.0)
    {
        return;
    }
    idx = 0;
    for (uint i=0;i<R_HEMICUBE_SECTOR_N_TYPES;i++)
    {
        for (uint j=0;j<nSubSectors*nSubSectors;j++)
        {
            for (uint k=0;k<this->sectors[idx]->getSize();k++)
            {
                this->sectors[idx]->getPixel(k).setWeight(this->sectors[idx]->getPixel(k).getWeight()/totalWeight);
            }
            idx++;
        }
    }
}
