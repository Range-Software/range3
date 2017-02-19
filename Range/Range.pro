#-------------------------------------------------
#
# Project created by QtCreator 2011-12-30T09:43:04
#
#-------------------------------------------------

QT += core gui opengl printsupport network
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

TARGET = Range
TEMPLATE = app

BUILDPATH = $${PWD}/../../build-range3

SOURCES += \
    src/action.cpp \
    src/action_definition.cpp \
    src/action_definition_item.cpp \
    src/action_list.cpp \
    src/application.cpp \
    src/application_settings.cpp \
    src/application_settings_dialog.cpp \
    src/bc_manager_tree.cpp \
    src/bc_tree.cpp \
    src/bool_difference_dialog.cpp \
    src/bool_intersection_dialog.cpp \
    src/bool_union_dialog.cpp \
    src/break_intersected_elements_dialog.cpp \
    src/clipping_plane_widget.cpp \
    src/coarsen_surface_dialog.cpp \
    src/color_combo_box.cpp \
    src/color_scale_combo_box.cpp \
    src/color_scale.cpp \
    src/color.cpp \
    src/component_edit_dialog.cpp \
    src/component_edit_table.cpp \
    src/convergence_graph_dialog.cpp \
    src/convergence_graph_object.cpp \
    src/cut_dialog.cpp \
    src/direction_widget.cpp \
    src/document_generator.cpp \
    src/document_tree.cpp \
    src/download_manager.cpp \
    src/draw_engine.cpp \
    src/draw_engine_circle.cpp \
    src/draw_engine_cylinder.cpp \
    src/draw_engine_ellipse.cpp \
    src/draw_engine_hexahedron.cpp \
    src/draw_engine_input.cpp \
    src/draw_engine_line.cpp \
    src/draw_engine_object.cpp \
    src/draw_engine_point.cpp \
    src/draw_engine_quadrilateral.cpp \
    src/draw_engine_sphere.cpp \
    src/draw_engine_tetrahedron.cpp \
    src/draw_engine_triangle.cpp \
    src/draw_input_tree.cpp \
    src/draw_input_widget.cpp \
    src/drop_results_dialog.cpp \
    src/ec_manager_tree.cpp \
    src/ec_tree.cpp \
    src/file_chooser_button.cpp \
    src/file_updater.cpp \
    src/fix_sliver_elements_dialog.cpp \
    src/geometry_rotate_widget.cpp \
    src/geometry_scale_widget.cpp \
    src/geometry_transform_input.cpp \
    src/geometry_transform_widget.cpp \
    src/geometry_translate_widget.cpp \
    src/gl_acion_event.cpp \
    src/gl_arrow.cpp \
    src/gl_axis.cpp \
    src/gl_cut_plane.cpp \
    src/gl_display_properties_dialog.cpp \
    src/gl_display_properties.cpp \
    src/gl_element.cpp \
    src/gl_element_base.cpp \
    src/gl_element_group.cpp \
    src/gl_entity_list.cpp \
    src/gl_entity.cpp \
    src/gl_functions.cpp \
    src/gl_interpolated_element.cpp \
    src/gl_interpolated_entity.cpp \
    src/gl_line.cpp \
    src/gl_list.cpp \
    src/gl_model_list.cpp \
    src/gl_node.cpp \
    src/gl_object.cpp \
    src/gl_point.cpp \
    src/gl_scalar_field.cpp \
    src/gl_simplex.cpp \
    src/gl_simplex_point.cpp \
    src/gl_simplex_polygon.cpp \
    src/gl_simplex_segment.cpp \
    src/gl_simplex_tetrahedra.cpp \
    src/gl_texture.cpp \
    src/gl_text_renderer.cpp \
    src/gl_text_renderer_item.cpp \
    src/gl_vector_field.cpp \
    src/gl_widget.cpp \
    src/graph_data.cpp \
    src/graph_dialog.cpp \
    src/graph_object.cpp \
    src/graph_widget.cpp \
    src/http_request_input.cpp \
    src/http_request_worker.cpp \
    src/ic_manager_tree.cpp \
    src/ic_tree.cpp \
    src/iso_dialog.cpp \
    src/job.cpp \
    src/job_manager.cpp \
    src/keyboard_shortcuts_edit.cpp \
    src/light_properties_widget.cpp \
    src/locker.cpp \
    src/logger.cpp \
    src/log_browser.cpp \
    src/log_browser_dialog.cpp \
    src/main.cpp \
    src/main_settings.cpp \
    src/main_window.cpp \
    src/mark_entity_dialog.cpp \
    src/material_list.cpp \
    src/material_manager_tree.cpp \
    src/material_property_add_dialog.cpp \
    src/material_property_edit_dialog.cpp \
    src/material_property_edit_table.cpp \
    src/material_property_line_edit.cpp \
    src/material_tree.cpp \
    src/material_updater.cpp \
    src/matrix_solver_config_dialog.cpp \
    src/merge_entity_dialog.cpp \
    src/merge_near_nodes_dialog.cpp \
    src/mesh_generator.cpp \
    src/mesh_generator_dialog.cpp \
    src/modal_setup_widget.cpp \
    src/model_action.cpp \
    src/model_action_input.cpp \
    src/model_entity_display_properties_tree.cpp \
    src/model_entity_geometric_properties_tree.cpp \
    src/model_io.cpp \
    src/model_records_selector.cpp \
    src/model_records_tree.cpp \
    src/model_statistics_dialog.cpp \
    src/model_tree_simple.cpp \
    src/model_tree.cpp \
    src/model.cpp \
    src/module_licenses_dialog.cpp \
    src/module_licenses_tree.cpp \
    src/monitoring_point_graph_dialog.cpp \
    src/monitoring_points_dialog.cpp \
    src/move_node_dialog.cpp \
    src/new_model_dialog.cpp \
    src/pick_details_tree.cpp \
    src/pick_item.cpp \
    src/pick_list.cpp \
    src/pick_value.cpp \
    src/point_inside_surface_dialog.cpp \
    src/position_widget.cpp \
    src/problem_selector_dialog.cpp \
    src/problem_selector_tree.cpp \
    src/problem_task_tree.cpp \
    src/problem_task_dialog.cpp \
    src/problem_tree.cpp \
    src/progress_bar.cpp \
    src/progress.cpp \
    src/push_button.cpp \
    src/quit_dialog.cpp \
    src/radiation_setup_widget.cpp \
    src/remove_entity_dialog.cpp \
    src/report_dialog.cpp \
    src/report_generator.cpp \
    src/results_variable_selector.cpp \
    src/results_variable_tree.cpp \
    src/rra_request_input.cpp \
    src/rra_request_worker.cpp \
    src/rra_session.cpp \
    src/scalar_field_dialog.cpp \
    src/session_entity_id.cpp \
    src/session_node_id.cpp \
    src/session.cpp \
    src/shortcut_line_edit.cpp \
    src/solver_manager.cpp \
    src/solver_process.cpp \
    src/solver_setup_checker.cpp \
    src/solver_task.cpp \
    src/solver_task_id.cpp \
    src/stream_line_dialog.cpp \
    src/sub_window.cpp \
    src/sub_window_manager.cpp \
    src/text_browser.cpp \
    src/time_solver_setup_widget.cpp \
    src/update_dialog.cpp \
    src/value_line_edit.cpp \
    src/value_scale.cpp \
    src/value_set_generator_dialog.cpp \
    src/value_table.cpp \
    src/variable_selector.cpp \
    src/variable_value_edit.cpp \
    src/vector_field_dialog.cpp

