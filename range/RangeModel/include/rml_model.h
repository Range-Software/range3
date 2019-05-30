/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_model.h                                              *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   8-th December 2011                                       *
 *                                                                   *
 *  DESCRIPTION: Model class declaration                             *
 *********************************************************************/

#ifndef RML_MODEL_H
#define RML_MODEL_H

#include <vector>

#include <rblib.h>

#include "rml_cut.h"
#include "rml_element.h"
#include "rml_interpolated_entity.h"
#include "rml_iso.h"
#include "rml_line.h"
#include "rml_node.h"
#include "rml_patch_book.h"
#include "rml_patch_input.h"
#include "rml_point.h"
#include "rml_surface.h"
#include "rml_volume.h"
#include "rml_results.h"
#include "rml_mesh_input.h"
#include "rml_model_data.h"
#include "rml_model_msh.h"
#include "rml_model_stl.h"
#include "rml_model_raw.h"
#include "rml_view_factor_matrix.h"
#include "rml_scalar_field.h"
#include "rml_stream_line.h"
#include "rml_vector_field.h"


//! Model problem types.
typedef enum _RModelProblemType
{
    R_MODEL_PROBLEM_NONE                 = 0,
    R_MODEL_PROBLEM_INVALID_ELEMENT_TYPE = 1 << 0,
    R_MODEL_PROBLEM_INVALID_NEIGHBORS    = 1 << 1
} RModelProblemType;

typedef int RModelProblemTypeMask;


//! Model class.
class RModel : public RProblem, public RResults
{

    private:

        //! Internal initialization function.
        void _init ( const RModel *pModel = nullptr );

        //! Add element to element group.
        void addElementToGroup ( uint elementID,
                                 uint groupID = 0 );

    protected:

        //! Model name.
        QString name;
        //! Model description.
        QString description;
        //! List of all nodes.
        std::vector <RNode> nodes;
        //! List of all elements.
        std::vector <RElement> elements;
        //! List of all point element groups.
        std::vector <RPoint> points;
        //! List of all line element groups.
        std::vector <RLine> lines;
        //! List of all surface element groups.
        std::vector <RSurface> surfaces;
        //! List of all volume element groups.
        std::vector <RVolume> volumes;
        //! List of all vector fields.
        std::vector <RVectorField> vectorFields;
        //! List of all scalar fields.
        std::vector <RScalarField> scalarFields;
        //! List of all stream lines.
        std::vector <RStreamLine> streamLines;
        //! List of all cuts.
        std::vector <RCut> cuts;
        //! List of all isos.
        std::vector <RIso> isos;
        //! Surface neighbors.
        std::vector<RUVector> surfaceNeigs;
        //! Volume neighbors.
        std::vector<RUVector> volumeNeigs;
        //! Display properties.
        RModelData modelData;

    public:

        //! Constructor.
        RModel ();

        //! Copy constructor.
        RModel ( const RModel &model );

        //! Convert constructor from MSH (old range) model.
        RModel ( const RModelMsh &modelMsh );

        //! Convert constructor from RAW model.
        RModel ( const RModelRaw   &modelRaw,
                 const QString &name = QString(),
                 const QString &description = QString() );

        //! Convert constructor from STL model.
        RModel ( const RModelStl &modelStl );

        //! Destructor.
        virtual ~RModel ();

        //! Assignment operator.
        RModel & operator = ( const RModel &model );

        //! Update model data.
        void update ( const RModel &rModel );

        //! Read mesh from the file.
        void read ( const QString &fileName );

        //! Write mesh to the file.
        //! Return actual filename to which the model was saved.
        QString write ( const QString &fileName, bool writeLinkFile = true ) const;

        //! Export model to MSH (old range) model.
        void exportTo ( RModelMsh &modelMsh ) const;

        //! Export model to RAW model.
        void exportTo ( RModelRaw &modelRaw ) const;

        //! Export model to STL model.
        void exportTo ( RModelStl &modelStl ) const;

        //! Get model name.
        QString getName ( void ) const;

        //! Set model name.
        void setName ( const QString &name );

