/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_file_io.h                                            *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   21-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: File IO operation functions declarations            *
 *********************************************************************/

#ifndef RML_FILE_IO_H
#define RML_FILE_IO_H

#include <rblib.h>

#include "rml_file.h"
#include "rml_save_file.h"

#include "rml_boundary_condition.h"
#include "rml_condition.h"
#include "rml_condition_component.h"
#include "rml_cut.h"
#include "rml_environment_condition.h"
#include "rml_gl_display_properties.h"
#include "rml_initial_condition.h"
#include "rml_file_header.h"
#include "rml_iso.h"
#include "rml_line.h"
#include "rml_material_property.h"
#include "rml_material.h"
#include "rml_mesh_setup.h"
#include "rml_modal_setup.h"
#include "rml_model_data.h"
#include "rml_monitoring_point.h"
#include "rml_monitoring_point_manager.h"
#include "rml_node.h"
#include "rml_element.h"
#include "rml_element_group.h"
#include "rml_entity_group.h"
#include "rml_entity_group_data.h"
#include "rml_scalar_field.h"
#include "rml_patch.h"
#include "rml_patch_book.h"
#include "rml_patch_input.h"
#include "rml_point.h"
#include "rml_problem_task_item.h"
#include "rml_sparse_vector.h"
#include "rml_stream_line.h"
#include "rml_surface.h"
#include "rml_volume.h"
#include "rml_variable.h"
#include "rml_vector_field.h"
#include "rml_view_factor_matrix_header.h"
#include "rml_view_factor_row.h"

class RFileIO
{
    private:

        //! Private constructor.
        explicit RFileIO () {}

    public:

        //! Write new line character.
        static void writeNewLineAscii(RSaveFile &outFile);
        static void writeNewLineAscii(RFile &outFile);

        // bool

        //! Read boolean value.
        static void readAscii(RFile &inFile, bool &bValue);
        //! Read boolean value.
        static void readBinary(RFile &inFile, bool &bValue);
        //! Write boolean value.
        static void writeAscii(RSaveFile &outFile, const bool &bValue, bool addNewLine = true);
        //! Write boolean value.
        static void writeBinary(RSaveFile &outFile, const bool &bValue);

        // char

        //! Read char value.
        static void readAscii(RFile &inFile, char &cValue);
        //! Read char value.
        static void readBinary(RFile &inFile, char &cValue);
        //! Write char value.
        static void writeAscii(RSaveFile &outFile, const char &cValue, bool addNewLine = true);
        static void writeAscii(RFile &outFile, const char &cValue, bool addNewLine = true);
        //! Write char value.
        static void writeBinary(RSaveFile &outFile, const char &cValue);

        // int

        //! Read int value.
        static void readAscii(RFile &inFile, int &iValue);
        //! Read int value.
        static void readBinary(RFile &inFile, int &iValue);
        //! Write int value.
        static void writeAscii(RSaveFile &outFile, const int &iValue, bool addNewLine = true);
        //! Write int value.
        static void writeBinary(RSaveFile &outFile, const int &iValue);

        // unsigned int

        //! Read unsigned int value.
        static void readAscii(RFile &inFile, unsigned int &uValue);
        //! Read unsigned int value.
        static void readBinary(RFile &inFile, unsigned int &uValue);
        //! Write unsigned int value.
        static void writeAscii(RSaveFile &outFile, const unsigned int &uValue, bool addNewLine = true);
        static void writeAscii(RFile &outFile, const unsigned int &uValue, bool addNewLine = true);
        //! Write unsigned int value.
        static void writeBinary(RSaveFile &outFile, const unsigned int &uValue);

        // double

        //! Read double value.
        static void readAscii(RFile &inFile, double &dValue);
        //! Read double value.
        static void readBinary(RFile &inFile, double &dValue);
        //! Write double value.
        static void writeAscii(RSaveFile &outFile, const double &dValue, bool addNewLine = true);
        static void writeAscii(RFile &outFile, const double &dValue, bool addNewLine = true);
        //! Write double value.
        static void writeBinary(RSaveFile &outFile, const double &dValue);

        // QString

        //! Read string value.
        static void readAscii(RFile &inFile, QString &sValue);
        //! Read string value.
        static void readBinary(RFile &inFile, QString &sValue);
        //! Write string value.
        static void writeAscii(RSaveFile &outFile, const QString &sValue, bool addNewLine = true);
        static void writeAscii(RFile &outFile, const QString &sValue, bool addNewLine = true);
        //! Write string value.
        static void writeBinary(RSaveFile &outFile, const QString &sValue);

        // std::string

        //! Read string value.
        static void readAscii(RFile &inFile, std::string &sValue);
        //! Read string value.
        static void readBinary(RFile &inFile, std::string &sValue);
        //! Write string value.
        static void writeAscii(RSaveFile &outFile, const char *sValue, bool addNewLine = true);
        //! Write string value.
        static void writeAscii(RSaveFile &outFile, const std::string &sValue, bool addNewLine = true);
        //! Write string value.
        static void writeBinary(RSaveFile &outFile, const char *sValue);
        //! Write string value.
        static void writeBinary(RSaveFile &outFile, const std::string &sValue);

        // RIVector