HEADERS += \
    src/action.h \
    src/action_definition.h \
    src/action_definition_item.h \
    src/action_list.h \
    src/application.h \
    src/application_settings.h \
    src/application_settings_dialog.h \
    src/bc_manager_tree.h \
    src/bc_tree.h \
    src/bool_difference_dialog.h \
    src/bool_intersection_dialog.h \
    src/bool_union_dialog.h \
    src/break_intersected_elements_dialog.h \
    src/clipping_plane_widget.h \
    src/coarsen_surface_dialog.h \
    src/color_combo_box.h \
    src/color_scale_combo_box.h \
    src/color_scale.h \
    src/color.h \
    src/component_edit_dialog.h \
    src/component_edit_table.h \
    src/convergence_graph_dialog.h \
    src/convergence_graph_object.h \
    src/cut_dialog.h \
    src/direction_widget.h \
    src/document_generator.h \
    src/document_tree.h \
    src/download_manager.h \
    src/draw_engine.h \
    src/draw_engine_circle.h \
    src/draw_engine_cylinder.h \
    src/draw_engine_ellipse.h \
    src/draw_engine_hexahedron.h \
    src/draw_engine_input.h \
    src/draw_engine_line.h \
    src/draw_engine_object.h \
    src/draw_engine_point.h \
    src/draw_engine_quadrilateral.h \
    src/draw_engine_sphere.h \
    src/draw_engine_tetrahedron.h \
    src/draw_engine_triangle.h \
    src/draw_input_tree.h \
    src/draw_input_widget.h \
    src/drop_results_dialog.h \
    src/ec_manager_tree.h \
    src/ec_tree.h \
    src/file_chooser_button.h \
    src/file_updater.h \
    src/fix_sliver_elements_dialog.h \
    src/geometry_rotate_widget.h \
    src/geometry_scale_widget.h \
    src/geometry_transform_input.h \
    src/geometry_transform_widget.h \
    src/geometry_translate_widget.h \
    src/gl_acion_event.h \
    src/gl_arrow.h \
    src/gl_axis.h \
    src/gl_cut_plane.h \
    src/gl_display_properties_dialog.h \
    src/gl_display_properties.h \
    src/gl_element_base.h \
    src/gl_element_group.h \
    src/gl_element.h \
    src/gl_entity_list.h \
    src/gl_entity.h \
    src/gl_functions.h \
    src/gl_interpolated_element.h \
    src/gl_interpolated_entity.h \
    src/gl_line.h \
    src/gl_list.h \
    src/gl_model_list.h \
    src/gl_node.h \
    src/gl_object.h \
    src/gl_point.h \
    src/gl_scalar_field.h \
    src/gl_simplex.h \
    src/gl_simplex_point.h \
    src/gl_simplex_polygon.h \
    src/gl_simplex_segment.h \
    src/gl_simplex_tetrahedra.h \
    src/gl_texture.h \
    src/gl_text_renderer.h \
    src/gl_text_renderer_item.h \
    src/gl_vector_field.h \
    src/gl_widget.h \
    src/graph_data.h \
    src/graph_dialog.h \
    src/graph_object.h \
    src/graph_widget.h \
    src/http_request_input.h \
    src/http_request_worker.h \
    src/ic_manager_tree.h \
    src/ic_tree.h \
    src/iso_dialog.h \
    src/job.h \
    src/job_manager.h \
    src/keyboard_shortcuts_edit.h \
    src/light_properties_widget.h \
    src/locker.h \
    src/logger.h \
    src/log_browser.h \
    src/log_browser_dialog.h \
    src/main_settings.h \
    src/main_window.h \
    src/mark_entity_dialog.h \
    src/material_list.h \
    src/material_manager_tree.h \
    src/material_property_add_dialog.h \
    src/material_property_edit_dialog.h \
    src/material_property_edit_table.h \
    src/material_property_line_edit.h \
    src/material_tree.h \
    src/material_updater.h \
    src/matrix_solver_config_dialog.h \
    src/merge_entity_dialog.h \
    src/merge_near_nodes_dialog.h \
    src/mesh_generator_dialog.h \
    src/mesh_generator.h \
    src/modal_setup_widget.h \
    src/model_action.h \
    src/model_action_input.h \
    src/model_entity_display_properties_tree.h \
    src/model_entity_geometric_properties_tree.h \
    src/model_io.h \
    src/model_records_selector.h \
    src/model_records_tree.h \
    src/model_statistics_dialog.h \
    src/model_tree_simple.h \
    src/model_tree.h \
    src/model.h \
    src/module_licenses_dialog.h \
    src/module_licenses_tree.h \
    src/monitoring_point_graph_dialog.h \
    src/monitoring_points_dialog.h \
    src/move_node_dialog.h \
    src/new_model_dialog.h \
    src/pick_details_tree.h \
    src/pick_item.h \
    src/pick_list.h \
    src/pick_value.h \
    src/point_inside_surface_dialog.h \
    src/position_widget.h \
    src/problem_selector_dialog.h \
    src/problem_selector_tree.h \
    src/problem_task_tree.h \
    src/problem_task_dialog.h \
    src/problem_tree.h \
    src/progress_bar.h \
    src/progress.h \
    src/push_button.h \
    src/quit_dialog.h \
    src/radiation_setup_widget.h \
    src/remove_entity_dialog.h \
    src/report_dialog.h \
    src/report_generator.h \
    src/results_variable_tree.h \
    src/results_variable_selector.h \
    src/rra_request_input.h \
    src/rra_request_worker.h \
    src/rra_session.h \
    src/scalar_field_dialog.h \
    src/session_entity_id.h \
    src/session_node_id.h \
    src/session.h \
    src/shortcut_line_edit.h \
    src/solver_manager.h \
    src/solver_process.h \
    src/solver_setup_checker.h \
    src/solver_task.h \
    src/solver_task_id.h \
    src/stream_line_dialog.h \
    src/sub_window.h \
    src/sub_window_manager.h \
    src/text_browser.h \
    src/time_solver_setup_widget.h \
    src/update_dialog.h \
    src/value_line_edit.h \
    src/value_scale.h \
    src/value_set_generator_dialog.h \
    src/value_table.h \
    src/variable_data.h \
    src/variable_selector.h \
    src/variable_value_edit.h \
    src/vector_field_dialog.h