        //! Get model description.
        QString getDescription ( void ) const;

        //! Set model description.
        void setDescription ( const QString &name );

        //! Set problem task tree.
        void setProblemTaskTree ( const RProblemTaskItem &taskTree );

        /*************************************************************
         * Variable interface                                        *
         *************************************************************/

        //! Remove variable.
        void removeVariable(uint position);

        //! Remove all variables.
        void removeAllVariables();

        /*************************************************************
         * Node interface                                            *
         *************************************************************/

        //! Return number of nodes.
        uint getNNodes ( void ) const;

        //! Set number of nodes.
        void setNNodes ( uint nnodes );

        //! Return pointer to node in model at given position.
        const RNode * getNodePtr ( uint position ) const;

        //! Return pointer to node in model at given position.
        RNode * getNodePtr ( uint position );

        //! Return reference to node in model at given position.
        const RNode & getNode ( uint position ) const;

        //! Return reference to node in model at given position.
        RNode & getNode ( uint position );

        //! Return const reference to array of all nodes.
        const std::vector <RNode> & getNodes ( void ) const;

        //! Add node to model.
        void addNode ( const RNode  &node,
                       double  value = 0.0 );

        //! Add node specified by its coordinates to model.
        void addNode ( double x,
                       double y,
                       double z,
                       double value = 0.0 );

        //! Set node in model at given position.
        void setNode ( uint  position,
                       const RNode  &node      );

        //! Remove node from model at given position.
        //! If specified return vecor of indexes of removed elements and
        //! element groups in ascending order.
        void removeNode ( uint                position,
                          std::vector<uint> * removedElements = 0,
                          std::vector<uint> * removedPoints = 0,
                          std::vector<uint> * removedLines = 0,
                          std::vector<uint> * removedSurfaces = 0,
                          std::vector<uint> * removedVolumes = 0 );

        //! Remove list of nodes.
        //! If closeHole is set to true resulting hole will be patched with new elements.
        //! Above applies only on surface elements.
        void removeNodes(const QList<uint> &nodeIDs, bool closeHole);

        //! Merge two nodes.
        void mergeNodes(uint position1, uint position2, bool average, bool allowDowngrade);

        //! Find near node positions.
        //! Return vector of node positions in model whose
        //! distance from node is smaller than specified distance.
        std::vector<uint> findNearNodePositions(const RNode &node, double tolerance = RConstants::eps) const;

        //! Merge near nodes.
        uint mergeNearNodes(double tolerance = RConstants::eps);

        //! Remove duplicate elements.
        uint removeDuplicateElements();

        //! Find node limits.
        void findNodeLimits( double &xmin, double &xmax,
                             double &ymin, double &ymax,
                             double &zmin, double &zmax ) const;

        //! Return scale of the model.
        //! Scale represent real number that its inverse multiplication
        //! with model coordinates will result in maximum size
        //! to be equal 1.
        double findNodeScale( void ) const;

        //! Return center of all nodes (geometric center).
        void findNodeCenter( double &xc, double &yc, double &zc ) const;

        //! Find node distance statistics.
        RStatistics findNodeDistanceStatistics() const;

        //! Check if node is used.
        bool isNodeUsed(uint nodeID) const;

        //! Purge unused nodes.
        uint purgeUnusedNodes();

        /*************************************************************
         * Element interface                                         *
         *************************************************************/

        //! Return number of elements.
        //! If element type is specified number of elements of that
        //! type will be returned.
        uint getNElements ( RElementType elementType = R_ELEMENT_NONE ) const;

        //! Return number of elements.
        //! If element group type is specified number of elements of that
        //! type will be returned.
        uint getNElements ( REntityGroupType elementGroupType ) const;

        //! Set number of elements.
        void setNElements ( uint nelements );

        //! Return pointer to element in model at given position.
        const RElement * getElementPtr ( uint position ) const;

        //! Return pointer to element in model at given position.
        RElement * getElementPtr ( uint position );

        //! Return reference to element in model at given position.
        const RElement & getElement ( uint position ) const;