        //! Read RIVector.
        static void readAscii(RFile &inFile, RIVector &iVector, bool readSize = true);
        //! Read RIVector.
        static void readBinary(RFile &inFile, RIVector &iVector, bool readSize = true);
        //! Write RIVector.
        static void writeAscii(RSaveFile &outFile, const RIVector &iVector, bool writeSize = true, bool addNewLine = true);
        //! Write RIVector.
        static void writeBinary(RSaveFile &outFile, const RIVector &iVector, bool writeSize = true);

        // RUVector

        //! Read RUVector.
        static void readAscii(RFile &inFile, RUVector &uVector, bool readSize = true);
        //! Read RUVector.
        static void readBinary(RFile &inFile, RUVector &uVector, bool readSize = true);
        //! Write RUVector.
        static void writeAscii(RSaveFile &outFile, const RUVector &uVector, bool writeSize = true, bool addNewLine = true);
        //! Write RUVector.
        static void writeBinary(RSaveFile &outFile, const RUVector &uVector, bool writeSize = true);

        // RRVector

        //! Read RRVector.
        static void readAscii(RFile &inFile, RRVector &rVector, bool readSize);
        //! Read RRVector.
        static void readBinary(RFile &inFile, RRVector &rVector, bool readSize);
        //! Write RRVector.
        static void writeAscii(RSaveFile &outFile, const RRVector &rVector, bool writeSize, bool addNewLine = true);
        static void writeAscii(RFile &outFile, const RRVector &rVector, bool writeSize, bool addNewLine = true);
        //! Write RRVector.
        static void writeBinary(RSaveFile &outFile, const RRVector &rVector, bool writeSize);

        // RR3Vector

        //! Read RR3Vector.
        static void readAscii(RFile &inFile, RR3Vector &rVector);
        //! Read RR3Vector.
        static void readBinary(RFile &inFile, RR3Vector &rVector);
        //! Write RR3Vector.
        static void writeAscii(RSaveFile &outFile, const RR3Vector &rVector, bool addNewLine = true);
        static void writeAscii(RFile &outFile, const RR3Vector &rVector, bool addNewLine = true);
        //! Write RR3Vector.
        static void writeBinary(RSaveFile &outFile, const RR3Vector &rVector);

        // RIMatrix

        //! Read RIMatrix.
        static void readAscii(RFile &inFile, RIMatrix &iMatrix, bool readSize = true);
        //! Read RIMatrix.
        static void readBinary(RFile &inFile, RIMatrix &iMatrix, bool readSize = true);
        //! Write RIMatrix.
        static void writeAscii(RSaveFile &outFile, const RIMatrix &iMatrix, bool writeSize = true, bool addNewLine = true);
        //! Write RIMatrix.
        static void writeBinary(RSaveFile &outFile, const RIMatrix &iMatrix, bool writeSize = true);

        // RRMatrix

        //! Read RRMatrix.
        static void readAscii(RFile &inFile, RRMatrix &rMatrix, bool readSize = true);
        //! Read RRMatrix.
        static void readBinary(RFile &inFile, RRMatrix &rMatrix, bool readSize = true);
        //! Write RRMatrix.
        static void writeAscii(RSaveFile &outFile, const RRMatrix &rMatrix, bool writeSize = true, bool addNewLine = true);
        //! Write RRMatrix.
        static void writeBinary(RSaveFile &outFile, const RRMatrix &rMatrix, bool writeSize = true);

        // RVersion

        //! Read RVersion.
        static void readAscii(RFile &inFile, RVersion &version);
        //! Read RVersion.
        static void readBinary(RFile &inFile, RVersion &version);
        //! Write RVersion.
        static void writeAscii(RSaveFile &outFile, const RVersion &version, bool addNewLine = true);
        //! Write RVersion.
        static void writeBinary(RSaveFile &outFile, const RVersion &version);

        // RValueVector

        //! Read RValueVector.
        static void readAscii(RFile &inFile, RValueVector &valueVector);
        //! Read RValueVector.
        static void readBinary(RFile &inFile, RValueVector &valueVector);
        //! Write RValueVector.
        static void writeAscii(RSaveFile &outFile, const RValueVector &valueVector, bool addNewLine = true);
        //! Write RValueVector.
        static void writeBinary(RSaveFile &outFile, const RValueVector &valueVector);

        // RPlane

        //! Read RPlane.
        static void readAscii(RFile &inFile, RPlane &plane);
        //! Read RPlane.
        static void readBinary(RFile &inFile, RPlane &plane);
        //! Write RPlane.
        static void writeAscii(RSaveFile &outFile, const RPlane &plane, bool addNewLine = true);
        //! Write RPlane.
        static void writeBinary(RSaveFile &outFile, const RPlane &plane);

        // RValueTable

        //! Read RValueTable.
        static void readAscii(RFile &inFile, RValueTable &valueTable);
        //! Read RValueTable.
        static void readBinary(RFile &inFile, RValueTable &valueTable);
        //! Write RValueTable.
        static void writeAscii(RSaveFile &outFile, const RValueTable &valueTable, bool addNewLine = true);
        //! Write RValueTable.
        static void writeBinary(RSaveFile &outFile, const RValueTable &valueTable);

        // QColor

        //! Read RLight.
        static void readAscii(RFile &inFile, QColor &color);
        //! Read RLight.
        static void readBinary(RFile &inFile, QColor &color);
        //! Write RLight.
        static void writeAscii(RSaveFile &outFile, const QColor &color, bool addNewLine = true);
        //! Write RLight.
        static void writeBinary(RSaveFile &outFile, const QColor &color);

        // RGLLight

