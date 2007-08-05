#include <QString>
#include <QFileDialog>
#include <QDir>

// qt source view includes
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
	QString dataPath  = QApplication::applicationDirPath() + "/../..";

	QsvLangDefFactory::getInstanse()->addMimeTypes( dataPath  + "/src/mime.types" );
	QsvLangDefFactory::getInstanse()->loadDirectory( dataPath + "/data/langs/" );
	EditorConfig::getInstance()->loadColorsDirectory( dataPath + "/data/colors/" );
	
	defColors = new QsvColorDefFactory( dataPath + "/data/colors/turbo.xml" );
	langDefinition = QsvLangDefFactory::getInstanse()->getHighlight("1.cpp");
	highlight = new QsvSyntaxHighlighter( textEdit, defColors, langDefinition );
	
	// set the background color of the text widget:
	QPalette p( textEdit->palette() );
	p.setColor( QPalette::Base, defColors->getColorDef("dsWidgetBackground").getBackground() );
	textEdit->setPalette( p );
	textEdit->setTextColor( defColors->getColorDef("dsNormal").getColor() );

	textEdit->setItemColor( LinesPanel, defColors->getColorDef("dsWidgetLinesPanel").getBackground() );
	textEdit->setItemColor( CurrentLine, defColors->getColorDef("dsWidgetCurLine").getBackground() );

	textEdit->loadFile( "mainwindowimpl.cpp" );
	connect( action_find, SIGNAL(triggered()), textEdit, SLOT(showFindWidget()) );
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
		tr("All files")          + " (*)"
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
	highlight->setHighlight( langDefinition );
	highlight->rehighlight();
}

void MainWindowImpl::on_actionE_xit_triggered()
{
	// TODO
}

void MainWindowImpl::on_action_setup_triggered()
{
	EditorConfig::getInstance()->showConfigDialog();
}
