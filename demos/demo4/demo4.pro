# shared temp dir for all projects
DESTDIR		=	../../

QT		=	gui core xml
CONFIG		+=	qt warn_on silent
TEMPLATE	=	app

QATE_SRC_DIR=../../src/
include(../../src/qate.pri)
LIBS		=	-L../../ -lqate

SOURCES = demo4.cpp qsvtextedit.cpp qsvsyntaxhighlighterbase.cpp qsvtextoperationswidget.cpp  qsvdefaulthighlighter.cpp
HEADERS += qsvtextedit.h qsvsyntaxhighlighterbase.h qsvtextoperationswidget.h mainwindow4.h
FORMS += searchform.ui replaceform.ui bannermessage.ui
RESOURCES += demo4.qrc
OTHER_FILES += readme.txt