        //! Return reference to element in model at given position.
        RElement & getElement ( uint position );

        //! Return const reference to array of all elements.
        const std::vector <RElement> & getElements ( void ) const;

        //! Return reference to array of all elements.
        std::vector <RElement> & getElements ( void );

        //! Add element to model.
        void addElement ( const RElement &element,
                          bool            addToGroup = false,
                          uint            groupID = 0);

        //! Set element in model at given position.
        void setElement ( uint    position,
                          const RElement &element,
                          bool            addToGroup = true);

        //! Remove element from model at given position.
        //! If specified return vecor of indexes of removed
        //! element groups in ascending order.
        void removeElement ( uint                position,
                             bool                removeGroups = true,
                             std::vector<uint> * removedPoints = 0,
                             std::vector<uint> * removedLines = 0,
                             std::vector<uint> * removedSurfaces = 0,
                             std::vector<uint> * removedVolumes = 0  );

        //! Remove list of elements.
        //! If closeHole is set to true resulting hole will be patched with new elements.
        //! Above applies only on surface elements.
        void removeElements(const QList<uint> &elementIDs, bool closeHole);

        //! Return vector of possitions of elements containing given
        //! node.
        std::vector<uint> findElementPositionsByNodeId
                                        ( uint nodeID ) const;

        //! Find line element size statistics.
        RStatistics findLineElementSizeStatistics() const;

        //! Find surface element size statistics.
        RStatistics findSurfaceElementSizeStatistics() const;

        //! Find volume element size statistics.
        RStatistics findVolumeElementSizeStatistics() const;

        //! Purge unused elements.
        uint purgeUnusedElements();

        /*************************************************************
         * Interpolated element interface                            *
         *************************************************************/

        //! Return const pointer to interpolated element.
        //! If no element can be found 0 is returned.
        const RInterpolatedElement *getInterpolatedElement(REntityGroupType type, uint entityID, uint elementID) const;

        //! Return pointer to interpolated element.
        //! If no element can be found 0 is returned.
        RInterpolatedElement *getInterpolatedElement(REntityGroupType type, uint entityID, uint elementID);

        /*************************************************************
         * Entity / Element group interface                          *
         *************************************************************/

        //! Return number of elements in given entity.
        uint getNEntityElements ( REntityGroupType entityType ) const;

        //! Return number of element groups.
        uint getNElementGroups ( void ) const;

        //! Return number of entity groups.
        uint getNEntityGroups ( bool onlyElements = false ) const;

        //! Return const pointer to element group.
        const RElementGroup * getElementGroupPtr ( uint groupID ) const;

        //! Return pointer to element group.
        RElementGroup * getElementGroupPtr ( uint groupID );

        //! Return const pointer to entity group.
        const REntityGroup * getEntityGroupPtr ( uint groupID,
                                                 bool         onlyElements = false  ) const;

        //! Return pointer to entity group.
        REntityGroup * getEntityGroupPtr ( uint groupID,
                                           bool         onlyElements = false  );

        //! Return entity group type.
        REntityGroupType getEntityGroupType ( uint groupID,
                                              bool         onlyElements = false  ) const;

        //! Return position of group within the list of its type.
        uint getEntityGroupPosition ( uint groupID,
                                      bool         onlyElements = false  ) const;

        //! Convert entity type and ID to element group ID.
        uint getElementGroupID ( REntityGroupType entityType,
                                         uint     entityID ) const;

        //! Convert entity type and ID to entity group ID.
        uint getEntityGroupID ( REntityGroupType entityType,
                                        uint     entityID,
                                        bool             onlyElements = false ) const;

        //! Return list of entity group IDs for given entity type.
        std::vector<uint> getEntityGroupIDs ( REntityGroupType entityType ) const;

        QMap<REntityGroupType,RUVector> getEntityIDMap() const;

        //! Convert entity group ID to entity type and entity ID.
        bool getEntityID ( uint      groupID,
                           REntityGroupType &entityType,
                           uint     &entityID ) const;