        //! Read RLight.
        static void readAscii(RFile &inFile, RGLLight &light);
        //! Read RLight.
        static void readBinary(RFile &inFile, RGLLight &light);
        //! Write RLight.
        static void writeAscii(RSaveFile &outFile, const RGLLight &light, bool addNewLine = true);
        //! Write RLight.
        static void writeBinary(RSaveFile &outFile, const RGLLight &light);

        // RGLDisplayProperties

        //! Read RLight.
        static void readAscii(RFile &inFile, RGLDisplayProperties &displayProperties);
        //! Read RLight.
        static void readBinary(RFile &inFile, RGLDisplayProperties &displayProperties);
        //! Write RLight.
        static void writeAscii(RSaveFile &outFile, const RGLDisplayProperties &displayProperties, bool addNewLine = true);
        //! Write RLight.
        static void writeBinary(RSaveFile &outFile, const RGLDisplayProperties &displayProperties);

        // RModelData

        //! Read RLight.
        static void readAscii(RFile &inFile, RModelData &modelData);
        //! Read RLight.
        static void readBinary(RFile &inFile, RModelData &modelData);
        //! Write RLight.
        static void writeAscii(RSaveFile &outFile, const RModelData &modelData, bool addNewLine = true);
        //! Write RLight.
        static void writeBinary(RSaveFile &outFile, const RModelData &modelData);

        // RVariableType

        //! Read RVariableType.
        static void readAscii(RFile &inFile, RVariableType &variableType);
        //! Read RVariableType.
        static void readBinary(RFile &inFile, RVariableType &variableType);
        //! Write RVariableType.
        static void writeAscii(RSaveFile &outFile, const RVariableType &variableType, bool addNewLine = true);
        static void writeAscii(RFile &outFile, const RVariableType &variableType, bool addNewLine = true);
        //! Write RVariableType.
        static void writeBinary(RSaveFile &outFile, const RVariableType &variableType);

        // RVariableApplyType

        //! Read RVariableApplyType.
        static void readAscii(RFile &inFile, RVariableApplyType &variableApplyType);
        //! Read RVariableApplyType.
        static void readBinary(RFile &inFile, RVariableApplyType &variableApplyType);
        //! Write RVariableApplyType.
        static void writeAscii(RSaveFile &outFile, const RVariableApplyType &variableApplyType, bool addNewLine = true);
        //! Write RVariableApplyType.
        static void writeBinary(RSaveFile &outFile, const RVariableApplyType &variableApplyType);

        // RVariableData

        //! Read RVariableData.
        static void readAscii(RFile &inFile, RVariableData &variableData);
        //! Read RVariableData.
        static void readBinary(RFile &inFile, RVariableData &variableData);
        //! Write RVariableData.
        static void writeAscii(RSaveFile &outFile, const RVariableData &variableData, bool addNewLine = true);
        //! Write RVariableData.
        static void writeBinary(RSaveFile &outFile, const RVariableData &variableData);

        // RVariable

        //! Read RVariable.
        static void readAscii(RFile &inFile, RVariable &variable);
        //! Read RVariable.
        static void readBinary(RFile &inFile, RVariable &variable);
        //! Write RVariable.
        static void writeAscii(RSaveFile &outFile, const RVariable &variable, bool addNewLine = true);
        //! Write RVariable.
        static void writeBinary(RSaveFile &outFile, const RVariable &variable);

        // RBoundaryConditionType

        //! Read RBoundaryConditionType.
        static void readAscii(RFile &inFile, RBoundaryConditionType &bcType);
        //! Read RBoundaryConditionType.
        static void readBinary(RFile &inFile, RBoundaryConditionType &bcType);
        //! Write RBoundaryConditionType.
        static void writeAscii(RSaveFile &outFile, const RBoundaryConditionType &bcType, bool addNewLine = true);
        //! Write RBoundaryConditionType.
        static void writeBinary(RSaveFile &outFile, const RBoundaryConditionType &bcType);

        // RInitialConditionType

        //! Read RInitialConditionType.
        static void readAscii(RFile &inFile, RInitialConditionType &icType);
        //! Read RInitialConditionType.
        static void readBinary(RFile &inFile, RInitialConditionType &icType);
        //! Write RInitialConditionType.
        static void writeAscii(RSaveFile &outFile, const RInitialConditionType &icType, bool addNewLine = true);
        //! Write RInitialConditionType.
        static void writeBinary(RSaveFile &outFile, const RInitialConditionType &icType);

        // REnvironmentConditionType

        //! Read REnvironmentConditionType.
        static void readAscii(RFile &inFile, REnvironmentConditionType &ecType);
        //! Read REnvironmentConditionType.
        static void readBinary(RFile &inFile, REnvironmentConditionType &ecType);
        //! Write REnvironmentConditionType.
        static void writeAscii(RSaveFile &outFile, const REnvironmentConditionType &ecType, bool addNewLine = true);
        //! Write REnvironmentConditionType.
        static void writeBinary(RSaveFile &outFile, const REnvironmentConditionType &ecType);

        // RConditionComponent

        //! Read RConditionComponent.
        static void readAscii(RFile &inFile, RConditionComponent &conditionComponent);
        //! Read RConditionComponent.
        static void readBinary(RFile &inFile, RConditionComponent &conditionComponent);
        //! Write RConditionComponent.
        static void writeAscii(RSaveFile &outFile, const RConditionComponent &conditionComponent, bool addNewLine = true);
        //! Write RConditionComponent.
        static void writeBinary(RSaveFile &outFile, const RConditionComponent &conditionComponent);

