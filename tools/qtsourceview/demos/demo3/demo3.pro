QT =   gui \
  core \
  xml
CONFIG +=   qt \
  warn_on \
  release
TEMPLATE =   app
SOURCES =   main3.cpp \
  mainwindow3.cpp
FORMS =   mainwindow3.ui
HEADERS =   mainwindow3.h
INCLUDEPATH =   ../../src \
  .
LIBS =   -L../../ \
  -lqtsourceview
TMP_DIR =   ../../tmp/
UI_DIR =   ../../tmp/
MOC_DIR =   ../../tmp/
OBJECTS_DIR =   ../../tmp/
DESTDIR=../../
