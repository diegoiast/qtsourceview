#include <QPainter>
#include <QTextEdit>
#include <QTextBlock>
#include <QTextLayout>
#include <QScrollBar>
#include <QPushButton>
#include <QAction>
#include <QKeySequence>
#include <QTimer>
#include <QPalette>
#include <QFile>
#include <QTextStream>
#include <QStyle>

#include <QDebug>

#include "lineseditor.h"
#include "samplepanel.h"
#include "transparentwidget.h"
#include "ui_findwidget.h"

static const char * tabPixmap_img[] = 
{
/* width height ncolors cpp [x_hot y_hot] */
	"8 8 3 2 0 0",
/* colors */
	"  s none       m none  c none",
	"O s iconColor1 m black c black",
	"X s iconColor2 m black c #E0E0E0",
/* pixels */
	"  X     X       ",
	"    X     X     ",
	"      X     X   ",
	"        X     X ",
	"      X     X   ",
	"    X     X     ",
	"  X     X       ",
	"                ",
};
/*
	"        X     X ",
	"      X     X   ",
	"    X     X     ",
	"  X     X       ",
	"    X     X     ",
	"      X     X   ",
	"        X     X ",
	"                ",
*/
static const char * spacePixmap_img[] = 
{
/* width height ncolors cpp [x_hot y_hot] */
	"8 8 3 2 0 0",
/* colors */
	"  s none       m none  c none",
	"O s iconColor1 m black c black",
	"X s iconColor2 m black c #E0E0E0",
/* pixels */
	"                ",
	"                ",
 	"                ",
	"                ",
	"                ",
	"      X         ",
	"      X X       ",
	"                ",
};

LinesEditor::LinesEditor( QWidget *p ) :QTextEdit(p)
{
	tabPixmap		= QPixmap( tabPixmap_img ); 
	spacePixmap		= QPixmap( spacePixmap_img ); 
	currentLineColor	= QColor( "#EEF6FF" );
	linesPanelColor		= QColor( "#FFFFD0" );
	matchBracesColor	= QColor( "#FF0000" );
	searchFoundColor	= QColor( "#DDDDFF" ); //QColor::fromRgb( 220, 220, 255)
	searchNotFoundColor	= QColor( "#FFAAAA" ); //QColor::fromRgb( 255, 102, 102) "#FF6666"
	highlightCurrentLine	= true;
	showWhiteSpaces		= true;
	showMatchingBraces	= true;
	matchStart		= -1;
	matchEnd		= -1;
	matchingString		= "(){}[]";
	
#ifdef WIN32
	setFont( QFont("Courier New", 10) );
#else
	setFont( QFont("Monospace", 9) );
#endif
	
	panel = new SamplePanel( this );
	panel->panelColor = linesPanelColor;
	setViewportMargins( panel->width()-1, 0, 0, 0);
	setFrameStyle( QFrame::NoFrame );	
	adjustMarginWidgets();
	setLineWrapMode( QTextEdit::NoWrap );
	
	findWidget = new TransparentWidget( this, 0.8 );
	ui_findWidget.setupUi( findWidget );
	ui_findWidget.searchText->setIcon( QPixmap(":/images/clear_left.png") );
	ui_findWidget.closeButton->setIcon( QPixmap(":/images/fileclose.png") );
	findWidget->hide();

	//connect( horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(adjustMarginWidgets()));
	//connect( verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(adjustMarginWidgets()));
	connect( this, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));
	connect( ui_findWidget.searchText, SIGNAL(textChanged(const QString)), this, SLOT(on_searchText_textChanged(const QString)) );
	connect( ui_findWidget.closeButton, SIGNAL(clicked()), this, SLOT(showFindWidget()));
}

void LinesEditor::setupActions()
{
	actionFind = new QAction( "Find", this );
	actionFind->setShortcut( QKeySequence("Ctrl+F") );
	connect( actionFind, SIGNAL(triggered()), this, SLOT(showFindWidget()) );
	
	addAction( actionFind );
}

QColor LinesEditor::getLinesPanelColor()
{
	return linesPanelColor;
}

QColor LinesEditor::getCurrentLineColor()
{
	return currentLineColor;
}

void LinesEditor::findMatching( QChar c1, QChar c2, bool forward, QTextBlock &block )
{
	int i = matchStart;
	int n = 1;
	QChar c;
	
	do	
	{
		if (forward)
		{
			i ++;
			if ((i - block.position()) == block.length())
				block = block.next();	
		}
		else
		{
			i --;
			if ((i - block.position()) == -1)
				block = block.previous();
		}
		c = block.text()[i - block.position() ];

		if (c == c1)
			n++;
		else if (c == c2)
			n--;
	} while (block.isValid() && (n!=0) );
	
	if (n == 0)
		matchEnd = i;
}

