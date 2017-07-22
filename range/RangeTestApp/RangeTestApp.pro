QT += core
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

TARGET = RangeTestApp
TEMPLATE = app

BUILDPATH = $${PWD}/../../build-range3

SOURCES += \
    src/main.cpp \
    src/application.cpp \
    src/main_task.cpp

HEADERS += \
    src/application.h \
    src/main_task.h


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
    -L../RangeAuth/ \
    -L../RangeModel/ \
    -L../RangeSolverLib/ \
    -lRangeSolverLib$${DEBUG_EXT} \
    -lRangeModel$${DEBUG_EXT} \
    -lRangeBase$${DEBUG_EXT} \
    -lRangeAuth$${DEBUG_EXT} \
    -lTetGen$${DEBUG_EXT}

PRE_TARGETDEPS += \
    ../TetGen/libTetGen$${DEBUG_EXT}.a \
    ../RangeBase/libRangeBase$${DEBUG_EXT}.a \
    ../RangeAuth/libRangeAuth$${DEBUG_EXT}.a \
    ../RangeModel/libRangeModel$${DEBUG_EXT}.a \
    ../RangeSolverLib/libRangeSolverLib$${DEBUG_EXT}.a

INCLUDEPATH += $${_PRO_FILE_PWD_}/../TetGen
INCLUDEPATH += $${_PRO_FILE_PWD_}/../RangeBase/include
INCLUDEPATH += $${_PRO_FILE_PWD_}/../RangeAuth/include
INCLUDEPATH += $${_PRO_FILE_PWD_}/../RangeModel/include
INCLUDEPATH += $${_PRO_FILE_PWD_}/../RangeSolverLib/include