        //! Return element ID for given entity ID and element position.
        uint findElementID(REntityGroupType entityType, uint entityID, uint elementPosition) const;

        //! Return node ID for given entity ID, element and node position.
        uint findNodeID(REntityGroupType entityType, uint entityID, uint elementPosition, uint nodePosition) const;

        //! Return list of element IDs for given list of entity IDs.
        std::vector<uint> findElementIDs(REntityGroupType entityType, const std::vector<uint> entityIDs) const;

        //! Merge entities.
        void mergeEntities(REntityGroupType entityType, const QList<uint> &entityIDs);

        //! Remove entities.
        void removeEntities(REntityGroupType entityType, const QList<uint> &entityIDs);

        /*************************************************************
         * Point interface                                           *
         *************************************************************/

        //! Return number of points.
        uint getNPoints ( void ) const;

        //! Set number of points.
        void setNPoints ( uint npoints );

        //! Return pointer to point in model at given position.
        const RPoint * getPointPtr ( uint position ) const;

        //! Return pointer to point in model at given position.
        RPoint * getPointPtr ( uint position );

        //! Return reference to point in model at given position.
        const RPoint & getPoint ( uint position ) const;

        //! Return reference to point in model at given position.
        RPoint & getPoint ( uint position );

        //! Add point to model.
        void addPoint ( const RPoint &point = RPoint() );

        //! Set point in model at given position.
        void setPoint ( uint  position,
                        const RPoint &point );

        //! Remove point from model at given position.
        void removePoint ( uint position );

        /*************************************************************
         * Line interface                                            *
         *************************************************************/

        //! Return number of lines.
        uint getNLines ( void ) const;

        //! Set number of lines.
        void setNLines ( uint nlines );

        //! Return pointer to line in model at given position.
        const RLine * getLinePtr ( uint position ) const;

        //! Return pointer to line in model at given position.
        RLine * getLinePtr ( uint position );

        //! Return reference to line in model at given position.
        const RLine & getLine ( uint position ) const;

        //! Return reference to line in model at given position.
        RLine & getLine ( uint position );

        //! Add line to model.
        void addLine ( const RLine &line );

        //! Set line in model at given position.
        void setLine ( uint  position,
                       const RLine  &line );

        //! Remove line from model at given position.
        void removeLine ( uint position );

        /*************************************************************
         * Surface interface                                         *
         *************************************************************/

        //! Return number of surfaces.
        uint getNSurfaces ( void ) const;

        //! Set number of surfaces.
        void setNSurfaces ( uint nsurfaces );

        //! Return pointer to surface in model at given position.
        const RSurface * getSurfacePtr ( uint position ) const;

        //! Return pointer to surface in model at given position.
        RSurface * getSurfacePtr ( uint position );

        //! Return reference to surface in model at given position.
        const RSurface & getSurface ( uint position ) const;

        //! Return ireference to surface in model at given position.
        RSurface & getSurface ( uint position );

        //! Add surface to model.
        void addSurface ( const RSurface &surface );

        //! Set surface in model at given position.
        void setSurface ( uint    position,
                          const RSurface &surface );

        //! Remove surface from model at given position.
        void removeSurface ( uint position );

        //! Check if given surface forms closed surface.
        bool checkIfSurfaceIsClosed(uint surfaceID) const;

        //! Check if given list of surfaces forms closed surface.
        bool checkIfSurfacesAreClosed(const QList<uint> &surfaceIDs) const;

        //! Synchronize surface normals.
        void syncSurfaceNormals();

        /*************************************************************
         * Volume interface                                          *
         *************************************************************/

        //! Return number of volumes.
        uint getNVolumes ( void ) const;

        //! Set number of volumes.
        void setNVolumes ( uint nvolumes );

        //! Return pointer to volume in model at given position.
        const RVolume * getVolumePtr ( uint position ) const;

        //! Return pointer to volume in model at given position.
        RVolume * getVolumePtr ( uint position );

        //! Return reference to volume in model at given position.
        const RVolume & getVolume ( uint position ) const;

        //! Return reference to volume in model at given position.
        RVolume & getVolume ( uint position );

