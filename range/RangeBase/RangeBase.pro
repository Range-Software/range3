QT += core

TARGET = RangeBase
TEMPLATE = lib

DEFINES += RANGEBASE_LIBRARY

INCLUDEPATH += include

SOURCES += \
    src/rbl_application_state.cpp \
    src/rbl_argument_option.cpp \
    src/rbl_arguments_parser.cpp \
    src/rbl_book.cpp \
    src/rbl_bvector.cpp \
    src/rbl_error.cpp \
    src/rbl_gl_light.cpp \
    src/rbl_imatrix.cpp \
    src/rbl_ivector.cpp \
    src/rbl_limit_box.cpp \
    src/rbl_locker.cpp \
    src/rbl_logger.cpp \
    src/rbl_message.cpp \
    src/rbl_plane.cpp \
    src/rbl_progress.cpp \
    src/rbl_r3vector.cpp \
    src/rbl_rmatrix.cpp \
    src/rbl_rvector.cpp \
    src/rbl_statistics.cpp \
    src/rbl_stop_watch.cpp \
    src/rbl_utils.cpp \
    src/rbl_uvector.cpp \
    src/rbl_value.cpp \
    src/rbl_value_table.cpp \
    src/rbl_value_vector.cpp \
    src/rbl_version.cpp \
    src/rbl_local_direction.cpp

HEADERS += \
    include/rbl_application_state.h \
    include/rbl_argument_option.h \
    include/rbl_arguments_parser.h \
    include/rbl_book.h \
    include/rbl_bvector.h \
    include/rbl_distance_vector.h \
    include/rbl_error.h \
    include/rbl_gl_light.h \
    include/rbl_imatrix.h \
    include/rbl_ivector.h \
    include/rbl_limit_box.h \
    include/rbl_locker.h \
    include/rbl_logger.h \
    include/rbl_message.h \
    include/rbl_plane.h \
    include/rbl_progress.h \
    include/rbl_r3vector.h \
    include/rbl_rmatrix.h \
    include/rbl_rvector.h \
    include/rbl_statistics.h \
    include/rbl_stop_watch.h \
    include/rbl_utils.h \
    include/rbl_uvector.h \
    include/rbl_value.h \
    include/rbl_value_table.h \
    include/rbl_value_vector.h \
    include/rbl_version.h \
    include/rblib.h \
    include/rbl_local_direction.h

CONFIG -= debug_and_release
CONFIG += copy_dir_files
CONFIG += rtti
CONFIG += exceptions
#CONFIG += dll
CONFIG += staticlib

CONFIG(debug,debug|release) {
    TARGET = $$join(TARGET,,,_debug)
    DEFINES += DEBUG
}

