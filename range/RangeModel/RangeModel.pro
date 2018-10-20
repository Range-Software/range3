QT += core
win*-msvc* {
    QMAKE_CXXFLAGS += -openmp
    LIBS += -openmp
} else {
    QMAKE_CXXFLAGS += -fopenmp
    LIBS += -fopenmp
}

TARGET = RangeModel
TEMPLATE = lib

DEFINES += RANGEMODEL_LIBRARY

DEFINES += "FILE_MAJOR_VERSION=1"
DEFINES += "FILE_MINOR_VERSION=0"
DEFINES += "FILE_RELEASE_VERSION=0"

INCLUDEPATH += include

SOURCES += \
    src/rml_boundary_condition.cpp \
    src/rml_condition.cpp \
    src/rml_condition_component.cpp \
    src/rml_cut.cpp \
    src/rml_eigen_value_solver_conf.cpp \
    src/rml_element.cpp \
    src/rml_element_group.cpp \
    src/rml_element_shape_derivation.cpp \
    src/rml_element_shape_function.cpp \
    src/rml_entity_group.cpp \
    src/rml_entity_group_data.cpp \
    src/rml_environment_condition.cpp \
    src/rml_file.cpp \
    src/rml_file_header.cpp \
    src/rml_file_io.cpp \
    src/rml_file_manager.cpp \
    src/rml_gl_display_properties.cpp \
    src/rml_initial_condition.cpp \
    src/rml_interpolated_element.cpp \
    src/rml_interpolated_entity.cpp \
    src/rml_interpolated_node.cpp \
    src/rml_iso.cpp \
    src/rml_line.cpp \
    src/rml_material.cpp \
    src/rml_material_list.cpp \
    src/rml_material_property.cpp \
    src/rml_matrix_solver_conf.cpp \
    src/rml_mesh_generator.cpp \
    src/rml_mesh_input.cpp \
    src/rml_mesh_setup.cpp \
    src/rml_modal_setup.cpp \
    src/rml_model.cpp \
    src/rml_model_data.cpp \
    src/rml_model_msh.cpp \
    src/rml_model_raw.cpp \
    src/rml_model_stl.cpp \
    src/rml_monitoring_point.cpp \
    src/rml_monitoring_point_manager.cpp \
    src/rml_node.cpp \
    src/rml_patch.cpp \
    src/rml_patch_book.cpp \
    src/rml_patch_input.cpp \
    src/rml_point.cpp \
    src/rml_polygon.cpp \
    src/rml_problem.cpp \
    src/rml_problem_setup.cpp \
    src/rml_problem_task_item.cpp \
    src/rml_radiation_setup.cpp \
    src/rml_results.cpp \
    src/rml_save_file.cpp \
    src/rml_scalar_field.cpp \
    src/rml_segment.cpp \
    src/rml_shape_generator.cpp \
    src/rml_sparse_matrix.cpp \
    src/rml_stream_line.cpp \
    src/rml_surface.cpp \
    src/rml_tetgen.cpp \
    src/rml_tetrahedron.cpp \
    src/rml_time_solver.cpp \
    src/rml_triangle.cpp \
    src/rml_triangulate.cpp \
    src/rml_variable.cpp \
    src/rml_variable_data.cpp \
    src/rml_vector_field.cpp \
    src/rml_view_factor_matrix.cpp \
    src/rml_view_factor_matrix_header.cpp \
    src/rml_view_factor_row.cpp \
    src/rml_volume.cpp

HEADERS += \
    include/rml_boundary_condition.h \
    include/rml_condition.h \
    include/rml_condition_component.h \
    include/rml_cut.h \
    include/rml_eigen_value_solver_conf.h \
    include/rml_element.h \
    include/rml_element_group.h \
    include/rml_element_shape_derivation.h \
    include/rml_element_shape_function.h \
    include/rml_entity_group.h \
    include/rml_entity_group_data.h \
    include/rml_environment_condition.h \
    include/rml_file.h \
    include/rml_file_header.h \
    include/rml_file_io.h \
    include/rml_file_manager.h \
    include/rml_gl_display_properties.h \
    include/rml_initial_condition.h \
    include/rml_interpolated_element.h \
    include/rml_interpolated_entity.h \
    include/rml_interpolated_node.h \
    include/rml_iso.h \
    include/rml_line.h \
    include/rml_material.h \
    include/rml_material_list.h \
    include/rml_material_property.h \
    include/rml_matrix_solver_conf.h \
    include/rml_mesh_generator.h \
    include/rml_mesh_input.h \
    include/rml_mesh_setup.h \
    include/rml_modal_setup.h \
    include/rml_model.h \
    include/rml_model_data.h \
    include/rml_model_msh.h \
    include/rml_model_raw.h \
    include/rml_model_stl.h \
    include/rml_monitoring_point.h \
    include/rml_monitoring_point_manager.h \
    include/rml_node.h \
    include/rml_patch.h \
    include/rml_patch_book.h \
    include/rml_patch_input.h \
    include/rml_point.h \
    include/rml_polygon.h \
    include/rml_problem.h \
    include/rml_problem_setup.h \
    include/rml_problem_task_item.h \
    include/rml_problem_type.h \
    include/rml_radiation_setup.h \
    include/rml_results.h \
    include/rml_save_file.h \
    include/rml_scalar_field.h \
    include/rml_segment.h \
    include/rml_shape_generator.h \
    include/rml_sparse_matrix.h \
    include/rml_sparse_vector.h \
    include/rml_stream_line.h \
    include/rml_surface.h \
    include/rml_tetgen.h \
    include/rml_tetrahedron.h \
    include/rml_time_solver.h \
    include/rml_triangle.h \
    include/rml_triangulate.h \
    include/rml_variable.h \
    include/rml_variable_data.h \
    include/rml_vector_field.h \
    include/rml_view_factor_matrix.h \
    include/rml_view_factor_matrix_header.h \
    include/rml_view_factor_row.h \
    include/rml_volume.h \
    include/rmlib.h

CONFIG -= debug_and_release
CONFIG += copy_dir_files
CONFIG += rtti
CONFIG += exceptions
#CONFIG += dll
CONFIG += staticlib

INCLUDEPATH += $${_PRO_FILE_PWD_}/../TetGen
INCLUDEPATH += $${_PRO_FILE_PWD_}/../RangeBase/include

DEFINES += TETLIBRARY

CONFIG(debug,debug|release) {
    TARGET = $$join(TARGET,,,_debug)
    DEFINES += DEBUG
}

