#ifndef __QE_LANGDEF_FACTORY_H__
#define __QE_LANGDEF_FACTORY_H__

#include <QMap>

class QsvLangDef;
class QString;

class QsvLangDefFactory
{
public:
	static QsvLangDefFactory *getInstanse();
	QsvLangDef *getHighlight( QString fileName );
	void loadDirectory( QString directory );

private:	
	QsvLangDefFactory(void);
	~QsvLangDefFactory(void);
	
	static QsvLangDefFactory *LangFactory;
	QList<QsvLangDef*> langList;
	QMap<QString,QStringList> mimeTypes;
};

#endif // __QE_LANGDEF_FACTORY_H__
