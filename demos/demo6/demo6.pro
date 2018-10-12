TEMPLATE	=	app
QT		=	gui core xml widgets network concurrent
CONFIG		+=	qt warn_on silent
DESTDIR		=	../../
INCLUDEPATH	=	../../src .
LIBS		=	-L../../ -lqate -lqsvte
TARGET          =       demo6
SOURCES		=	main6.cpp
DEFINES         +=      CORE_EXPORT=Q_DECL_EXPORT
