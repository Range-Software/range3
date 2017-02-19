#-------------------------------------------------
#
# Project created by QtCreator 2012-09-04T07:56:31
#
#-------------------------------------------------

QT       -= core gui

TARGET = TetGen
TEMPLATE = lib

BUILDPATH = $${PWD}/../../../build-range3

SOURCES += \
    predicates.cxx \
    tetgen.cxx

HEADERS += tetgen.h

OTHER_FILES += \
    README \
    LICENSE

#SOURCES_NOOPTIMIZE = predicates.cxx
#nooptimize.name = nooptimize
#nooptimize.input = SOURCES_NOOPTIMIZE
#nooptimize.dependency_type = TYPE_C
#nooptimize.variable_out = OBJECTS
#nooptimize.output = ${QMAKE_VAR_OBJECTS_DIR}${QMAKE_FILE_IN_BASE}$${first(QMAKE_EXT_OBJ)}
#nooptimize.commands = $${QMAKE_CXX} $(CXXFLAGS) -O0 $(INCPATH) -c ${QMAKE_FILE_IN} -o ${QMAKE_FILE_OUT} # Note the -O0
#QMAKE_EXTRA_COMPILERS += nooptimize

CONFIG -= debug_and_release
CONFIG += copy_dir_files
CONFIG += staticlib
CONFIG += exceptions

QMAKE_CXXFLAGS_WARN_ON = -w
QMAKE_CFLAGS_WARN_ON = -w

DEFINES += TETLIBRARY

CONFIG(debug,debug|release) {
    TARGET = $$join(TARGET,,,_debug)
    DEFINES += DEBUG
}

unix {
    target.path = $${BUILDPATH}/lib
    headers.path = $${BUILDPATH}/include
}
win32 {
    target.path = $${BUILDPATH}\\lib
    headers.path = $${BUILDPATH}\\include
}

headers.files += $$HEADERS
INSTALLS += headers
INSTALLS += target
