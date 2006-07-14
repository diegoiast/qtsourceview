/**
 * \file qsvlangdeffactory.cpp
 * \brief Implementation of the language definition factory
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 * \see qmdiActionGroup
 */
 
#include <QMessageBox>
#include <QRegExp>
#include <QFile>
#include <QDir>
#include <QTextStream>

#include "qelangdeffactory.h"
#include "qegtklangdef.h"

#include "debug_info.h"

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
	QMessageBox::information(0, "Application name", "Not found any highlighter for " + fileName );
		
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
	//


	// load mime types
	QFile file( ":/mime.types" );

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::information(0, "Application name",
		    "The factory default will be used instead.");
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
