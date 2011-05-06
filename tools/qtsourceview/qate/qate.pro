QTCREATR_DIR=/home/elcuco/src/gitorious/qt-creator

!exists(qate.pri){
	message("Please create a file called qate.pri, containing:")
	message("QTCREATR_DIR=...")
	error("configuration failed")
}
include(qate.pri)

EDITOR_DIR = $$QTCREATR_DIR/src/plugins/texteditor/generichighlighter
COREPLUGIN_DIR = $$QTCREATR_DIR/src/plugins/coreplugin

QT += xml
TARGET=qate-demo

SOURCES +=  \
	$$EDITOR_DIR/context.cpp \
	$$EDITOR_DIR/dynamicrule.cpp \
	$$EDITOR_DIR/highlightdefinition.cpp \
	$$EDITOR_DIR/highlighter.cpp \
	$$EDITOR_DIR/itemdata.cpp \
	$$EDITOR_DIR/includerulesinstruction.cpp \
	$$EDITOR_DIR/keywordlist.cpp \
	$$EDITOR_DIR/progressdata.cpp \
	$$EDITOR_DIR/rule.cpp \
	$$EDITOR_DIR/specificrules.cpp \
	qate/mimedatabase.cpp \
	qate/highlightdefinitionhandler-v2.cpp \
	qate/highlightdefinitionmanager.cpp \
	main.cpp\
	formats.cpp
#    highlightermock.cpp \

HEADERS += \
	$$EDITOR_DIR/context.h \
	$$EDITOR_DIR/dynamicrule.h \
	$$EDITOR_DIR/highlighter.h \
	$$EDITOR_DIR/highlightdefinition.h \
	$$EDITOR_DIR/includerulesinstruction.cpp \
	$$EDITOR_DIR/itemdata.h \
	$$EDITOR_DIR/keywordlist.h \
	$$EDITOR_DIR/progressdata.h \
	$$EDITOR_DIR/rule.h \
	$$EDITOR_DIR/specificrules.h \
	qate/mimedatabase.h \
	qate/highlightdefinitionmanager.h \
	qate/syntaxhighlighter.h \
	qate/basetextdocumentlayout.h \
	qate/tabsettings.h \
	formats.h 
#    highlightermock.h

INCLUDEPATH += $$EDITOR_DIR $$COREPLUGIN_DIR


#	qate/highlightdefinitionhanlder-v2.h \
