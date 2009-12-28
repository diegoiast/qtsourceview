#include <QApplication>
#include <QScrollBar>
#include <QAbstractSlider>
#include <QList>
#include <QPainter>
#include <QAction>
#include "qsvtextedit.h"
#include "qsvsyntaxhighlighterbase.h"

// private class
class QsvEditorPanel : public QWidget
{
public:
	QsvEditorPanel(QsvTextEdit *editor)
	 :QWidget(editor)
	{
		setFixedWidth(50);
	}

private:
	void paintEvent(QPaintEvent*e)
	{
		((QsvTextEdit*)parent())->paintPanel(e);
	}
friend class QsvTextEdit;
};


QsvTextEdit::QsvTextEdit( QWidget *parent, QsvSyntaxHighlighterBase *s ):
	QPlainTextEdit(parent)
{
	m_highlighter = s;
	m_highlighter->setDocument( document() );
	m_panel = new QsvEditorPanel(this);
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(on_cursor_positionChanged()));
	connect(document(), SIGNAL(contentsChange(int,int,int)), this, SLOT(on_textDocument_contentsChange(int,int,int)));
	connect(verticalScrollBar(), SIGNAL(valueChanged(int)), m_panel, SLOT(update()));

	QFont f;
	f.setBold(true);
	m_matchesFormat = QTextCharFormat();
	m_matchesFormat.setBackground( QBrush(QColor(0xff,0xff,0x00,0xff) ));
	m_matchesFormat.setForeground( QBrush(QColor(0x00,0x80,0x00,0xff) ));
	m_matchesFormat.setFont(f);
	m_currentLineBackground = QColor(0xc0,0xff,0xc0,0x80);
	m_modifiedColor         = QColor(0x00,0xff,0x00,0xff);
	m_panelColor            = QColor(0xff,0xff,0xd0,0xff);
	actionCapitalize        = NULL;
	actionLowerCase         = NULL;
	actionChangeCase        = NULL;

	setDefaultConfig();
	setupActions();
	setMatchBracketList(m_config.matchBracketsList);
	setFont(m_config.currentFont);
	setLineWrapMode(QPlainTextEdit::NoWrap);
}

void	QsvTextEdit::setupActions()
{
	if (actionCapitalize) delete actionCapitalize;
	actionCapitalize = new QAction( tr("Change to &capital letters"), this );
	actionCapitalize->setObjectName( "qsvEditor::actionCapitalize" );
	actionCapitalize->setShortcut( QKeySequence( Qt::CTRL | Qt::Key_U ) );
	connect( actionCapitalize, SIGNAL(triggered()), this, SLOT(transformBlockToUpper()) );

	if (actionLowerCase) delete actionLowerCase;
	actionLowerCase = new QAction( tr("Change to &lower letters"), this );
	actionLowerCase->setObjectName( "qsvEditor::actionLowerCase" );
	actionLowerCase->setShortcut( QKeySequence( Qt::CTRL | Qt::SHIFT | Qt::Key_U  ) );
	connect( actionLowerCase, SIGNAL(triggered()), this, SLOT(transformBlockToLower()) );

	if (actionChangeCase) delete actionChangeCase;
	actionChangeCase = new QAction( tr("Change ca&se"), this );
	actionChangeCase->setObjectName( "qsvEditor::actionChangeCase" );
	connect( actionChangeCase, SIGNAL(triggered()), this, SLOT(transformBlockCase()) );	
}

void	QsvTextEdit::setMarkCurrentLine( bool on )
{
	m_config.markCurrentLine = on;
	on_cursor_positionChanged();
}

bool	QsvTextEdit::getmarkCurrentLine() const
{
	return m_config.markCurrentLine;
}

void	QsvTextEdit::setStartHome( bool on )
{
	m_config.smartHome = on;
}

bool	QsvTextEdit::getStartHome() const
{
	return m_config.smartHome;
}

