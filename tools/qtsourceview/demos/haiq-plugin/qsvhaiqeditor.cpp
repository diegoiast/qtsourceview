// Qt includes
#include <QtPlugin>
#include <QAction>
#include <QApplication>
#include <QStringList>

// HaiQ includes
#include <haiqinterfaces.h>

// QtSourceView includes
// #include "qsvcolordeffactory.h"
#include "qsvlangdeffactory.h"
#include "editorconfig.h"

// plugin includes
#include "qsvhaiqeditor.h"
#include "qsvhaiqeditorinterface.h"

// qtsourceview is the library file name
Q_EXPORT_PLUGIN2(qtsourceview,QsvHaiqEditorPlugin)


QsvHaiqEditorPlugin::QsvHaiqEditorPlugin()
{
}

QsvHaiqEditorPlugin::~QsvHaiqEditorPlugin()
{
}

void QsvHaiqEditorPlugin::initialize(HaiQCoreInterface *core_in) 
{
	m_core = core_in;
	
	m_actionConfigQtSourceView = new QAction( tr("Configure QtSourceView"), this );
	connect(m_actionConfigQtSourceView,SIGNAL(triggered()),this,SLOT(on_actionConfigQtSourceView_triggered()));
	
	HaiQActionList *list = core_in->mainWindowManager->actionList("Tools Menu");
	list->addAction( m_actionConfigQtSourceView );
	
	QString langsPath = m_core->settingsManager->property("qtsourceview","langs_path").toString();
	QString colorsPath = m_core->settingsManager->property("qtsourceview","colors_path").toString();
	QString mimeTypesFile = m_core->settingsManager->property("qtsourceview","mime_types_file").toString();
	
	if (langsPath.isEmpty())
	{
		langsPath = qApp->applicationDirPath()+"/share/langs";
		m_core->settingsManager->setProperty("qtsourceview","langs_path",langsPath);
		
	}
	
	if (colorsPath.isEmpty())
	{
		colorsPath = qApp->applicationDirPath()+"/share/colors";
		m_core->settingsManager->setProperty("qtsourceview","colors_path",colorsPath);
		
	}
	
	if (mimeTypesFile.isEmpty())
	{
		mimeTypesFile = qApp->applicationDirPath()+"/share/mime.types";
		m_core->settingsManager->setProperty("qtsourceview","mime_types_file",mimeTypesFile);
	}
	
	QsvLangDefFactory::getInstanse()->addMimeTypes( mimeTypesFile );
	QsvLangDefFactory::getInstanse()->loadDirectory( langsPath );
	EditorConfig::getInstance()->loadColorsDirectory( colorsPath );
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
}

HaiQEditorInterface* QsvHaiqEditorPlugin::newInterface()
{
	HaiQEditorInterface *e = new QsvHaiqEditorInterface;
	//LinesEditor *editor = qobject_cast<LinesEditor*>(e->editorWindow());
	//editor->adjustMarginWidgets();
	return e;
}

void QsvHaiqEditorPlugin::activate(HaiQEditorInterface *interface)
{
	QsvHaiqEditorInterface *qsvInterface = dynamic_cast<QsvHaiqEditorInterface*>(interface);
	if (!qsvInterface)
		return;
	
	LinesEditor *editor = qobject_cast<LinesEditor*>(qsvInterface->editorWindow());
	if (!editor)
		return;
	
	editor->adjustMarginWidgets();
}

void QsvHaiqEditorPlugin::deactivate()
{
}

QStringList QsvHaiqEditorPlugin::fileTypes()
{
	QStringList ret;
	ret << "c" << "cpp" << "h" << "pro" << "";
	return ret;
}

void QsvHaiqEditorPlugin::on_actionConfigQtSourceView_triggered()
{
	EditorConfig::getInstance()->showConfigDialog();
}
