#include <QApplication>
#include <QScrollBar>
#include <QAbstractSlider>
#include <QList>
#include <QPainter>

#include "qsvtextedit.h"
#include "qsvsyntaxhighlighterbase.h"
#include "qsveditorpanel.h"

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

	m_panel = new QsvEditorPanel(this);
}

// helper function, in Pascal it would have been an internal
// procedure inside cursorMove()
void appendExtraSelection( QList<QTextEdit::ExtraSelection> &selections,
	int position, QPlainTextEdit *self, QTextCharFormat matchesFormat )
{
	if (position==-1)
		return;

	QTextEdit::ExtraSelection selection;
	QTextCursor cursor = self->textCursor();
	selection.format = matchesFormat;
	cursor.setPosition(position);
	cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
	selection.cursor = cursor;

	selections.append(selection);
}

void QsvTextEdit::cursorMoved()
{
	// does this line have any brakcets?
	QsvBlockData *data = static_cast<QsvBlockData*>(textCursor().block().userData());
	if (!data){
		m_panel->update();
		return;
	}

	// clear out previous matches
	QList<QTextEdit::ExtraSelection> selections;
	setExtraSelections(selections);

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

void	QsvTextEdit::paintEvent(QPaintEvent *e)
{
	QPlainTextEdit::paintEvent(e);

	// TODO - paint the line number
}

void	QsvTextEdit::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);

	// this get connected in QsvTextOperationsWidget
	emit(widgetResized());

	setViewportMargins( m_panel->width()-1, 0, 0, 0 );
	QRect viewportRect = viewport()->geometry();
	QRect lrect = QRect(viewportRect.topLeft(), viewportRect.bottomLeft());
	lrect.adjust( -m_panel->width(), 0, 0, 0 );
	m_panel->setGeometry(lrect);
}

void	QsvTextEdit::keyPressEvent(QKeyEvent *e)
{
	QTextCursor c = textCursor();
	switch (e->key()) {
		case Qt::Key_Escape:
			if (c.hasSelection()) {
				c.clearSelection();
				setTextCursor(c);
				e->accept();
			}
			break;
		case Qt::Key_Down:
			if (!QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
				break;
			verticalScrollBar()->triggerAction( QAbstractSlider::SliderSingleStepAdd );
			e->accept();
			break;
		case Qt::Key_Up:
			if (!QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
				break;
			verticalScrollBar()->triggerAction( QAbstractSlider::SliderSingleStepSub );
			e->accept();
			break;
		case Qt::Key_PageDown:
			if (!QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
				break;
			verticalScrollBar()->triggerAction( QAbstractSlider::SliderPageStepAdd);
			e->accept();
			break;
		case Qt::Key_PageUp:
			if (!QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
				break;
			verticalScrollBar()->triggerAction( QAbstractSlider::SliderPageStepSub );
			e->accept();
			break;
#ifdef TAB_INDENTS
		case Qt::Key_Tab:
			if (tabIndents) {
				if (handleIndentEvent( !(event->modifiers() & Qt::ShiftModifier) ))
					// do not call original hanlder, if this was handled by that function
					return;
			}
			break;
		case Qt::Key_Backtab:
			if (tabIndents)
				if (handleIndentEvent(false))
					return;
#endif

#ifdef SMARTHOME
		case Qt::Key_Home:
			if (!usingSmartHome || QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) )
				break;
			smartHome();
			e->accept();
			return;
		case Qt::Key_End:
			if (!usingSmartHome || QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) )
				break;
			smartEnd();
			e->accept();
			return;
#endif

#ifdef AUTO_BRAKETS
		default:
			if (usingAutoBrackets && handleKeyPressEvent(event))
				return;
#endif
	} // end case
	QPlainTextEdit::keyPressEvent(e);
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

void QsvTextEdit::paintPanel(QPaintEvent*e)
{
	QPainter p(m_panel);
	QRect r = geometry();
	QTextBlock block = firstVisibleBlock();
	int y = blockBoundingRect(block).translated(contentOffset()).top();
	int l = block.blockNumber();
	QString s;

	p.setFont(font());

	p.fillRect( e->rect(), m_panel->m_panelColor );
	while (block.isValid() && block.isVisible()){
		s = s.number(l);
		p.drawText( 0, y, m_panel->width() - 5, fontMetrics().height(), Qt::AlignRight, s );

		y += blockBoundingRect(block).height();
		block = block.next();
		l ++;
	}

#if 0
		if (data) {
			if (data->m_isBookmark)
				p.drawPixmap( 2, qRound(position.y() - contentsY + ascent - m_bookMarkImage.height()), m_bookMarkImage );
			if (data->m_isModified)
				p.fillRect( width()- 3, qRound(position.y()-contentsY), 2, qRound(boundingRect.height()), m_modifiedColor );
		}
#endif
}
