/**
 * \file qsvsyntaxhighlighter.cpp
 * \brief Implementation of the syntax highlighter
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 * \see qmdiActionGroup
 */

#include <QString>
#include <QStringList>
#include <QDomDocument>
#include <QDomNode>
#include <QFile>
#include <QTextCharFormat>

#include "qorderedmap.h"
#include "qsvlangdef.h"
#include "qsvcolordef.h"
#include "qsvcolordeffactory.h"
#include "qsvsyntaxhighlighter.h"

#include "debug_info.h"

QsvSyntaxHighlighter::QsvSyntaxHighlighter( QTextDocument *parent, QsvColorDefFactory *colors, QsvLangDef *lang )
	:QSyntaxHighlighter(parent)
{
	language = NULL;
	this->colors = colors;

	setHighlight( lang );
}

QsvSyntaxHighlighter::QsvSyntaxHighlighter( QTextEdit *parent, QsvColorDefFactory *colors, QsvLangDef *lang )
	:QSyntaxHighlighter(parent)
{
	language = NULL;
	this->colors = colors;

	setHighlight( lang );
}

QsvSyntaxHighlighter::~QsvSyntaxHighlighter()
{
}

void QsvSyntaxHighlighter::setHighlight( QsvLangDef *newLang )
{
	QString str;
	language = newLang;

	mappings.clear();
	
	if (language == NULL)
		return;

	//first match keyword lists
	// TODO: optimizations
	foreach( QsvEntityKeywordList l, language->keywordListDefs )
	{
		foreach( QString s, l.list )
		{
//			TODO use these defintions
// 			if (l.matchEmptyStringAtBeginning)
// 			if (l.matchEmptyStringAtEnd)

			s = l.startRegex + s + l.endRegex;
			addMapping( s, l.style );
		}
	}

	// syntax itmes...
	foreach( QsvEntityBlockComment l, language->syntaxItemDefs )
	{
		QString s;
		if (l.endRegex == "\\n")
			s  = l.startRegex + ".*$";
		else
			s  = l.startRegex + ".*" + l.endRegex;
 		addMapping( s, l.style );
	}

	// later, pattern items
	// TODO: optimizations
	foreach( QsvEntityPatternItem l, language->patternItems )
	{
		addMapping( l.regex, l.style, !true );
	}

	// strings...
	foreach( QsvEntityString l, language->stringsDefs )
	{
		if (!l.atEOL)
			continue;

		QString s = l.startRegex + QString("[^%1]*").arg(l.startRegex) + l.endRegex;
		addMapping( s, l.style );
	}

	// and finally... line comments...
	// block comments are handeled in the drawing function	
	foreach( QsvEntityLineComment l, language->lineCommentsDefs )
	{
		addMapping( QString("%1.*").arg(l.start), l.style );
	}

	// now we need to re-highlight, how the hell can we do this?
	setDocument( document() );
}

void QsvSyntaxHighlighter::setColorsDef( QsvColorDefFactory *newColors )
{
	delete colors;
	colors = newColors;
	
	// now we need to re-highlight, how the hell can we do this?
	setDocument( document() );
}

// called when need to update a paragraph
void QsvSyntaxHighlighter::highlightBlock(const QString &text)
{
	if (language == NULL)
	{
#ifdef		__DEBUG_HIGHLIGHT__		
		qDebug( "%s %d - no language defined", __FILE__, __LINE__ );
#endif		
		return;
	}
	
	// set the whole text to the defautl format to begin with
	QOrderedMapNode<QString,QTextCharFormat> pattern;
	
	// optimizations...
	if (text.simplified().isEmpty())
		goto HANDLE_BLOCK_COMMENTS;
		
	foreach( QsvEntityLineComment l, language->lineCommentsDefs )
	{
		if (text.startsWith( l.start ))
		{
			setFormat( 0, text.length(), colors->getColorDef("dsComment").toCharFormat() );
			return;
		}
	}
	setFormat( 0, text.length(), colors->getColorDef("dsNormal").toCharFormat() );

	// this code draws each line
	foreach ( pattern, mappings.keys())
		drawText( text, pattern.key, pattern.value );

	setCurrentBlockState(0);

HANDLE_BLOCK_COMMENTS:
	// what if not block comments defined...?
	if (language->blockCommentsDefs.count() == 0)
		return;

	QRegExp startExpression( language->blockCommentsDefs.at(0).startRegex );
	QRegExp endExpression  ( language->blockCommentsDefs.at(0).endRegex );
	
	int startIndex = 0;
	if (previousBlockState() != 1)
		startIndex = text.indexOf(startExpression);
	
	while (startIndex >= 0) 
	{
		int endIndex = text.indexOf(endExpression, startIndex);
		int commentLength;
		
		if (endIndex == -1) 
		{
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		} 
		else 
		{
			commentLength = endIndex - startIndex
				+ endExpression.matchedLength();
		}
		setFormat( startIndex, commentLength, colors->getColorDef("dsComment").toCharFormat() );
		startIndex = text.indexOf( startExpression, startIndex + commentLength );
	}
}