void	QsvTextEdit::setMatchBracketList( const QString &m )
{
	m_config.matchBracketsList = m;
	if (m_highlighter)
		m_highlighter->setMatchBracketList( m );
}

const QString QsvTextEdit::getMatchBracketList() const
{
	return m_config.matchBracketsList;
}

void	QsvTextEdit::setMatchBracket( bool on )
{
	m_config.matchBrackets = on;
	on_cursor_positionChanged();
}

bool	QsvTextEdit::getMatchBracket() const
{
	return m_config.matchBrackets;
}

void	QsvTextEdit::setLineWrapping( bool on )
{
	m_config.lineWrapping = on;
	setLineWrapMode(on?QPlainTextEdit::NoWrap:QPlainTextEdit::WidgetWidth);
}

bool	QsvTextEdit::getLineWrapping() const
{
	return m_config.lineWrapping;
}

void	QsvTextEdit::setModificationsLookupEnabled( bool on )
{
	m_config.modificationsLookupEnabled = on;
	if (m_panel)
		m_panel->update();
}

bool	QsvTextEdit::getModificationsLookupEnabled() const
{
	return m_config.modificationsLookupEnabled;
}

void	QsvTextEdit::setShowLineNumbers( bool on )
{
	m_config.showLineNumbers = on;
	m_panel->setVisible(on);
	updateMargins();
}

bool	QsvTextEdit::getShowLineNumbers() const
{
	return m_config.showLineNumbers;
}

// helper function, in Pascal it would have been an internal
// procedure inside on_cursor_moved();
// TODO move it to the class, as a helper method by adding extra selections
// to the class
inline void appendExtraSelection( QList<QTextEdit::ExtraSelection> &selections,
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

void QsvTextEdit::on_cursor_positionChanged()
{
	// clear out previous matches
	QList<QTextEdit::ExtraSelection> selections;
	QTextCursor cursor = textCursor();
	QTextBlock  block;
	int blockPosition;
	int cursorPosition;
	int relativePosition;
	QChar currentChar;
	QsvBlockData *data;

	if (m_config.markCurrentLine) {
		QTextEdit::ExtraSelection sel;
		sel.format.setBackground( m_currentLineBackground );
		sel.format.setProperty(QTextFormat::FullWidthSelection, true);
		sel.cursor = cursor;
		sel.cursor.clearSelection();
		selections.append(sel);
	}

	if (!m_config.matchBrackets)
		goto NO_MATCHES;

	// does this line have any brakcets?
	data = static_cast<QsvBlockData*>(textCursor().block().userData());
	if (!data)
		goto NO_MATCHES;

	block             = cursor.block();
	blockPosition     = block.position();
	cursorPosition    = cursor.position();
	relativePosition  = cursorPosition - blockPosition;
	currentChar       = block.text()[relativePosition];

	for ( int k=0; k<data->matches.length(); k++) {
		MatchData m = data->matches.at(k);
		if (m.position != relativePosition)
			continue;

		appendExtraSelection(selections, cursorPosition, this, m_matchesFormat);

		// lets find it's partner
		// in theory, no errors shuold not happen, but one can never be too sure
		int j = m_config.matchBracketsList.indexOf(currentChar);
		if (j==-1)
			continue;

		if (m_config.matchBracketsList[j] != m_config.matchBracketsList[j+1])
			if (j %2 == 0)
				j = findMatchingChar( m_config.matchBracketsList[j], m_config.matchBracketsList[j+1], true , block, cursorPosition );
			else
				j = findMatchingChar( m_config.matchBracketsList[j], m_config.matchBracketsList[j-1], false, block, cursorPosition  );
		else
			j = findMatchingChar( m_config.matchBracketsList[j], m_config.matchBracketsList[j+1], true , block, cursorPosition );
		appendExtraSelection(selections, j, this,m_matchesFormat);
	}

NO_MATCHES:
	setExtraSelections(selections);
	if (m_panel)
		m_panel->update();
}

void	QsvTextEdit::smartHome()
{
	QTextCursor c = textCursor();
	int blockLen = c.block().text().length();
	if (blockLen == 0 )
		return;

	int originalPosition = c.position();
	QTextCursor::MoveMode moveAnchor = QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)?
		 QTextCursor::KeepAnchor:QTextCursor::MoveAnchor;
	c.movePosition(QTextCursor::StartOfLine, moveAnchor);
	int startOfLine = c.position();
	int i = 0;
	while ( c.block().text()[i].isSpace()) {
		i ++;
		if (i==blockLen) {
			i = 0;
			break;
		}
	}
	if ((originalPosition == startOfLine) || (startOfLine + i != originalPosition ))
		c.setPosition( startOfLine + i, moveAnchor );
	setTextCursor( c );
}