#if 1
void LinesEditor::on_searchText_textChanged( const QString & text )
{	
	if (text.isEmpty())
	{
		QPalette p = palette();
		p.setColor( QPalette::Base, QColor("#FFFFFF" )); // white
		ui_findWidget.searchText->setPalette( p );
	}
	else
	{
		QPalette ok, bad;
		
		ok = this->palette();
		bad = this->palette();
		ok.setColor( QPalette::Base, searchFoundColor );
		bad.setColor( QPalette::Base, searchNotFoundColor );
		
		QTextCursor oldCursor = textCursor();
		QTextCursor c = oldCursor;
		c = document()->find( text, c, !QTextDocument::FindCaseSensitively );
		bool found = ! c.isNull();
				
		if (!found)
		{
			// lets try again, from the start
			c = QTextCursor(document());
			c.movePosition(QTextCursor::Start);
			c = document()->find( text, c, !QTextDocument::FindCaseSensitively );
			found = ! c.isNull();
		}
		
		if (found)
		{
			ui_findWidget.searchText->setPalette( ok );
			//c.setPosition( c.selectionStart(), QTextCursor::KeepAnchor );
			setTextCursor( c );	
		}
		else
		{
			ui_findWidget.searchText->setPalette( bad );	
			setTextCursor( oldCursor );	
		}
	}
}
#else
void LinesEditor::on_searchText_textChanged( const QString & text )
{
	QTextDocument *doc = document();
	QString oldText = ui_findWidget.searchText->text();
	QTextCursor c = textCursor();
	QTextDocument::FindFlags options;
	QPalette p = palette();
	p.setColor(QPalette::Active, QPalette::Base, Qt::white);

	if (c.hasSelection())
		//c.setPosition(forward ? c.position() : c.anchor(), QTextCursor::MoveAnchor);
		c.setPosition(c.position(), QTextCursor::MoveAnchor);

	QTextCursor newCursor = c;

	if (!text.isEmpty()) {
		/*if (backward)
			options |= QTextDocument::FindBackward;

		if (ui.checkCase->isChecked())
			options |= QTextDocument::FindCaseSensitively;

		if (ui.checkWholeWords->isChecked())
			options |= QTextDocument::FindWholeWords;
*/
		newCursor = doc->find(text, c, options);
		//ui.labelWrapped->hide();

		if (newCursor.isNull()) {
			QTextCursor ac(doc);
			//ac.movePosition(options & QTextDocument::FindBackward ? QTextCursor::End : QTextCursor::Start);
			ac.movePosition(QTextCursor::Start);
			newCursor = doc->find(text, ac, options);
			if (newCursor.isNull()) {
				//p.setColor(QPalette::Active, QPalette::Base, QColor(255, 102, 102));
				p.setColor(QPalette::Active, QPalette::Base, searchNotFoundColor);
				newCursor = c;
			} else
			{
				p.setColor(QPalette::Active, QPalette::Base, searchFoundColor);
				//ui.labelWrapped->show();
				;
			}
		}
		else
			p.setColor(QPalette::Active, QPalette::Base, searchFoundColor);

	}

	//if (!findWidget->isVisible())
		//findWidget->show();
	setTextCursor(newCursor);
	ui_findWidget.searchText->setPalette(p);
	/*if (!ui.editFind->hasFocus())
		autoHideTimer->start();*/
}
#endif

void LinesEditor::setLinesPanelColor( QColor c )
{
	linesPanelColor = c;
	panel->panelColor = linesPanelColor;
	panel->update();
}

void LinesEditor::setCurrentLineColor( QColor c )
{
	currentLineColor = c;
	update();
}

void LinesEditor::showFindWidget()
{	
	if (findWidget->isVisible())
	{
		findWidget->hide();
		this->setFocus();
		return;		
	}
	
	widgetToBottom( findWidget );
	//widgetToTop( findWidget );		
	ui_findWidget.searchText->setFocus();
}

int LinesEditor::loadFile( QString s )
{
	QFile file(s);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	    return -1;
	
	QTextStream in(&file);
	setPlainText( in.readAll() );
	
	return 0;
}

void LinesEditor::keyReleaseEvent( QKeyEvent * event )
{
	switch (event->key())
	{		
		case Qt::Key_Escape:
			if (findWidget->isVisible())
				// hide  it
				showFindWidget();
			else
			{
				// clear selection
				QTextCursor c = textCursor();
				if (c.hasSelection())
				{
					c.clearSelection();
					setTextCursor(c);
				}
			}
			break;
			
		case Qt::Key_Enter:
		case Qt::Key_Return:
			// TODO, BUG: ignore "enter" keypresses, if looking for text
			if (findWidget->isVisible())
				return;
			break;
	} // end case
	
	QTextEdit::keyReleaseEvent( event );
}

void LinesEditor::resizeEvent ( QResizeEvent *event )
{
	adjustMarginWidgets();
	QTextEdit::resizeEvent( event );
	adjustMarginWidgets();

	if (findWidget->isVisible())
	{
		findWidget->hide();
		showFindWidget();

	}
}

void LinesEditor::paintEvent(QPaintEvent *e)
{
	// if no special paiting,no need to create the QPainter
	if (highlightCurrentLine || showWhiteSpaces || showMatchingBraces)
	{		
		QPainter p( viewport() );
	
		if (highlightCurrentLine)
			printCurrentLine( p );
				
		if (showWhiteSpaces)
			printWhiteSpaces( p );

		QTextEdit::paintEvent(e);
			
		if (showMatchingBraces)
			printMatchingBraces( p );
	}
	else
		QTextEdit::paintEvent(e);
}

