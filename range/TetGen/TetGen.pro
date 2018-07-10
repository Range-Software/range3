QT -= core gui

TARGET = TetGen
TEMPLATE = lib

SOURCES += \
    predicates.cxx \
    tetgen.cxx

HEADERS += tetgen.h

OTHER_FILES += \
    README \
    LICENSE

#QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O3

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

