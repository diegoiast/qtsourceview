#include <QApplication>
#include <QAction>
#include <QToolBar>
#include <QTimer>
#include <QMainWindow>
#include <QFileDialog>
#include <QSyntaxHighlighter>

#include "qsvsh/qsvcolordef.h"
#include "qsvsh/qsvcolordeffactory.h"
#include "qsvsh/qsvlangdef.h"
#include "qsvsh/qsvlangdeffactory.h"
#include "qsvsh/qsvsyntaxhighlighter.h"

#include "qsvtextedit.h"
#include "qsvsyntaxhighlighterbase.h"
#include "qsvtextoperationswidget.h"


class MyHighlighter: public QsvSyntaxHighlighter, public QsvSyntaxHighlighterBase
{
public:
	MyHighlighter(QTextDocument *parent): QsvSyntaxHighlighter(parent)
	{
		setMatchBracketList("{}()[]''\"\"");
	}

	void highlightBlock(const QString &text)
	{
		QsvSyntaxHighlighterBase::highlightBlock(text);
		QsvSyntaxHighlighter::highlightBlock(text);
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
	QsvTextEdit *editor;
	QsvSyntaxHighlighterBase * syntax;
	QsvColorDefFactory 	 * defColors;
	QsvLangDef 		 * langDefinition;
	MyHighlighter    	 * highlight;
	QsvTextOperationsWidget  * textOpetations;
public:
	MainWindow( const QString &file )
	{
		QString dataPath  = QApplication::applicationDirPath();
		QsvLangDefFactory::getInstanse()->addMimeTypes( dataPath +"/src/qsvsh/mime.types" );
		QsvLangDefFactory::getInstanse()->loadDirectory( dataPath + "/data/langs/" );
		editor           = new QsvTextEdit(this, NULL);
		defColors        = new QsvColorDefFactory( dataPath + "/data/colors/kate.xml" );
		langDefinition   = QsvLangDefFactory::getInstanse()->getHighlight("1.cpp");
		highlight        = new MyHighlighter(editor->document());
		textOpetations = new QsvTextOperationsWidget(editor);

		highlight->setColorsDef(defColors);
		highlight->setHighlight(langDefinition);
		editor->setHighlighter(highlight);


		QToolBar *b = addToolBar( "" );
		b->addAction( tr("&New"), editor, SLOT(newDocument()))              ->setShortcut(QKeySequence("Ctrl+N"));
		b->addAction( tr("&Open"), this, SLOT(loadFile()))                  ->setShortcut(QKeySequence("Ctrl+O"));
		b->addAction( tr("&Save"), editor, SLOT(saveFile()))                ->setShortcut(QKeySequence("Ctrl+S"));
		b->addAction( tr("&Find"), textOpetations, SLOT(showSearch()))      ->setShortcut(QKeySequence("Ctrl+F"));
		b->addAction( tr("&Replace"), textOpetations, SLOT(showReplace()))  ->setShortcut(QKeySequence("Ctrl+R"));
		b->addAction( tr("Find &next"), textOpetations, SLOT(searchNext())) ->setShortcut(QKeySequence("F3"));
		b->setMovable(false);
		b->addAction( tr("Find &prev"), textOpetations, SLOT(searchPrev())) ->setShortcut(QKeySequence("Shift+F3"));
		b->setMovable(false);
		
		setCentralWidget(editor);
		showMaximized();
		editor->displayBannerMessage(tr("Click \"Open\" if you dare"));
		
		if (!file.isEmpty())
			loadFile(file);
		else {
			setWindowTitle("QtSourceView demo7");
			QFile f(":/qedit/readme.txt");
			if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
				return;
			editor->setPlainText(f.readAll());
			editor->removeModifications();
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
//		TODO
//		if (e->isModified){
//			e->save();
//		}
		
		editor->clear();
		langDefinition = QsvLangDefFactory::getInstanse()->getHighlight(filename);
		highlight->setHighlight(langDefinition);
		editor->loadFile(filename);
		editor->removeModifications();
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

#include "main7.moc"
