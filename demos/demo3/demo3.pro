DESTDIR		=	../../

QT		=	gui core xml widgets
CONFIG		+=	qt warn_on silent
TEMPLATE	=	app
SOURCES		=	main3.cpp   mainwindow3.cpp
FORMS		=	mainwindow3.ui
HEADERS		=	mainwindow3.h
INCLUDEPATH	=	../../src .
LIBS		=	-L../../ -lqvshl
