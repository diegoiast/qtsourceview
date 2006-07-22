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
