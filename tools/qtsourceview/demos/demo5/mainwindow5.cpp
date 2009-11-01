#include "mainwindow5.h"
#include "qsvtextedit.h"
#include "qsvsyntaxhighlighterbase.h"


MainWindow5::MainWindow5(QWidget *parent, Qt::WindowFlags flags ):
		QMainWindow(parent,flags)
{
	QsvSyntaxHighlighterBase *s = new QsvSyntaxHighlighterBase;
	QsvTextEdit *e = new QsvTextEdit(this, s);
	e->setFont( QFont("Courier new", 10) );
	e->setLineWrapMode( QPlainTextEdit::NoWrap );

#if 1
	e->setPlainText(
"#include <stdio.h>\n\n"
"int main()\n"
"{\n"
"	{ printf(\"Hello world!\\n\"); } \n"
"}\n" );
#else
	e->setPlainText("( adsdsad )\n[aaaa]\n{ qwqew { qeweqw } qwe ( asd a) }\n'sada\"s\"da'");
#endif
	setCentralWidget(e);
	//showMaximized();
}