        // RCondition

        //! Read RCondition.
        static void readAscii(RFile &inFile, RCondition &condition);
        //! Read RCondition.
        static void readBinary(RFile &inFile, RCondition &condition);
        //! Write RCondition.
        static void writeAscii(RSaveFile &outFile, const RCondition &condition, bool addNewLine = true);
        //! Write RCondition.
        static void writeBinary(RSaveFile &outFile, const RCondition &condition);

        // RBoundaryCondition

        //! Read RBoundaryCondition.
        static void readAscii(RFile &inFile, RBoundaryCondition &boundaryCondition);
        //! Read RBoundaryCondition.
        static void readBinary(RFile &inFile, RBoundaryCondition &boundaryCondition);
        //! Write RBoundaryCondition.
        static void writeAscii(RSaveFile &outFile, const RBoundaryCondition &boundaryCondition, bool addNewLine = true);
        //! Write RBoundaryCondition.
        static void writeBinary(RSaveFile &outFile, const RBoundaryCondition &boundaryCondition);

        // RInitialCondition

        //! Read RInitialCondition.
        static void readAscii(RFile &inFile, RInitialCondition &initialCondition);
        //! Read RInitialCondition.
        static void readBinary(RFile &inFile, RInitialCondition &initialCondition);
        //! Write RInitialCondition.
        static void writeAscii(RSaveFile &outFile, const RInitialCondition &initialCondition, bool addNewLine = true);
        //! Write RInitialCondition.
        static void writeBinary(RSaveFile &outFile, const RInitialCondition &initialCondition);

        // REnvironmentCondition

        //! Read REnvironmentCondition.
        static void readAscii(RFile &inFile, REnvironmentCondition &environmentCondition);
        //! Read REnvironmentCondition.
        static void readBinary(RFile &inFile, REnvironmentCondition &environmentCondition);
        //! Write REnvironmentCondition.
        static void writeAscii(RSaveFile &outFile, const REnvironmentCondition &environmentCondition, bool addNewLine = true);
        //! Write REnvironmentCondition.
        static void writeBinary(RSaveFile &outFile, const REnvironmentCondition &environmentCondition);

        // RMaterialPropertyType

        //! Read RMaterialPropertyType.
        static void readAscii(RFile &inFile, RMaterialPropertyType &mpType);
        //! Read RMaterialPropertyType.
        static void readBinary(RFile &inFile, RMaterialPropertyType &mpType);
        //! Write RMaterialPropertyType.
        static void writeAscii(RSaveFile &outFile, const RMaterialPropertyType &mpType, bool addNewLine = true);
        //! Write RMaterialPropertyType.
        static void writeBinary(RSaveFile &outFile, const RMaterialPropertyType &mpType);

        // RMaterialState

        //! Read RMaterialState.
        static void readAscii(RFile &inFile, RMaterialState &state);
        //! Read RMaterialState.
        static void readBinary(RFile &inFile, RMaterialState &state);
        //! Write RMaterialState.
        static void writeAscii(RSaveFile &outFile, const RMaterialState &state, bool addNewLine = true);
        //! Write RMaterialState.
        static void writeBinary(RSaveFile &outFile, const RMaterialState &state);

        // RMaterialProperty

        //! Read RMaterialProperty.
        static void readAscii(RFile &inFile, RMaterialProperty &materialProperty);
        //! Read RMaterialProperty.
        static void readBinary(RFile &inFile, RMaterialProperty &materialProperty);
        //! Write RMaterialProperty.
        static void writeAscii(RSaveFile &outFile, const RMaterialProperty &materialProperty, bool addNewLine = true);
        //! Write RMaterialProperty.
        static void writeBinary(RSaveFile &outFile, const RMaterialProperty &materialProperty);

        // RMaterial

        //! Read RMaterial.
        static void readAscii(RFile &inFile, RMaterial &material);
        //! Read RMaterial.
        static void readBinary(RFile &inFile, RMaterial &material);
        //! Write RMaterial.
        static void writeAscii(RSaveFile &outFile, const RMaterial &material, bool addNewLine = true);
        //! Write RMaterial.
        static void writeBinary(RSaveFile &outFile, const RMaterial &material);

        // RNode

        //! Read RNode.
        static void readAscii(RFile &inFile, RNode &node);
        //! Read RNode.
        static void readBinary(RFile &inFile, RNode &node);
        //! Write RNode.
        static void writeAscii(RSaveFile &outFile, const RNode &node, bool addNewLine = true);
        //! Write RNode.
        static void writeBinary(RSaveFile &outFile, const RNode &node);

        // RElementType

        //! Read RElementType.
        static void readAscii(RFile &inFile, RElementType &elementType);
        //! Read RElementType.
        static void readBinary(RFile &inFile, RElementType &elementType);
        //! Write RElementType.
        static void writeAscii(RSaveFile &outFile, const RElementType &elementType, bool addNewLine = true);
        //! Write RElementType.
        static void writeBinary(RSaveFile &outFile, const RElementType &elementType);

        // RElement

        //! Read RElement.
        static void readAscii(RFile &inFile, RElement &element);
        //! Read RElement.
        static void readBinary(RFile &inFile, RElement &element);
        //! Write RElement.
        static void writeAscii(RSaveFile &outFile, const RElement &element, bool addNewLine = true);
        //! Write RElement.
        static void writeBinary(RSaveFile &outFile, const RElement &element);

