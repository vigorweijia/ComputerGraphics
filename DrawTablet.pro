#-------------------------------------------------
#
# Project created by QtCreator 2019-10-13T11:12:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DrawTablet
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    NewCanvasDialog.cpp \
    LineDialog.cpp \
    EllipseDialog.cpp \
    PolygonDialog.cpp \
    ColorDialog.cpp \
    TranslateDialog.cpp \
    RotateDialog.cpp \
    ScaleDialog.cpp \
    ClipDialog.cpp \
    CurveDialog.cpp \
    GalaxyDialog.cpp

HEADERS += \
        mainwindow.h \
    NewCanvasDialog.h \
    LineDialog.h \
    EllipseDialog.h \
    PolygonDialog.h \
    ColorDialog.h \
    TranslateDialog.h \
    RotateDialog.h \
    ScaleDialog.h \
    ClipDialog.h \
    CurveDialog.h \
    GalaxyDialog.h

FORMS += \
        mainwindow.ui \
    NewCanvasDialog.ui \
    LineDialog.ui \
    EllipseDialog.ui \
    PolygonDialog.ui \
    ColorDialog.ui \
    TranslateDialog.ui \
    RotateDialog.ui \
    ScaleDialog.ui \
    ClipDialog.ui \
    CurveDialog.ui \
    GalaxyDialog.ui

RESOURCES += \
    resources.qrc

