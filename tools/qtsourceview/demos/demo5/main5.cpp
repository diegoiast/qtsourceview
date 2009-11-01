#include <QtGui/QApplication>
#include "mainwindow5.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow5 w;
	w.show();
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}
