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
#include "mainwindow1.h"

MainWindow1::MainWindow1( QMainWindow *parent )
:QMainWindow( parent )
{
	setupUi( this );
//	QString dataPath  = QApplication::applicationDirPath();
	QString dataPath  = QApplication::applicationDirPath() + "/../../";
	
	// load a default color set
	defColors = new QsvColorDefFactory( dataPath + "/data/colors/kate.xml" );

        // load a default language definition
        langCpp   = new QsvLangDef( dataPath + "/data/langs/cpp.lang" );
        
	// assign to it the new syntax highlighter, with the default colors and language
	highlight = new QsvSyntaxHighlighter( textEdit, defColors, langCpp );
	
        statusBar()->showMessage(tr("Welcome, the default syntax is C++"), 10000);
		
}

void MainWindow1::on_action_New_triggered()
{
	textEdit->clear();
}

void MainWindow1::on_action_Open_triggered()
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

void MainWindow1::on_action_About_triggered()
{
	QMessageBox::information(this, "Demo 1",
    "First demo of the qtsourceview library.\n"
    "Diego Iastrubni <elcuco@kde.org> 2006, lincensed under the terms of the LGPL.");
}

void MainWindow1::on_actionAbout_Qt_triggered()
{
	QMessageBox::aboutQt( this, "diego" );
}

void MainWindow1::on_actionE_xit_triggered()
{
	this->close();
}
