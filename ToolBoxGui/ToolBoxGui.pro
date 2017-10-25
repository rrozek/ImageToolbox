#-------------------------------------------------
#
# Project created by QtCreator 2017-10-15T13:22:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ToolBoxGui
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

DEFINES += QT_MESSAGELOGCONTEXT
#DEFINES += QT_NO_DEBUG_OUTPUT

VERSION = 1.0.1
QMAKE_TARGET_COMPANY = "PBS Network GmbH"
QMAKE_TARGET_PRODUCT = "ImageToolbox"
QMAKE_TARGET_DESCRIPTION = "Image Toolbox"
QMAKE_TARGET_COPYRIGHT = "copyright 2017 PBS Network GmbH"

SOURCES += \
        main.cpp \
        ToolBoxGui.cpp \
        model/JsonModel.cpp \
        ThumbnailLoader.cpp \
        UtilsGUI.cpp \
        ImageManipulationToolbox.cpp

HEADERS += \
        ToolBoxGui.h \
        model/JsonModel.h \
        ThumbnailLoader.h \
        UtilsGUI.h \
        ImageManipulationToolbox.h

LIBS += -L$$PWD/../build-GSNImageToolbox-Desktop_Qt_5_9_1_MSVC2015_64bit-Release/release/ -lGSNImageToolbox
#LIBS += -L$$PWD/../build-GSNImageToolbox-Desktop_Qt_5_9_1_MSVC2015_64bit-Debug/debug/ -lGSNImageToolbox

INCLUDEPATH += $$PWD/../GSNImageToolbox/interface
DEPENDPATH += $$PWD/../GSNImageToolbox/interface

RESOURCES += \
    resources.qrc

FORMS += \
    ImageManipulationToolbox.ui

