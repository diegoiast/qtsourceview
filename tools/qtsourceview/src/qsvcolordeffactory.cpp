/**
 * \file qsvcolordeffactory.cpp
 * \brief Implementation of the color defintion factory
 * \author Diego Iastrubni (elcuco@kde.org)
 * \date 2006-07-22 01:58:07
 * License LGPL
 * \see qmdiActionGroup
 */

#include <QtDebug>
#include <QFile>

#include <QBrush>
#include <QTextCharFormat>
#include <QColor>

#include "qsvcolordef.h"
#include "qsvcolordeffactory.h"
#include "debug_info.h"

/**
 * \class QsvColorDefFactory
 * \brief An abstract factory for languages definitions
 *
 *
 * \see QsvColorDef
 */

QsvColorDefFactory::QsvColorDefFactory()
{
}

QsvColorDefFactory::QsvColorDefFactory( QDomDocument doc )
{
	load( doc );
}

QsvColorDefFactory::QsvColorDefFactory( QString fileName )
{
	load( fileName );
}

QsvColorDefFactory::~QsvColorDefFactory()
{
}

QsvColorDef QsvColorDefFactory::getColorDef( QString name )
{
#ifdef __DEBUG_NO_ITEM_FOUND
	qDebug( "%s %d", __FILE__, __LINE__ );
#endif

	foreach(QsvColorDef color, colorDefs)
	{	
		if (color.getStyleNum() == name )
		{
#ifdef __DEBUG_NO_ITEM_FOUND__
	// new empthy one	
	qDebug( "%s %d - fond a color definition named %s - %s", __FILE__, __LINE__, 
		qPrintable(name), 
		qPrintable(color.toCharFormat().foreground().color().name()) 
	);
#endif				
			return color;
		}
	}

#ifdef __DEBUG_NO_ITEM_FOUND__
	// new empthy one	
	qDebug( "%s %d - could not find a color definition named (%s)", __FILE__, __LINE__, qPrintable(name) );
#endif	
	return QsvColorDef();
}

bool QsvColorDefFactory::load( QDomDocument doc )
{
	// load the attributes of this language
	QDomNodeList list = doc.elementsByTagName("itemData");

	for( uint n=0; n<list.length(); n++ )
	{
		QsvColorDef item;
		if (item.load(list.item(n)))
			colorDefs.append( item );
	}

	return true;
}

bool QsvColorDefFactory::load( QString fileName )
{
	QFile file(fileName);

	if (!file.open(QIODevice::ReadOnly))
	{
		// TODO: display debug message, saying that
		// this is an invalid XML
		return false;
	}

	QDomDocument doc("language");
	if (!doc.setContent(&file))
	{
		file.close();
		return false;
	}
	file.close();

	return load( doc );
}
