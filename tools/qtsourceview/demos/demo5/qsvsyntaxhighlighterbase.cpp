#include "qsvsyntaxhighlighterbase.h"

#include <QDebug>
#include <QTextBlockUserData>
#include <QTextEdit>

QsvSyntaxHighlighterBase::QsvSyntaxHighlighterBase( QObject *parent ):
	QSyntaxHighlighter(parent)
{
}

void QsvSyntaxHighlighterBase::highlightBlock(const QString &text)
{
	QsvBlockData *data = static_cast<QsvBlockData*>(currentBlockUserData());
	if (data)
		data->matches.clear();

	if (text.isEmpty())
		return;

	// look up for each of our machting bracket list
	for ( int bracketIndex=0; bracketIndex<m_matchBracketsList.length(); bracketIndex++ )
	{
		QChar bracket       = m_matchBracketsList[bracketIndex];
		int bracketPosition = text.indexOf(bracket);

		while ( bracketPosition != -1 )
		{
			// lazy creation, only for needed blocks
			if (!data) data = new QsvBlockData;

			MatchData m;
			m.matchedChar = m_matchBracketsList[bracketIndex];
			m.position    = bracketPosition;
			data->matches << m;

			bracketPosition = text.indexOf(bracket, bracketPosition+1);
		}
	}
	setCurrentBlockUserData( data );
}

void QsvSyntaxHighlighterBase::setMatchBracketList( const QString &m )
{
	m_matchBracketsList = m;
}

const QString QsvSyntaxHighlighterBase::getMatchBracketList()
{
	return m_matchBracketsList;
}
