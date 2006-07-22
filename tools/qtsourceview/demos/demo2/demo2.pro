QT =   gui \
  core \
  xml
CONFIG +=   qt \
  warn_on \
  release
TEMPLATE =   app
SOURCES =   main.cpp \
  mainwindow2.cpp
FORMS =   mainwindow2.ui
HEADERS =   mainwindow2.h
INCLUDEPATH =   ../../src \
  .
LIBS =   -L../../ \
  -lqtsourceview
