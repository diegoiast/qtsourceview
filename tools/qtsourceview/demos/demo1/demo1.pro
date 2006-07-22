QT =   gui \
  core \
  xml
CONFIG +=   qt \
  warn_on \
  release
TEMPLATE =   app
SOURCES =   main1.cpp \
  mainwindow1.cpp
FORMS =   mainwindow1.ui
HEADERS =   mainwindow1.h
INCLUDEPATH =   ../../src \
  .
LIBS =   -L../../ \
  -lqtsourceview
TMP_DIR =   ../../tmp/
UI_DIR =   ../../tmp/
MOC_DIR =   ../../tmp/
OBJECTS_DIR =   ../../tmp/
DESTDIR=../../
