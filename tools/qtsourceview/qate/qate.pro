!exists(qate.pri){
	message("Please create a file called qate.pri, containing:")
	message("QTCREATOR_DIR=...")
	error("configuration failed")
}
include(qate.pri)

EDITOR_DIR = $$QTCREATOR_DIR/src/plugins/texteditor/generichighlighter
COREPLUGIN_DIR = $$QTCREATOR_DIR/src/plugins/coreplugin

QT += xml network
CONFIG+=silent
TARGET=qate-demo
DEFINES+= CORE_EXPORT=Q_DECL_EXPORT

SOURCES +=  \
	$$EDITOR_DIR/context.cpp \
	$$EDITOR_DIR/definitiondownloader.cpp \
	$$EDITOR_DIR/dynamicrule.cpp \
	$$EDITOR_DIR/highlightdefinition.cpp \
	$$EDITOR_DIR/highlightdefinitionmetadata.cpp \
	$$EDITOR_DIR/highlighter.cpp \
	$$EDITOR_DIR/itemdata.cpp \
	$$EDITOR_DIR/includerulesinstruction.cpp \
	$$EDITOR_DIR/keywordlist.cpp \
	$$EDITOR_DIR/progressdata.cpp \
	$$EDITOR_DIR/rule.cpp \
	$$EDITOR_DIR/specificrules.cpp \
	qate/mimedatabase.cpp \
	qate/highlightdefinitionhandler.cpp \
	qate/highlightdefinitionmanager.cpp \
	qate/defaultcolors.cpp \

HEADERS += \
	$$EDITOR_DIR/context.h \
	$$EDITOR_DIR/definitiondownloader.h \
	$$EDITOR_DIR/dynamicrule.h \
	$$EDITOR_DIR/highlighter.h \
	$$EDITOR_DIR/highlightdefinition.h \
	$$EDITOR_DIR/highlightdefinitionmetadata.h \
	$$EDITOR_DIR/includerulesinstruction.h \
	$$EDITOR_DIR/itemdata.h \
	$$EDITOR_DIR/keywordlist.h \
	$$EDITOR_DIR/progressdata.h \
	$$EDITOR_DIR/rule.h \
	$$EDITOR_DIR/specificrules.h \
	qate/mimedatabase.h \
	qate/highlightdefinitionhandler.h \
	qate/highlightdefinitionmanager.h \
	qate/syntaxhighlighter.h \
	qate/basetextdocumentlayout.h \
	qate/tabsettings.h \
	qate/defaultcolors.h 

INCLUDEPATH += qate $$EDITOR_DIR $$QTCREATOR_DIR/src/libs/


#	qate/highlightdefinitionhanlder-v2.h \
