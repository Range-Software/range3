QT += core
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

TARGET = RangeSolver
TEMPLATE = app

BUILDPATH = $${PWD}/../../build-range3

SOURCES += \
    src/main.cpp \
    src/application.cpp \
    src/solver_task.cpp \
    src/main_task.cpp \
    src/command_processor.cpp \
    src/solver_input.cpp

HEADERS += \
    src/application.h \
    src/solver_task.h \
    src/main_task.h \
    src/command_processor.h \
    src/solver_input.h


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

INCLUDEPATH += $${_PRO_FILE_PWD_}/../TetGen
INCLUDEPATH += $${_PRO_FILE_PWD_}/../RangeBase/include
INCLUDEPATH += $${_PRO_FILE_PWD_}/../RangeAuth/include
INCLUDEPATH += $${_PRO_FILE_PWD_}/../RangeModel/include
INCLUDEPATH += $${_PRO_FILE_PWD_}/../RangeSolverLib/include

LIB_EXT = "a"

unix {
    target.path = $${BUILDPATH}/bin
}
win32 {
    target.path = $${BUILDPATH}\\bin
}

INSTALLS += target
