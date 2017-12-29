#-------------------------------------------------
#
# Project created by QtCreator 2017-08-23T13:06:10
#
#-------------------------------------------------

QT       -= gui

TARGET = ImageToolBoxLib
TEMPLATE = lib

DESTDIR = ../build/lib
OBJECTS_DIR = ../build/$${TARGET}/obj
MOC_DIR = ../build/$${TARGET}/moc

CONFIG += c++11 c++14
DEFINES += IMAGETOOLBOXLIB

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "PBS Network GmbH"
QMAKE_TARGET_PRODUCT = "ImageToolboxLib"
QMAKE_TARGET_DESCRIPTION = "Image Toolbox Library"
QMAKE_TARGET_COPYRIGHT = "copyright 2017 PBS Network GmbH"

SOURCES += \
    src/impl/ToolBox.cpp \
    src/impl/ImageInfo.cpp \
    src/impl/handlers/MultiPageImageHandler.cpp \
    src/impl/handlers/SingleImageHandler.cpp \
    src/impl/handlers/IHandler.cpp \
    src/impl/jsonObjects/JsonObjectBase.cpp \
    src/impl/jsonObjects/JsonObjectRoot.cpp


HEADERS += \
    src/include/ToolBox.h \
    src/include/ImageInfo.h \
    src/impl/handlers/IHandler.h \
    src/impl/handlers/MultiPageImageHandler.h \
    src/impl/handlers/SingleImageHandler.h \
    src/impl/jsonObjects/JsonFields.h \
    src/impl/jsonObjects/JsonObjectBase.h \
    src/impl/jsonObjects/JsonObjectRoot.h \
    src/include/ImageToolboxLib_global.h

win32: QMAKE_PRE_LINK += if not exist \"$$absolute_path(../build/include)\" $$QMAKE_MKDIR \"$$absolute_path(../build/include)\" $$escape_expand(\\n\\t)
unix: QMAKE_PRE_LINK += $$QMAKE_MKDIR -p \"$$absolute_path(../build/include)\" $$escape_expand(\\n\\t)
QMAKE_PRE_LINK += $$QMAKE_COPY \"$$absolute_path(./src/include)/\\\*.h\" \"$$absolute_path(./interface)\" $$escape_expand(\\n\\t)
QMAKE_PRE_LINK += $$QMAKE_COPY \"$$absolute_path(./src/impl/Common)/\\\*.h\" \"$$absolute_path(./interface)\" $$escape_expand(\\n\\t)
QMAKE_PRE_LINK += $$QMAKE_COPY \"$$absolute_path(./src/impl/jsonObjects)/\\\*.h\" \"$$absolute_path(./interface)\" $$escape_expand(\\n\\t)
QMAKE_PRE_LINK += $$QMAKE_COPY \"$$absolute_path(./interface)/\\\*.h\" \"$$absolute_path(../build/include)\" $$escape_expand(\\n\\t)

win32: QMAKE_POST_LINK += if not exist \"$$absolute_path(../build/bin)\" $$QMAKE_MKDIR \"$$absolute_path(../build/bin)\" $$escape_expand(\\n\\t)
unix: QMAKE_POST_LINK += $$QMAKE_MKDIR -p \"$$absolute_path(../build/bin)\" $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$QMAKE_COPY \"$$absolute_path(../build/lib/)\" \"$$absolute_path(../build/bin/)\" $$escape_expand(\\n\\t)
export(QMAKE_POST_LINK)

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include(src/impl/Common/Common.pri)

LIBS += -L"C:/Program Files/ImageMagick-7.0.7-Q16/lib" -lCORE_RL_Magick++_
LIBS += -L"C:/Program Files/ImageMagick-7.0.7-Q16/lib" -lCORE_RL_MagickCore_

INCLUDEPATH += $$PWD/src/include
DEPENDPATH += $$PWD/src/include
INCLUDEPATH += "C:/Program Files/ImageMagick-7.0.7-Q16/include"
DEPENDPATH += "C:/Program Files/ImageMagick-7.0.7-Q16/include"

RESOURCES += \
    resources.qrc
