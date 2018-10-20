QT += core
win*-msvc* {
    QMAKE_CXXFLAGS += -openmp
    LIBS += -openmp
} else {
    QMAKE_CXXFLAGS += -fopenmp
    LIBS += -fopenmp
}

TARGET = RangeSolverLib
TEMPLATE = lib

DEFINES += RANGESOLVERLIB_LIBRARY

INCLUDEPATH += include

SOURCES += \
    src/rconvection.cpp \
    src/reigenvaluesolver.cpp \
    src/rhemicube.cpp \
    src/rhemicubepixel.cpp \
    src/rhemicubesector.cpp \
    src/riterationinfo.cpp \
    src/riterationinfovalue.cpp \
    src/rlocalrotation.cpp \
    src/rmatrixmanager.cpp \
    src/rmatrixpreconditioner.cpp \
    src/rmatrixsolver.cpp \
    src/rscales.cpp \
    src/rsolver.cpp \
    src/rsolveracoustic.cpp \
    src/rsolverelectrostatics.cpp \
    src/rsolverfluid.cpp \
    src/rsolverfluidheat.cpp \
    src/rsolverfluidparticle.cpp \
    src/rsolvergeneric.cpp \
    src/rsolverheat.cpp \
    src/rsolvermagnetostatics.cpp \
    src/rsolvermesh.cpp \
    src/rsolverradiativeheat.cpp \
    src/rsolvershareddata.cpp \
    src/rsolverstress.cpp \
    src/rsolverwave.cpp

HEADERS += \
    include/rconvection.h \
    include/reigenvaluesolver.h \
    include/rhemicube.h \
    include/rhemicubepixel.h \
    include/rhemicubesector.h \
    include/riterationinfo.h \
    include/riterationinfovalue.h \
    include/rlocalrotation.h \
    include/rmatrixmanager.h \
    include/rmatrixpreconditioner.h \
    include/rmatrixsolver.h \
    include/rscales.h \
    include/rsolver.h \
    include/rsolveracoustic.h \
    include/rsolverelectrostatics.h \
    include/rsolverfluid.h \
    include/rsolverfluidheat.h \
    include/rsolverfluidparticle.h \
    include/rsolvergeneric.h \
    include/rsolverheat.h \
    include/rsolverlib.h \
    include/rsolvermagnetostatics.h \
    include/rsolvermesh.h \
    include/rsolverradiativeheat.h \
    include/rsolvershareddata.h \
    include/rsolverstress.h \
    include/rsolverwave.h

CONFIG -= debug_and_release
CONFIG += copy_dir_files
CONFIG += rtti
CONFIG += exceptions
#CONFIG += dll
CONFIG += staticlib

INCLUDEPATH += $${_PRO_FILE_PWD_}/../TetGen
INCLUDEPATH += $${_PRO_FILE_PWD_}/../RangeBase/include
INCLUDEPATH += $${_PRO_FILE_PWD_}/../RangeModel/include

DEFINES += TETLIBRARY

CONFIG(debug,debug|release) {
    TARGET = $$join(TARGET,,,_debug)
    DEFINES += DEBUG
}

