DESTDIR		=	../../

QT		=	gui core xml widgets
CONFIG		+=	qt warn_on silent
TEMPLATE	=	app
SOURCES         =       demo4.cpp 
HEADERS         =       mainwindow4.h
OTHER_FILES     +=      readme.txt
LIBS		=	-L../../ -lqsvte
INCLUDEPATH     =       ../../src/ .