        //! Add volume to model.
        void addVolume ( const RVolume &volume );

        //! Set volume in model at given position.
        void setVolume ( uint   position,
                         const RVolume &volume );

        //! Remove volume from model at given position.
        void removeVolume ( uint position );

        /*************************************************************
         * Vector field interface                                    *
         *************************************************************/

        //! Return number of vector fields.
        uint getNVectorFields ( void ) const;

        //! Set number of vector fields.
        void setNVectorFields ( uint nVectorFields );

        //! Return pointer to vector field in model at given position.
        const RVectorField * getVectorFieldPtr ( uint position ) const;

        //! Return pointer to vector field in model at given position.
        RVectorField * getVectorFieldPtr ( uint position );

        //! Return reference to vector field in model at given position.
        const RVectorField & getVectorField ( uint position ) const;

        //! Return reference to vector field in model at given position.
        RVectorField & getVectorField ( uint position );

        //! Add vector field to model.
        void addVectorField ( const RVectorField &vectorField );

        //! Set vector field in model at given position.
        void setVectorField ( uint        position,
                              const RVectorField &vectorField );

        //! Remove vector field from model at given position.
        void removeVectorField ( uint position );

        /*************************************************************
         * Scalar field interface                                  *
         *************************************************************/

        //! Return number of scalar fields.
        uint getNScalarFields ( void ) const;

        //! Set number of scalar fields.
        void setNScalarFields ( uint nScalarFields );

        //! Return pointer to scalar field in model at given position.
        const RScalarField * getScalarFieldPtr ( uint position ) const;

        //! Return pointer to scalar field in model at given position.
        RScalarField * getScalarFieldPtr ( uint position );

        //! Return reference to scalar field in model at given position.
        const RScalarField & getScalarField ( uint position ) const;

        //! Return reference to scalar field in model at given position.
        RScalarField & getScalarField ( uint position );

        //! Add scalar field to model.
        void addScalarField ( const RScalarField &scalarField );

        //! Set scalar field in model at given position.
        void setScalarField ( uint          position,
                                const RScalarField &scalarField );

        //! Remove scalar field from model at given position.
        void removeScalarField ( uint position );

        /*************************************************************
         * Stream line interface                                     *
         *************************************************************/

        //! Return number of stream lines.
        uint getNStreamLines ( void ) const;

        //! Set number of stream lines.
        void setNStreamLines ( uint nStreamLines );

        //! Return pointer to stream line in model at given position.
        const RStreamLine * getStreamLinePtr ( uint position ) const;

        //! Return pointer to stream line in model at given position.
        RStreamLine * getStreamLinePtr ( uint position );

        //! Return reference to stream line in model at given position.
        const RStreamLine & getStreamLine ( uint position ) const;

        //! Return reference to stream line in model at given position.
        RStreamLine & getStreamLine ( uint position );

        //! Add stream line to model.
        void addStreamLine ( const RStreamLine &streamLine );

        //! Set stream line in model at given position.
        void setStreamLine ( uint       position,
                              const RStreamLine &streamLine );

        //! Remove stream line from model at given position.
        void removeStreamLine ( uint position );

        /*************************************************************
         * Cut interface                                             *
         *************************************************************/

        //! Return number of cuts.
        uint getNCuts ( void ) const;

        //! Set number of cuts.
        void setNCuts ( uint nCuts );

        //! Return pointer to cut in model at given position.
        const RCut * getCutPtr ( uint position ) const;

        //! Return pointer to cut in model at given position.
        RCut * getCutPtr ( uint position );

        //! Return reference to cut in model at given position.
        const RCut & getCut ( uint position ) const;

        //! Return reference to cut in model at given position.
        RCut & getCut ( uint position );

        //! Add cut to model.
        void addCut ( const RCut &cut );

        //! Set cut in model at given position.
        void setCut ( uint  position,
                      const RCut   &cut );

        //! Remove cut from model at given position.
        void removeCut ( uint position );

        /*************************************************************
         * Iso interface                                             *
         *************************************************************/

