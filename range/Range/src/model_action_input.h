/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   model_action_input.h                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   20-th November 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Model action input class declaration                *
 *********************************************************************/

#ifndef MODEL_ACTION_INPUT_H
#define MODEL_ACTION_INPUT_H

#include <rmlib.h>

#include "geometry_transform_input.h"
#include "pick_list.h"

typedef enum _ModelActionType
{
    MODEL_ACTION_NONE = 0,
    MODEL_ACTION_AUTOMARK_SURFACES,
    MODEL_ACTION_MARK_SURFACES,
    MODEL_ACTION_CLOSE_SURFACE_HOLE,
    MODEL_ACTION_MERGE_NEAR_NODES,
    MODEL_ACTION_MERGE_ENTITIES,
    MODEL_ACTION_REMOVE_ENTITIES,
    MODEL_ACTION_CREATE_ELEMENT,
    MODEL_ACTION_REMOVE_NODES,
    MODEL_ACTION_PURGE_UNUSED_NODES,
    MODEL_ACTION_PURGE_UNUSED_ELEMENTS,
    MODEL_ACTION_REMOVE_ELEMENTS,
    MODEL_ACTION_GENERATE_LINE_FROM_EDGES,
    MODEL_ACTION_FIND_SLIVER_ELEMENTS,
    MODEL_ACTION_FIX_SLIVER_ELEMENTS,
    MODEL_ACTION_FIND_INTERSECTED_ELEMENTS,
    MODEL_ACTION_BREAK_INTERSECTED_ELEMENTS,
    MODEL_ACTION_EXPORT_SLIVER_ELEMENTS,
    MODEL_ACTION_EXPORT_INTERSECTED_ELEMENTS,
    MODEL_ACTION_BOOL_DIFFERENCE,
    MODEL_ACTION_BOOL_INTERSECTION,
    MODEL_ACTION_BOOL_UNION,
    MODEL_ACTION_CHECK_POINT_INSIDE_SURFACE,
    MODEL_ACTION_GENERATE_PATCHES,
    MODEL_ACTION_CALCULATE_VIEWFACTORS,
    MODEL_ACTION_SWAP_SURFACE_ELEMENT_NORMAL,
    MODEL_ACTION_SWAP_SURFACE_NORMALS,
    MODEL_ACTION_SYNC_SURFACE_NORMALS,
    MODEL_ACTION_COARSEN_SURFACE,
    MODEL_ACTION_TETRAHEDRALIZE_SURFACE,
    MODEL_ACTION_CONSOLIDATE,
    MODEL_ACTION_GEOMETRY_TRANSFORM,
    MODEL_ACTION_APPLY_VARIABLE_ON_NODE,
    MODEL_ACTION_REMOVE_VARIABLE,
    MODEL_ACTION_N_TYPES
} ModelActionType;

class ModelActionInput
{

    protected:

        //! Model ID.
        uint modelID;
        //! Priority.
        uint priority;
        //! Model action type.
        ModelActionType type;
        //! Number of iterations.
        uint nIterations;
        //! Separation angle.
        double separationAngle;
        //! Close hole.
        bool closeHole;
        //! Element type.
        RElementType elementType;
        //! Node IDs;
        QList<uint> nodeIDs;
        //! Element IDs;
        QList<uint> elementIDs;
        //! List of entities.
        QList<SessionEntityID> entityIDs;
        //! Point.
        RR3Vector point;
        //! Edge ration.
        double edgeRatio;
        //! Edge length.
        double edgeLength;
        //! Element area.
        double elementArea;
        //! Tomerance.
        double tolerance;
        //! Geometry transform input.
        GeometryTransformInput geometryTransformInput;
        //! Variable.
        RVariableType variableType;

    private:

        //! Internal initialization function.
        void _init(const ModelActionInput *pModelActionInput = 0);

    public:

        //! Constructor.
        explicit ModelActionInput(uint modelID = 0, uint priority = 1000);

        //! Copy constructor.
        ModelActionInput(const ModelActionInput &modelActionInput);

        //! Destructor.
        ~ModelActionInput();

        //! Assignment operator.
        ModelActionInput &operator =(const ModelActionInput &modelActionInput);

        //! Less then operator.
        bool operator <(const ModelActionInput &modelActionInput) const;

        //! Return model ID.
        uint getModelID(void) const;

        //! Return model action type.
        ModelActionType getType(void) const;