        // REntityGroupVariableDisplayType

        //! Read REntityGroupVariableDisplayType.
        static void readAscii(RFile &inFile, REntityGroupVariableDisplayType &elementGroupVariableDisplayType);
        //! Read REntityGroupVariableDisplayType.
        static void readBinary(RFile &inFile, REntityGroupVariableDisplayType &elementGroupVariableDisplayType);
        //! Write REntityGroupVariableDisplayType.
        static void writeAscii(RSaveFile &outFile, const REntityGroupVariableDisplayType &elementGroupVariableDisplayType, bool addNewLine = true);
        //! Write REntityGroupVariableDisplayType.
        static void writeBinary(RSaveFile &outFile, const REntityGroupVariableDisplayType &elementGroupVariableDisplayType);

        // REntityGroupData

        //! Read REntityGroupData.
        static void readAscii(RFile &inFile, REntityGroupData &elementGroupData);
        //! Read REntityGroupData.
        static void readBinary(RFile &inFile, REntityGroupData &elementGroupData);
        //! Write REntityGroupData.
        static void writeAscii(RSaveFile &outFile, const REntityGroupData &elementGroupData, bool addNewLine = true);
        //! Write REntityGroupData.
        static void writeBinary(RSaveFile &outFile, const REntityGroupData &elementGroupData);

        // REntityGroup

        //! Read REntityGroup.
        static void readAscii(RFile &inFile, REntityGroup &entityGroup);
        //! Read REntityGroup.
        static void readBinary(RFile &inFile, REntityGroup &entityGroup);
        //! Write REntityGroup.
        static void writeAscii(RSaveFile &outFile, const REntityGroup &entityGroup, bool addNewLine = true);
        //! Write REntityGroup.
        static void writeBinary(RSaveFile &outFile, const REntityGroup &entityGroup);

        // RElementGroup

        //! Read RElementGroup.
        static void readAscii(RFile &inFile, RElementGroup &elementGroup);
        //! Read RElement.
        static void readBinary(RFile &inFile, RElementGroup &elementGroup);
        //! Write RElement.
        static void writeAscii(RSaveFile &outFile, const RElementGroup &elementGroup, bool addNewLine = true);
        //! Write RElement.
        static void writeBinary(RSaveFile &outFile, const RElementGroup &elementGroup);

        // RPoint

        //! Read RPoint.
        static void readAscii(RFile &inFile, RPoint &point);
        //! Read RPoint.
        static void readBinary(RFile &inFile, RPoint &point);
        //! Write RPoint.
        static void writeAscii(RSaveFile &outFile, const RPoint &point, bool addNewLine = true);
        //! Write RPoint.
        static void writeBinary(RSaveFile &outFile, const RPoint &point);

        // RLine

        //! Read RLine.
        static void readAscii(RFile &inFile, RLine &line);
        //! Read RLine.
        static void readBinary(RFile &inFile, RLine &line);
        //! Write RLine.
        static void writeAscii(RSaveFile &outFile, const RLine &line, bool addNewLine = true);
        //! Write RLine.
        static void writeBinary(RSaveFile &outFile, const RLine &line);

        // RSurface

        //! Read RSurface.
        static void readAscii(RFile &inFile, RSurface &surface);
        //! Read RSurface.
        static void readBinary(RFile &inFile, RSurface &surface);
        //! Write RSurface.
        static void writeAscii(RSaveFile &outFile, const RSurface &surface, bool addNewLine = true);
        //! Write RSurface.
        static void writeBinary(RSaveFile &outFile, const RSurface &surface);

        // RVolume

        //! Read RVolume.
        static void readAscii(RFile &inFile, RVolume &volume);
        //! Read RVolume.
        static void readBinary(RFile &inFile, RVolume &volume);
        //! Write RVolume.
        static void writeAscii(RSaveFile &outFile, const RVolume &volume, bool addNewLine = true);
        //! Write RVolume.
        static void writeBinary(RSaveFile &outFile, const RVolume &volume);

        // RVectorField

        //! Read RVectorField.
        static void readAscii(RFile &inFile, RVectorField &vectorField);
        //! Read RVectorField.
        static void readBinary(RFile &inFile, RVectorField &vectorField);
        //! Write RVectorField.
        static void writeAscii(RSaveFile &outFile, const RVectorField &vectorField, bool addNewLine = true);
        //! Write RVectorField.
        static void writeBinary(RSaveFile &outFile, const RVectorField &vectorField);

        // RScalarField

        //! Read RScalarField.
        static void readAscii(RFile &inFile, RScalarField &scalarField);
        //! Read RScalarField.
        static void readBinary(RFile &inFile, RScalarField &scalarField);
        //! Write RScalarField.
        static void writeAscii(RSaveFile &outFile, const RScalarField &scalarField, bool addNewLine = true);
        //! Write RScalarField.
        static void writeBinary(RSaveFile &outFile, const RScalarField &scalarField);

        // RStreamLine

        //! Read RStreamLine.
        static void readAscii(RFile &inFile, RStreamLine &streamLine);
        //! Read RStreamLine.
        static void readBinary(RFile &inFile, RStreamLine &streamLine);
        //! Write RStreamLine.
        static void writeAscii(RSaveFile &outFile, const RStreamLine &streamLine, bool addNewLine = true);
        //! Write RStreamLine.
        static void writeBinary(RSaveFile &outFile, const RStreamLine &streamLine);

