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
	
//	bool			showLineNumbers;
//	bool			showWhiteSpaces;
//	bool			showMargins;
//	bool			insertSpacesInsteadOfTabs;
//	int			tabSize;
//	int			marginsWidth;
//	EndOfLineType		endOfLine;
//	QsvColorDefFactory	*currentColorScheme;
};


class QsvTextEdit : public QPlainTextEdit
{
	Q_OBJECT
public:
	QsvTextEdit( QWidget *parent = 0, QsvSyntaxHighlighterBase *s = 0 );

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
	void modificationsLookupEnabled( bool on );
	bool getModificationsLookupEnabled() const;

	void paintPanel(QPaintEvent *e);

public slots:
	void cursorMoved();
	void smartHome();
	void smartEnd();
	void removeModifications();
	void on_textDocument_contentsChange( int position, int charsRemoved, int charsAdded );

signals:
	void widgetResized();

protected:
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);
	void keyPressEvent(QKeyEvent *e);
	bool handleKeyPressEvent(QKeyEvent *e);

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
