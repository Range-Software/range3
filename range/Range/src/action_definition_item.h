/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   action_definition_item.h                                 *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   10-th September 2014                                     *
 *                                                                   *
 *  DESCRIPTION: Action definition item class declaration            *
 *********************************************************************/

#ifndef ACTION_DEFINITION_ITEM_H
#define ACTION_DEFINITION_ITEM_H

#include <QString>

#define ACTION_TYPE_IS_VALID(_type) \
            ( _type >= ACTION_NONE && _type < ACTION_N_TYPES )

#define ACTION_TYPE_IS_ACTION(_type) \
            ( _type > ACTION_NONE && _type < ACTION_N_TYPES && _type != ACTION_SEPARATOR )

typedef enum _ActionGroupType
{
    ACTION_GROUP_NONE        = 0,
    ACTION_GROUP_SESSION     = 1 << 0,
    ACTION_GROUP_MODEL       = 1 << 1,
    ACTION_GROUP_MATERIAL    = 1 << 2,
    ACTION_GROUP_GEOMETRY    = 1 << 3,
    ACTION_GROUP_PROBLEM     = 1 << 4,
    ACTION_GROUP_SOLVER      = 1 << 5,
    ACTION_GROUP_REPORT      = 1 << 6,
    ACTION_GROUP_APPLICATION = 1 << 7
} ActionGroupType;

#define ACTION_GROUP_ALL (     \
    ACTION_GROUP_SESSION     | \
    ACTION_GROUP_MODEL       | \
    ACTION_GROUP_MATERIAL    | \
    ACTION_GROUP_GEOMETRY    | \
    ACTION_GROUP_PROBLEM     | \
    ACTION_GROUP_SOLVER      | \
    ACTION_GROUP_REPORT      | \
    ACTION_GROUP_APPLICATION   \
    )

typedef int ActionGroupTypeMask;

