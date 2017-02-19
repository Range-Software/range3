QT += core
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

TARGET = RangeLicense
TEMPLATE = app

BUILDPATH = $${PWD}/../../build-range3

SOURCES += \
    src/main.cpp \
    src/application.cpp \
    src/main_task.cpp

HEADERS += \
    src/application.h \
    src/main_task.h

OTHER_FILES +=

RESOURCES +=

FORMS +=


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

LIBS += -L$${BUILDPATH}/lib/ -lRangeAuth$${DEBUG_EXT} -lRangeBase$${DEBUG_EXT}

INCLUDEPATH += $${BUILDPATH}/include
DEPENDPATH += $${BUILDPATH}/include

LIB_EXT = "a"

unix {
    target.path = $${BUILDPATH}/bin
}
win32 {
    target.path = $${BUILDPATH}\\bin
}

POST_TARGETDEPS += $${BUILDPATH}/lib/libRangeAuth$${DEBUG_EXT}.$${LIB_EXT}
POST_TARGETDEPS += $${BUILDPATH}/lib/libRangeBase$${DEBUG_EXT}.$${LIB_EXT}

INSTALLS += target
