# shared temp dir for all projects
TMP_DIR		=	../../tmp/
UI_DIR		=	../../tmp/
MOC_DIR		=	../../tmp/
OBJECTS_DIR	=	../../tmp/
DESTDIR		=	../../

# set the Qate source dir, and include the pri file
# then add your sources and that's it
INCLUDEPATH	=	../../src .
LIBS		=	-L../../ -lqtsourceview

QT		=	gui core xml


CONFIG		+=	qt warn_on silent
TARGET		=	demo7
SOURCES		+=	main7.cpp \
    ../demo5/qsvtextedit.cpp \
    ../demo5/qsvsyntaxhighlighterbase.cpp \
    ../demo5/qsvtextoperationswidget.cpp

HEADERS += ../demo5/qsvtextedit.h \
    ../demo5/qsvsyntaxhighlighterbase.h \
    ../demo5/qsvtextoperationswidget.h
FORMS += ../demo5/searchform.ui \
    ../demo5/replaceform.ui \
    ../demo5/bannermessage.ui
RESOURCES += ../demo5/demo5.qrc
INCLUDEPATH+=../demo5/