typedef enum _ActionType
{
    ACTION_NONE = 0,
    ACTION_SESSION_NEW,
    ACTION_SESSION_OPEN,
    ACTION_SESSION_SAVE,
    ACTION_SESSION_SAVE_AS,
    ACTION_SESSION_CLOSE,
    ACTION_MODEL_NEW,
    ACTION_MODEL_OPEN,
    ACTION_MODEL_SAVE,
    ACTION_MODEL_SAVE_AS,
    ACTION_MODEL_EXPORT_MSH,
    ACTION_MODEL_EXPORT_RAW,
    ACTION_MODEL_EXPORT_STL_ASCII,
    ACTION_MODEL_EXPORT_STL_BINARY,
    ACTION_MODEL_CLOSE,
    ACTION_MODEL_RELOAD_RESULTS,
    ACTION_MODEL_DROP_RESULTS,
    ACTION_MODEL_RENAME,
    ACTION_MATERIAL_NEW,
    ACTION_MATERIAL_IMPORT,
    ACTION_MATERIAL_DELETE,
    ACTION_GEOMETRY_UNDO,
    ACTION_GEOMETRY_REDO,
    ACTION_GEOMETRY_DRAW_POINT,
    ACTION_GEOMETRY_DRAW_LINE,
    ACTION_GEOMETRY_DRAW_TRIANGLE,
    ACTION_GEOMETRY_DRAW_QUADRILATERAL,
    ACTION_GEOMETRY_DRAW_CIRCLE,
    ACTION_GEOMETRY_DRAW_ELLIPSE,
    ACTION_GEOMETRY_DRAW_TETRAHEDRON,
    ACTION_GEOMETRY_DRAW_HEXAHEDRON,
    ACTION_GEOMETRY_DRAW_CYLINDER,
    ACTION_GEOMETRY_DRAW_SPHERE,
    ACTION_GEOMETRY_DRAW_RAW,
    ACTION_GEOMETRY_CREATE_ELEMENT,
    ACTION_GEOMETRY_FIND_SLIVER_ELEMENTS,
    ACTION_GEOMETRY_FIX_SLIVER_ELEMENTS,
    ACTION_GEOMETRY_FIND_INTERSECTED_ELEMENTS,
    ACTION_GEOMETRY_BREAK_INTERSECTED_ELEMENTS,
    ACTION_GEOMETRY_BOOL_UNION,
    ACTION_GEOMETRY_BOOL_DIFFERENCE,
    ACTION_GEOMETRY_BOOL_INTERSECTION,
    ACTION_GEOMETRY_MERGE_NEAR_NODES,
    ACTION_GEOMETRY_MOVE_NODE,
    ACTION_GEOMETRY_REMOVE_NODE,
    ACTION_GEOMETRY_REMOVE_NODE_AND_CLOSE_HOLE,
    ACTION_GEOMETRY_REMOVE_ELEMENT,
    ACTION_GEOMETRY_LINE_GENERATE_FROM_EDGES,
    ACTION_GEOMETRY_SURFACE_MARK,
    ACTION_GEOMETRY_SURFACE_SWAP_ELEMENT_NORMAL,
    ACTION_GEOMETRY_SURFACE_SWAP_NORMALS,
    ACTION_GEOMETRY_SURFACE_SYNC_NORMALS,
    ACTION_GEOMETRY_SURFACE_CLOSE_HOLE,
    ACTION_GEOMETRY_SURFACE_COARSEN,
    ACTION_GEOMETRY_VOLUME_GENERATE_TETRAHEDRA,
    ACTION_GEOMETRY_VECTOR_FIELD_CREATE,
    ACTION_GEOMETRY_VECTOR_FIELD_EDIT,
    ACTION_GEOMETRY_SCALAR_FIELD_CREATE,
    ACTION_GEOMETRY_SCALAR_FIELD_EDIT,
    ACTION_GEOMETRY_STREAM_LINE_CREATE,
    ACTION_GEOMETRY_STREAM_LINE_EDIT,
    ACTION_GEOMETRY_CUT_CREATE,
    ACTION_GEOMETRY_CUT_EDIT,
    ACTION_GEOMETRY_ISO_CREATE,
    ACTION_GEOMETRY_ISO_EDIT,
    ACTION_GEOMETRY_ENTITY_MERGE,
    ACTION_GEOMETRY_ENTITY_REMOVE,
    ACTION_GEOMETRY_TRANSFORM,
    ACTION_GEOMETRY_DEV_EXPORT_SLIVER_ELEMENTS,
    ACTION_GEOMETRY_DEV_EXPORT_INTERSECTED_ELEMENTS,
    ACTION_GEOMETRY_DEV_PURGE_UNUSED_NODES,
    ACTION_GEOMETRY_DEV_PURGE_UNUSED_ELEMENTS,
    ACTION_GEOMETRY_DEV_REMOVE_DUPLICATE_NODES,
    ACTION_GEOMETRY_DEV_REMOVE_DUPLICATE_ELEMENTS,
    ACTION_GEOMETRY_DEV_POINT_INSIDE_SURFACE,
    ACTION_GEOMETRY_DEV_TETRAHEDRALIZE_SURFACE,
    ACTION_GEOMETRY_DEV_CONSOLIDATE,
    ACTION_PROBLEM_TASK_FLOW,
    ACTION_PROBLEM_SOLVER_SETUP,
    ACTION_PROBLEM_DEFINE_MONITORING_POINTS,
    ACTION_PROBLEM_RESET,
    ACTION_SOLVER_START,
    ACTION_SOLVER_STOP,
    ACTION_SOLVER_KILL,
    ACTION_REPORT_MODEL_STATISTICS,
    ACTION_REPORT_SOLVER_LOG,
    ACTION_REPORT_CONVERGENCE_GRAPH,
    ACTION_REPORT_MONITORING_POINT_GRAPH,
    ACTION_REPORT_CREATE_REPORT,
    ACTION_APPLICATION_SETTINGS,
    ACTION_APPLICATION_UPDATE,
    ACTION_APPLICATION_ABOUT,
    ACTION_APPLICATION_ABOUT_QT,
    ACTION_APPLICATION_LICENSE,
    ACTION_APPLICATION_RELEASE_NOTES,
    ACTION_APPLICATION_HELP,
    ACTION_APPLICATION_QUIT,
    ACTION_SEPARATOR,
    ACTION_N_TYPES
} ActionType;

class Action;

typedef void (Action::*PointerToMemberFunction)();

class ActionDefinitionItem
{

    protected:

        ActionType type;
        ActionGroupType groupType;
        QString text;
        QString desc;
        QString shortCut;
        QString icon;
        PointerToMemberFunction slot;

    private:

        //! Internal initialization function.
        void _init(const ActionDefinitionItem *pActionDefinitionItem = nullptr);

    public:

        //! Constructor.
        ActionDefinitionItem(ActionType type = ACTION_NONE,
                             ActionGroupType groupType = ACTION_GROUP_NONE,
                             const QString &text = QString(),
                             const QString &desc = QString(),
                             const QString &shortCut = QString(),
                             const QString &icon = QString(),
                             PointerToMemberFunction slot = nullptr);

        //! Copy constructor.
        ActionDefinitionItem(const ActionDefinitionItem &actionDefinitionItem);

        //! Destructor.
        ~ActionDefinitionItem();

        //! Assignment operator.
        ActionDefinitionItem &operator =(const ActionDefinitionItem &actionDefinitionItem);

        //! Return action type.
        ActionType getType() const;

        //! Return action group type.
        ActionGroupType getGroupType() const;

        //! Return const reference to shortcut.
        const QString &getShortcut() const;

        //! Return action group name.
        static QString getGroupName(ActionGroupType groupType);

        //! Return list of group types.
        static QList<ActionGroupType> getGroupTypes(ActionGroupTypeMask groupTypeMask = ACTION_GROUP_ALL);

        //! ActionDefinition is a container class and need to have an access to protected members.
        friend class ActionDefinition;

};

#endif // ACTION_DEFINITION_ITEM_H