void LinesEditor::printWhiteSpaces( QPainter &p )
{		
	const int contentsY = verticalScrollBar()->value();
	const qreal pageBottom = contentsY + viewport()->height();
	const QFontMetrics fm = QFontMetrics( currentFont() );
	
	for ( QTextBlock block = document()->begin(); block.isValid(); block = block.next() )
	{
		QTextLayout* layout = block.layout();
		const QRectF boundingRect = layout->boundingRect();
		QPointF position = layout->position();
		
		if ( position.y() +boundingRect.height() < contentsY )
			continue;
		if ( position.y() > pageBottom )
			break;
		
		const QString txt = block.text();
		const int len = txt.length();
		
		for ( int i=0; i<len; i++)
		{
			QPixmap *p1 = 0;
			
			if (txt[i] == ' ' )
				p1 = &spacePixmap;
			else if (txt[i] == '\t' )
				p1 = &tabPixmap;
			else 
				continue;
			
			// pixmaps are of size 8x8 pixels
			QTextCursor cursor = textCursor();
			cursor.setPosition( block.position() + i, QTextCursor::MoveAnchor);
			
			QRect r = cursorRect( cursor );
			int x = r.x() + 4;
			int y = r.y() + fm.height() / 2 - 5;
			p.drawPixmap( x, y, *p1 );
		}
	}
}

void LinesEditor::printCurrentLine( QPainter &p )
{
	QRect r = cursorRect();
	r.setX( 0 );
	r.setWidth( viewport()->width() );
	p.fillRect( r, currentLineColor );
}

void LinesEditor::cursorPositionChanged()
{
	QTextCursor cursor = textCursor();
	int pos = cursor.position();
	if (pos == -1)
	{
		matchStart = matchEnd = -1;
		currentChar = matchChar = 0;
		if (highlightCurrentLine)
			updateCurrentLine();
		return;
	}
		
	QTextBlock  block = cursor.block();
		int i = cursor.position();
	currentChar = block.text()[i - block.position() ];
	matchStart = i;

	int j = matchingString.indexOf( currentChar );

	if (j == -1)
	{
		matchStart = matchEnd = -1;
		currentChar = matchChar = 0;
		if (highlightCurrentLine)
			updateCurrentLine();
		return;
	}
	
	if (j %2 == 0)
		findMatching( matchingString[j], matchChar = matchingString[j+1], true, block );
	else
		findMatching( matchingString[j], matchChar = matchingString[j-1], false, block );

	updateCurrentLine();
}

void LinesEditor::updateCurrentLine()
{
	if (highlightCurrentLine)
		viewport()->update();
}

void LinesEditor::adjustMarginWidgets()
{
	QRect viewportRect = viewport()->geometry();
	QRect lrect = QRect(viewportRect.topLeft(), viewportRect.bottomLeft());
	lrect.adjust( -panel->width(), 0, 0, 0 );
	panel->setGeometry(lrect);
	
	//QRect trect = QRect(viewportRect.topLeft(), viewportRect.topRight());
	//trect.adjust(-DEFAULT_MARGIN, -DEFAULT_MARGIN, 0, 0);
	//mTopWidget->setGeometry(trect);
}

void LinesEditor::printMatchingBraces( QPainter &p )
{
	if (matchStart == -1)
		return;
		
	QFont f = font();
	QTextCursor cursor = textCursor();

	f.setBold( true );
	p.setFont( f );
	p.setPen( matchBracesColor );
	cursor.setPosition(matchStart+1, QTextCursor::MoveAnchor);
	QRect r = cursorRect( cursor );
	p.drawText(r.x()-2, r.y(), r.width(), r.height(), Qt::AlignLeft | Qt::AlignVCenter, currentChar );

	if (matchEnd == -1)
		return;
		
	cursor.setPosition(matchEnd+1, QTextCursor::MoveAnchor);
	r = cursorRect( cursor );
		p.drawText(r.x(), r.y(), r.width(), r.height(), Qt::AlignLeft | Qt::AlignVCenter, matchChar );
}

void LinesEditor::widgetToBottom( QWidget *w )
{
	QRect r1 = viewport()->geometry();	
	QRect r2 = w->geometry();

	int i = r2.height();
	r2.setX( r1.left() + 5 );
	r2.setY( r1.height() - i - 5 );
	r2.setWidth( r1.width() - 10 );
	r2.setHeight( i );
	
	w->setGeometry(r2);
	w->show();	
}

void LinesEditor::widgetToTop( QWidget *w )
{
	QRect r1 = viewport()->geometry();	
	QRect r2 = w->geometry();

	int i = r2.height();
	r2.setX( r1.left() + 5 );
	r2.setY( 5 );
	r2.setWidth( r1.width() - 10 );
	r2.setHeight( i );
	
	w->setGeometry(r2);
	w->show();	
}
