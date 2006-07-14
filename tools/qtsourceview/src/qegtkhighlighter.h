#ifndef __QSV_SYNTAXHIGHLIGHTER__
#define __QSV_SYNTAXHIGHLIGHTER__

#include <QSyntaxHighlighter>
#include "qorderedmap.h"

class QTextCharFormat;
class QsvLangDef;
class QsvColorDefFactory;

class QsvSyntaxHighlighter: public QSyntaxHighlighter
{
public:
	QsvSyntaxHighlighter( QTextDocument *parent = 0, QsvColorDefFactory *colors=0, QsvLangDef *lang=0 );
	QsvSyntaxHighlighter( QTextEdit *parent = 0, QsvColorDefFactory *colors=0, QsvLangDef *lang=0 );
	void setHighlight( QsvLangDef *lang );

protected:
	void highlightBlock(const QString &text);

private:
	void addMapping(const QString &pattern, const QTextCharFormat &format, bool fullWord=false );
	void addMapping(const QString &pattern, const QString formatName, bool fullWord=false );

	void drawText    ( QString text, QString s, QTextCharFormat &format );
	void drawRegExp  ( QString text, QString s, QTextCharFormat &format );
	void drawKeywords( QString text, QString s, QTextCharFormat &format );

	QsvColorDefFactory	*colors;
	QsvLangDef	*language;

	QOrderedMap<QString,QTextCharFormat> mappings;
};

#endif  // __QE_GTK_HIGHLIGHTER_H__
