QT += core
QT -= gui

CONFIG += c++11 c++14

TARGET = testApp
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

unix:!macx|win32: LIBS += -L$$PWD/../build-GSNImageToolbox-Desktop_Qt_5_9_1_MSVC2015_64bit-Release/release/ -lGSNImageToolbox
#unix:!macx|win32: LIBS += -L$$PWD/../build-GSNImageToolbox-Desktop_Qt_5_9_1_MSVC2015_64bit-Debug/debug/ -lGSNImageToolbox

#unix:!macx|win32: LIBS += -L$$PWD/../build-GSNImageToolbox-Desktop_Qt_5_7_0_GCC_64bit-Debug/ -lGSNImageToolbox
#unix:!macx|win32: LIBS += -L$$PWD/../build-GSNImageToolbox-Desktop_Qt_5_7_0_GCC_64bit-Release/ -lGSNImageToolbox


INCLUDEPATH += $$PWD/../GSNImageToolbox/src/include
DEPENDPATH += $$PWD/../GSNImageToolbox/src/include
