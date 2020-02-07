QT += core testlib

win*-msvc* {
    QMAKE_CXXFLAGS += -openmp
    LIB_EXT = "lib"
    LIB_PRE = ""
} else {
    QMAKE_CXXFLAGS += -fopenmp
    LIBS += -fopenmp
    LIB_EXT = "a"
    LIB_PRE = "lib"
}

TARGET = RangeTests
TEMPLATE = app

BUILDPATH = $${PWD}/../../build-range3

SOURCES += \
    TestRangeBase/tst_rbl_r3vector.cpp \
    TestRangeBase/tst_rbl_rmatrix.cpp \
    TestRangeBase/tst_rbl_rvector.cpp \
    TestRangeModel/tst_rml_sparse_vector.cpp \
    TestRangeModel/tst_rml_sparse_matrix.cpp \
    tst_main.cpp

HEADERS += \
    TestRangeBase/tst_rbl_r3vector.h \
    TestRangeBase/tst_rbl_rmatrix.h \
    TestRangeBase/tst_rbl_rvector.h \
    TestRangeModel/tst_rml_sparse_vector.h \
    TestRangeModel/tst_rml_sparse_matrix.h


CONFIG -= debug_and_release
CONFIG += rtti
CONFIG += exceptions
CONFIG += console
CONFIG -= app_bundle

DEBUG_EXT = ""

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_debug)
    DEFINES += DEBUG
    DEBUG_EXT = "_debug"
}

LIBS += \
    -L../TetGen/ \
    -L../RangeBase/ \
    -L../RangeModel/ \
    -L../RangeSolverLib/ \
    -lRangeSolverLib$${DEBUG_EXT} \
    -lRangeModel$${DEBUG_EXT} \
    -lRangeBase$${DEBUG_EXT} \
    -lTetGen$${DEBUG_EXT}

PRE_TARGETDEPS += \
    ../TetGen/$${LIB_PRE}TetGen$${DEBUG_EXT}.$${LIB_EXT} \
    ../RangeBase/$${LIB_PRE}RangeBase$${DEBUG_EXT}.$${LIB_EXT} \
    ../RangeModel/$${LIB_PRE}RangeModel$${DEBUG_EXT}.$${LIB_EXT} \
    ../RangeSolverLib/$${LIB_PRE}RangeSolverLib$${DEBUG_EXT}.$${LIB_EXT}

INCLUDEPATH += $${_PRO_FILE_PWD_}/../TetGen
INCLUDEPATH += $${_PRO_FILE_PWD_}/../RangeBase/include
INCLUDEPATH += $${_PRO_FILE_PWD_}/../RangeModel/include
INCLUDEPATH += $${_PRO_FILE_PWD_}/../RangeSolverLib/include
