HAIQDIR=/home/elcuco/src/svn/haiq

TEMPLATE = lib
CONFIG += plugin qt release
#CONFIG += warn_on console debug_and_release

TARGET = qtsourceview
include($$HAIQDIR/translations.pri)
include($$HAIQDIR/haiqplugin.pri)
DESTDIR=./

TMP_DIR =   tmp
UI_DIR =   tmp
MOC_DIR =   tmp
OBJECTS_DIR =   tmp

LIBS +=
QT += xml
INCLUDEPATH += $$HAIQDIR/haiqcoreinterface ../../src ../demo4 .

SOURCES += qsvhaiqeditor.cpp  qsvhaiqeditorinterface.cpp \
 ../demo4/samplepanel.cpp \
 ../demo4/lineseditor.cpp \
 ../demo4/transparentwidget.cpp \
 ../demo4/newlineedit.cpp \
 ../demo4/mainwindowimpl.cpp \
 ../demo4/editorconfig.cpp \
 ../demo4/colorsmodel.cpp \
 ../demo4/privateblockdata.cpp \
 ../../src/qsvcolordef.cpp \
 ../../src/qsvcolordeffactory.cpp \
 ../../src/qsvlangdef.cpp \
 ../../src/qsvlangdeffactory.cpp \
 ../../src/qsvsyntaxhighlighter.cpp

HEADERS += qsvhaiqeditor.h qsvhaiqeditorinterface.h \
 ../demo4/samplepanel.h \
 ../demo4/lineseditor.h \
 ../demo4/transparentwidget.h \
 ../demo4/newlineedit.h \
 ../demo4/mainwindowimpl.h \
 ../demo4/editorconfig.h \
 ../demo4/colorsmodel.h \
 ../demo4/privateblockdata.h \
 ../../src/debug_info.h \
 ../../src/qorderedmap.h \
 ../../src/qsvcolordef.h \
 ../../src/qsvcolordeffactory.h \
 ../../src/qsvlangdef.h \
 ../../src/qsvlangdeffactory.h \
 ../../src/qsvsyntaxhighlighter.h
 
FORMS += ../demo4/findwidget.ui \
 ../demo4/mainwindow.ui \
 ../demo4/configdialog.ui \
 ../demo4/filemessage.ui \
 ../demo4/replacewidget.ui \
 ../demo4/gotolinewidget.ui

RESOURCES += ../demo4/textlines.qrc
