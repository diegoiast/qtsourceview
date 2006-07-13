#include <QtDebug>
#include <QFile>

#include "kateitemdatamanager.h"
#include "debug_info.h"

kateItemDataManager::kateItemDataManager()
{
}

kateItemDataManager::kateItemDataManager( QDomDocument doc )
{
	load( doc );
}

kateItemDataManager::kateItemDataManager( QString fileName )
{
	load( fileName );
}

kateItemDataManager::~kateItemDataManager()
{
}

kateItemData kateItemDataManager::getItemData( QString name )
{
#ifdef __DEBUG_NO_ITEM_FOUND__
	qDebug( "%s %d", __FILE__, __LINE__ );
#endif

	foreach(kateItemData item, itemDatas)
	{	
		if (item.getStyleNum() == name )
			return item;
	}

#ifdef __DEBUG_NO_ITEM_FOUND__
	// new empthy one	
	qDebug( "%s $d - could not find an item data (%s)", __FILE__, __LINE__, qPrintable(fileName) );
#endif	
	return kateItemData();
}

bool kateItemDataManager::load( QDomDocument doc )
{
	/* load the attributes of this language */
	QDomNodeList list = doc.elementsByTagName("itemData");

	for( uint n=0 ;n<list.length(); n++ )
	{
		kateItemData item;
		if (item.load(list.item(n)))
			itemDatas.append( item );
	}

	return true;
}

bool kateItemDataManager::load( QString fileName )
{
	QDomDocument doc("language");
	QFile file(fileName);
	QString s;

	if (!file.open(QIODevice::ReadOnly))
		return false;

	if (!doc.setContent(&file))
	{
		file.close();
		return false;
	}
	file.close();

	return load( doc );
}
