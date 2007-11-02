#ifndef __QSV_SYNTAXHIGHLIGHTER__
#define __QSV_SYNTAXHIGHLIGHTER__

/**
 * \file qsvsyntaxhighlighter.h
 * \brief Definition of the syntax highlighter
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 * \see qmdiActionGroup
 */

#include <QSyntaxHighlighter>
#include "qorderedmap.h"

class QTextCharFormat;
class QsvLangDef;
class QsvColorDefFactory;

struct LanguageEntity
{
	QString name;
	QTextCharFormat charFormat;
};

class QsvSyntaxHighlighter: public QSyntaxHighlighter
{
public:
	QsvSyntaxHighlighter( QTextDocument *parent = 0, QsvColorDefFactory *colors=0, QsvLangDef *lang=0 );
	QsvSyntaxHighlighter( QTextEdit *parent = 0, QsvColorDefFactory *colors=0, QsvLangDef *lang=0 );
	~QsvSyntaxHighlighter();
	void setHighlight( QsvLangDef *newLang=0 );
	void setColorsDef( QsvColorDefFactory *newColors=0 );

protected:
	void highlightBlock(const QString &text);

private:
	void addMapping( const QString mappingName, const QString &pattern, const QTextCharFormat &format, bool fullWord=false );
	void addMappingFromName( const QString &pattern, const QString formatName, bool fullWord=false );

	void drawText    ( QString text, QString s, QTextCharFormat &format );
	void drawRegExp  ( QString text, QString s, QTextCharFormat &format );
	void drawKeywords( QString text, QString s, QTextCharFormat &format );

	QsvColorDefFactory	*colors;
	QsvLangDef		*language;
	QOrderedMap<QString,LanguageEntity> mappings;
};

#endif  // __QSV_SYNTAXHIGHLIGHTER__