        // RCut

        //! Read RCut.
        static void readAscii(RFile &inFile, RCut &cut);
        //! Read RCut.
        static void readBinary(RFile &inFile, RCut &cut);
        //! Write RCut.
        static void writeAscii(RSaveFile &outFile, const RCut &cut, bool addNewLine = true);
        //! Write RCut.
        static void writeBinary(RSaveFile &outFile, const RCut &cut);

        // RIso

        //! Read RIso.
        static void readAscii(RFile &inFile, RIso &iso);
        //! Read RIso.
        static void readBinary(RFile &inFile, RIso &iso);
        //! Write RIso.
        static void writeAscii(RSaveFile &outFile, const RIso &iso, bool addNewLine = true);
        //! Write RIso.
        static void writeBinary(RSaveFile &outFile, const RIso &iso);

        // RFileType

        //! Read RFileType.
        static void readAscii(RFile &inFile, RFileType &fileType);
        //! Read RFileType.
        static void readBinary(RFile &inFile, RFileType &fileType);
        //! Write RFileType.
        static void writeAscii(RSaveFile &outFile, const RFileType &fileType, bool addNewLine = true);
        //! Write RFileType.
        static void writeBinary(RSaveFile &outFile, const RFileType &fileType);

        // RFileHeader

        //! Read RFileHeader.
        static void readAscii(RFile &inFile, RFileHeader &fileHeader);
        //! Read RFileHeader.
        static void readBinary(RFile &inFile, RFileHeader &fileHeader);
        //! Write RFileHeader.
        static void writeAscii(RSaveFile &outFile, const RFileHeader &fileHeader, bool addNewLine = true);
        //! Write RFileHeader.
        static void writeBinary(RSaveFile &outFile, const RFileHeader &fileHeader);

        // RTimeMarchApproximation

        //! Read RTimeMarchApproximation.
        static void readAscii(RFile &inFile, RTimeMarchApproximation &timeMarchApproximation);
        //! Read RTimeMarchApproximation.
        static void readBinary(RFile &inFile, RTimeMarchApproximation &timeMarchApproximation);
        //! Write RTimeMarchApproximation.
        static void writeAscii(RSaveFile &outFile, const RTimeMarchApproximation &timeMarchApproximation, bool addNewLine = true);
        //! Write RTimeMarchApproximation.
        static void writeBinary(RSaveFile &outFile, const RTimeMarchApproximation &timeMarchApproximation);

        // RModalMethod

        //! Read RModalMethod.
        static void readAscii(RFile &inFile, RModalMethod &method);
        //! Read RModalMethod.
        static void readBinary(RFile &inFile, RModalMethod &method);
        //! Write RModalMethod.
        static void writeAscii(RSaveFile &outFile, const RModalMethod &method, bool addNewLine = true);
        //! Write RModalMethod.
        static void writeBinary(RSaveFile &outFile, const RModalMethod &mMethod);

        // RTimeSolver

        //! Read RTimeSolver.
        static void readAscii(RFile &inFile, RTimeSolver &timeSolver);
        //! Read RTimeSolver.
        static void readBinary(RFile &inFile, RTimeSolver &timeSolver);
        //! Write RTimeSolver.
        static void writeAscii(RSaveFile &outFile, const RTimeSolver &timeSolver, bool addNewLine = true);
        //! Write RTimeSolver.
        static void writeBinary(RSaveFile &outFile, const RTimeSolver &timeSolver);

        // RMatrixSolverConf

        //! Read RMatrixSolverConf.
        static void readAscii(RFile &inFile, RMatrixSolverConf &matrixSolver);
        //! Read RMatrixSolverConf.
        static void readBinary(RFile &inFile, RMatrixSolverConf &matrixSolver);
        //! Write RMatrixSolverConf.
        static void writeAscii(RSaveFile &outFile, const RMatrixSolverConf &matrixSolver, bool addNewLine = true);
        //! Write RMatrixSolverConf.
        static void writeBinary(RSaveFile &outFile, const RMatrixSolverConf &matrixSolver);

        // RMonitoringPoint

        //! Read RMonitoringPoint.
        static void readAscii(RFile &inFile, RMonitoringPoint &monitoringPoint);
        //! Read RMonitoringPoint.
        static void readBinary(RFile &inFile, RMonitoringPoint &monitoringPoint);
        //! Write RMonitoringPoint.
        static void writeAscii(RSaveFile &outFile, const RMonitoringPoint &monitoringPoint, bool addNewLine = true);
        static void writeAscii(RFile &outFile, const RMonitoringPoint &monitoringPoint, bool addNewLine = true);
        //! Write RMonitoringPoint.
        static void writeBinary(RSaveFile &outFile, const RMonitoringPoint &monitoringPoint);

        // RMonitoringPointManager

        //! Read RMonitoringPointManager.
        static void readAscii(RFile &inFile, RMonitoringPointManager &monitoringPointManager);
        //! Read RMonitoringPointManager.
        static void readBinary(RFile &inFile, RMonitoringPointManager &monitoringPointManager);
        //! Write RMonitoringPointManager.
        static void writeAscii(RSaveFile &outFile, const RMonitoringPointManager &monitoringPointManager, bool addNewLine = true);
        //! Write RMonitoringPointManager.
        static void writeBinary(RSaveFile &outFile, const RMonitoringPointManager &monitoringPointManager);

        // RProblemType

