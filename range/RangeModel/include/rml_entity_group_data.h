/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_entity_group_data.h                                  *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   17-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Entity group data class declaration                *
 *********************************************************************/

#ifndef RML_ENTITY_GROUP_DATA_H
#define RML_ENTITY_GROUP_DATA_H

#include <map>

#include "rml_variable.h"

#define R_ENTITY_GROUP_VARIABLE_TYPE_IS_VALID(_type) \
( \
    _type >= R_ELEMENT_GROUP_VARIABLE_DISPLAY_SCALAR && \
    _type < R_ELEMENT_GROUP_VARIABLE_DISPLAY_N_TYPES \
)

typedef enum _REntityGroupVariableDisplayType
{
    R_ENTITY_GROUP_VARIABLE_DISPLAY_NONE         = 0,
    R_ENTITY_GROUP_VARIABLE_DISPLAY_SCALAR       = 1 << 0,
    R_ENTITY_GROUP_VARIABLE_DISPLAY_DISPLACEMENT = 1 << 1
} REntityGroupVariableDisplayType;

typedef int REntityGroupVariableDisplayTypeMask;

class REntityGroupData
{
    private:

        //! Internal initialization function.
        void _init ( const REntityGroupData *pElementGroupData = nullptr );

    protected:

        //! Entity is selected.
        bool selected;
        //! Entity is visible.
        bool visible;
        //! Entity color (r,g,b,a).
        int rgba[4];
        //! Display entity as wireframe model.
        bool drawWire;
        //! Display entity element edges.
        bool drawEdges;
        //! Display entity element nodes.
        bool drawNodes;
        //! Display entity element numbers.
        bool drawElementNumbers;
        //! Display entity node numbers.
        bool drawNodeNumbers;
        //! Draw arrow heads.
        bool drawArrowHeads;
        //! Draw arrows with equal length.
        bool drawEqualArrowLength;
        //! Draw arrow heads from (if false arrows will point at).
        bool drawArrowFrom;
        //! Color by element patch.
        bool colorByPatch;
        //! Color by view-factor.
        bool colorByViewFactor;
        //! Vector of results variables data
        std::map<RVariableType,REntityGroupVariableDisplayTypeMask> variableData;

    public:

        //! Constructor.
        REntityGroupData();

        //! Copy constructor.
        REntityGroupData(const REntityGroupData &elementGroupData);

        //! Destructor.
        ~REntityGroupData();

        //! Assignment operator.
        REntityGroupData & operator = (const REntityGroupData &elementGroupData);

        //! Return whether the entity is selected or not.
        bool getSelected(void) const;

        //! Set whether the entity is selected.
        void setSelected(bool selected);

        //! Return whether the entity is visible or not.
        bool getVisible(void) const;

        //! Set whether the entity is visible.
        void setVisible(bool visible);

        //! Return rgba color.
        void getColor(int &r, int &g, int &b, int &a) const;

        //! Set rgba color.
        void setColor(int r, int g, int b, int a = 255);

        //! Return information on whether the entity should be drawn as a wireframe model.
        bool getDrawWire(void) const;

        //! Set whether the entity should be drawn as a wireframe model.
        void setDrawWire(bool drawWire);

        //! Return information on whether the entity element edges should be drawn.
        bool getDrawEdges(void) const;

        //! Set whether the entity element edges should be drawn.
        void setDrawEdges(bool drawEdges);

        //! Return information on whether the entity element nodes should be drawn.
        bool getDrawNodes(void) const;

        //! Set whether the entity element nodes should be drawn.
        void setDrawNodes(bool drawNodes);

        //! Return information on whether the entity element numbers should be drawn.
        bool getDrawElementNumbers(void) const;

        //! Set whether the entity element numbers should be drawn.
        void setDrawElementNumbers(bool drawElementNumbers);

        //! Return information on whether the entity node numbers should be drawn.
        bool getDrawNodeNumbers(void) const;

        //! Set whether the entity node numbers should be drawn.
        void setDrawNodeNumbers(bool drawNodeNumbers);

        //! Return information on whether the entity arrow heads should be drawn.
        bool getDrawArrowHeads(void) const;

        //! Set whether the entity arrow heads should be drawn.
        void setDrawArrowHeads(bool drawArrowHeads);

        //! Return information on whether the entity arrow lengths should be drawn equal.
        bool getDrawEqualArrowLength(void) const;

        //! Set whether the entity arrow lengths should be drawn equal.
        void setDrawEqualArrowLength(bool drawEqualArrowLength);

        //! Return information on whether the arrow should be drawn from.
        bool getDrawArrowFrom(void) const;

        //! Set whether the whether the arrow should be drawn from.
        void setDrawArrowFrom(bool drawArrowFrom);

        //! Return information on whether the entity should be colored by patch ID.
        bool getColorByPatch(void) const;

        //! Set whether the entity should be colored by patch ID.
        void setColorByPatch(bool colorByPatch);

        //! Return information on whether the entity should be colored by view-factor.
        bool getColorByViewFactor(void) const;

        //! Set whether the entity should be colored by view-factor.
        void setColorByViewFactor(bool colorByViewFactor);

        //! Return const reference to vector of results variable data.
        const std::map<RVariableType,REntityGroupVariableDisplayTypeMask> & getVariableData(void) const;

        //! Return reference to vector of results variable data.
        std::map<RVariableType,REntityGroupVariableDisplayTypeMask> & getVariableData(void);

        //! Return set vector of results variable data.
        void setVariableData(std::map<RVariableType,REntityGroupVariableDisplayTypeMask> &variableData);

        //! Find variable type by display type.
        RVariableType findVariableByDisplayType(REntityGroupVariableDisplayTypeMask displayType) const;

        //! Return variable display name.
        static const QString & getVariableDisplayName ( REntityGroupVariableDisplayTypeMask type );

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif /* RML_ENTITY_GROUP_DATA_H */
