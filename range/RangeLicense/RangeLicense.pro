QT += core
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

TARGET = RangeLicense
TEMPLATE = app

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
    -L../RangeBase/ \
    -L../RangeAuth/ \
    -lRangeAuth$${DEBUG_EXT} \
    -lRangeBase$${DEBUG_EXT}

PRE_TARGETDEPS += \
    ../RangeBase/libRangeBase$${DEBUG_EXT}.a \
    ../RangeAuth/libRangeAuth$${DEBUG_EXT}.a

INCLUDEPATH += $${_PRO_FILE_PWD_}/../RangeBase/include
INCLUDEPATH += $${_PRO_FILE_PWD_}/../RangeAuth/include

