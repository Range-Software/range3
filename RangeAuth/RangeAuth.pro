QT += core

TARGET = RangeAuth
TEMPLATE = lib

BUILDPATH = $${PWD}/../../../build-range3

DEFINES += RANGEAUTH_LIBRARY

INCLUDEPATH += include

SOURCES += \
    src/rauthenticator.cpp \
    src/rlicenserecord.cpp \
    src/rlicense.cpp

HEADERS += \
    include/ralib.h \
    include/rauthenticator.h \
    include/rlicenserecord.h \
    include/rlicense.h

CONFIG -= debug_and_release
CONFIG += copy_dir_files
CONFIG += rtti
CONFIG += exceptions
#CONFIG += dll
CONFIG += staticlib

CONFIG(debug, debug|release) {
    LIBS += -L$${BUILDPATH}/lib/ -lRangeBase_debug
}
else {
    LIBS += -L$${BUILDPATH}/lib/ -lRangeBase
}

INCLUDEPATH += $${BUILDPATH}/include
DEPENDPATH += $${BUILDPATH}/include

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

