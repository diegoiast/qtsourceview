#include <QApplication>
#include "mainwindow2.h"

/**
 * \brief Second demo - real life example for 
 *
 * This example opens a main window, which lets the user load a text file.
 * Then the user will be able to choose the syntax highlighter and color
 * definitions / pallete for the editor. 
 */


int main(int argc, char *argv[])
{
        QApplication a(argc, argv);
        MainWindow2 w;
        w.show();
        a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
        return a.exec();
}
