#include <QObject>
#include <QSyntaxHighlighter>
#include <QApplication>
#include <QAction>
#include <QToolBar>
#include <QTimer>
#include <QMainWindow>
#include <QFileDialog>

#include "qsvtextedit.h"
#include "qsvsyntaxhighlighterbase.h"
#include "qsvtextoperationswidget.h"

class DefaultHighlighter: public QSyntaxHighlighter, public QsvSyntaxHighlighterBase
{
public:
	DefaultHighlighter( QObject *parent=NULL);
	void highlightBlock(const QString &text);
	virtual void toggleBookmark(QTextBlock &block);
	virtual void removeModification(QTextBlock &block);
	virtual void setBlockModified(QTextBlock &block, bool on);
	virtual bool isBlockModified(QTextBlock &block);
	virtual bool isBlockBookmarked(QTextBlock &block);
	virtual QsvBlockData::LineFlags getBlockFlags(QTextBlock &block);
	QsvBlockData *getBlockData(QTextBlock &block);
};

