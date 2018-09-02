# shared temp dir for all projects
DESTDIR		=	../../

CONFIG		+=	qt warn_on
QT              +=      xml network widgets concurrent
TARGET		=	demo-qate
SOURCES		+=	demo-qate.cpp

# TODO
#   in the future, the correct syntax should be
#   right now this will not work, because of include path not set correctly
DEFINES         +=      CORE_EXPORT=Q_DECL_EXPORT
LIBS		=	-L../../ -lqate
INCLUDEPATH	=	../../src ../../src/qate
TARGET		=	demo6-qate

