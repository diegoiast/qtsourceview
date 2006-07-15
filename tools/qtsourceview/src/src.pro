TARGET =   qtsourceview
TEMPLATE =   lib
QT =   xml \
  core \
  gui
CONFIG +=   staticlib \
  debug \
  warn_on \
  lib
VERSION =   0.0.1
DEPENDPATH =   .
INCLUDEPATH =   .
DESTDIR =   ../
TMP_DIR =   ../tmp
UI_DIR =   ../tmp
MOC_DIR =   ../tmp
OBJECTS_DIR =   ../tmp
HEADERS =   qorderedmap.h \
  qsvcolordef.h \
  qsvcolordeffactory.h \
  qsvlangdef.h \
  qsvlangdeffactory.h \
  qsvsyntaxhighlighter.h \
  debug_info.h
SOURCES =   qsvcolordef.cpp \
  qsvcolordeffactory.cpp \
  qsvlangdef.cpp \
  qsvlangdeffactory.cpp \
  qsvsyntaxhighlighter.cpp
RESOURCES =   qtsourceview.qrc
