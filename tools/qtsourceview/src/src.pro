TARGET = qtsourceview
TEMPLATE = lib
QT += xml
CONFIG += staticlib lib

VERSION = 0.0.1
DEPENDPATH += .
INCLUDEPATH += .

DESTDIR=../
TMP_DIR = ../tmp
UI_DIR = ../tmp
MOC_DIR = ../tmp
OBJECTS_DIR = ../tmp



# Input
HEADERS += qorderedmap.h \
           qsvcolordef.h \
           qsvcolordeffactory.h \
           qsvlangdef.h \
           qsvlangdeffactory.h \
           qsvsyntaxhighlighter.h
           
SOURCES += qsvcolordef.cpp \
           qsvcolordeffactory.cpp \
           qsvlangdef.cpp \
           qsvlangdeffactory.cpp \
           qsvsyntaxhighlighter.cpp