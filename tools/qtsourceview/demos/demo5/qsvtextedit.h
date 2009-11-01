#ifndef QSVTEXTEDIT_H
#define QSVTEXTEDIT_H

#include <QPlainTextEdit>
#include <QTextCharFormat>

class QsvSyntaxHighlighterBase;

class QsvTextEdit : public QPlainTextEdit
{
	Q_OBJECT
public:
	QsvTextEdit( QWidget *parent = 0, QsvSyntaxHighlighterBase *s = 0 );
	void setMatchBracketList( const QString &m );
	const QString getMatchBracketList();
public slots:
	void cursorMoved();
protected:
	int findMatchingChar( QChar c1, QChar c2, bool forward, QTextBlock &block, int from );
	QString m_matchBracketsList;
	QsvSyntaxHighlighterBase *m_highlighter;

	QTextCharFormat matchesFormat;
};

#endif // QSVTEXTEDIT_H
