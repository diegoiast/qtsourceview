#ifndef __LINESEDITOR_H__
#define __LINESEDITOR_H__

#include <QTextEdit>
#include "ui_findwidget.h"

class SamplePanel;
class TransparentWidget;

enum ItemColors {
	 LinesPanel, CurrentLine, MatchBrackets, NoText, TextFound, TextNoFound
};

class LinesEditor: public QTextEdit
{
	Q_OBJECT
public:
	
	LinesEditor( QWidget *p=NULL );
	void	setupActions();
	QColor	getItemColor( ItemColors role );
	void	setItemColor( ItemColors role, QColor );
	virtual void findMatching( QChar c1, QChar c2, bool forward, QTextBlock &block); 

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
	//void	showPanel( bool visible );
	
protected:
	void	keyReleaseEvent ( QKeyEvent * event );
	void	resizeEvent ( QResizeEvent *event );
	void	paintEvent(QPaintEvent *e);
	void	printWhiteSpaces( QPainter &p );
	void	printCurrentLine( QPainter &p );
	void	printMatchingBraces( QPainter &p );
	void	widgetToBottom( QWidget *w );
	void	widgetToTop( QWidget *w );

private:
	QPixmap	tabPixmap;
	QPixmap spacePixmap;
	QColor	currentLineColor;
	QColor	matchBracesColor;
	QColor	searchFoundColor;
	QColor	searchNotFoundColor;
	QColor	searchNoText;
	
	bool	highlightCurrentLine;
	bool	showWhiteSpaces;
	bool	showMatchingBraces;
	QString matchingString;

	int	matchStart;
	int	matchEnd;
	QChar	currentChar;
	QChar	matchChar;
	QString	fileName;
	
	SamplePanel		*panel;
	QAction			*actionFind;
	TransparentWidget	*findWidget;
	Ui::FindWidget		ui_findWidget;
};

#endif // __LINESEDITOR_H__
