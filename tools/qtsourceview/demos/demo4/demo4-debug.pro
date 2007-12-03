TEMPLATE = app
SOURCES += main.cpp \
 samplepanel.cpp \
 lineseditor.cpp \
 transparentwidget.cpp \
 newlineedit.cpp \
 mainwindowimpl.cpp \
 editorconfig.cpp \
 colorsmodel.cpp \
 ../../src/qsvcolordef.cpp \
 ../../src/qsvcolordeffactory.cpp \
 ../../src/qsvlangdef.cpp \
 ../../src/qsvlangdeffactory.cpp \
 ../../src/qsvsyntaxhighlighter.cpp \
 privateblockdata.cpp
HEADERS += samplepanel.h \
 lineseditor.h \
 transparentwidget.h \
 newlineedit.h \
 mainwindowimpl.h \
 editorconfig.h \
 colorsmodel.h \
 ../../src/debug_info.h \
 ../../src/qorderedmap.h \
 ../../src/qsvcolordef.h \
 ../../src/qsvcolordeffactory.h \
 ../../src/qsvlangdef.h \
 ../../src/qsvlangdeffactory.h \
 ../../src/qsvsyntaxhighlighter.h \
 privateblockdata.h
CONFIG += warn_on console debug_and_release
FORMS += findwidget.ui \
 mainwindow.ui \
 configdialog.ui \
 filemessage.ui \
 replacewidget.ui \
 gotolinewidget.ui
RESOURCES += textlines.qrc
INCLUDEPATH += ../../src src
LIBS +=
QT += xml
