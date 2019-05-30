/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_vector_field.h                                        *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   15-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: OpenGL vector field class declaration               *
 *********************************************************************/

#ifndef GL_VECTOR_FIELD_H
#define GL_VECTOR_FIELD_H

#include <rmlib.h>

#include "gl_object.h"
#include "gl_texture.h"
#include "model.h"

class VectorFieldItem;

class GLVectorField : public GLObject, public RVectorField
{

    protected:

        //! entity ID.
        SessionEntityID entityID;
        // Environment settings
        GLboolean lightingEnabled;
        GLboolean normalize;
        GLfloat lineWidth;

    private:

        //! Internal initialization function.
        void _init(const GLVectorField *pGlVectorField = nullptr);

    public:

        //! Constructor.
        GLVectorField(GLWidget *glWidget, const RVectorField &vectorFied, const SessionEntityID &entityID);

        //! Copy constructor.
        GLVectorField(const GLVectorField &glVectorField);

        //! Destructor.
        virtual ~GLVectorField() {}

        //! Assignment operator.
        GLVectorField & operator = (const GLVectorField &glVectorField);

        //! Returtn entity ID.
        const SessionEntityID & getEntityID(void) const;

    protected:

        //! Initialize scene.
        void initialize(void);

        //! Finalize scene.
        void finalize(void);

        //! Draw scene.
        void draw(void);

        //! Calculate the vector field.
        std::vector<VectorFieldItem> calculateField(const RVariable *pScalarVariable, const RVariable *pDisplacementVariable) const;

        //! Return vector of scalar variable values.
        std::vector<double> getScalarValues(uint elementID, RVariableApplyType variableApplyType, const RVariable *pVariable) const;

        //! Return vector of displacement variable values.
        std::vector<RR3Vector> getDisplacementValues(uint elementID, RVariableApplyType variableApplyType, const RVariable *pVariable) const;

};

#endif // GL_VECTOR_FIELD_H
