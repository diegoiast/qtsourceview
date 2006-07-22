/**
 * \file qsvlangdeffactory.cpp
 * \brief Implementation of the language definition factory
 * \date 2006-07-21 23:15:55
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 * \see qmdiActionGroup
 */
 
#include <QMessageBox>
#include <QRegExp>
#include <QFile>
#include <QDir>
#include <QTextStream>

#include "qsvlangdeffactory.h"
#include "qsvlangdef.h"

#include "debug_info.h"

/**
 * \class QsvLangDefFactory
 * \brief An abstract factory for languages definitions
 *
 * When making an editor, you will like to load different file types, for
 * every different one the use might expect a different highlight defintion.
 * This class is a factory for those defintions: you load the defintions
 * from a directory, and then you start quering for hightlighs for specific
 * file names.
 *
 * The class is defined as a singleton class, which means you do not
 * need to make an instanse, and not cary about memory allocations.
 *
 * \see QsvLangDef
 */


QsvLangDefFactory *QsvLangDefFactory::LangFactory = NULL;


// public....
QsvLangDefFactory *QsvLangDefFactory::getInstanse()
{
	if (LangFactory == NULL)
	{
		LangFactory = new QsvLangDefFactory();
	}

	return LangFactory;
}

QsvLangDef *QsvLangDefFactory::getHighlight( QString fileName )
{
	QsvLangDef *langDef;
	QString langMimeType;

	foreach( langDef, langList )
	{
		foreach( langMimeType, langDef->getMimeTypes() )
		{
			// if do we recognize the mime type defined in that
			// syntax highligh definition, check if matches this file

			if ( mimeTypes.find(langMimeType) )
			{
				for ( int j=0; j<mimeTypes[langMimeType].count(); j ++ ) 
				{
					QString s = "*." + mimeTypes[langMimeType][j];
					if (QDir::match( s, fileName) || QDir::match( mimeTypes[langMimeType][j], fileName))

// 					QString s = "*" + mimeTypes[langMimeType][j];
// 					if (QDir::match( s, fileName))
					{
#ifdef __DEBUG_FOUND_LANG_DEF__	
						qDebug( "%s %d - Found language definition %s [%s,%s]",
							__FILE__, __LINE__,
							qPrintable(langDef->getName()),
							qPrintable(fileName), qPrintable( "*" + mimeTypes[langMimeType][j] )
						);
#endif						
						return langDef;
					}
				}
			}
			else
			{
				qDebug( "%s %d : Unknown mimetype [%s] at highlight file %s",
					__FILE__, __LINE__, qPrintable(langMimeType), qPrintable(langDef->getName()) );
			}
		}
	}

	qDebug( "%s %d : Not found any highlighter for [%s]", __FILE__, __LINE__, qPrintable(fileName) );
	return NULL;
}

void QsvLangDefFactory::loadDirectory( QString directory )
{
	if (directory.isEmpty())
		directory = QDir::currentPath();
	QDir dir(directory, "*.lang");

	QStringList files = dir.entryList(QDir::Files | QDir::NoSymLinks);
	int fileCount =	files.count();

	if (fileCount == 0)
	{
		qDebug( "%s %d - Error: no highlight definitions found at directory: %s", __FILE__, __LINE__, qPrintable(directory) );
		QMessageBox::information(0, "Application name", 
			"no highlight definitions found at directory " + directory
		);
		return;
	}

	for (int i = 0; i < fileCount; ++i)
	{
		QsvLangDef *langDef = new QsvLangDef ( directory + "/" + files[i] );
		langList << langDef;
		QString langMimeType;

#ifdef __DEBUG_LANGS_MIMES__
		foreach( langMimeType, langDef->getMimeTypes() )
		{
			if ( mimeTypes.find(langMimeType) == mimeTypes.end() )
			{
 				qDebug("%s %d - Warning: highlight file %s - unknown mimetype [%s]",
					__FILE__, __LINE__,
					qPrintable(langDef->getName()), qPrintable(langMimeType)
				);
			}
		}
#endif		
	}
}

// private...
QsvLangDefFactory::QsvLangDefFactory(void)
{
	// TODO: fix the code to use
	// /usr/share/mime/globs

	// load mime types
	QFile file( ":/mime.types" );

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug("%s %d - Error: could not load the default mime.types [%s]",
			__FILE__, __LINE__,
			":/mime.types"
		);
		return;
	}
	
	// parse built in mime types definitions
	QTextStream in(&file);
	while (!in.atEnd())
	{
		QString line = in.readLine();
		
		if (line.startsWith("#"))
			continue;

		QStringList l = line.split( QRegExp("\\s+") );
		QString     name = l[0];
		l.removeAt( 0 );

		if (!l.empty())
		{
// 			QString s;
// 			for ( int j=0; j<l.count(); j ++ ) s = s + "*." + l[j] + ",";
// 			qDebug( "%s -> %s", qPrintable(name), qPrintable(s) );		
			mimeTypes[name] = l;
		}
	}
	file.close();
}

QsvLangDefFactory::~QsvLangDefFactory(void)
{
}
