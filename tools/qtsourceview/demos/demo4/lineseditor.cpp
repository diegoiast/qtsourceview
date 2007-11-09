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
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDebug>

#include "qsvsyntaxhighlighter.h"
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
	matchBracesColor	= QColor( "#FF0000" );
	searchFoundColor	= QColor( "#DDDDFF" ); //QColor::fromRgb( 220, 220, 255)
	searchNotFoundColor	= QColor( "#FFAAAA" ); //QColor::fromRgb( 255, 102, 102) "#FF6666"
	whiteSpaceColor		= QColor( "#E0E0E0" );
	highlightCurrentLine	= true;
	showWhiteSpaces		= true;
	showMatchingBraces	= true;
	showPrintingMargins	= false;
	printMarginWidth	= 80;
	matchStart		= -1;
	matchEnd		= -1;
	matchingString		= "(){}[]";
		
	panel = new SamplePanel( this );
	panel->panelColor = QColor( "#FFFFD0" );
	panel->setVisible( true );

	setFrameStyle( QFrame::NoFrame );
	setLineWrapMode( QTextEdit::NoWrap );
	setAcceptRichText( false );
	QTimer::singleShot( 0, this, SLOT(adjustMarginWidgets()));
	syntaxHighlighter = NULL;

#ifdef WIN32
	QFont f("Courier New", 10);
#else
	QFont f("Monospace", 9);
#endif
	setFont( f );
	panel->setFont( f );
	
	findWidget = new TransparentWidget( this, 0.8 );
	ui_findWidget.setupUi( findWidget );
	ui_findWidget.searchText->setIcon( QPixmap(":/images/clear_left.png") );
	findWidget->hide();
	
	fileMessage = new TransparentWidget( this, 0.8 );
	ui_fileMessage.setupUi( fileMessage );
	connect( ui_fileMessage.label, SIGNAL(linkActivated(const QString&)), this, SLOT(on_fileMessage_clicked(QString)));
	fileMessage->hide();
	
	fileSystemWatcher = new QFileSystemWatcher(this);

	//connect( horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(adjustMarginWidgets()));
	//connect( verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(adjustMarginWidgets()));
	connect( this, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));
	connect( ui_findWidget.searchText, SIGNAL(textChanged(const QString)), this, SLOT(on_searchText_textChanged(const QString)) );
	connect( ui_findWidget.closeButton, SIGNAL(clicked()), this, SLOT(showFindWidget()));
	connect( ui_fileMessage.closeButton, SIGNAL(clicked()), fileMessage, SLOT(hide()));
	connect( fileSystemWatcher, SIGNAL(fileChanged(const QString&)), this, SLOT(on_fileChanged(const QString&)));
}

void LinesEditor::setupActions()
{
	actionFind = new QAction( "Find (inline)", this );
	actionFind->setObjectName("actionFind");
	actionFind->setShortcut( QKeySequence("Ctrl+F") );
	connect( actionFind, SIGNAL(triggered()), this, SLOT(showFindWidget()) );	
	//addAction( actionFind );

	actionCapitalize = new QAction( "Change to capital letters", this );
	actionCapitalize->setObjectName( "actionCapitalize" );
	actionCapitalize->setShortcut( QKeySequence( Qt::CTRL | Qt::Key_U ) );
	connect( actionCapitalize, SIGNAL(triggered()), this, SLOT(transformBlockToUpper()) );	
	//addAction( actionCapitalize );

	actionLowerCase = new QAction( "Change to lower letters", this );
	actionLowerCase->setObjectName( "actionLowerCase" );
	actionLowerCase->setShortcut( QKeySequence( Qt::CTRL | Qt::SHIFT | Qt::Key_U  ) );
	connect( actionLowerCase, SIGNAL(triggered()), this, SLOT(transformBlockToLower()) );	
	//addAction( actionLowerCase );

	actionChangeCase = new QAction( "Change case", this );
	actionChangeCase->setObjectName( "actionChangeCase" );
	connect( actionChangeCase, SIGNAL(triggered()), this, SLOT(transformBlockCase()) );	
	//addAction( actionChangeCase );
}

QColor LinesEditor::getItemColor( ItemColors role )
{
	switch (role)
	{
		case LinesPanel:	return panel->panelColor; 
		case CurrentLine:	return currentLineColor;
		case MatchBrackets:	return matchBracesColor;
		case NoText:		return searchNoText;
		case TextFound:		return searchFoundColor;
		case TextNoFound:	return searchNotFoundColor;
		case WhiteSpaceColor:	return whiteSpaceColor;
	}
	
	// just to keep gcc happy, will not get executed
	return QColor();
}

void LinesEditor::setItemColor( ItemColors role, QColor c )
{
	switch (role)
	{
		case LinesPanel:	
			panel->panelColor = c;
			panel->update();
			break;
		case CurrentLine:	
			currentLineColor = c;
			break;
		case MatchBrackets:
			matchBracesColor = c;
			break;
		case NoText:	
			searchNoText = c;
			break;
		case TextFound:
			searchFoundColor = c;
			break;
		case TextNoFound:
			searchNotFoundColor = c;
			break;
		case WhiteSpaceColor:
			whiteSpaceColor = c;
			updateMarkIcons();
			break;
	}
}

