QT += core
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

TARGET = RangeLicense
TEMPLATE = app

SOURCES += \
    src/application.cpp \
    src/main.cpp \
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
    ../RangeBase/$${LIB_PRE}RangeBase$${DEBUG_EXT}.$${LIB_EXT} \
    ../RangeAuth/$${LIB_PRE}RangeAuth$${DEBUG_EXT}.$${LIB_EXT}

INCLUDEPATH += $${_PRO_FILE_PWD_}/../RangeBase/include
INCLUDEPATH += $${_PRO_FILE_PWD_}/../RangeAuth/include

