/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_mesh_generator.h                                     *
 *  GROUP:  RModel                                                   *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   5-th August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: RMeshGenerator class declaration                    *
 *********************************************************************/

#ifndef RML_MESH_GENERATOR_H
#define RML_MESH_GENERATOR_H

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
        static void generate(const std::vector<RNode> &nodes, const std::vector<RElement> &elements, std::vector<RNode> &steinerNodes, std::vector<RElement> &volumeElements);

        //! Tetrahedralize triangulated surface.
        static std::vector<RTetrahedron> generate(const std::vector<RTriangle> &triangles);

};

#endif // RML_MESH_GENERATOR_H