        //! Return number of iterations.
        uint getNIterations(void) const;

        //! Return separation angle.
        double getSeparationAngle(void) const;

        //! Return close hole.
        bool getCloseHole(void) const;

        //! Return element type.
        RElementType getElementType(void) const;

        //! Return vector of node IDs.
        const QList<uint> &getNodeIDs(void) const;

        //! Return vector of element IDs.
        const QList<uint> &getElementIDs(void) const;

        //! Return vector of entity IDs.
        const QList<SessionEntityID> &getEntityIDs(void) const;

        //! Return point.
        const RR3Vector &getPoint(void) const;

        //! Return edge ration.
        double getEdgeRatio(void) const;

        //! Return edge length.
        double getEdgeLength(void) const;

        //! Return element area.
        double getElementArea(void) const;

        //! Return tolerance.
        double getTolerance(void) const;

        //! Return geometry transform input.
        const GeometryTransformInput &getGeometryTransformInput(void) const;

        //! Return variable type.
        RVariableType getVariableType(void) const;

        //! Set auto-mark surfaces and its input.
        void setAutoMarkSurfaces(double separationAngle);

        //! Set mark surfaces and its input.
        void setMarkSurfaces(double separationAngle, const QList<uint> &elementIDs);

        //! Set remove nodes.
        void setCloseSurfaceHole(const QList<uint> &edgeIDs);

        //! Set merge near nodes and its input.
        void setMergeNearNodes(double tolerance);

        //! Set merge entities and its input.
        void setMergeEntities(const QList<SessionEntityID> &entityIDs);

        //! Set remove entities and its input.
        void setRemoveEntities(const QList<SessionEntityID> &entityIDs);

        //! Set create element.
        void setCreateElement(const QList<uint> &nodeIDs, RElementType elementType);

        //! Set remove nodes.
        void setRemoveNodes(const QList<uint> &nodeIDs, bool closeHole);

        //! Set purge unused nodes.
        void setPurgeUnusedNodes(void);

        //! Set purge unused elements.
        void setPurgeUnusedElements(void);

        //! Set remove elements.
        void setRemoveElements(const QList<uint> &elementIDs, bool closeHole);

        //! Set generate lines from surface edges.
        void setGenerateLinesFromEdges(double separationAngle);

        //! Set find sliver elements.
        void setFindSliverElements(double edgeRatio);

        //! Set fix sliver elements.
        void setFixSliverElements(double edgeRatio);

        //! Set find intersected surface elements.
        void setFindIntersectedElements(void);

        //! Set break intersected surface elements.
        void setBreakIntersectedElements(uint nIterations);

        //! Set export sliver surface elements.
        void setExportSliverElements(void);

        //! Set export intersected surface elements.
        void setExportIntersectedElements(void);

        //! Set bool difference.
        void setBoolDifference(const QList<SessionEntityID> &entityIDs);

        //! Set bool intersection.
        void setBoolIntersection(const QList<SessionEntityID> &entityIDs);

        //! Set bool union.
        void setBoolUnion(const QList<SessionEntityID> &entityIDs);

        //! Set to check if point is inside surface.
        void setCheckPointInsideSurface(const RR3Vector &point, const QList<SessionEntityID> &entityIDs);

        //! Set generate patches.
        void setGeneratePatches(void);

        //! Set calculate view-factors.
        void setCalculateViewFactors(void);

        //! Set swap surface element normal.
        void setSwapSurfaceElementNormal(const QList<uint> &elementIDs);

        //! Set swap surface normals.
        void setSwapSurfaceNormals(const QList<SessionEntityID> &entityIDs);

        //! Set synchronize surface normals.
        void setSyncSurfaceNormals(void);

        //! Set coarsen surface.
        void setCoarsenSurface(const QList<SessionEntityID> &entityIDs, double edgeLength, double elementArea);

        //! Set tetrahedralize surface.
        void setTetrahedralizeSurface(const QList<SessionEntityID> &entityIDs);

        //! Set consolidate.
        void setConsolidate(void);

        //! Set geometry transform.
        void setGeometryTransform(GeometryTransformInput geometryTransformInput);

        //! Set apply variable on node.
        void setApplyVariableOnNode(RVariableType variableType);

        //! Set remove variable.
        void setRemoveVariable(RVariableType variableType);

};

#endif // MODEL_ACTION_INPUT_H