        //! Return number of isos.
        uint getNIsos ( void ) const;

        //! Set number of isos.
        void setNIsos ( uint nIsos );

        //! Return pointer to iso in model at given position.
        const RIso * getIsoPtr ( uint position ) const;

        //! Return pointer to iso in model at given position.
        RIso * getIsoPtr ( uint position );

        //! Return reference to iso in model at given position.
        const RIso & getIso( uint position ) const;

        //! Return reference to iso in model at given position.
        RIso & getIso( uint position );

        //! Add iso to model.
        void addIso( const RIso &iso );

        //! Set iso in model at given position.
        void setIso( uint  position,
                      const RIso   &iso );

        //! Remove iso from model at given position.
        void removeIso( uint position );

        /*************************************************************
         * Model data interface                                      *
         *************************************************************/

        //! Return const reference to data object.
        inline const RModelData & getData( void ) const
        {
            return this->modelData;
        }

        //! Return reference to data object.
        inline RModelData & getData( void )
        {
            return this->modelData;
        }

        //! Set data object.
        inline void setData( const RModelData &modelData )
        {
            this->modelData = modelData;
        }

        /*************************************************************
         * Get results values interface                              *
         *************************************************************/

        //! Find variable for given entity type and ID.
        //! If no such variable can be found 0 is returned.
        const RVariable *findVariableByDisplayType(REntityGroupVariableDisplayTypeMask displayTypeMask, REntityGroupType entityType, uint entityID) const;

        //! Return vector of results vector for given element ID.
        //! If variable applies on nodes vector will have a size of number of nodes.
        //! If results vector is scalar then its size will be 1.
        std::vector<RRVector> getElementResultsValues( RVariableType variableType,
                                                       uint elementID ) const;

        //! Return vector of results vector for given node ID.
        //! If results vector is scalar then its size will be 1.
        RRVector getNodeResultsValues( RVariableType variableType,
                                       uint nodeID ) const;

        //! Return vector of results vectors for given interpolated element ID.
        //! If variable applies on nodes vector will have a size of number of nodes.
        //! If results vector is scalar then its size will be 1.
        //! This function returns valid results only for ISO and CUT entity group types.
        std::vector<RRVector> getInterpolatedElementResultsValues( RVariableType variableType,
                                                                   REntityGroupType entityGroupType,
                                                                   uint entityID,
                                                                   uint elementID ) const;

        //! Return results vector for given interpolated node ID.
        //! If results vector is scalar then its size will be 1.
        //! This function returns valid results only for ISO and CUT entity group types.
        RRVector getInterpolatedNodeResultsValues( RVariableType variableType,
                                                   REntityGroupType entityGroupType,
                                                   uint entityID,
                                                   uint elementID,
                                                   uint nodeID ) const;

        //! Return results vector for given node.
        //! If results vector is scalar then its size will be 1.
        //! If node is outside of the model empty vector is returned.
        RRVector getInterpolatedResultsValues( RVariableType variableType,
                                               const RNode &rNode,
                                               REntityGroupTypeMask entityGroup = R_ENTITY_GROUP_ELEMENT ) const;


        /*************************************************************
         * Geometry transformation                                   *
         *************************************************************/

        //! Rotate model.
        void rotateGeometry(const QSet<uint> &nodeIDs,
                            const RR3Vector &rotationVector,
                            const RR3Vector &rotationCenter);

        //! Scale model.
        void scaleGeometry(const QSet<uint> &nodeIDs,
                           const RR3Vector &scaleVector,
                           const RR3Vector &scaleCenter);

        //! Scale model.
        void scaleGeometry(double scaleFactor);

        //! Translate model.
        void translateGeometry(const QSet<uint> &nodeIDs,
                               const RR3Vector &translateVector);


        /*************************************************************
         * Other methods                                             *
         *************************************************************/

        //! Return true if model is empty.
        inline bool isEmpty( void ) const
        {
            return (this->getNNodes() == 0);
        }

        //! Check mesh consistency.
        RModelProblemTypeMask checkMesh(bool printOutput = true) const;

