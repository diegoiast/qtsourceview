TEMPLATE = app
SOURCES += main.cpp \
 samplepanel.cpp \
 lineseditor.cpp \
 transparentwidget.cpp \
 newlineedit.cpp \
 mainwindowimpl.cpp \
 editorconfig.cpp
HEADERS += samplepanel.h \
 lineseditor.h \
 transparentwidget.h \
 newlineedit.h \
 mainwindowimpl.h \
 editorconfig.h
CONFIG += debug_and_release warn_on
FORMS += findwidget.ui mainwindow.ui configdialog.ui
RESOURCES += textlines.qrc
INCLUDEPATH += ../../src src
LIBS += -L../../ -lqtsourceview
QT += xml
