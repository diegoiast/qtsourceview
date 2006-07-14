#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QSyntaxHighlighter;
class QTextEdit;

class MainWindow1: public QMainWindow
{
    Q_OBJECT

public:
	MainWindow1(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow1();

	void setupActions();
	void createMenus();
	void createToolbars();

public slots:
	void fileNew();
	void fileOpen();
	
private:
	QSyntaxHighlighter *highlight;
	QTextEdit	*textEditor;
	QToolBar	*toolbar;

	QAction		*actionNew;
	QAction		*actionOpen;
	QAction		*actionSave;
	QAction		*actionSaveAs;
	QAction		*actionConfigure;
	QAction		*actionQuit;
};

#endif // MAINWINDOW_H
