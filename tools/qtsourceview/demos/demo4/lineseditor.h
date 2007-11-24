#ifndef __LINESEDITOR_H__
#define __LINESEDITOR_H__

#include <QTextEdit>
#include "ui_findwidget.h"
#include "ui_replacewidget.h"
#include "ui_gotolinewidget.h"
#include "ui_filemessage.h"

class QsvSyntaxHighlighter;
class QFileSystemWatcher;
class QTextCursor;
enum QTextDocument::FindFlag;

class SamplePanel;
class TransparentWidget;
class PrivateBlockData;

enum ItemColors {
	 LinesPanel, CurrentLine, MatchBrackets, NoText, TextFound, TextNoFound, WhiteSpaceColor, 
	 BookmarkLineColor, BreakpointLineColor
};

enum BookmarkAction {
	Toggle, Enable, Disable
};

class LinesEditor: public QTextEdit
{
	Q_OBJECT
public:
	
	LinesEditor( QWidget *p=NULL );
	void		setupActions();
	virtual void	findMatching( QChar c1, QChar c2, bool forward, QTextBlock &block );
	virtual void	findMatching( QChar c, QTextBlock &block );
	PrivateBlockData*	getPrivateBlockData( QTextBlock block, bool createIfNotExisting=false );
	QsvSyntaxHighlighter*	getSyntaxHighlighter();
	
public slots:
	void		showFindWidget();
	void		showReplaceWidget();
	void		showGotoLineWidget();
	void		findNext();
	void		findPrev();
	bool		issue_search( const QString &text, QTextCursor newCursor, QFlags<QTextDocument::FindFlag> findOptions );
	int		loadFile( QString );

	QColor		getItemColor( ItemColors role );
	void		setItemColor( ItemColors role, QColor );
	void		setMargin( int width );
	//int		getMargin();
	void		setTabSize( int size );
	//int		getTabSize();
	void		setSyntaxHighlighter( QsvSyntaxHighlighter *newSyntaxHighlighter );	
	QTextCursor	getCurrentTextCursor();
	void		setDisplayCurrentLine( bool );
	//bool		getDisplayCurrentLine();
	void		setDisplayWhiteSpaces( bool );
	//bool		getDisplayWhiteSpaces();
	void		setDisplatMatchingBrackets( bool );
	//bool		getDisplatMatchingBrackets();
	void		setMatchingString( QString );
	//QString		getMatchingString();
	void		setBookmark( BookmarkAction action, QTextBlock block );
	void		toggleBookmark();
	void		gotoNextBookmark();
	void		gotoPrevBookmark();
	void		setBreakpoint( BookmarkAction action, QTextBlock block );
	void		toggleBreakpoint();
	
	void		transformBlockToUpper();
	void		transformBlockToLower();
	void		transformBlockCase();
	
	QWidget*	getPanel();
	void		adjustMarginWidgets();
		
protected slots:
	void	updateCurrentLine();
	void	on_searchText_textChanged( const QString & text );
	void	on_replaceWidget_expand( bool checked );
	void	on_replaceOldText_textChanged( const QString & text );
	void	on_replaceOldText_valueChanged(int i);
	void	on_replaceOldText_editingFinished();
	void	on_cursorPositionChanged();
	void	on_textDocument_contentsChanged();
	void	on_fileChanged( const QString &fName );
	void	on_fileMessage_clicked( QString s );
	
protected:
	void	keyPressEvent ( QKeyEvent *event );
	void	resizeEvent ( QResizeEvent *event );
	void	paintEvent(QPaintEvent *e);
	void	timerEvent( QTimerEvent *event );
	void	printBackgrounds( QPainter &p );
	void	printWhiteSpaces( QPainter &p, QTextBlock &block );
	void	printCurrentLines( QPainter &p, QTextBlock &block );
	void	printMatchingBraces( QPainter &p );
	void	printMargins( QPainter &p );
	void	widgetToBottom( QWidget *w );
	void	widgetToTop( QWidget *w );
	bool	handleKeyPressEvent( QKeyEvent *event );
	bool	handleIndentEvent( bool forward );

public:
	QAction	*actionFind;
	QAction	*actionFindNext;
	QAction	*actionFindPrev;
	QAction	*actionReplace;
	QAction	*actionGotoLine;
	
	QAction	*actionNextBookmark;
	QAction	*actionPrevBookmark;
	QAction	*actionCapitalize;
	QAction	*actionLowerCase;
	QAction	*actionChangeCase;
	QAction	*actionToggleBookmark;
	QAction	*actionTogglebreakpoint;

private:
	void	updateMarkIcons();

	QPixmap	tabPixmap;
	QPixmap spacePixmap;
	QColor	currentLineColor;
	QColor	bookmarkLineColor;
	QColor	breakpointLineColor;
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
	
	QTextCursor		searchCursor;
	SamplePanel		*panel;
	QWidget			*findWidget;
	QWidget			*replaceWidget;
	QWidget			*gotoLineWidget;
	QWidget			*fileMessage;
	Ui::FindWidget		ui_findWidget;
	Ui::ReplaceWidget	ui_replaceWidget;
	Ui::GotoLineWidget	ui_gotoLineWidget;
	Ui::FileMessage		ui_fileMessage;
};

#endif // __LINESEDITOR_H__
