TEMPLATE =   app
CONFIG +=   release \
  warn_on
QT =   xml \
  gui \
  core
INCLUDEPATH =   . \
  ../../src/
TMP_DIR =   ../../tmp
UI_DIR =   ../../tmp
MOC_DIR =   ../../tmp
OBJECTS_DIR =   ../../tmp
SOURCES =   main3.cpp \
  mainwindow3.cpp
HEADERS =   mainwindow3.h
unix:LIBS =   -L../../ \
  -lqtsourceview
RESOURCES =   ../../src/qtsourceview.qrc
DESTDIR=../../