void	LinesEditor::setMargin( int width )
{
	printMarginWidth = width;
	showPrintingMargins = (width>0);
}

void	LinesEditor::setTabSize( int size )
{
	const QFontMetrics fm = QFontMetrics( document()->defaultFont() );
	int j = fm.width( " " ) * size;
	setTabStopWidth( j );
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

QsvSyntaxHighlighter* LinesEditor::getSyntaxHighlighter()
{
	return syntaxHighlighter;
}

void LinesEditor::setSyntaxHighlighter( QsvSyntaxHighlighter *newSyntaxHighlighter )
{
	syntaxHighlighter = newSyntaxHighlighter;
	syntaxHighlighter->rehighlight();
}

QTextCursor	LinesEditor::getCurrentTextCursor()
{
	QTextCursor cursor = textCursor();
	if (!cursor.hasSelection())
		cursor.select(QTextCursor::WordUnderCursor);
	return cursor;
}

#if 0
void LinesEditor::on_searchText_textChanged( const QString & text )
{	
	if (text.isEmpty())
	{
		QPalette p = palette();
		p.setColor( QPalette::Base, QColor("#FFFFFF") ); // white
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
			int w = c.selectionEnd() - c.selectionStart() ;
			c.setPosition( c.selectionStart() );
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
	QFileInfo fileInfo(file);
	
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return -1;
	
	QTextStream in(&file);
	setPlainText( in.readAll() );
	
	// clear older watches, and add a new one
	QStringList sl = fileSystemWatcher->directories();
	if (!sl.isEmpty())
		fileSystemWatcher->removePaths( sl );
	
	fileName = fileInfo.absoluteFilePath();
	fileSystemWatcher->addPath( fileName );
	
	return 0;
}

void LinesEditor::setDisplayCurrentLine( bool b )
{
	highlightCurrentLine = b;
}

void LinesEditor::setDisplayWhiteSpaces( bool b )
{
	showWhiteSpaces = b;
}

void LinesEditor::setDisplatMatchingBrackets( bool b )
{
	
	showMatchingBraces = b;
}

void LinesEditor::setMatchingString( QString s )
{
	matchingString = s;	
}

void	LinesEditor::transformBlockToUpper()
{
	//QTextCursor cursor = textCursor();
	//if (!cursor.hasSelection())
		//cursor.select(QTextCursor::WordUnderCursor);
	//return cursor;

	QTextCursor cursor = getCurrentTextCursor();
	QString s_before = cursor.selectedText();
	QString s_after  = s_before.toUpper();
	
	if (s_before != s_after)
	{
		cursor.beginEditBlock();
		cursor.deleteChar();
		cursor.insertText( s_after );
		cursor.endEditBlock();
		setTextCursor( cursor );
	}
}

void	LinesEditor::transformBlockToLower()
{
	QTextCursor cursor = getCurrentTextCursor();
	QString s_before = cursor.selectedText();
	QString s_after  = s_before.toLower();
	
	if (s_before != s_after)
	{
		cursor.beginEditBlock();
		cursor.deleteChar();
		cursor.insertText( s_after );
		cursor.endEditBlock();
		setTextCursor( cursor );
	}
}

void	LinesEditor::transformBlockCase()
{
	QTextCursor cursor = getCurrentTextCursor();
	QString s_before = cursor.selectedText();
	QString s_after = s_before;
	uint s_len = s_before.length();
	
	for( uint i=0; i< s_len; i++ )
		if (s_after[i].isLower())
			s_after[i] = s_after[i].toUpper();
		else if (s_after[i].isUpper())
			s_after[i] = s_after[i].toLower();
		
		
	if (s_before != s_after)
	{
		cursor.beginEditBlock();
		cursor.deleteChar();
		cursor.insertText( s_after );
		cursor.endEditBlock();
		setTextCursor( cursor );
	}
}

void LinesEditor::keyPressEvent( QKeyEvent *event )
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
			
		case Qt::Key_Tab:
			//if (tabIndents)
			{
				if (handleIndentEvent( !(event->modifiers() & Qt::ShiftModifier) ))
					// do not call original hanlder, if this was handled by that function
					return; 
			}
	} // end case
	
	QTextEdit::keyPressEvent( event );
}

void LinesEditor::resizeEvent ( QResizeEvent *event )
{
	QTextEdit::resizeEvent( event );
	adjustMarginWidgets();

	if (findWidget->isVisible())
	{
		findWidget->hide();
		showFindWidget();
	}

	if (fileMessage->isVisible())
	{
		fileMessage->hide();
		widgetToTop( fileMessage );
	}
}

