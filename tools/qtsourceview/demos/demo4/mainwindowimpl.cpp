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

/*     -                       -                                               | 80 chars
12345678901234567890123456789012345678901234567890123456789012345678901234567890
         10        20        30        40        50        60        70        80
	8 CHARS			32 chars					80 chars
*/

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
	
	QString loadedFile;
	loadedFile = "mainwindowimpl.cpp";
	//loadedFile = "lineseditor.cpp";
	//loadedFile = "../../tests/highlight.pas";

	langDefinition = QsvLangDefFactory::getInstanse()->getHighlight(loadedFile);
	QsvSyntaxHighlighter *newSyntaxHighlighter = new QsvSyntaxHighlighter( textEdit->document(), defColors, langDefinition ); // bing 
	textEdit->setSyntaxHighlighter( newSyntaxHighlighter );
	textEdit->loadFile( loadedFile ); // bing
	setWindowTitle( tr("QtSourceView demo4 - %1").arg(loadedFile));
	connect( EditorConfig::getInstance(), SIGNAL(configurationModified()), this, SLOT(configuration_updated()));
	
	textEdit->setupActions();
	QMenu *tmpMenu = menuBar()->findChildren<QMenu*>( "menu_Edit" )[0];
	if (tmpMenu)
	{
		QMenu *new_menu = new QMenu( tr("Text actions"), tmpMenu );
		new_menu->setObjectName("actionsMenu");
		new_menu->addAction( textEdit->actionCapitalize );
		new_menu->addAction( textEdit->actionLowerCase );
		new_menu->addAction( textEdit->actionChangeCase );
		tmpMenu->addMenu( new_menu );

		new_menu = new QMenu( tr("Bookmarks"), tmpMenu );
		new_menu->addAction( textEdit->actionToggleBookmark );
		new_menu->addSeparator();
		new_menu->addAction( textEdit->actionNextBookmark );
		new_menu->addAction( textEdit->actionPrevBookmark );
		tmpMenu->addMenu( new_menu );

		tmpMenu->addAction( textEdit->actionTogglebreakpoint );
		tmpMenu->addAction( textEdit->actionFindMatchingBracket );
	}
	
	tmpMenu = menuBar()->findChildren<QMenu*>( "menu_Search" )[0];
	if (tmpMenu)
	{
		tmpMenu->addAction( textEdit->actionFind );
		tmpMenu->addAction( textEdit->actionFindNext );
		tmpMenu->addAction( textEdit->actionFindPrev );
		tmpMenu->addAction( textEdit->actionClearSearchHighlight );
		tmpMenu->addSeparator();
		tmpMenu->addAction( textEdit->actionReplace );
		tmpMenu->addSeparator();
		tmpMenu->addAction( textEdit->actionGotoLine );
	}
	
	connect( actionUndo, SIGNAL(triggered()), textEdit, SLOT(undo()));
	connect( actionRedo, SIGNAL(triggered()), textEdit, SLOT(redo()));

	connect( actionCopy, SIGNAL(triggered()), textEdit, SLOT(copy()));
	connect( actionCut, SIGNAL(triggered()), textEdit, SLOT(cut()));
	connect( actionPaste, SIGNAL(triggered()), textEdit, SLOT(paste()));
}

void MainWindowImpl::on_action_New_triggered()
{
	textEdit->clearEditor();
	setWindowTitle( tr("QtSourceView demo4 - no file loaded") );
	statusBar()->showMessage( tr("New file"), 5000 );
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
	{
		lastDir = s.left( i );
		s = s.mid( i+1 );
	}
	else
		lastDir.clear();
	
	textEdit->clear();	// bing
	langDefinition = QsvLangDefFactory::getInstanse()->getHighlight( s );
	textEdit->getSyntaxHighlighter()->setHighlight( langDefinition );
	textEdit->removeModifications();
	textEdit->loadFile(s);	// bing

	setWindowTitle( tr("QtSourceView demo4 - %1").arg(s));
	statusBar()->showMessage( tr("File %1 loaded").arg(s), 5000 );
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
