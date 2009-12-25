#ifndef QSVSYNTAXHIGHLIGHTERBASE_H
#define QSVSYNTAXHIGHLIGHTERBASE_H

#include <QSyntaxHighlighter>

class MatchData {
public:
	QChar matchedChar;
	int position;
};

class QsvBlockData : public QTextBlockUserData {
public:
	QList<MatchData> matches;
	bool m_isCurrentDebugLine;
	bool m_isBookmark;
	bool m_isBreakpoint;
	bool m_isModified;

	QsvBlockData(){
		m_isCurrentDebugLine = false;
		m_isBookmark = false;
		m_isBreakpoint = false;
		m_isModified = false;
	}
};

class QsvSyntaxHighlighterBase : public QSyntaxHighlighter
{
	Q_OBJECT
public:
	QsvSyntaxHighlighterBase( QObject* parent=NULL );

	void setMatchBracketList( const QString &m );
	const QString getMatchBracketList();
protected:
	void highlightBlock(const QString &text);
	QString m_matchBracketsList;
};

#endif // QSVSYNTAXHIGHLIGHTERBASE_H
