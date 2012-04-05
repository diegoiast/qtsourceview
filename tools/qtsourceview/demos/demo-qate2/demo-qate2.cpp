#include <unistd.h>
#include <QApplication>
#include "demo-qate2.h"

int main( int argc, char* argv[] )
{
	QApplication app( argc, argv );
	QMainWindow *mainWindow = new Demo2MainWindow;
	mainWindow->show();
	return app.exec();
}
