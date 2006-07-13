#include <QtGui/QApplication>
#include <QTextEdit>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>

#include "mainwindow1.h"

#include "kateitemdatamanager.h"
#include "qegtklangdef.h"
#include "qegtklangdef.h"
#include "qegtkhighlighter.h"

MainWindow1::MainWindow1(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
	QTextEdit              *edit      = new QTextEdit;
	kateItemDataManager    *defColors = new kateItemDataManager;
	defColors->load( QApplication::applicationDirPath() + "/../../data/colors/kate.xml" );
	QeGtkSourceViewLangDef *langCpp   = new QeGtkSourceViewLangDef( QApplication::applicationDirPath() + "/../../data/langs/cpp.lang" );
	QeGTK_Highlighter      *highlight = new QeGTK_Highlighter( edit, defColors );
	highlight->setHighlight( langCpp );

	
	setCentralWidget( edit );

	setupActions();
	createMenus();
	createToolbars();

	statusBar()->showMessage( "Welcome", 5000 );
}

MainWindow1::~MainWindow1()
{
// 	delete defColors;	
// 	delete configDialog;
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
	QMenu *file = menuBar()->addMenu( "&File" );
	file->addAction( actionNew );
	file->addAction( actionOpen );
	file->addAction( actionSave );
	file->addAction( actionSaveAs );
	file->addSeparator();
	file->addAction( actionQuit );

	QMenu *edit = menuBar()->addMenu( "&Edit" );
// 	edit->addAction( textDisplay->actionUndo );
// 	edit->addAction( textDisplay->actionRedo );
// 	edit->addSeparator();
// 	edit->addAction( textDisplay->actionCut );
// // 	edit->addAction( textDisplay->actionCopy );
// 	edit->addAction( textDisplay->actionPaste );
// 	edit->addSeparator();
// 	edit->addAction( actionConfigure );

	QMenu *help = menuBar()->addMenu( "&Help" );
// 	help->addAction( new QAction( tr("About QT"), qApp, SLOT(aboutQt) ));
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
}

void MainWindow1::fileOpen()
{
	QString fileName = QFileDialog::getOpenFileName( this, "Open file", "", "*" );
	
// 	if (!fileName.isEmpty() )
// 		textDisplay->loadFile( fileName );
}