        //! Read RProblemType.
        static void readAscii(RFile &inFile, RProblemType &problemType);
        //! Read RProblemType.
        static void readBinary(RFile &inFile, RProblemType &problemType);
        //! Write RProblemType.
        static void writeAscii(RSaveFile &outFile, const RProblemType &problemType, bool addNewLine = true);
        //! Write RProblemType.
        static void writeBinary(RSaveFile &outFile, const RProblemType &problemType);

        // RProblemTaskItem

        //! Read RProblemTaskItem.
        static void readAscii(RFile &inFile, RProblemTaskItem &problemTaskItem);
        //! Read RProblemTaskItem.
        static void readBinary(RFile &inFile, RProblemTaskItem &problemTaskItem);
        //! Write RProblemTaskItem.
        static void writeAscii(RSaveFile &outFile, const RProblemTaskItem &problemTaskItem, bool addNewLine = true);
        //! Write RProblemTaskItem.
        static void writeBinary(RSaveFile &outFile, const RProblemTaskItem &problemTaskItem);

        // RProblemSetup

        //! Read RProblemSetup.
        static void readAscii(RFile &inFile, RProblemSetup &problemSetup);
        //! Read RProblemSetup.
        static void readBinary(RFile &inFile, RProblemSetup &problemSetup);
        //! Write RProblemSetup.
        static void writeAscii(RSaveFile &outFile, const RProblemSetup &problemSetup, bool addNewLine = true);
        //! Write RProblemSetup.
        static void writeBinary(RSaveFile &outFile, const RProblemSetup &problemSetup);

        // RRadiationResolution

        //! Read RRadiationResolution.
        static void readAscii(RFile &inFile, RRadiationResolution &radiationResolution);
        //! Read RRadiationResolution.
        static void readBinary(RFile &inFile, RRadiationResolution &radiationResolution);
        //! Write RRadiationResolution.
        static void writeAscii(RSaveFile &outFile, const RRadiationResolution &radiationResolution, bool addNewLine = true);
        //! Write RRadiationResolution.
        static void writeBinary(RSaveFile &outFile, const RRadiationResolution &radiationResolution);

        // RRadiationSetup

        //! Read RRadiationSetup.
        static void readAscii(RFile &inFile, RRadiationSetup &radiationSetup);
        //! Read RRadiationSetup.
        static void readBinary(RFile &inFile, RRadiationSetup &radiationSetup);
        //! Write RRadiationSetup.
        static void writeAscii(RSaveFile &outFile, const RRadiationSetup &radiationSetup, bool addNewLine = true);
        //! Write RRadiationSetup.
        static void writeBinary(RSaveFile &outFile, const RRadiationSetup &radiationSetup);

        // RModalSetup

        //! Read RModalSetup.
        static void readAscii(RFile &inFile, RModalSetup &modalSetup);
        //! Read RModalSetup.
        static void readBinary(RFile &inFile, RModalSetup &modalSetup);
        //! Write RModalSetup.
        static void writeAscii(RSaveFile &outFile, const RModalSetup &modalSetup, bool addNewLine = true);
        //! Write RModalSetup.
        static void writeBinary(RSaveFile &outFile, const RModalSetup &modalSetup);

        // RMeshSetup

        //! Read RMeshSetup.
        static void readAscii(RFile &inFile, RMeshSetup &meshSetup);
        //! Read RMeshSetup.
        static void readBinary(RFile &inFile, RMeshSetup &meshSetup);
        //! Write RMeshSetup.
        static void writeAscii(RSaveFile &outFile, const RMeshSetup &meshSetup, bool addNewLine = true);
        //! Write RMeshSetup.
        static void writeBinary(RSaveFile &outFile, const RMeshSetup &meshSetup);

        // RBook

        //! Read RBook.
        static void readAscii(RFile &inFile, RBook &book);
        //! Read RBook.
        static void readBinary(RFile &inFile, RBook &book);
        //! Write RBook.
        static void writeAscii(RSaveFile &outFile, const RBook &book, bool addNewLine = true);
        //! Write RBook.
        static void writeBinary(RSaveFile &outFile, const RBook &book);

        // std::vector

        //! Read std::vector.
        template <class T> static void readAscii(RFile &inFile, std::vector<T> &v)
        {
            uint nValues = 0;
            RFileIO::readAscii(inFile,nValues);
            v.resize(nValues);
            for (uint i=0;i<nValues;i++)
            {
                RFileIO::readAscii(inFile,v[i]);
            }
        }
        //! Read std::vector.
        template <class T> static void readBinary(RFile &inFile, std::vector<T> &v)
        {
            uint nValues = 0;
            RFileIO::readBinary(inFile,nValues);
            v.resize(nValues);
            for (uint i=0;i<nValues;i++)
            {
                RFileIO::readBinary(inFile,v[i]);
            }
        }
        //! Write std::vector.
        template <class T> static void writeAscii(RSaveFile &outFile, const std::vector<T> &v, bool addNewLine = true)
        {
            uint nValues = uint(v.size());
            RFileIO::writeAscii(outFile,nValues,addNewLine);
            if (!addNewLine)
            {
                RFileIO::writeAscii(outFile,' ',false);
            }
            for (uint i=0;i<nValues;i++)
            {
                RFileIO::writeAscii(outFile,v[i],addNewLine);
                if (i<nValues-1 && !addNewLine)
                {
                    RFileIO::writeAscii(outFile,' ',false);
                }
            }
        }
        //! Write std::vector.
        template <class T> static void writeBinary(RSaveFile &outFile, const std::vector<T> &v)
        {
            uint nValues = uint(v.size());
            RFileIO::writeBinary(outFile,nValues);
            for (uint i=0;i<nValues;i++)
            {
                RFileIO::writeBinary(outFile,v[i]);
            }
        }

