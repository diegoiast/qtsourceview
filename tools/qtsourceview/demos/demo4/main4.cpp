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
	
	virtual void toggleBookmark(QTextBlock &block)
	{
		QsvBlockData *data = getBlockData(block);
		if (data == NULL)
			return;
		data->toggleBookmark();
	}

	virtual void removeModification(QTextBlock &block)
	{
		QsvBlockData *data = getBlockData(block);
		if (data == NULL)
			return;
		data->m_isModified = false;
	}

	virtual void setBlockModified(QTextBlock &block, bool on)
	{
		QsvBlockData *data = getBlockData(block);
		if (data == NULL)
			return;
		data->m_isModified =  true;
	}

	virtual bool isBlockModified(QTextBlock &block)
	{
		QsvBlockData *data = getBlockData(block);
		if (data == NULL)
			false;
		return data->m_isModified;
	}

	virtual bool isBlockBookmarked(QTextBlock &block)
	{
		QsvBlockData *data = getBlockData(block);
		if (data == NULL)
			return false;
		return data->isBookmark();
	}

	virtual QsvBlockData::LineFlags getBlockFlags(QTextBlock &block)
	{
		QsvBlockData *data = getBlockData(block);
		if (data == NULL)
			return false;
		return data->m_flags;
	}
	
	QsvBlockData *getBlockData(QTextBlock &block)
	{
		QTextBlockUserData *userData  = block.userData();
		QsvBlockData       *blockData = NULL;
		
		if (userData == NULL){
			blockData =  new QsvBlockData();
			block.setUserData(blockData);
		} else {
			blockData = dynamic_cast<QsvBlockData*>(userData);
		}
		return blockData;
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
			setWindowTitle("QtSourceView demo4");
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

#include "main4.moc"
