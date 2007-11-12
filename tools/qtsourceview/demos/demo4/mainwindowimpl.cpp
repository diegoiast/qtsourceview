#include <QString>
#include <QFileDialog>
#include <QDir>
#include <QMenu>
#include <QMenuItem>
#include <QDebug>

// QtSourceView includes
#include "qsvcolordef.h"
#include "qsvcolordeffactory.h"
#include "qsvlangdef.h"
#include "qsvlangdeffactory.h"
#include "qsvsyntaxhighlighter.h"

// local includes
#include "mainwindowimpl.h"
#include "editorconfig.h"

MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f) 
	: QMainWindow(parent, f)
{
	setupUi(this);
#ifdef WIN32
	QString dataPath  = QApplication::applicationDirPath() + "/../../..";
#else
	QString dataPath  = QApplication::applicationDirPath() + "/../..";
#endif

	QsvLangDefFactory::getInstanse()->addMimeTypes( dataPath  + "/src/mime.types" );
	QsvLangDefFactory::getInstanse()->loadDirectory( dataPath + "/data/langs/" );
	EditorConfig::getInstance()->loadColorsDirectory( dataPath + "/data/colors/" );
	
	defColors = new QsvColorDefFactory( dataPath + "/data/colors/kate.xml" );
	langDefinition = QsvLangDefFactory::getInstanse()->getHighlight("1.cpp");
	//highlight = new QsvSyntaxHighlighter( textEdit, defColors, langDefinition );
	
	textEdit->loadFile( "mainwindowimpl.cpp" );
	textEdit->setSyntaxHighlighter( new QsvSyntaxHighlighter( textEdit, defColors, langDefinition ) );	
	connect( EditorConfig::getInstance(), SIGNAL(configurationModified()), this, SLOT(configuration_updated()));
	
	textEdit->setupActions();
	QMenu *tmpMenu = menuBar()->findChildren<QMenu*>( "menu_Edit" )[0];
	if (tmpMenu)
	{
		QMenu *actionsMenu = new QMenu( "Text actions", tmpMenu );
		actionsMenu->setObjectName("actionsMenu");
		//actionsMenu->addAction( textEdit->actionFind );
		tmpMenu->actions().prepend( textEdit->actionFind );
		actionsMenu->addAction( textEdit->actionCapitalize );
		actionsMenu->addAction( textEdit->actionLowerCase );
		actionsMenu->addAction( textEdit->actionChangeCase );
		tmpMenu->addMenu( actionsMenu );
		tmpMenu->addAction( textEdit->actionToggleBookmark );
	}
	tmpMenu = menuBar()->findChildren<QMenu*>( "menu_Edit" )[0];
	if (tmpMenu)
		tmpMenu->addAction( textEdit->actionFind );	
	
	// debug
	//dumpObjectTree();
}

void MainWindowImpl::on_action_New_triggered()
{
	// TODO
}

void MainWindowImpl::on_action_Open_triggered()
{	
	QString s = QFileDialog::getOpenFileName( this, tr("Open File"),
		lastDir,
		tr("C/C++ source files")	+ " (*.c *.cpp *.h *.hpp );;" + 
		tr("Perl scripts")		+ " (*.pl);;" +
		tr("Shell scripts")		+ " (*.sh);;" +
		tr("All files")			+ " (*)"
	);
	
	if (s.isEmpty())
		return;
		
	int i = s.lastIndexOf("/");
	if (i==-1)
		s = s.lastIndexOf("\\");
	
	if (i!=-1)
		lastDir = s.left( i );
	else
		lastDir.clear();
	
	textEdit->loadFile(s);
	langDefinition = QsvLangDefFactory::getInstanse()->getHighlight( s );
	textEdit->getSyntaxHighlighter()->setHighlight( langDefinition );
}

void MainWindowImpl::on_actionE_xit_triggered()
{
	// TODO
}

void MainWindowImpl::on_action_setup_triggered()
{
	EditorConfig::getInstance()->showConfigDialog();
}

void MainWindowImpl::configuration_updated()
{
	EditorConfigData data = EditorConfig::getInstance()->getCurrentConfiguration();
	EditorConfig::applyConfiguration( data, textEdit );
}
