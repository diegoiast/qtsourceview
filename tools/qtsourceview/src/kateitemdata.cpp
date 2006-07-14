#include <QString>
#include <QDomNode>
#include <QColor>
#include <QTextCharFormat>

#include "kateitemdata.h"

/**
 * \class kateItemData
 * \brief and item data contains the color definition for a context
 */
QsvColorDef::QsvColorDef()
{
// 	attributes["color"] = "black";
// 	attributes["background"] = "white";
}

QsvColorDef::QsvColorDef( QDomNode node )
{
	load( node );
}

bool QsvColorDef::load( QDomNode node )
{
	uint attrCount = node.attributes().count();
	for( uint i=0; i< attrCount; i++ )
	{
		attributes[node.attributes().item(i).nodeName()] =
		   node.attributes().item(i).nodeValue();
	}

	return true;
}

bool QsvColorDef::save( QDomNode node )
{
//QStringMap attributes;
	return true;
}


bool QsvColorDef::isBold()
{
	if (!attributes.contains("bold"))
		return false;
	
	QString boldAttr = attributes["bold"].toLower();
	if ((boldAttr == "1") || (boldAttr == "true") || (boldAttr == "yes"))
		return true;
	else
		return false;
}

bool QsvColorDef::isUnderline()
{
	if (!attributes.contains("underline"))
		return false;
	
	QString boldAttr = attributes["underline"].toLower();
	if ((boldAttr == "1") || (boldAttr == "true") || (boldAttr == "yes"))
		return true;
	else
		return false;
}

bool QsvColorDef::isItalic()
{
	if (!attributes.contains("italic"))
		return false;
	
	QString boldAttr = attributes["italic"].toLower();
	if ((boldAttr == "1") || (boldAttr == "true") || (boldAttr == "yes"))
		return true;
	else
		return false;

}

QColor QsvColorDef::getColor()
{
	if (!attributes.contains("color"))
		return QColor();
	else 
		return QColor( attributes["color"] );
}

QColor QsvColorDef::getSelColor()
{
	if (!attributes.contains("selColor"))
		return QColor();
	else 
		return QColor( attributes["selColor"] );
}

QColor QsvColorDef::getBackground()
{
	if (!attributes.contains("background"))
		return QColor();
	else 
		return QColor( attributes["background"] );
}

QString QsvColorDef::getStyleNum()
{
// 	if (!attributes.contains("defStyleNum"))
// 		return QString();

	return attributes["defStyleNum"];
}

QTextCharFormat QsvColorDef::toCharFormat()
{
	QTextCharFormat f;

	if (attributes.contains("color"))
		f.setForeground( QColor(attributes["color"]) );

	if (attributes.contains("background"))
		f.setBackground( QColor(attributes["background"]) );

	if (isBold()) 
		f.setFontWeight(QFont::Bold);
	
	if (isItalic())
		f.setFontItalic( true );

	if (isUnderline())
		f.setFontUnderline( true );

	return f;
}
