TARGET = demo1
TEMPLATE = app

QT += xml
#unix:LIBS  += -L../../ -lqtsourceview

INCLUDEPATH += . ../../src/ 
#DESTDIR=../../
TMP_DIR = ../../tmp
UI_DIR = ../../tmp
MOC_DIR = ../../tmp
OBJECTS_DIR = ../../tmp

# Input
HEADERS += mainwindow1.h
SOURCES += main1.cpp mainwindow1.cpp


# Input - library
HEADERS += ../../src/kateitemdata.h \
           ../../src/kateitemdatamanager.h \
           ../../src/kateqtglobal.h \
           ../../src/qegtkhighlighter.h \
           ../../src/qegtklangdef.h \
           ../../src/qelangdeffactory.h
SOURCES += ../../src/kateitemdata.cpp \
           ../../src/kateitemdatamanager.cpp \
           ../../src/qegtkhighlighter.cpp \
           ../../src/qegtklangdef.cpp \
           ../../src/qelangdeffactory.cpp

