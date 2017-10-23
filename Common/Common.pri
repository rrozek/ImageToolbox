INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/Utils.h

SOURCES += $$PWD/Utils.cpp

#win32: QMAKE_PRE_LINK += if not exist \"$$absolute_path(../build/include)\" $$QMAKE_MKDIR \"$$absolute_path(../build/include)\" $$escape_expand(\\n\\t)
#unix: QMAKE_PRE_LINK += $$QMAKE_MKDIR -p \"$$absolute_path(../build/include)\" $$escape_expand(\\n\\t)
#QMAKE_PRE_LINK += $$QMAKE_COPY \"$$absolute_path(.)/\\\*.h\" \"$$absolute_path(../build/include)\" $$escape_expand(\\n\\t)
