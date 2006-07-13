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
HEADERS += kateitemdata.h \
           kateitemdatamanager.h \
           kateqtglobal.h \
           qegtkhighlighter.h \
           qegtklangdef.h \
           qelangdeffactory.h
SOURCES += kateitemdata.cpp \
           kateitemdatamanager.cpp \
           qegtkhighlighter.cpp \
           qegtklangdef.cpp \
           qelangdeffactory.cpp
