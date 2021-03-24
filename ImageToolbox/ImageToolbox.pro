#-------------------------------------------------
#
# Project created by QtCreator 2017-10-15T13:22:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageToolBox
TEMPLATE = app

CONFIG += console
DESTDIR = ../build/bin
OBJECTS_DIR = ../build/$${TARGET}/obj
MOC_DIR = ../build/$${TARGET}/moc

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
DEFINES += MAGICKCORE_HDRI_ENABLE
DEFINES += MAGICKCORE_QUANTUM_DEPTH=32
message($$DEFINES)

VERSION = 1.0.0
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
        ImageManipulationToolbox.cpp \
        WaitingSpinnerWidget.cpp \
        MetadataLoader.cpp \
        FileConverter.cpp \
        ConfigDialog.cpp \
        ConfigItemWidget.cpp \
        ConfigItemWidgetBool.cpp \
        ConfigItemWidgetSystemPath.cpp \
        ConfigItemWidgetString.cpp

HEADERS += \
        ToolBoxGui.h \
        model/JsonModel.h \
        ThumbnailLoader.h \
        UtilsGUI.h \
        ImageManipulationToolbox.h \
        WaitingSpinnerWidget.h \
        MetadataLoader.h \
        FileConverter.h \
        ConfigDialog.h \
        ConfigItemWidget.h \
        ConfigItemWidgetBool.h \
        ConfigItemWidgetSystemPath.h \
        ConfigItemWidgetString.h
LIBS += -L$$PWD/../build/lib/ -lImageToolBoxLib

INCLUDEPATH += $$PWD/../build/include
DEPENDPATH += $$PWD/../build/include

RESOURCES += \
    resources.qrc

FORMS += \
    ImageManipulationToolbox.ui \
    ConfigDialog.ui