void QsvSyntaxHighlighter::addMapping(const QString &pattern, const QTextCharFormat &format, bool fullWord )
{	
	QString p = pattern;
	if (fullWord)
		p = "\\b" + p + "\\b";
		
#ifdef __DEBUG_ADD_MAPPING__
	qDebug( "%s %d new mapping - [%s] %s - ", __FILE__, __LINE__, 
		qPrintable(pattern), 
		qPrintable(format.foreground().color().name()) 		
	);
#endif

	mappings.add( p, format );
}

void QsvSyntaxHighlighter::addMapping(const QString &pattern, const QString formatName, bool fullWord )
{
	QString s = formatName;
	if (!colors)
		return;

	// convert GTK formats to Kate
	if (s == "Comment")
		s = "dsComment";
	else if (s == "String")
		s = "dsString";
	else if (s == "Preprocessor")
		s = "dsOthers";
	else if (s == "Keyword")
		s = "dsKeyword";
	else if (s == "Data Type")
		s = "dsDataType";
	else if (s == "Decimal")
		s = "dsDecVal";
	else if (s == "Floating Point")
		s = "dsFloat";
	else if (s == "Base-N Integer")
		s = "dsBaseN";
	else if (s == "Function")
		s = "dsFunction";
	else if (s == "Others 2")
		s = "dsOthers2";
	else if (s == "Others 3")
		s = "dsOthers3";

	addMapping( pattern, colors->getColorDef(s).toCharFormat(), fullWord );
}

void QsvSyntaxHighlighter::drawText( QString text, QString s, QTextCharFormat &format )
{
//	using regex is a smart idea, but slow
//	if (s.contains( QRegExp("[^\\\\*+()?]") )){
	if  (
		s.contains('^') || s.contains('+') || s.contains('*') || s.contains('?') || 
		s.contains('(') || s.contains(')') || s.contains('[') || s.contains(']')
	     )
		drawRegExp( text, s, format );
	else
		drawKeywords( text, s, format );
}

void QsvSyntaxHighlighter::drawRegExp( QString text, QString s, QTextCharFormat &format )
{	
	QRegExp expression(s);
	int index = text.indexOf(expression);

#ifdef __DEBUG_HIGHLIGHT__
	qDebug( "%s %d -  %s )", __FILE__, __LINE__, qPrintable(s) );
#endif

	while (index >= 0)
	{
		int length = expression.matchedLength();
		setFormat(index, length, format );
		index = text.indexOf(expression, index + length);
	}
}

void QsvSyntaxHighlighter::drawKeywords( QString text, QString s, QTextCharFormat &format )
{
#ifdef __DEBUG_HIGHLIGHT__
	qDebug( "%s %d drawing - %s", __FILE__, __LINE__, qPrintable(s) );
#endif

	int index = text.indexOf(s);
	int length = s.length();
	int txtLen = text.length();
	
	while ( (index >= 0) && (index < txtLen) )
	{
		
		// paint keyword, only if its suoorunded by white chars
		// regexp are slow, this code looks bad, but faster :)
		if (
                   ((index==0) || 
                   	(!text[index-1].isLetterOrNumber() && (text[index-1] != '_'))) &&
                   ((index+length>=txtLen) || 
                   	(!text[index+length].isLetterOrNumber() && (text[index+length] != '_') ))
                   )
			setFormat(index, length, format );
		index = text.indexOf(s, index + length);
	}
}

