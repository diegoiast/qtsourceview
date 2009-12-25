#ifndef QSVTEXTEDIT_H
#define QSVTEXTEDIT_H

#include <QPlainTextEdit>
#include <QTextCharFormat>
#include <QColor>

class QsvSyntaxHighlighterBase;
class QsvEditorPanel;

class QsvTextEdit : public QPlainTextEdit
{
	Q_OBJECT
public:
	QsvTextEdit( QWidget *parent = 0, QsvSyntaxHighlighterBase *s = 0 );
	void setMatchBracketList( const QString &m );
	const QString getMatchBracketList();
	void paintPanel(QPaintEvent *e);

public slots:
	void cursorMoved();
	void smartHome();
	void smartEnd();

signals:
	void widgetResized();

protected:
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);
	void keyPressEvent(QKeyEvent *e);

	int findMatchingChar( QChar c1, QChar c2, bool forward, QTextBlock &block, int from );

	QsvSyntaxHighlighterBase *m_highlighter;
	QsvEditorPanel *m_panel;

	QString         m_matchBracketsList;
	QTextCharFormat m_matchesFormat;
	QColor          m_currentLineBackground;
	bool            m_smartHome;
};

#endif // QSVTEXTEDIT_H
