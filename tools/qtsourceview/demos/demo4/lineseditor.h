#ifndef __LINESEDITOR_H__
#define __LINESEDITOR_H__

#if QT_VERSION >= 0x040400
#	include <QPlainTextEdit>
#	define	QTextEditorControl	QPlainTextEdit
#	warning	"Using QPlainTextEdit as the text editor control"
#else
#	include <QTextEdit>
#	define	QTextEditorControl	QTextEdit
#endif

#include <QTextBlock>
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
	 BookmarkLineColor, BreakpointLineColor, ModifiedColor
};

enum BookmarkAction {
	Toggle, Enable, Disable
};

class LinesEditor: public QTextEditorControl
{
	Q_OBJECT
public:
	
	LinesEditor( QWidget *p=NULL );
	virtual		~LinesEditor();
	void		setupActions();

	QColor		getItemColor( ItemColors role );
	void		setItemColor( ItemColors role, QColor );
	int		getMargin();
	void		setMargin( int width );
	int		getTabSize();
	void		setTabSize( int size );
	QsvSyntaxHighlighter*	getSyntaxHighlighter();
	void		setSyntaxHighlighter( QsvSyntaxHighlighter *newSyntaxHighlighter );
	QTextCursor	getCurrentTextCursor();
	bool		getDisplayCurrentLine();
	void		setDisplayCurrentLine( bool );
	bool		getDisplayWhiteSpaces();
	void		setDisplayWhiteSpaces( bool );
	bool		getDisplayMatchingBrackets();
	void		setDisplayMatchingBrackets( bool );
	QString		getMatchingString();
	void		setMatchingString( QString );
	bool		getUsingSmartHome();
	void		setUsingSmartHome( bool );
	bool		getUsingAutoBrackets();
	void		setUsingAutoBrackets( bool );
	void		setBookmark( BookmarkAction action, QTextBlock block );
	void		setBreakpoint( BookmarkAction action, QTextBlock block );
	QWidget*	getPanel();
	void		displayBannerMessage( QString );
	void		hideBannerMessage();
	void		clearEditor();
	int		loadFile( QString );
	void		removeModifications();
	
public slots:
	void		pauseFileSystemWatch();
	void		resumeFileSystemWatch();
	void		adjustMarginWidgets();
	void		showFindWidget();
	void		showReplaceWidget();
	void		showGotoLineWidget();
	void		clearSearchHighlight();
	void		findNext();
	void		findPrev();
	void		toggleBookmark();
	void		toggleBreakpoint();
	void		gotoPrevBookmark();
	void		gotoNextBookmark();
	void		transformBlockToUpper();
	void		transformBlockToLower();
	void		transformBlockCase();
	void		smartHome();
	void		smartEnd();

protected slots:
	void		updateCurrentLine();
	void		on_searchText_textChanged( const QString & text );
	void		on_searchText_editingFinished();
	void		on_replaceWidget_expand( bool checked );
	void		on_replaceOldText_textChanged( const QString & text );
	void		on_replaceOldText_returnPressed();
	void		on_replaceAll_clicked();
	void		on_lineNumber_editingFinished();
	void		on_lineNumber_valueChanged(int i);
	void		on_cursorPositionChanged();
	void		on_textDocument_contentsChange( int position, int charsRemoved, int charsAdded );
	void		on_fileChanged( const QString &fName );
	void		on_fileMessage_clicked( QString s );
	
protected:
	void		keyPressEvent ( QKeyEvent *event );
	void		resizeEvent ( QResizeEvent *event );
	void		timerEvent( QTimerEvent *event );

	void		paintEvent(QPaintEvent *e);
	void		printBackgrounds( QPainter &p );
	void		printWhiteSpaces( QPainter &p, const QTextBlock &block, const QFontMetrics &fm );
	void		printCurrentLines( QPainter &p, const QTextBlock &block );
	void		printMatchingBraces( QPainter &p );
	void		printHighlightString( QPainter &p, const QTextBlock &block, const QFontMetrics &fm );
	void		printMargins( QPainter &p );
	
	void		widgetToBottom( QWidget *w );
	void		widgetToTop( QWidget *w );
	bool		handleKeyPressEvent( QKeyEvent *event );
	bool		handleIndentEvent( bool forward );
	bool		issue_search( const QString &text, QTextCursor newCursor, QFlags<QTextDocument::FindFlag> findOptions );
	virtual void	findMatching( QChar c1, QChar c2, bool forward, QTextBlock &block );
	virtual void	findMatching( QChar c, QTextBlock &block );
	PrivateBlockData*	getPrivateBlockData( QTextBlock block, bool createIfNotExisting=false );
	QFlags<QTextDocument::FindFlag> getSearchFlags();
	QFlags<QTextDocument::FindFlag> getReplaceFlags();

public:
	QAction	*actionFind;
	QAction	*actionFindNext;
	QAction	*actionFindPrev;
	QAction	*actionClearSearchHighlight;
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
	bool	usingSmartHome;
	bool	usingAutoBrackets;
	int	printMarginWidth;
	QString	matchingString;
	
	bool	ignoreFileSystemWatch;
	int	matchStart;
	int	matchEnd;
	QChar	currentChar;
	QChar	matchChar;
	QString	highlightString;
	QString	fileName;
	QsvSyntaxHighlighter	*syntaxHighlighter;
	QFileSystemWatcher	*fileSystemWatcher;
	
	QTextCursor		searchCursor;
	SamplePanel		*panel;
	TransparentWidget	*findWidget;
	TransparentWidget	*replaceWidget;
	TransparentWidget	*gotoLineWidget;
	TransparentWidget	*fileMessage;
	Ui::FindWidget		ui_findWidget;
	Ui::ReplaceWidget	ui_replaceWidget;
	Ui::GotoLineWidget	ui_gotoLineWidget;
	Ui::FileMessage		ui_fileMessage;
};

#endif // __LINESEDITOR_H__
