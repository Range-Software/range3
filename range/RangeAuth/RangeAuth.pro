QT += core

TARGET = RangeAuth
TEMPLATE = lib

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

INCLUDEPATH += $${_PRO_FILE_PWD_}/../RangeBase/include

DEFINES += TETLIBRARY

CONFIG(debug,debug|release) {
    TARGET = $$join(TARGET,,,_debug)
    DEFINES += DEBUG
}

