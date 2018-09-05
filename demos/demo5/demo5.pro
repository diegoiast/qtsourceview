# shared temp dir for all projects
DESTDIR		=	../../
#DEFINES		+=	Q_CONCURRENT_EXPORT= CORE_EXPORT=Q_DECL_EXPORT

# set the Qate source dir, and include the pri file
# then add your sources and that's it
INCLUDEPATH	=	../../src .
LIBS		=	-L../../ -lqsvshl -lqsvte

QT		=	gui core xml widgets


CONFIG		+=	qt warn_on
TARGET		=	demo5
SOURCES		+=	main5.cpp 
