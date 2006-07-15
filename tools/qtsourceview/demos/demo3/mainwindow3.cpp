#include <QApplication>
#include <QTextEdit>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QMessageBox>

#include "qsvcolordef.h"
#include "qsvcolordeffactory.h"
#include "qsvlangdef.h"
#include "qsvlangdeffactory.h"
#include "qsvsyntaxhighlighter.h"

#include "mainwindow3.h"

MainWindow3::MainWindow3(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
//	QString	dataPath  = QApplication::applicationDirPath() + "/../../";
	QString	dataPath  = QApplication::applicationDirPath();
	
	QsvLangDefFactory::getInstanse()->loadDirectory( dataPath + "/data/langs/" );
	
	// load a default color set
	defaultColors = new QsvColorDefFactory( dataPath + "/data/colors/kate.xml" );
	
	// create a new text editor
	textEditor = new QTextEdit;
	textEditor->setAcceptRichText(false);

	// assign to it the new syntax highlighter, with the default colors and 
	// use by default the C++ language definitions
 	highlight = new QsvSyntaxHighlighter( textEditor, defaultColors, 
	 	QsvLangDefFactory::getInstanse()->getHighlight("1.cpp")
 	);

	setCentralWidget( textEditor );
	setupActions();
	createMenus();
	createToolbars();

	statusBar()->showMessage( "Welcome", 5000 );
}

MainWindow3::~MainWindow3()
{
	delete defaultColors;
}

void MainWindow3::setupActions()
{
	actionNew = new QAction( QIcon(":images/filenew.png"), tr("&New"), this );
	actionNew->setShortcut(tr("Ctrl+N"));
	actionNew->setStatusTip(tr("Create a new file"));
	connect( actionNew, SIGNAL(triggered()), this, SLOT(fileNew()) );
	
	actionOpen = new QAction( QIcon(":images/fileopen.png"), tr("&Open..."), this );
	actionOpen->setShortcut(tr("Ctrl+O"));
	actionOpen->setStatusTip(tr("Open a new file"));
	connect( actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()) );

	actionSave = new QAction( QIcon(":images/filesave.png"), tr("&Save"), this);
	actionSave->setShortcut(tr("Ctrl+S"));
	actionSave->setStatusTip(tr("Save the file"));
// 	connect( actionSave, SIGNAL(triggered()), this, SLOT(save()) );

	actionSaveAs = new QAction( QIcon(":images/filesaveas.png"), tr("&Save as..."), this);
	actionSaveAs->setStatusTip(tr("Save the file under a new name"));
//  	connect( actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()) );

	actionQuit = new QAction( QIcon(":images/exit.png"), tr("&Quit"), this);
	actionQuit->setShortcut(tr("Ctrl+Q"));
	actionQuit->setStatusTip(tr("Quit the application"));
	connect( actionQuit, SIGNAL(triggered()), this, SLOT(close()) );
}

void MainWindow3::createMenus()
{	
	QAction *aboutQtAct = new QAction(tr("About &Qt"), this);
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	
	QMenu *file = menuBar()->addMenu( "&File" );
	file->addAction( actionNew );
	file->addAction( actionOpen );
	file->addAction( actionSave );
	file->addAction( actionSaveAs );
	file->addSeparator();
	file->addAction( actionQuit );
	
	QMenu *help = menuBar()->addMenu( "&Help" );
	help->addAction( aboutQtAct );
}

void MainWindow3::createToolbars()
{
	toolbar = new QToolBar;
	toolbar->addAction( actionNew );
	toolbar->addAction( actionOpen );
	toolbar->addAction( actionSave );
	toolbar->addSeparator();
	toolbar->addAction( actionQuit );

	addToolBar( toolbar );
}

void MainWindow3::fileNew()
{
	textEditor->clear();
}

void MainWindow3::fileOpen()
{
	QString fileName = QFileDialog::getOpenFileName( this, "Open file", "", "*" );
	
	if (fileName.isEmpty() )
		return;
	
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(this, tr("Application"),
		       tr("Cannot read file %1:\n%2.")
		       .arg(fileName)
		       .arg(file.errorString()));
		return;
	}

	// detect the best hightlight for this file type
	// clear the document, so it will not get painted as well
	textEditor->clear();
	QsvLangDef *lang= QsvLangDefFactory::getInstanse()->getHighlight( fileName );
	highlight->setHighlight( lang );
	
	QTextStream in(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	textEditor->setPlainText(in.readAll());
	QApplication::restoreOverrideCursor();


	if (lang)
		statusBar()->showMessage( tr("File loaded"), 2000);
	else
		statusBar()->showMessage( tr("File loaded, but no highlight definition found"), 5000 );
}
 