QT =   gui \
  core \
  xml
CONFIG +=   qt \
  warn_on \
  release
TEMPLATE =   app
SOURCES =   main2.cpp \
  mainwindow2.cpp \
  ../../src/kateitemdata.cpp \
  ../../src/kateitemdatamanager.cpp \
  ../../src/qegtkhighlighter.cpp \
  ../../src/qegtklangdef.cpp \
  ../../src/qelangdeffactory.cpp
HEADERS =   mainwindow2.h \
  ../../src/debug_info.h \
  ../../src/kateitemdata.h \
  ../../src/kateitemdatamanager.h \
  ../../src/kateqtglobal.h \
  ../../src/qegtkhighlighter.h \
  ../../src/qegtklangdef.h \
  ../../src/qelangdeffactory.h \
  ../../src/qorderedmap.h
INCLUDEPATH =   ../../src/ \
  .
RESOURCES =   demo2.qrc
