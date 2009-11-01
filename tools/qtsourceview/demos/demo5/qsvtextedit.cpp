#include <QList>

#include "qsvtextedit.h"
#include "qsvsyntaxhighlighterbase.h"

QsvTextEdit::QsvTextEdit( QWidget *parent, QsvSyntaxHighlighterBase *s ):
	QPlainTextEdit(parent)
{
	m_highlighter = s;
	connect( this, SIGNAL(cursorPositionChanged()), this, SLOT(cursorMoved()));
	m_highlighter->setDocument( document() );

	setMatchBracketList( "()[]{}\"\"''" );

	QFont f;
	f.setBold(true);
	matchesFormat = QTextCharFormat();
	matchesFormat.setBackground( QBrush(QColor(0xff,0xff,0x00,0xff) ));
	matchesFormat.setForeground( QBrush(QColor(0x00,0x80,0x00,0xff) ));
	matchesFormat.setFont(f);

}

// helper function, in Pascal it would have been an internal
// procedure inside cursorMove()
void appendExtraSelection( QList<QTextEdit::ExtraSelection> &selections,
	int position, QPlainTextEdit *self, QTextCharFormat matchesFormat )
{
	if (position==-1)
		return;

	QTextEdit::ExtraSelection selection;
//	QTextCharFormat format = selection.format;
//	format.setBackground(Qt::green);
//	selection.format = format;

	selection.format = matchesFormat;

	QTextCursor cursor = self->textCursor();
	cursor.setPosition(position);
	cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
	selection.cursor = cursor;

	selections.append(selection);
}

void QsvTextEdit::cursorMoved()
{
	// clear out previous matches
	QList<QTextEdit::ExtraSelection> selections;
	setExtraSelections(selections);

	// does this line have any brakcets?
	QsvBlockData *data = static_cast<QsvBlockData*>(textCursor().block().userData());
	if (!data)
		return;

	QTextCursor cursor         = textCursor();
	QTextBlock  block          = cursor.block();
	int blockPosition          = block.position();
	int cursorPosition         = cursor.position();
	int relativePosition       = cursorPosition - blockPosition;
	QChar currentChar          = block.text()[relativePosition];

	for( int k=0; k<data->matches.length(); k++)
	{
		MatchData m = data->matches.at(k);
		if (m.position != relativePosition)
			continue;

		appendExtraSelection(selections, cursorPosition, this, matchesFormat);

		// lets find it's partner
		// in theory, no errors shuold not happen, but one can never be too sure
		int j = m_matchBracketsList.indexOf( currentChar );
		if (j==-1)
			continue;

		if ( m_matchBracketsList[j] != m_matchBracketsList[j+1] )
			if (j %2 == 0)
				j = findMatchingChar( m_matchBracketsList[j], m_matchBracketsList[j+1], true , block, cursorPosition );
			else
				j = findMatchingChar( m_matchBracketsList[j], m_matchBracketsList[j-1], false, block, cursorPosition  );
		else
			j = findMatchingChar( m_matchBracketsList[j], m_matchBracketsList[j+1], true , block, cursorPosition );
		appendExtraSelection(selections, j, this,matchesFormat);
	}
	setExtraSelections(selections);
}

int	QsvTextEdit::findMatchingChar( QChar c1, QChar c2, bool forward, QTextBlock &block, int from )
{
	int i = 1;
	while (block.isValid())
	{
		QsvBlockData *data = static_cast<QsvBlockData*>(block.userData());
		for( int k=0; (data) && (k<data->matches.length()); k++)
		{
			int j = forward? k : data->matches.length() - k - 1;
			MatchData m = data->matches.at(j);
			int globalPosition = m.position + block.position();
			if (forward) {
				if (globalPosition <= from) continue;
			}	else if (globalPosition >= from) continue;

			if (m.matchedChar == c1)
				i ++;
			if (m.matchedChar == c2)
				i --;
			// we found the braket
			if (i==0)
			{
				return globalPosition;
			}
		}

		if (forward)
			block = block.next();
		else
			block = block.previous();
	}
	return -1;
}

void QsvTextEdit::setMatchBracketList( const QString &m )
{
	m_matchBracketsList = m;
	if (m_highlighter)
		m_highlighter->setMatchBracketList( m );
}

const QString QsvTextEdit::getMatchBracketList()
{
	return m_matchBracketsList;
}
