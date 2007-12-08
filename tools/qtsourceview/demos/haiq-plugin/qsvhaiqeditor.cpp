#include <QtPlugin>
#include <QApplication>
#include <QStringList>
#include <haiqcoreinterface.h>

#include "qsvhaiqeditor.h"

Q_EXPORT_PLUGIN2(qtsourceview,QsvHaiqEditor)


QsvHaiqEditor::QsvHaiqEditor()
{
	qDebug("constcutor");
}

QsvHaiqEditor::~QsvHaiqEditor()
{
}

void QsvHaiqEditor::initialize(HaiQCoreInterface *core_in) 
{
	qDebug("initialize");
	core=core_in;
}

QString QsvHaiqEditor::name()
{
	return "QsvHaiqEditor";
}

QString QsvHaiqEditor::description()
{
	return "General purpose, kickass editor based on QtSourceView";
}

QString QsvHaiqEditor::authors()
{
	return "Diego Iastrubni";
}

QString QsvHaiqEditor::lastModified()
{
	return __DATE__;
}

double QsvHaiqEditor::loadOrder()
{
	return 50;
};

HaiQEditorInterface* QsvHaiqEditor::newInterface()
{
// 	DefaultEditorInterface *ret=new DefaultEditorInterface;
// 	ProHighlighter *highlighter=new ProHighlighter;
// 	highlighter->setDocument(ret->textedit_window()->document());
	return NULL;
}

QStringList QsvHaiqEditor::fileTypes()
{
	QStringList ret;
	ret << "*.cpp" << "*.h" << "*.h" << "*.*";
	return ret;
};
