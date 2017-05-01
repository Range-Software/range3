/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_entity_list.h                                         *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   20-th March 2012                                         *
 *                                                                   *
 *  DESCRIPTION: OpenGL entity list class declaration                *
 *********************************************************************/

#ifndef __GL_ENTITY_LIST_H__
#define __GL_ENTITY_LIST_H__

#include "gl_list.h"

typedef enum _GLEntityListItemType
{
    GL_ENTITY_LIST_ITEM_NORMAL = 0,
    GL_ENTITY_LIST_ITEM_PICK_ELEMENT,
    GL_ENTITY_LIST_ITEM_PICK_NODE,
    GL_ENTITY_LIST_ITEM_N_LISTS
} GLEntityListItemType;

class GLEntityList : public GLList
{

    private:

        //! Internal initialization function.
        void _init (const GLEntityList *pGlEntityList = 0);

    public:

        //! Constructor.
        GLEntityList();

        //! Copy constructor.
        GLEntityList(const GLEntityList &glEntityList);

        //! Destructor.
        ~GLEntityList();

        //! Assignment operator.
        GLList & operator = (const GLEntityList &glEntityList);
};

#endif /* __GL_ENTITY_LIST_H__ */
