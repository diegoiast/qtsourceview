#include <QApplication>
#include <QAction>
#include <QToolBar>
#include <QTimer>
#include <QMainWindow>
#include <QFileDialog>
#include <QSyntaxHighlighter>

#include "qsvtextedit.h"
#include "qsvsyntaxhighlighterbase.h"
#include "qsvtextoperationswidget.h"

class MyHighlighter: public QSyntaxHighlighter, public QsvSyntaxHighlighterBase
{
public:
	MyHighlighter( QObject *parent=NULL) : QSyntaxHighlighter(parent)
	{
		setMatchBracketList("()[]''\"\"");
	}

	void highlightBlock(const QString &text)
	{
		QsvSyntaxHighlighterBase::highlightBlock(text);
	}
	
	virtual QTextBlockUserData* currentBlockUserDataProxy() 
	{
		return currentBlockUserData();
	}

	virtual void setCurrentBlockUserDataProxy(QTextBlockUserData * data)
	{
		setCurrentBlockUserData(data);
	}
	
};

class MainWindow : QMainWindow
{
	Q_OBJECT
public:
	MainWindow( const QString &file )
	{
		MyHighlighter             *s              = new MyHighlighter;
		QsvTextEdit             *e              = new QsvTextEdit(this, s);
		QsvTextOperationsWidget *textOpetations = new QsvTextOperationsWidget(e);
	
		QToolBar *b = addToolBar( "" );
		b->addAction( tr("&New"), e, SLOT(newDocument()))
		 ->setShortcut(QKeySequence("Ctrl+N"));
		b->addAction( tr("&Open"), this, SLOT(loadFile()))
		 ->setShortcut(QKeySequence("Ctrl+O"));
		b->addAction( tr("&Save"), e, SLOT(saveFile()))
		 ->setShortcut(QKeySequence("Ctrl+S"));
		b->addAction( tr("&Find"), textOpetations, SLOT(showSearch()))
		 ->setShortcut(QKeySequence("Ctrl+F"));
		b->addAction( tr("&Replace"), textOpetations, SLOT(showReplace()))
		 ->setShortcut(QKeySequence("Ctrl+R"));
		b->addAction( tr("Find &next"), textOpetations, SLOT(searchNext()))
		 ->setShortcut(QKeySequence("F3"));
		b->setMovable(false);
		b->addAction( tr("Find &prev"), textOpetations, SLOT(searchPrev()))
				->setShortcut(QKeySequence("Shift+F3"));
		b->setMovable(false);
		
		setCentralWidget(e);
		showMaximized();
		e->displayBannerMessage(tr("Click \"Open\" if you dare"));
		
		if (!file.isEmpty())
			loadFile(file);
		else {
			setWindowTitle("QtSourceView demo5");
			QFile f(":/qedit/readme.txt");
			if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
				return;
			e->setPlainText(f.readAll());
			e->removeModifications();
		}

#if 0
		e->setMarkCurrentLine(false);
		// tests for defaults
		e->setShowLineNumbers(true);
		e->setShowMargins(true);
		e->setTabSize(8);
		e->setTabIndents(true);
		e->setInsertSpacesInsteadOfTabs(true);
		e->setShowWhiteSpace(true);
#endif
	}
	
public slots:
	void loadFile( QString filename ="" )
	{
		const QString dir;
		if (filename.isEmpty()) {
			filename = QFileDialog::getOpenFileName(this,tr("Load file"),dir);
			if (filename.isEmpty())
				return;
		}
		QsvTextEdit *e = findChild<QsvTextEdit*>();
		if (!e)
			return;
		
//		TODO
//		if (e->isModified){
//			e->save();
//		}
		
		e->loadFile(filename);
		e->removeModifications();
		setWindowTitle(filename);
	}

private:
};

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w( a.arguments().count()>=2 ? a.arguments().at(1) : QString() );
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}

#include "main5.moc"
