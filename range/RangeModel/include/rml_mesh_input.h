/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_mesh_input.h                                         *
 *  GROUP:  RModel                                                   *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   5-th August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: RMeshInput class declaration                        *
 *********************************************************************/

#ifndef RML_MESH_INPUT_H
#define RML_MESH_INPUT_H

#include <QString>

#include <rblib.h>

class RMeshInput
{

    protected:

        //! Verbose output.
        bool verbose;
        //! Generate volume mesh.
        bool volumeMesh;
        //! Generate quality mesh.
        bool qualityMesh;
        //! Output edges.
        bool outputEdges;
        //! Reconstruct.
        bool reconstruct;
        //! Use size function.
        bool useSizeFunction;
        //! Size function values.
        RRVector sizeFunctionValues;
        //! Radius/edge ratio.
        double radiusEdgeRatio;
        //! Volume constrain.
        double volumeConstraint;
        //! Tolerance.
        double tolerance;
        //! Perform surface integrity check before mesh generation.
        bool surfaceIntegrityCheck;
        //! Keep results after mesh generation is done.
        bool keepResults;

        //! Use provided TetGen mesh input line directly.
        bool useTetGenInputParams;
        //! TetGen input parameters.
        QString tetGenInputParams;

    private:

        //! Internal initialization function.
        void _init ( const RMeshInput *pMeshInput = nullptr );

    public:

        //! Constructor.
        RMeshInput();

        //! Copy constructor.
        RMeshInput(const RMeshInput &meshInput);

        //! Destructor.
        ~RMeshInput();

        //! Assignment operator.
        RMeshInput & operator = ( const RMeshInput &meshInput );

        //! Return verbose.
        bool getVerbose(void) const;

        //! Set verbose.
        void setVerbose(bool verbose);

        //! Return generate volume mesh.
        bool getVolumeMesh(void) const;

        //! Set generate volume mesh.
        void setVolumeMesh(bool volumeMesh);

        //! Return generate quality mesh.
        bool getQualityMesh(void) const;

        //! Set generate quality mesh.
        void setQualityMesh(bool qualityMesh);

        //! Return output edges.
        bool getOutputEdges(void) const;

        //! Set output edges.
        void setOutputEdges(bool outputEdges);

        //! Return whether to reconstruct mesh.
        bool getReconstruct(void) const;

        //! Set whether to reconstruct mesh.
        void setReconstruct(bool reconstruct);

        //! Return whether to use size function.
        bool getUseSizeFunction(void) const;

        //! Set whether to use size function.
        void setUseSizeFunction(bool useSizeFunction);

        //! Return size function values.
        const RRVector &getSizeFunctionValues(void) const;

        //! Set size function values.
        void setSizeFunctionValues(const RRVector &sizeFunctionValues);

        //! Return radius/edge ratio.
        double getRadiusEdgeRatio(void) const;

        //! Set radius/edge ratio.
        void setRadiusEdgeRatio(double radiusEdgeRatio);

        //! Return volume constrain.
        double getVolumeConstraint(void) const;

        //! Set volume constrain.
        void setVolumeConstraint(double volumeConstraint);

        //! Return tolerance.
        double getTolerance(void) const;

        //! Set tolerance.
        void setTolerance(double tolerance);

        //! Return whether to perform surface integrity check before mesh generation.
        bool getSurfaceIntegrityCheck(void) const;

        //! Set whether to perform surface integrity check before mesh generation.
        void setSurfaceIntegrityCheck(bool surfaceIntegrityCheck);

        //! Return whether results should be kept.
        bool getKeepResults(void) const;

        //! Set whether results should be kept.
        void setKeepResults(bool keepResults);

        //! Return whether to use TetGen input parameters directly.
        bool getUseTetGenInputParams(void) const;

        //! Set whether to use TetGen input parameters directly.
        void setUseTetGenInputParams(bool useTetGenInputParams);

        //! Return TetGen input parameters directly.
        const QString &getTetGenInputParams(void) const;

        //! Set TetGen input parameters directly.
        void setTetGenInputParams(const QString &tetGenInputParams);

};

#endif // RML_MESH_INPUT_H
