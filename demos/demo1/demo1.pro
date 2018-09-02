DESTDIR		=	../../

QT		=	gui core xml widgets
CONFIG		+=	qt warn_on silent
TEMPLATE	=	app
SOURCES		=	main1.cpp  
INCLUDEPATH	=	../../src .
LIBS		=	-L../../ -lqvshl
