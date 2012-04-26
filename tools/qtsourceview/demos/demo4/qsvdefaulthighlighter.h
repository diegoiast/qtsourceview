#include <QSyntaxHighlighter>
#include "qsvsyntaxhighlighterbase.h"

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

