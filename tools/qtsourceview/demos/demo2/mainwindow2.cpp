#include <QTimer>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QSyntaxHighlighter>
#include <QFile>

#include "qsvcolordef.h"
#include "qsvcolordeffactory.h"
#include "qsvlangdef.h"
#include "qsvsyntaxhighlighter.h"
#include "mainwindow2.h"

//	QString dataPath  = QApplication::applicationDirPath();
QString dataPath;

MainWindow2::MainWindow2( QMainWindow *parent )
:QMainWindow( parent )
{
	dataPath  = QApplication::applicationDirPath() + "/../../";	
	setupUi( this );
        statusBar()->showMessage(tr("Welcome, the default syntax is C++"), 10000);
	QTimer::singleShot( 0, this, SLOT(fillComboBoxes()));
	
	// load a default color set
	defColors = new QsvColorDefFactory( dataPath + "/data/colors/kate.xml" );

        // load a default language definition
        langCpp   = new QsvLangDef( dataPath + "/data/langs/cpp.lang" );
        
	// assign to it the new syntax highlighter, with the default colors and language
	highlight = new QsvSyntaxHighlighter( textEdit, defColors, langCpp );
}

//comboBox_colors
void MainWindow2::fillComboBoxes()
{
	textEdit->setPlainText("done");
	QString directory = dataPath + "/data/langs/";
	QDir dir;
	
	dir = QDir( dataPath + "/data/langs/", "*.lang");
	syntaxFiles = dir.entryList(QDir::Files);
	comboBox_syntax->addItems( syntaxFiles );
		
	dir = QDir( dataPath + "/data/colors/", "*.xml");
	colorFiles = dir.entryList(QDir::Files);	
	comboBox_colors->addItems( colorFiles );
}

void MainWindow2::on_action_New_triggered()
{
	textEdit->clear();
}

void MainWindow2::on_action_Open_triggered()
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
        textEdit->setPlainText(in.readAll());
        QApplication::restoreOverrideCursor();

        statusBar()->showMessage(tr("File loaded"), 2000);
}

void MainWindow2::on_action_About_triggered()
{
	QMessageBox::information(this, "Demo 2",
    "First demo of the qtsourceview library.\n"
    "Diego Iastrubni <elcuco@kde.org> 2006, lincensed under the terms of the LGPL.");
}

void MainWindow2::on_actionAbout_Qt_triggered()
{
	QMessageBox::aboutQt( this, "Demo2" );
}

void MainWindow2::on_actionE_xit_triggered()
{
	this->close();
}
