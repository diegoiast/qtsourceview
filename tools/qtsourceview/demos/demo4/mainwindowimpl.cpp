#include <QString>
#include <QFileDialog>
#include <QDir>

#include <QDebug>

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
	highlight = new QsvSyntaxHighlighter( textEdit, defColors, langDefinition );
	
	// set the background color of the text widget:
	QPalette p( textEdit->palette() );
	p.setColor( QPalette::Base, defColors->getColorDef("dsWidgetBackground").getBackground() );
	textEdit->setPalette( p );
	textEdit->setTextColor( defColors->getColorDef("dsNormal").getColor() );

	//textEdit->setCurrentLineColor( defColors->getColorDef("dsWidgetCurLine").getBackground() );
	//textEdit->setLinesPanelColor( defColors->getColorDef("dsWidgetLinesPanel").getBackground() );
	textEdit->setItemColor( LinesPanel, defColors->getColorDef("dsWidgetLinesPanel").getBackground() );
	textEdit->setItemColor( CurrentLine, defColors->getColorDef("dsWidgetCurLine").getBackground() );

	textEdit->loadFile( "mainwindowimpl.cpp" );
	connect( action_find, SIGNAL(triggered()), textEdit, SLOT(showFindWidget()) );
	
	connect( EditorConfig::getInstance(), SIGNAL(configurationModified()),
		this, SLOT(configuration_updated()) );

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

void MainWindowImpl::configuration_updated()
{
	//qDebug() << config->showLineNumbers;
	qDebug() << "reloading configuration";
#if 0	
	EditorConfigData configuration = EditorConfig::getInstance()->getCurrentConfiguration();
	
	//textEdit->setAutoBrackets( currentConfig.autoBrackets );
	textEdit->setDisplayCurrentLine( configuration.markCurrentLine );
	//currentConfig.showLineNumbers
	textEdit->getPanel()->setVisible( configuration.showLineNumbers );
	textEdit->setDisplayWhiteSpaces( configuration.showWhiteSpaces );
	textEdit->setDisplatMatchingBrackets( configuration.matchBrackes );
	textEdit->setMatchingString( configuration.matchBrackesList );
	
	//currentConfig.tabSize		= sbTabSize->value();
	textEdit->document()->setDefaultFont( configuration.currentFont );
	textEdit->getPanel()->setFont( configuration.currentFont );
	
	//currentConfig.currentColorScheme= colorSchemes[0];

	textEdit->adjustMarginWidgets();
	textEdit->update();
	textEdit->viewport()->update();
#else
	EditorConfig::getInstance()->applyCurrentConfiguration( textEdit );
#endif
}
