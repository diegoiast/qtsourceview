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

#include "kateitemdatamanager.h"
#include "qegtklangdef.h"
#include "qegtkhighlighter.h"

#include "mainwindow1.h"

MainWindow1::MainWindow1(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
	QString	dataPath  = QApplication::applicationDirPath() + "/../../../";
	
	// load a default color set
	defColors = new QsvColorDefFactory( dataPath + "/data/colors/kate.xml" );
	
	// load a default language definition
	langCpp   = new QsvLangDef( dataPath + "/data/langs/cpp.lang" );
	
	// create a new text editor
	textEditor = new QTextEdit;
	textEditor->setAcceptRichText(false);

	// assign to it the new syntax highlighter, with the default colors and language
 	highlight = new QsvSyntaxHighlighter( textEditor, defColors, langCpp );

	setCentralWidget( textEditor );
	setupActions();
	createMenus();
	createToolbars();

	statusBar()->showMessage( "Welcome", 5000 );
}

MainWindow1::~MainWindow1()
{
	delete defColors;
	delete langCpp;
}

void MainWindow1::setupActions()
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

void MainWindow1::createMenus()
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

void MainWindow1::createToolbars()
{
	toolbar = new QToolBar;
	toolbar->addAction( actionNew );
	toolbar->addAction( actionOpen );
	toolbar->addAction( actionSave );
	toolbar->addSeparator();
	toolbar->addAction( actionQuit );

	addToolBar( toolbar );
}

void MainWindow1::fileNew()
{
	textEditor->clear();
}

void MainWindow1::fileOpen()
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

	QTextStream in(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	textEditor->setPlainText(in.readAll());
	QApplication::restoreOverrideCursor();

	statusBar()->showMessage(tr("File loaded"), 2000);
}