        //! Return book vector of edge nodes.
        QVector<bool> findEdgeNodes( void ) const;

        //! Sort given list of line elements into continuous chain.
        //! Elements which are not in the chain are not listed in returned index list.
        QList<uint> sortLineElements(const QList<RElement> &edges, uint firstID = RConstants::eod) const;

        //! Return list of nodes forming ring around node made of edge elements form elements which contain given node.
        QList<uint> findNodeEdgeRing(uint nodeID) const;

        //! Return minimum distance between two nodes.
        double findMinimumNodeDistance() const;

        //! Create interpolated entity from plane and list of element IDs.
        void createCut( RCut &rCut ) const;

        //! Create interpolated entity from variable type, variable value and list of element IDs.
        void createIso( RIso &rIso ) const;

        //! Create interpolated entity from variable type, variable value.
        void createStreamLine( RStreamLine &rStreamLine ) const;

        //! Recreate dependent entities such as cuts or isos.
        void createDependentEntities( void );

        //! Return neighbor ID at given position.
        //! If no neighbor is found RConstants::eod is returned.
        uint getNeighbor( uint elementID,
                          uint neighborPosition ) const;

        //! Return pointer to vector of neighbor IDs.
        //! If element has no neighbors assigned NULL pointer is returned.
        const std::vector<uint> * getNeighborIDs( uint elementID ) const;

        //! Set surface neighbors book.
        void setSurfaceNeighbors(const std::vector<RUVector> &surfaceNeigs);

        //! Set volume neighbors book.
        void setVolumeNeighbors(const std::vector<RUVector> &volumeNeigs);

        //! Clear surface neighbors book.
        void clearSurfaceNeighbors();

        //! Clear volume neighbors book.
        void clearVolumeNeighbors();

        //! Fix sliver elements.
        //! Return number of affected elements.
        uint fixSliverElements(double edgeRatio);

        //! Fix element to group relations.
        //! Move elements from inapropriate to apropriate groups.
        //! Return number of affected elements.
        uint fixElementGroupRelations();

        //! Find list of sliver elements.
        QList<uint> findSliverElements(double edgeRatio) const;

        //! Find list of intersected elements.
        QList<uint> findIntersectedElements() const;

        //! Break intersected elements.
        uint breakIntersectedElements(uint nIterations);

        //! Break intersected elements.
        uint breakIntersectedElements(uint nIterations, const std::vector<uint> &elementIDs);

        //! Perform boolean difference operation.
        //! Return true if successful.
        bool boolDifference(uint nIterations, QList<uint> surfaceEntityIDs, uint cuttingSurfaceEntityId);

        //! Perform boolean intersection operation.
        //! entityIDs specifies global entity ID.
        //! Return true if successful.
        bool boolIntersection(uint nIterations, QList<uint> surfaceEntityIDs);

        //! Perform boolean union operation.
        //! Return true if successful.
        bool boolUnion(uint nIterations, QList<uint> surfaceEntityIDs);

        //! Coarsen surface elements.
        uint coarsenSurfaceElements(const std::vector<uint> surfaceIDs, double edgeLength, double elementArea);

        //! Tetrahedralize surface.
        uint tetrahedralizeSurface(const std::vector<uint> surfaceIDs);

        //! Generate vector of mesh size values for each node.
        RRVector generateMeshSizeFunction(const QSet<RVariableType> variableTypes, double minValue, double maxValue, double trimValueRatio) const;

        //! Generate input parameters for TetGen mesh generator.
        QString generateMeshTetGenInputParams(const RMeshInput &meshInput) const;

        //! Generate patch surface.
        void generatePatchSurface(const std::vector<RPatchInput> &patchInput, RPatchBook &book) const;

        //! Generate patch input vector.
        void generatePatchInputVector(std::vector<RPatchInput> &patchInput) const;

        //! Find patch center.
        void findPatchCenter(const RPatch &rPatch, double &cx, double &cy, double &cz) const;

        //! Find patch normal.
        void findPatchNormal(const RPatch &rPatch, double &nx, double &ny, double &nz) const;

