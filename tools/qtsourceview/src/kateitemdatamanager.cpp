#include <QtDebug>
#include <QFile>

#include "kateitemdatamanager.h"
#include "debug_info.h"

//QsvColorDefFactory
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
#ifdef __DEBUG_NO_ITEM_FOUND__
	qDebug( "%s %d", __FILE__, __LINE__ );
#endif

	foreach(QsvColorDef item, colorDefs)
	{	
		if (item.getStyleNum() == name )
			return item;
	}

#ifdef __DEBUG_NO_ITEM_FOUND__
	// new empthy one	
	qDebug( "%s $d - could not find a color definition named (%s)", __FILE__, __LINE__, qPrintable(fileName) );
#endif	
	return QsvColorDef();
}

bool QsvColorDefFactory::load( QDomDocument doc )
{
	/* load the attributes of this language */
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
