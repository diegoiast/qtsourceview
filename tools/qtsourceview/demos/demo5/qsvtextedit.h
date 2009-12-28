#ifndef QSVTEXTEDIT_H
#define QSVTEXTEDIT_H

#include <QPlainTextEdit>
#include <QTextCharFormat>
#include <QColor>

class QsvSyntaxHighlighterBase;
class QsvEditorPanel;
class QsvBlockData;

enum EndOfLineType {
	DOS, Unix, Mac, KeepOldStyle
};

struct QsvEditorConfigData {
	bool			markCurrentLine;
	bool			smartHome;
	bool			matchBrackets;
	QString			matchBracketsList;
	QFont			currentFont;
	bool			lineWrapping;
	bool			modificationsLookupEnabled;
	bool			autoBrackets;
	bool			showLineNumbers;
	bool			showMargins;
	uint			marginsWidth;
	int			tabSize;
	bool			insertSpacesInsteadOfTabs;
	bool			tabIndents;
	
//	bool			showWhiteSpaces;
//	EndOfLineType		endOfLine;
//	QsvColorDefFactory	*currentColorScheme;
};


class QsvTextEdit : public QPlainTextEdit
{
	Q_OBJECT
public:
	QsvTextEdit( QWidget *parent = 0, QsvSyntaxHighlighterBase *s = 0 );
	void setupActions();

	void setMarkCurrentLine( bool on );
	bool getmarkCurrentLine() const;
	void setStartHome( bool on );
	bool getStartHome() const;
	void setMatchBracketList( const QString &m );
	const QString getMatchBracketList() const;
	void setMatchBracket( bool on );
	bool getMatchBracket() const;
	void setLineWrapping( bool on );
	bool getLineWrapping() const;
	void setModificationsLookupEnabled( bool on );
	bool getModificationsLookupEnabled() const;
	void setShowLineNumbers( bool on );
	bool getShowLineNumbers() const;
	void setShowMargins( bool on );
	bool getShowMargins() const;
	void setMarginsWidth( uint i );
	uint getMarginsWidth() const;
	void setTabSize( int size );
	int  getTabSize() const;
	void setInsertSpacesInsteadOfTabs( bool on );
	bool getInsertSpacesInsteadOfTabs() const;
	void setTabIndents( bool on );
	bool getTabIndents() const;
	
	void setDefaultConfig();
	static void setDefaultConfig( QsvEditorConfigData *config );
	
	QTextCursor getCurrentTextCursor();
	void paintPanel(QPaintEvent *e);

public slots:
	void smartHome();
	void smartEnd();
	void transformBlockToUpper();
	void transformBlockToLower();
	void transformBlockCase();
	
	void removeModifications();
	void on_cursor_positionChanged();
	void on_textDocument_contentsChange( int position, int charsRemoved, int charsAdded );

signals:
	void widgetResized();

public:
	QAction *actionCapitalize;
	QAction *actionLowerCase;
	QAction *actionChangeCase;
	
protected:
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);
	void keyPressEvent(QKeyEvent *e);
	bool handleKeyPressEvent(QKeyEvent *e);
	virtual void updateMargins();

	bool handleIndentEvent( bool forward );
	int getIndentationSize( const QString s );
	QString updateIndentation( QString s, int indentation );
	int findMatchingChar( QChar c1, QChar c2, bool forward, QTextBlock &block, int from );
	QsvBlockData *getPrivateBlockData( QTextBlock block, bool createIfNotExisting );

	QsvSyntaxHighlighterBase *m_highlighter;
	QsvEditorPanel *m_panel;

	QTextCharFormat m_matchesFormat;
	QColor          m_currentLineBackground;
	QColor          m_panelColor;
	QColor          m_modifiedColor;
	QPixmap m_bookMarkImage;

	QsvEditorConfigData m_config;
};

#endif // QSVTEXTEDIT_H