void	QsvTextEdit::smartEnd()
{
	QTextCursor c = textCursor();
	int blockLen = c.block().text().length();
	if (blockLen == 0)
		return;

	int originalPosition = c.position();
	QTextCursor::MoveMode moveAnchor = QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)?
		 QTextCursor::KeepAnchor:QTextCursor::MoveAnchor;
	c.movePosition(QTextCursor::StartOfLine,moveAnchor);
	int startOfLine = c.position();
	c.movePosition(QTextCursor::EndOfLine,moveAnchor);
	int i = blockLen;
	while (c.block().text()[i-1].isSpace())	{
		i --;
		if (i==1)		{
			i = blockLen;
			break;
		}
	}
	if ((originalPosition == startOfLine) || (startOfLine + i != originalPosition ))
		c.setPosition( startOfLine + i, moveAnchor );

	setTextCursor( c );
}

void	QsvTextEdit::transformBlockToUpper()
{
	QTextCursor cursor = getCurrentTextCursor();
	QString s_before   = cursor.selectedText();
	QString s_after    = s_before.toUpper();
	
	if (s_before != s_after) {
		cursor.beginEditBlock();
		cursor.deleteChar();
		cursor.insertText( s_after );
		cursor.endEditBlock();
		setTextCursor( cursor );
	}
}

void	QsvTextEdit::transformBlockToLower()
{
	QTextCursor cursor = getCurrentTextCursor();
	QString s_before   = cursor.selectedText();
	QString s_after    = s_before.toLower();
	
	if (s_before != s_after) {
		cursor.beginEditBlock();
		cursor.deleteChar();
		cursor.insertText( s_after );
		cursor.endEditBlock();
		setTextCursor( cursor );
	}
}

void	QsvTextEdit::transformBlockCase()
{
	QTextCursor cursor = getCurrentTextCursor();
	QString s_before   = cursor.selectedText();
	QString s_after    = s_before;
	uint s_len = s_before.length();
	
	for( uint i=0; i< s_len; i++ ) {
		QChar c = s_after[i];
		if (c.isLower())
			c = c.toUpper();
		else if (c.isUpper())
			c = c.toLower();
		s_after[i] = c;
	}
		
	if (s_before != s_after) {
		cursor.beginEditBlock();
		cursor.deleteChar();
		cursor.insertText( s_after );
		cursor.endEditBlock();
		setTextCursor( cursor );
	}
}

void	QsvTextEdit::removeModifications()
{
	int i = 1;
	for ( QTextBlock block = document()->begin(); block.isValid(); block = block.next() ) {
		QsvBlockData *data = dynamic_cast<QsvBlockData*>( block.userData() );
		if (!data)
			continue;
		data->m_isModified = false;
		i ++;
	}
	m_panel->update();
}