        //! Find patch area.
        void findPatchArea(const RPatch &rPatch, double area) const;

        //! Find most recent view factor file.
        QString findRecentViewFactorMatrixFile() const;

        //! Generate view-factor matrix header from problem data.
        void generateViewFactorMatrixHeader(RViewFactorMatrixHeader &viewFactorMatrixHeader) const;

        //! Write view-factor matrix to file.
        QString writeViewFactorMatrix(const RViewFactorMatrix &viewFactorMatrix, const QString &fileName) const;

        //! Generate default boundary condition.
        RBoundaryCondition generateDefaultBoundayCondition(RBoundaryConditionType type, REntityGroupType entityGroupType, uint entityID) const;

        //! Generate next entity name.
        QString generateNextEntityName(REntityGroupType groupType, const QString &suggestedName = QString()) const;

        //! Fill element vector to node vector values.
        void convertElementToNodeVector(const RRVector &elementValues,
                                        const RBVector &setValues,
                                        RRVector &nodeValues,
                                        bool onlySetValues = false) const;

        //! Fill node vector to element vector values.
        void convertNodeToElementVector(const RRVector &nodeValues,
                                        RRVector &elementValues);

        //! Clear boundary conditions.
        void clearBoundaryConditions();

        //! Clear initial conditions.
        void clearInitialConditions();

        //! Clear environment conditions.
        void clearEnvironmentConditions();

        //! Clear entity variable data.
        void clearEntityVariableData();

        //! Clear entity variable data from given variable type.
        void clearEntityVariableData(RVariableType variableType);


        /*************************************************************
         * Static methods                                            *
         *************************************************************/

        //! Return default file extension.
        static QString getDefaultFileExtension ( bool binary = true );

        //! Write link file.
        static void writeLink ( const QString &linkFileName,
                                const QString &targetFileName );

    protected:

        //! Read from the ASCII file.
        //! If file is a link target filename is returned.
        QString readAscii ( const QString &fileName );

        //! Read from the binary file.
        //! If file is a link target filename is returned.
        QString readBinary ( const QString &fileName );

        //! Write to the ASCII file.
        void writeAscii ( const QString &fileName ) const;

        //! Write to the binary file.
        void writeBinary ( const QString &fileName ) const;

    protected:

        //! Find surface neighbors book.
        std::vector<RUVector> findSurfaceNeighbors() const;

        //! Find volume neighbors book.
        std::vector<RUVector> findVolumeNeighbors() const;

        //! Find volume elements neighbor position.
        uint findVolumeNeighborPosition(uint elementID, uint neighborID) const;

        //! Internal function to mark surfaces neighbors with the same mark as given surface.
        void markSurfaceNeighbors(uint                         elementID,
                                  double                       angle,
                                  const std::vector<RUVector> &neighbors,
                                  std::vector<uint>           &marks);

        //! Add group ID reference.
        //! Increase all group ID references +1 if equal or grater than groupID.
        void addEntityGroupIdReference(uint entityGroupId);

        //! Remove all references to group ID.
        void removeEntityGroupIdReference(uint entityGroupId);

        //! Update all references based on differences in ID maps.
        void updateEntityGroupIdReferences(const QMap<REntityGroupType,RUVector> oldMap);

        //! Generate element neighbor distance vector.
        //! Stop criteria are:
        //!  1. maximum distance from starting element in number of steps
        //!  2. separation angle between two elements in radians.
        void generateElementDistanceVector( uint                   startElementID,
                                            uint                   maximumDistance,
                                            double                 separationAngle,
                                            RDistanceVector<uint> &distanceVector ) const;

        //! Find near node to the given node.
        //! If no node was found a RConstants::eod is returned.
        //! If findNearest is set to true algorithm will find the nearest node. Otherwise first satisfiyng will be returned.
        //! If nodeID is provided node with such ID will be ignored.
        uint findNearNode(const RNode &node, double tolerance = 0.0, bool findNearest = false, uint nodeID = RConstants::eod) const;

};

#endif /* RML_MODEL_H */
