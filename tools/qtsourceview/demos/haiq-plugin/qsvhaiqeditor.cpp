#include <QtPlugin>
#include <QApplication>
#include <QStringList>

#include "qsvhaiqeditor.h"
#include "qsvhaiqeditorinterface.h"

Q_EXPORT_PLUGIN2(qtsourceview,QsvHaiqEditorPlugin)


QsvHaiqEditorPlugin::QsvHaiqEditorPlugin()
{
}

QsvHaiqEditorPlugin::~QsvHaiqEditorPlugin()
{
}

void QsvHaiqEditorPlugin::initialize(HaiQCoreInterface *core_in) 
{
	Q_UNUSED( core_in );
}

QString QsvHaiqEditorPlugin::name()
{
	return "QsvHaiqEditorPlugin";
}

QString QsvHaiqEditorPlugin::description()
{
	return "General purpose, kickass editor based on QtSourceView";
}

QString QsvHaiqEditorPlugin::authors()
{
	return "Diego Iastrubni";
}

QString QsvHaiqEditorPlugin::lastModified()
{
	return __DATE__;
}

double QsvHaiqEditorPlugin::loadOrder()
{
	return 50;
};

HaiQEditorInterface* QsvHaiqEditorPlugin::newInterface()
{
	return new QsvHaiqEditorInterface;
}

QStringList QsvHaiqEditorPlugin::fileTypes()
{
	QStringList ret;
	ret << "c" << "cpp" << "h" << "";
	return ret;
};
