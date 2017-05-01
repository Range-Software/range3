/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_mesh_generator.h                                     *
 *  GROUP:  RModel                                                   *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   5-th August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: RMeshGenerator class declaration                    *
 *********************************************************************/

#ifndef __RML_MESH_GENERATOR_H__
#define __RML_MESH_GENERATOR_H__

#include "rml_mesh_input.h"
#include "rml_model.h"
#include "rml_triangle.h"
#include "rml_tetrahedron.h"

class RMeshGenerator
{

    private:

        //! Constructor.
        RMeshGenerator() {}

    public:

        //! Generate volume mesh.
        static void generate(const RMeshInput &meshInput, RModel &model);

        //! Generate volume mesh from triangulated surface.
        static std::vector<RElement> generate(const std::vector<RNode> &nodes, const std::vector<RElement> &elements);

        //! Tetrahedralize triangulated surface.
        static std::vector<RTetrahedron> generate(const std::vector<RTriangle> &triangles);

};

#endif // __RML_MESH_GENERATOR_H__