void LinesEditor::paintEvent(QPaintEvent *e)
{
	// if no special painting, no need to create the QPainter
	if (highlightCurrentLine || showWhiteSpaces || showMatchingBraces)
	{		
		QPainter p( viewport() );
		
		if (highlightCurrentLine)
			printCurrentLine( p );
		
		if (showPrintingMargins)
			printMargins( p );
		
		if (showWhiteSpaces)
			printWhiteSpaces( p );
		
		QTextEdit::paintEvent(e);
			
		if (showMatchingBraces)
			printMatchingBraces( p );
	}
	else
		QTextEdit::paintEvent(e);
}

void	LinesEditor::timerEvent( QTimerEvent *event )
{
	// TODO
	Q_UNUSED( event );
}

void	LinesEditor::printWhiteSpaces( QPainter &p )
{		
	const int contentsY = verticalScrollBar()->value();
	const qreal pageBottom = contentsY + viewport()->height();
	const QFontMetrics fm = QFontMetrics( document()->defaultFont() );
	
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

void	LinesEditor::printCurrentLine( QPainter &p )
{
	QRect r = cursorRect();
	r.setX( 0 );
	r.setWidth( viewport()->width() );
	p.fillRect( r, currentLineColor );
}

QWidget* LinesEditor::getPanel()
{
	return panel;
}

void	LinesEditor::cursorPositionChanged()
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

void	LinesEditor::updateCurrentLine()
{
	if (highlightCurrentLine)
		viewport()->update();
}

void	LinesEditor::on_fileChanged( const QString &fName )
{
	if (fName != fileName)
		return;
		
	QFileInfo f (fileName);
	
	if (f.exists())
		ui_fileMessage.label->setText( tr("File has been modified outside the editor. <a href=':reload' title='Clicking this links will revert all changes to this editor'>Click here to reload.</a>") );
	else
		ui_fileMessage.label->setText( tr("File has been deleted outside the editor.") );
	
	widgetToTop( fileMessage );
}

void	LinesEditor::on_fileMessage_clicked( QString s )
{
	if (s == ":reload")
	{
		loadFile( fileName );
		ui_fileMessage.label->setText( "" );
		fileMessage->hide();
	}
}

void	LinesEditor::adjustMarginWidgets()
{
	if (panel->isVisible())
	{
		setViewportMargins( panel->width()-1, 0, 0, 0);
		QRect viewportRect = viewport()->geometry();
		QRect lrect = QRect(viewportRect.topLeft(), viewportRect.bottomLeft());
		lrect.adjust( -panel->width(), 0, 0, 0 );
		panel->setGeometry(lrect);		
	}
	else{
		setViewportMargins( 0, 0, 0, 0);
	}
}

void	LinesEditor::printMatchingBraces( QPainter &p )
{
	if (matchStart == -1)
		return;
		
	QFont f = document()->defaultFont();
	QTextCursor cursor = textCursor();

	f.setBold( true );
	p.setFont( f );
	p.setPen( matchBracesColor );
	cursor.setPosition(matchStart+1, QTextCursor::MoveAnchor);
	QRect r = cursorRect( cursor );
	p.drawText(r.x()-1, r.y(), r.width(), r.height(), Qt::AlignLeft | Qt::AlignVCenter, currentChar );

	if (matchEnd == -1)
		return;
		
	cursor.setPosition(matchEnd+1, QTextCursor::MoveAnchor);
	r = cursorRect( cursor );
		p.drawText(r.x()-1, r.y(), r.width(), r.height(), Qt::AlignLeft | Qt::AlignVCenter, matchChar );
}

void	LinesEditor::printMargins( QPainter &p )
{
	int lineLocation;
	
	p.setFont( document()->defaultFont() );
	p.setPen( whiteSpaceColor );
	lineLocation = p.fontMetrics().width( " " ) * printMarginWidth + 0;
	p.drawLine( lineLocation, 0, lineLocation, height() );
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

bool	LinesEditor::handleIndentEvent( bool forward )
{
	QTextCursor cursor = textCursor();
	if (!cursor.hasSelection())
	{
		qDebug("no selection, not handeling");
		return false;
	}
	
	return true;
}


void LinesEditor::updateMarkIcons()
{
	int x, y;
	QImage img;
	
	img = tabPixmap.toImage();
	for( x=0; x< tabPixmap.width(); x++ )
		for( y=0; y< tabPixmap.height(); y++ )
		{
			uint rgb = qRgb(  whiteSpaceColor.red(), whiteSpaceColor.green(), whiteSpaceColor.blue() );
			if (img.pixel(x,y) != 0)
				img.setPixel( x, y, rgb );
		}
	tabPixmap = QPixmap::fromImage( img );

	img = spacePixmap.toImage();
	for( x=0; x< tabPixmap.width(); x++ )
		for( y=0; y< tabPixmap.height(); y++ )
		{
			uint rgb = qRgb(  whiteSpaceColor.red(), whiteSpaceColor.green(), whiteSpaceColor.blue() );
			if (img.pixel(x,y) != 0)
				img.setPixel( x, y, rgb );
		}
	spacePixmap = QPixmap::fromImage( img );
}