void	QsvTextEdit::on_textDocument_contentsChange( int position, int charsRemoved, int charsAdded )
{
	if (!m_config.modificationsLookupEnabled)
		return;

	if (charsAdded < 2) {
		QsvBlockData* data = getPrivateBlockData( textCursor().block(), true );
		if (!data)		return;
		if (data->m_isModified)	return;
		data->m_isModified = true;
	}
	else {
		int remaining = 0;
		QTextCursor cursor( document() );
		cursor.setPosition( position );
		int oldRemaining = -1;
		while ( (remaining+1 < charsAdded) && (oldRemaining != remaining) ){
			oldRemaining = remaining;
			QsvBlockData* data = getPrivateBlockData( cursor.block(), true );
			// should not happen, but can't be too safe
			if (data)
				data->m_isModified = true;
			cursor.movePosition( QTextCursor::NextBlock );
			remaining = cursor.position() - position;
		}
	}
	m_panel->update();
	Q_UNUSED( charsRemoved );
}

void	QsvTextEdit::paintEvent(QPaintEvent *e)
{
	if (m_config.showMargins) {
		uint position = fontMetrics().width(' ') * m_config.marginsWidth;
		QPainter p(viewport());
		p.drawLine(position,0,position,viewport()->height());
	}
	QPlainTextEdit::paintEvent(e);
}

void	QsvTextEdit::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);

	// this get connected in QsvTextOperationsWidget
	emit(widgetResized());
	updateMargins();
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
		case Qt::Key_Up:
			if (!QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
				break;
			verticalScrollBar()->triggerAction( 
				e->key() == Qt::Key_Down?
				QAbstractSlider::SliderSingleStepAdd:
				QAbstractSlider::SliderSingleStepSub
			);
			e->accept();
			break;
		case Qt::Key_PageDown:
		case Qt::Key_PageUp:
			if (!QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
				break;
			verticalScrollBar()->triggerAction(
				e->key() == Qt::Key_PageDown?
				QAbstractSlider::SliderPageStepAdd:
				QAbstractSlider::SliderPageStepSub
			);
			e->accept();
			break;
		
		case Qt::Key_Tab:
			if (m_config.tabIndents) {
				if (handleIndentEvent( !(e->modifiers() & Qt::ShiftModifier) ))
					// do not call original hanlder, if this was handled by that function
					return;
			}
			// TODO else insert_spaced_as_needed()
			break;
		case Qt::Key_Backtab:
			if (m_config.tabIndents)
				if (handleIndentEvent(false))
					return;

		case Qt::Key_Home:
		case Qt::Key_End:
			if (!m_config.smartHome || QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) )
				break;
			if (e->key() == Qt::Key_Home)
				smartHome();
			else
				smartEnd();
			e->accept();
			return;
		
		default:
			if (m_config.autoBrackets && handleKeyPressEvent(e))
				return;
	} // end case
	QPlainTextEdit::keyPressEvent(e);
}

bool	QsvTextEdit::handleKeyPressEvent(QKeyEvent *e)
{
	QTextCursor cursor = textCursor();
	int i,j;
	QString s;
	
	// handle automatic deletcion of mathcing brackets
	if ((e->key() == Qt::Key_Delete) || (e->key() == Qt::Key_Backspace)) {
		if (cursor.hasSelection())
			return false;
		i = cursor.position() - cursor.block().position();
		QChar c1 = cursor.block().text()[ i ];
		j =  m_config.matchBracketsList.indexOf( c1 );
		if (j == -1)
			return false;
		if (i == 0)
			return false;
		if (m_config.matchBracketsList[j+1] == m_config.matchBracketsList[j])
			j++;
		QChar c2 = cursor.block().text()[ i-1 ];
		if (c2 != m_config.matchBracketsList[j-1])
			return false;
		cursor.deletePreviousChar();
		cursor.deleteChar();
		goto FUNCTION_END;
	}
	
	// handle only normal key presses
	s = e->text();
	if (s.isEmpty())
		return false;
	
	// don't handle if not in the matching list
	j = m_config.matchBracketsList.indexOf(s[0]);
	if ((j == -1) || (j%2 == 1))
		return false;
	
	// handle automatic insert of matching brackets
	i = cursor.position();
	if (!cursor.hasSelection()) {
		cursor.insertText( QString(m_config.matchBracketsList[j]) );
		cursor.insertText( QString(m_config.matchBracketsList[j+1]) );
	} else {
		QString s = cursor.selectedText();
		cursor.beginEditBlock();
		cursor.deleteChar();
		s = m_config.matchBracketsList[j] + s + m_config.matchBracketsList[j+1];
		cursor.insertText(s);
		cursor.endEditBlock();
	}
	cursor.setPosition(i+1);
	setTextCursor(cursor);
	
FUNCTION_END:
	e->accept();
	return true;
}

