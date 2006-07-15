TARGET =   demo1
TEMPLATE =   app
QT =   xml \
  gui \
  core
INCLUDEPATH =   . \
  ../../src/
TMP_DIR =   ../../tmp
UI_DIR =   ../../tmp
MOC_DIR =   ../../tmp
OBJECTS_DIR =   ../../tmp
HEADERS =   mainwindow1.h \
  ../../src/debug_info.h \
  ../../src/qorderedmap.h \
  ../../src/qsvcolordef.h \
  ../../src/qsvcolordeffactory.h \
  ../../src/qsvlangdef.h \
  ../../src/qsvlangdeffactory.h \
  ../../src/qsvsyntaxhighlighter.h
SOURCES =   main1.cpp \
  mainwindow1.cpp \
  ../../src/qsvcolordef.cpp \
  ../../src/qsvcolordeffactory.cpp \
  ../../src/qsvlangdef.cpp \
  ../../src/qsvlangdeffactory.cpp \
  ../../src/qsvsyntaxhighlighter.cpp
CONFIG +=   qt \
  release
