#ifndef __MAINWINDOW2_H__
#define __MAINWINDOW2_H__

#include <QMainWindow>
#include "ui_mainwindow1.h"

class QsvColorDefFactory;
class QsvLangDef;
class QSyntaxHighlighter;

class MainWindow1: public QMainWindow, private Ui::MainWindow1
{
	Q_OBJECT
public:	
	MainWindow1( QMainWindow *parent=0 ); 
	~MainWindow1(); 
	
public slots:
	void on_action_New_triggered();
	void on_action_Open_triggered();
	void on_action_About_triggered();
	void on_actionAbout_Qt_triggered();
	void on_actionE_xit_triggered();
	
private:
	QsvColorDefFactory	*defColors;
	QsvLangDef		*langCpp;
	QSyntaxHighlighter	*highlight;
};

#endif //__MAINWINDOW1_H__