void	QsvTextEdit::updateMargins()
{
	if (!m_panel || !m_config.showLineNumbers) {
		setViewportMargins( 0, 0, 0, 0 );
		return;
	}
	
	QRect viewportRect = viewport()->geometry();
	QRect lrect = QRect(viewportRect.topLeft(), viewportRect.bottomLeft());
	int panelWidth = m_panel->width();
	lrect.adjust( -panelWidth, 0, 0, 0 );
	
	setViewportMargins( panelWidth-1, 0, 0, 0 );
	m_panel->setGeometry(lrect);
}

bool	QsvTextEdit::handleIndentEvent( bool forward )
{
	QTextCursor cursor1 = textCursor();
	bool reSelect = true;

	if (!cursor1.hasSelection()) {
		reSelect = false;
		cursor1.select( QTextCursor::LineUnderCursor );
	}
	
	QTextCursor cursor2 = cursor1;
	cursor1.setPosition( cursor1.selectionStart() );
	cursor2.setPosition( cursor2.selectionEnd() );
	int endBlock = cursor2.blockNumber();
	
	QString spaces;
	if (m_config.insertSpacesInsteadOfTabs) {
		int k = getTabSize();
		for(int i=0; i< k; i++ )
			spaces = spaces.insert(0,' ');
	}
	
	int baseIndentation = getIndentationSize( cursor1.block().text() );
	
	if (forward)
		baseIndentation++;
	else if (baseIndentation!=0) 
		baseIndentation--;

	cursor1.beginEditBlock();
	int origPos = cursor1.position();
	do {
		int pos = cursor1.position();
		QString s = cursor1.block().text();
		cursor1.select( QTextCursor::LineUnderCursor );
		if (forward)
			cursor1.insertText( m_config.insertSpacesInsteadOfTabs? spaces + s : '\t' + s );
		else
			cursor1.insertText( updateIndentation(s,baseIndentation) );
		cursor1.setPosition( pos );
		if (!cursor1.movePosition(QTextCursor::NextBlock))
			break;
	} while( cursor1.blockNumber() < endBlock );
	
	if (reSelect)
		cursor1.setPosition( origPos, QTextCursor::KeepAnchor );
	else
		cursor1.setPosition( origPos, QTextCursor::MoveAnchor );
	cursor1.endEditBlock();
	setTextCursor( cursor1 );
	return true;
}

int	QsvTextEdit::getIndentationSize( const QString s )
{
	int indentation = 0;
	int i = 0;
	int l = s.length();
	if (l == 0)
		return 0;
	QChar c = s.at(i);
	while( (i<l) && ((c == ' ') || (c == '\t')) ) {
		if (c == '\t') {
			indentation ++;
			i ++;
		} else if (c == ' ') {
			int k=0;
			while( (i+k<l) && (s.at(i+k) == ' ' ))
				k ++;
			i += k;
			indentation += k / getTabSize();
		}
		if (i<l)
			c = s.at(i);
	}
	return indentation;
}

