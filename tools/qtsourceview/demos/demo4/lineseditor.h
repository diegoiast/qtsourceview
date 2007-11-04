#ifndef __LINESEDITOR_H__
#define __LINESEDITOR_H__

#include <QTextEdit>
#include "ui_findwidget.h"
#include "ui_filemessage.h"

class SamplePanel;
class TransparentWidget;
class QsvSyntaxHighlighter;
class QFileSystemWatcher;

enum ItemColors {
	 LinesPanel, CurrentLine, MatchBrackets, NoText, TextFound, TextNoFound, WhiteSpaceColor
};

class LinesEditor: public QTextEdit
{
	Q_OBJECT
public:
	
	LinesEditor( QWidget *p=NULL );
	QColor	getItemColor( ItemColors role );
	void	setItemColor( ItemColors role, QColor );
	void	setMargin( int width );
	void	setTabSize( int size );
	virtual void findMatching( QChar c1, QChar c2, bool forward, QTextBlock &block );
	QsvSyntaxHighlighter* getSyntaxHighlighter();
	void setSyntaxHighlighter( QsvSyntaxHighlighter *newSyntaxHighlighter );
	
public slots:
	void	on_searchText_textChanged( const QString & text );
	void	showFindWidget();
	int	loadFile( QString );
	
	void	setDisplayCurrentLine( bool );
	void	setDisplayWhiteSpaces( bool );
	void	setDisplatMatchingBrackets( bool );
	void	setMatchingString( QString );
	
	QWidget*	getPanel();
	void	adjustMarginWidgets();
		
protected slots:
	void	cursorPositionChanged();
	void	updateCurrentLine();
	void	on_fileChanged( const QString &fName );
	void	on_fileMessage_clicked( QString s );
	//void	showPanel( bool visible );
	
protected:
	void	keyReleaseEvent ( QKeyEvent * event );
	void	resizeEvent ( QResizeEvent *event );
	void	paintEvent(QPaintEvent *e);
	void	timerEvent( QTimerEvent *event );
	void	printWhiteSpaces( QPainter &p );
	void	printCurrentLine( QPainter &p );
	void	printMatchingBraces( QPainter &p );
	void	printMargins( QPainter &p );
	void	widgetToBottom( QWidget *w );
	void	widgetToTop( QWidget *w );

private:
	void	setupActions();
	void	updateMarkIcons();

	QPixmap	tabPixmap;
	QPixmap spacePixmap;
	QColor	currentLineColor;
	QColor	matchBracesColor;
	QColor	searchFoundColor;
	QColor	searchNotFoundColor;
	QColor	searchNoText;
	QColor	whiteSpaceColor;
	
	bool	highlightCurrentLine;
	bool	showWhiteSpaces;
	bool	showMatchingBraces;
	bool	showPrintingMargins;
	int	printMarginWidth;
	QString matchingString;

	int	matchStart;
	int	matchEnd;
	QChar	currentChar;
	QChar	matchChar;
	QString	fileName;
	QsvSyntaxHighlighter	*syntaxHighlighter;
	QFileSystemWatcher	*fileSystemWatcher;
	
	QAction			*actionFind;
	SamplePanel		*panel;
	TransparentWidget	*findWidget;
	TransparentWidget	*fileMessage;
	Ui::FindWidget		ui_findWidget;
	Ui::FileMessage		ui_fileMessage;
};

#endif // __LINESEDITOR_H__