        // RSparseVector

        //! Read RSparseVector.
        template <class T> static void readAscii(RFile &inFile, RSparseVector<T> &sparseVector)
        {
            sparseVector.clear();

            uint nValues = 0;

            RFileIO::readAscii(inFile,nValues);

            uint index = 0;
            T value;

            for (uint i=0;i<nValues;i++)
            {
                RFileIO::readAscii(inFile,index);
                RFileIO::readAscii(inFile,value);

                sparseVector.addValue(index,value);
            }
        }
        //! Read RSparseVector.
        template <class T> static void readBinary(RFile &inFile, RSparseVector<T> &sparseVector)
        {
            sparseVector.clear();

            uint nValues = 0;

            RFileIO::readBinary(inFile,nValues);

            uint index = 0;
            T value;

            for (uint i=0;i<nValues;i++)
            {
                RFileIO::readBinary(inFile,index);
                RFileIO::readBinary(inFile,value);

                sparseVector.addValue(index,value);
            }
        }
        //! Write RSparseVector.
        template <class T> static void writeAscii(RSaveFile &outFile, const RSparseVector<T> &sparseVector, bool addNewLine = true)
        {
            uint nValues = uint(sparseVector.size());

            RFileIO::writeAscii(outFile,nValues,addNewLine);
            if (!addNewLine)
            {
                RFileIO::writeAscii(outFile,' ',false);
            }

            for (uint i=0;i<nValues;i++)
            {
                RFileIO::writeAscii(outFile,sparseVector.getIndex(i),addNewLine);
                if (!addNewLine)
                {
                    RFileIO::writeAscii(outFile,' ',false);
                }
                RFileIO::writeAscii(outFile,sparseVector.getValue(i),addNewLine);
                if (!addNewLine && i == nValues-1)
                {
                    RFileIO::writeAscii(outFile,' ',false);
                }
            }
        }
        //! Write RSparseVector.
        template <class T> static void writeBinary(RSaveFile &outFile, const RSparseVector<T> &sparseVector)
        {
            uint nValues = sparseVector.size();

            RFileIO::writeBinary(outFile,nValues);

            for (uint i=0;i<nValues;i++)
            {
                RFileIO::writeBinary(outFile,sparseVector.getIndex(i));
                RFileIO::writeBinary(outFile,sparseVector.getValue(i));
            }
        }

        // RPatch

        //! Read RPatch.
        static void readAscii(RFile &inFile, RPatch &patch);
        //! Read RPatch.
        static void readBinary(RFile &inFile, RPatch &patch);
        //! Write RPatch.
        static void writeAscii(RSaveFile &outFile, const RPatch &patch, bool addNewLine = true);
        //! Write RPatch.
        static void writeBinary(RSaveFile &outFile, const RPatch &patch);

        // RPatchBook

        //! Read RPatchBook.
        static void readAscii(RFile &inFile, RPatchBook &patchBook);
        //! Read RPatchBook.
        static void readBinary(RFile &inFile, RPatchBook &patchBook);
        //! Write RPatchBook.
        static void writeAscii(RSaveFile &outFile, const RPatchBook &patchBook, bool addNewLine = true);
        //! Write RPatchBook.
        static void writeBinary(RSaveFile &outFile, const RPatchBook &patchBook);

        // RPatchInput

        //! Read RPatchInput.
        static void readAscii(RFile &inFile, RPatchInput &patchInput);
        //! Read RPatchInput.
        static void readBinary(RFile &inFile, RPatchInput &patchInput);
        //! Write RPatchInput.
        static void writeAscii(RSaveFile &outFile, const RPatchInput &patchInput, bool addNewLine = true);
        //! Write RPatchInput.
        static void writeBinary(RSaveFile &outFile, const RPatchInput &patchInput);

        // RViewFactorMatrixHeader

        //! Read RViewFactorMatrixHeader.
        static void readAscii(RFile &inFile, RViewFactorMatrixHeader &viewFactorMatrixHeader);
        //! Read RViewFactorMatrixHeader.
        static void readBinary(RFile &inFile, RViewFactorMatrixHeader &viewFactorMatrixHeader);
        //! Write RViewFactorMatrixHeader.
        static void writeAscii(RSaveFile &outFile, const RViewFactorMatrixHeader &viewFactorMatrixHeader, bool addNewLine = true);
        //! Write RViewFactorMatrixHeader.
        static void writeBinary(RSaveFile &outFile, const RViewFactorMatrixHeader &viewFactorMatrixHeader);

        // RViewFactorRow

        //! Read RViewFactorRow.
        static void readAscii(RFile &inFile, RViewFactorRow &viewFactorRow);
        //! Read RViewFactorRow.
        static void readBinary(RFile &inFile, RViewFactorRow &viewFactorRow);
        //! Write RViewFactorRow.
        static void writeAscii(RSaveFile &outFile, const RViewFactorRow &viewFactorRow, bool addNewLine = true);
        //! Write RViewFactorRow.
        static void writeBinary(RSaveFile &outFile, const RViewFactorRow &viewFactorRow);

};

#endif /* RML_FILE_IO_H */
