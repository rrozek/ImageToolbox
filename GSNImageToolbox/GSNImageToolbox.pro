#-------------------------------------------------
#
# Project created by QtCreator 2017-08-23T13:06:10
#
#-------------------------------------------------

QT       -= gui

TARGET = GSNImageToolbox
TEMPLATE = lib

DEFINES += GSNIMAGETOOLBOX_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/impl/ToolBox.cpp \
    src/impl/ImageInfo.cpp \


HEADERS += \
        src/include/ToolBox.h \
        src/include/gsnimagetoolbox_global.h \
        src/include/ImageInfo.h \
        src/include/Common.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


unix:!macx|win32: LIBS += -L$$PWD/src/lib/ -lCORE_RL_Magick++_
unix:!macx|win32: LIBS += -L$$PWD/src/lib/ -lCORE_RL_MagickCore_

INCLUDEPATH += $$PWD/src/include/ImageMagick
DEPENDPATH += $$PWD/src/include/ImageMagick
INCLUDEPATH += $$PWD/src/include
DEPENDPATH += $$PWD/src/include
