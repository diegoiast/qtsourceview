/**
 * \brief Third demo - usable editor
 *
 * This example opens a main window, in which a user open a file
 * and edit it. The app will then decide the best highlight defintion
 * for the loaded file.
 *
 * A better notepad than notepad.exe
 */

#include <QApplication>
#include "mainwindow3.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow3 w;
	w.show();
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}