CONFIG -= debug_and_release
#CONFIG += rtti
CONFIG += exceptions

DEBUG_EXT = ""

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_debug)
    DEFINES += DEBUG
    DEBUG_EXT = "_debug"
}

LIBS += -L$${BUILDPATH}/lib/ -lRangeSolverLib$${DEBUG_EXT} -lRangeModel$${DEBUG_EXT} -lRangeBase$${DEBUG_EXT} -lRangeAuth$${DEBUG_EXT} -lTetGen$${DEBUG_EXT}

INCLUDEPATH += $${BUILDPATH}/include
DEPENDPATH += $${BUILDPATH}/include

LIB_EXT = "a"

unix {
    target.path = $${BUILDPATH}/bin
}
win32 {
    LIBS += -lopengl32
    target.path = $${BUILDPATH}\\bin
}

POST_TARGETDEPS += $${BUILDPATH}/lib/libRangeSolverLib$${DEBUG_EXT}.$${LIB_EXT}
POST_TARGETDEPS += $${BUILDPATH}/lib/libRangeModel$${DEBUG_EXT}.$${LIB_EXT}
POST_TARGETDEPS += $${BUILDPATH}/lib/libRangeBase$${DEBUG_EXT}.$${LIB_EXT}
POST_TARGETDEPS += $${BUILDPATH}/lib/libRangeAuth$${DEBUG_EXT}.$${LIB_EXT}
POST_TARGETDEPS += $${BUILDPATH}/lib/libTetGen$${DEBUG_EXT}.$${LIB_EXT}

INSTALLS += target

OTHER_FILES +=

RESOURCES += \
    range.qrc

win32:RC_ICONS += pixmaps/range.ico

FORMS +=

# Copy materials files
materials.path = $${BUILDPATH}/materials
materials.files += materials/*.rbmt
INSTALLS += materials

# Copy data files
data.path = $${BUILDPATH}/data
data.files += data/*.rbm
INSTALLS += data

# Copy doc files
doc.path = $${BUILDPATH}/doc
doc.files += doc/*.pdf
doc.files += doc/*.txt
INSTALLS += doc

# Copy man files
man.path = $${BUILDPATH}/man
man.files += man/*
INSTALLS += man

# Copy desktop files
desktop.path = $${BUILDPATH}/desktop
desktop.files += desktop/*
INSTALLS += desktop