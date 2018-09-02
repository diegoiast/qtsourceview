DESTDIR		=	../../

QT		=	gui core xml widgets
CONFIG		+=	qt warn_on silent
TEMPLATE	=	app
SOURCES		=	main2.cpp   mainwindow2.cpp
FORMS		=	mainwindow2.ui
HEADERS		=	mainwindow2.h
INCLUDEPATH	=	../../src .
LIBS		=	-L../../ -lqvshl
