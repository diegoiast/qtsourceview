# shared temp dir for all projects
TMP_DIR		=	../../tmp/
UI_DIR		=	../../tmp/
MOC_DIR		=	../../tmp/
OBJECTS_DIR	=	../../tmp/
DESTDIR		=	../../

QT		=	gui core xml
CONFIG		+=	qt warn_on silent
TEMPLATE	=	app
SOURCES		=	main1.cpp   mainwindow1.cpp
FORMS		=	mainwindow1.ui
HEADERS		=	mainwindow1.h
INCLUDEPATH	=	../../src .
LIBS		=	-L../../ -lqtsourceview
