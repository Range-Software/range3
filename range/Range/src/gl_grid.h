/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_grid.h                                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   27-th September 2020                                     *
 *                                                                   *
 *  DESCRIPTION: OpenGL grid class declaration                       *
 *********************************************************************/

#ifndef GL_GRID_H
#define GL_GRID_H

#include "gl_object.h"

class GLGrid : public GLObject
{

    private:

        // Environment settings.

    protected:

        double scale;
        RLimitBox limitBox;
        double gMin;
        double gMax;
        double gdt;

    private:

        //! Internal initialization function.
        void _init(const GLGrid *pGlGrid = nullptr);

    public:

        //! Constructor.
        explicit GLGrid(GLWidget *glWidget,
                        double scale,
                        const RLimitBox &limitBox);

        //! Copy constructor.
        GLGrid(const GLGrid &glGrid);

        //! Destructor.
        virtual ~GLGrid() {}

        //! Assignment operator.
        GLGrid &operator=(const GLGrid &glGrid);

    protected:

        //! Initialize scene.
        void initialize(void);

        //! Finalize scene.
        void finalize(void);

        //! Draw scene.
        void draw(void);

};

#endif // GL_GRID_H