QString	QsvTextEdit::updateIndentation( QString s, int indentation )
{
	if (s.isEmpty())
		return s;
	QString spaces;
	if (m_config.insertSpacesInsteadOfTabs) {
		int k = getTabSize();
		for (int i=0; i< k; i++ )
			spaces = spaces.insert(0,' ');
	}
	while ((s.at(0) == ' ') || (s.at(0) == '\t')) {
		s.remove(0,1);
		if (s.isEmpty())
			break;
	}
	while( indentation != 0 ) {
		if (m_config.insertSpacesInsteadOfTabs)
			s = s.insert(0,spaces);
		else
			s = s.insert(0,QChar('\t'));
		indentation --;
	}
	return s;
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

QsvBlockData* QsvTextEdit::getPrivateBlockData( QTextBlock block, bool createIfNotExisting )
{
	QTextBlockUserData  *d1   = block.userData();
	QsvBlockData *data = dynamic_cast<QsvBlockData*>(d1);

	// a user data has been defined, and it's not our structure
	if (d1 && !data)
		return NULL;

	if (!data && createIfNotExisting) {
		data = new QsvBlockData;
		block.setUserData(data);
	}
	return data;
}

void	QsvTextEdit::setShowMargins( bool on )
{
	if (m_config.showMargins == on)
		return;
	m_config.showMargins = on;
	viewport()->update();
}

bool	QsvTextEdit::getShowMargins() const
{
	return m_config.showMargins;
}

void	QsvTextEdit::setMarginsWidth( uint i )
{
	if (m_config.marginsWidth == i)
		return;
	m_config.marginsWidth = i;
	if (m_config.showMargins)
		viewport()->update();
}

uint	QsvTextEdit::getMarginsWidth() const
{
	return m_config.marginsWidth;
}

void	QsvTextEdit::setTabSize( int size )
{
	m_config.tabSize = size;
	setTabStopWidth(fontMetrics().width(" ")*size);
}

int	QsvTextEdit::getTabSize() const
{
	return m_config.tabSize;
}

void QsvTextEdit::setInsertSpacesInsteadOfTabs( bool on )
{
	m_config.insertSpacesInsteadOfTabs = on;
}

bool QsvTextEdit::getInsertSpacesInsteadOfTabs() const
{
	return m_config.insertSpacesInsteadOfTabs;
}

void QsvTextEdit::setTabIndents( bool on )
{
	m_config.tabIndents = on;
}

bool QsvTextEdit::getTabIndents() const
{
	return m_config.tabIndents;
}

void QsvTextEdit::setDefaultConfig()
{
	setDefaultConfig(&m_config);
}

void QsvTextEdit::setDefaultConfig( QsvEditorConfigData *config ) // static
{
	config->currentFont        = QFont("Courier new", 10);
	config->showLineNumbers    = true;
	config->smartHome          = true;
	config->markCurrentLine    = true;
	config->matchBrackets      = true;
	config->lineWrapping       = false;
	config->matchBracketsList  = "()[]{}\"\"''";
	config->modificationsLookupEnabled = true;
	config->autoBrackets       = true;
	config->showMargins        = true;
	config->marginsWidth       = 80;
	config->tabSize            = 8;
	config->insertSpacesInsteadOfTabs = false;
	config->tabIndents         = false;
}

QTextCursor	QsvTextEdit::getCurrentTextCursor()
{
	QTextCursor cursor = textCursor();
	if (!cursor.hasSelection())
		cursor.select(QTextCursor::WordUnderCursor);
	return cursor;
}

void	QsvTextEdit::paintPanel(QPaintEvent*e)
{
	if (!m_panel)
		return;

	QPainter p(m_panel);
	QTextBlock block = firstVisibleBlock();
	int y = blockBoundingRect(block).translated(contentOffset()).top();
	int l = block.blockNumber();
	int h = fontMetrics().height();
	int w = m_panel->width();
	QString s;

	p.setFont(font());
	p.fillRect( e->rect(), m_panelColor );
	while (block.isValid() && block.isVisible()){
		s = s.number(l);
		p.drawText( 0, y, w-5, h, Qt::AlignRight, s );

		QsvBlockData *data = dynamic_cast<QsvBlockData*>(block.userData());
		if (data) {
//			if (data->m_isBookmark)
//				p.drawPixmap( 2, y, m_panel->m_bookMarkImage );
			if (data->m_isModified)
				p.fillRect( w-3, y, 2, h, m_modifiedColor );
		}
		y += blockBoundingRect(block).height();
		block = block.next();
		l ++;
	}
}
