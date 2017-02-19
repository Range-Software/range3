QT += core gui opengl printsupport network
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

TARGET = GuiTest
TEMPLATE = app

BUILDPATH = $${PWD}/../../../build-range3

SOURCES += \
    src/main.cpp \
    src/application.cpp \
    src/mainwindow.cpp \
    src/glwidget.cpp

HEADERS += \
    src/application.h \
    src/mainwindow.h \
    src/glwidget.h

CONFIG -= debug_and_release
#CONFIG += rtti
CONFIG += exceptions

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,_debug)
    DEFINES += DEBUG
    LIBS += -L$${BUILDPATH}/lib/ -lRangeSolverLib_debug -lRangeModel_debug -lRangeBase_debug -lTetGen_debug
}
else {
    LIBS += -L$${BUILDPATH}/lib/ -lRangeSolverLib -lRangeModel -lRangeBase -lTetGen
}

INCLUDEPATH += $${BUILDPATH}/include
DEPENDPATH += $${BUILDPATH}/include

unix {
    LIBS += -lGL -lGLU
    target.path = $${BUILDPATH}/bin
}
win32 {
    LIBS += -lopengl32 -lglu32
    target.path = $${BUILDPATH}\\bin
}
INSTALLS += target

OTHER_FILES +=

RESOURCES += \
    guitest.qrc

FORMS +=

