#-------------------------------------------------
#
# Project created by QtCreator 2013-07-12T09:08:26
#
#-------------------------------------------------

QT += core
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

TARGET = RangeSolver
TEMPLATE = app

BUILDPATH = $${PWD}/../../../build-range3

SOURCES += \
    src/main.cpp \
    src/application.cpp \
    src/solver_task.cpp \
    src/main_task.cpp \
    src/command_processor.cpp \
    src/solver_input.cpp


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

LIBS += -L$${BUILDPATH}/lib/ -lRangeSolverLib$${DEBUG_EXT} -lRangeModel$${DEBUG_EXT} -lRangeBase$${DEBUG_EXT} -lRangeAuth$${DEBUG_EXT} -lTetGen$${DEBUG_EXT}

INCLUDEPATH += $${BUILDPATH}/include
DEPENDPATH += $${BUILDPATH}/include

LIB_EXT = "a"

unix {
    target.path = $${BUILDPATH}/bin
}
win32 {
    target.path = $${BUILDPATH}\\bin
}

POST_TARGETDEPS += $${BUILDPATH}/lib/libRangeSolverLib$${DEBUG_EXT}.$${LIB_EXT}
POST_TARGETDEPS += $${BUILDPATH}/lib/libRangeModel$${DEBUG_EXT}.$${LIB_EXT}
POST_TARGETDEPS += $${BUILDPATH}/lib/libRangeBase$${DEBUG_EXT}.$${LIB_EXT}
POST_TARGETDEPS += $${BUILDPATH}/lib/libRangeAuth$${DEBUG_EXT}.$${LIB_EXT}
POST_TARGETDEPS += $${BUILDPATH}/lib/libTetGen$${DEBUG_EXT}.$${LIB_EXT}

INSTALLS += target

OTHER_FILES +=

RESOURCES +=

FORMS +=

HEADERS += \
    src/application.h \
    src/solver_task.h \
    src/main_task.h \
    src/command_processor.h \
    src/solver_input.h
