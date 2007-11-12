#ifndef __LINESEDITOR_H__
#define __LINESEDITOR_H__

#include <QTextEdit>
#include "ui_findwidget.h"
#include "ui_filemessage.h"

class SamplePanel;
class TransparentWidget;
class PrivateBlockData;
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
	void		setupActions();
	virtual void	findMatching( QChar c1, QChar c2, bool forward, QTextBlock &block );
	PrivateBlockData*	getPrivateBlockData( QTextBlock block );
	QsvSyntaxHighlighter*	getSyntaxHighlighter();
	
public slots:
	void		on_searchText_textChanged( const QString & text );
	void		showFindWidget();
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
	void		toggleBookmark();
	
	void		transformBlockToUpper();
	void		transformBlockToLower();
	void		transformBlockCase();
	
	QWidget*	getPanel();
	void		adjustMarginWidgets();
		
protected slots:
	void	cursorPositionChanged();
	void	updateCurrentLine();
	void	on_textDocument_contentsChanged();
	void	on_fileChanged( const QString &fName );
	void	on_fileMessage_clicked( QString s );
	
protected:
	void	keyPressEvent ( QKeyEvent * event );
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
	bool	handleIndentEvent( bool forward );

public:
	QAction	*actionFind;
	QAction	*actionCapitalize;
	QAction	*actionLowerCase;
	QAction	*actionChangeCase;
	QAction	*actionToggleBookmark;

private:
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
		
	SamplePanel		*panel;
	TransparentWidget	*findWidget;
	TransparentWidget	*fileMessage;
	Ui::FindWidget		ui_findWidget;
	Ui::FileMessage		ui_fileMessage;
};

#endif // __LINESEDITOR_H__
