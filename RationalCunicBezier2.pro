#-------------------------------------------------
#
# Project created by QtCreator 2011-09-14T20:47:45
#
#-------------------------------------------------

QT       += core
QT       += gui
isEqual(QT_MAJOR_VERSION, 5): QT += opengl
isEqual(QT_MAJOR_VERSION, 6): QT += openglwidgets
isEqual(QT_MAJOR_VERSION, 6): QT += widgets

TARGET = QtSkeletonWithSettings3
CONFIG   += console
CONFIG   -= app_bundle

#QMAKE_CXXFLAGS  += -Wno-deprecated
##-Wdeprecated-declarations

TEMPLATE = app


SOURCES += main.cpp \
    CubicBezier.cpp \
    CurvatureWindow.cpp \
    glMainWindow.cpp \
    glWindow.cpp \
    mainWindowSlot.cpp \
    mainWindow.cpp \
    settingswindow.cpp

HEADERS += \
    CubicBezier.h \
    CurvatureWindow.h \
    axis.h \
    glMainWindow.h \
    glWindow.h \
    glWinsc.h \
    glWinsub3d.h \
    globject.h \
    mainWindow.h \
    settings.h \
    shaderfile.h \
    settingswindow.h



mac {
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib/
}

win32 {
INCLUDEPATH += c:/usr/local/include
LIBS += -LC:/usr/local/lib
LIBS += -lopengl32
LIBS += -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32
}

DISTFILES += \
    Shader/s2.frag \
    Shader/simple.frag \
    Shader/simple.vert
