#include "mainwindow5.h"
#include "qsvtextedit.h"
#include "qsvsyntaxhighlighterbase.h"
#include "qsvtextoperationswidget.h"

#include <QAction>
#include <QToolBar>
#include <QTimer>

MainWindow5::MainWindow5(QWidget *parent, Qt::WindowFlags flags ):
		QMainWindow(parent,flags)
{
	QsvSyntaxHighlighterBase *s = new QsvSyntaxHighlighterBase;
	QsvTextEdit *e = new QsvTextEdit(this, s);
	QsvTextOperationsWidget *textOpetations = new QsvTextOperationsWidget(e);

	e->setFont( QFont("Courier new", 10) );
	e->setLineWrapMode( QPlainTextEdit::NoWrap );

	QToolBar *b = this->addToolBar( "" );
	b->addAction( tr("Find"), textOpetations, SLOT(showSearch()))
	 ->setShortcut(QKeySequence("Ctrl+F"));
	b->addAction( tr("Replace"), textOpetations, SLOT(showReplace()))
	 ->setShortcut(QKeySequence("Ctrl+R"));
	b->setMovable(false);

#if 1
	e->setPlainText(
"#include <stdio.h>\n\n"
"int main()       \n"
"{       \n"
"	{ printf(\"Hello world!\\n\"); }           \n"
"		}\n" );
#else
	e->setPlainText("( adsdsad )\n[aaaa]\n{ qwqew { qeweqw } qwe ( asd a) }\n'sada\"s\"da'");
#endif
	setCentralWidget(e);
	showMaximized();
}
