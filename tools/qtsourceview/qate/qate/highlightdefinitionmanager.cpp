#include "highlightdefinitionmanager.h"
#include "qate/mimedatabase.h"

#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QXmlStreamAttributes>

using namespace TextEditor;
using namespace Internal;

HighlightDefinitionManager *HighlightDefinitionManager::m_instance = NULL;

HighlightDefinitionManager::HighlightDefinitionManager()
{
	m_instance = this;
}

const QString HighlightDefinitionManager::definitionIdByName(const QString &externalName)
{
	const QString s;
	return s;
}

bool HighlightDefinitionManager::isBuildingDefinition(const QString &externalName)
{
	return false;
}

QSharedPointer<HighlightDefinition> HighlightDefinitionManager::definition(const QString &externalName)
{
	return QSharedPointer<HighlightDefinition>();
}

HighlightDefinitionManager *HighlightDefinitionManager::instance()
{
	if (!m_instance){
		m_instance = new HighlightDefinitionManager;
	}
	
	return m_instance;
}

void HighlightDefinitionManager::parseDefinitionMetadata(const QFileInfo &fileInfo,
	QString *comment,
	QStringList *mimeTypes,
	QStringList *patterns)
{
	static const QLatin1Char kSemiColon(';');
	static const QLatin1Char kSlash('/');
	static const QLatin1String kLanguage("language");
	static const QLatin1String kName("name");
	static const QLatin1String kExtensions("extensions");
	static const QLatin1String kMimeType("mimetype");
	static const QLatin1String kPriority("priority");
	static const QLatin1String kArtificial("artificial");
	
	const QString &id = fileInfo.absoluteFilePath();
	
	QFile definitionFile(id);
	if (!definitionFile.open(QIODevice::ReadOnly | QIODevice::Text))
	return;
	
	QXmlStreamReader reader(&definitionFile);
	while (!reader.atEnd() && !reader.hasError()) {
		if (reader.readNext() == QXmlStreamReader::StartElement &&
		reader.name() == kLanguage) {
			const QXmlStreamAttributes &attr = reader.attributes();
			
			*comment = attr.value(kName).toString();
			m_idByName.insert(*comment, id);
			*patterns = attr.value(kExtensions).toString().split(kSemiColon, QString::SkipEmptyParts);
			*mimeTypes = attr.value(kMimeType).toString().split(kSemiColon,QString::SkipEmptyParts);
			if (mimeTypes->isEmpty()) {
				// There are definitions which do not specify a MIME type, but specify file
				// patterns. Creating an artificial MIME type is a workaround.
				QString artificialType(kArtificial);
				artificialType.append(kSlash).append(*comment);
				m_idByMimeType.insert(artificialType, id);
				mimeTypes->append(artificialType);
			} else {
				foreach (const QString &type, *mimeTypes)
					m_idByMimeType.insert(type, id);
			}
			
			// The priority below should not be confused with the priority used when matching files
			// to MIME types. Kate uses this when there are definitions which share equal
			// extensions/patterns. Here it is for choosing a highlight definition if there are
			// multiple ones associated with the same MIME type (should not happen in general).
			// TODO
//			m_priorityComp.m_priorityById.insert(id, attr.value(kPriority).toString().toInt());
			
			break;
		}
	}
	reader.clear();
	definitionFile.close();
}

void HighlightDefinitionManager::buildMimeDatabase( const QString &originDir )
{
	QDir definitionsDir( originDir );
	
	QStringList filter(QLatin1String("*.xml"));
	definitionsDir.setNameFilters(filter);
	
	const QFileInfoList &filesInfo = definitionsDir.entryInfoList();
	foreach (const QFileInfo &fileInfo, filesInfo) {
		QString comment;
		QStringList mimeTypes;
		QStringList patterns;
		parseDefinitionMetadata(fileInfo, &comment, &mimeTypes, &patterns);
		
		// A definition can specify multiple MIME types and file extensions/patterns. However, each
		// thing is done with a single string. Then, there is no direct way to tell which patterns
		// belong to which MIME types nor whether a MIME type is just an alias for the other.
		// Currently, I associate all expressions/patterns with all MIME types from a definition.
		
		static const QStringList textPlain(QLatin1String("text/plain"));
		
		QList<Core::MimeGlobPattern> expressions;
		foreach (const QString &type, mimeTypes) {
//			Core::MimeType mimeType = Core::ICore::instance()->mimeDatabase()->findByType(type);
			Core::MimeType mimeType;
			
			// lets assume we can work without a mime database
			if (m_mime)
				mimeType = m_mime->findByType(type);
			if (mimeType.isNull()) {
				if (expressions.isEmpty()) {
					foreach (const QString &pattern, patterns) {
						QRegExp r = QRegExp(pattern, Qt::CaseSensitive, QRegExp::Wildcard);
						Core::MimeGlobPattern pattern(r);
						expressions.append(pattern);
					}
				}
			
				mimeType.setType(type);
				mimeType.setSubClassesOf(textPlain);
				mimeType.setComment(comment);
				mimeType.setGlobPatterns(expressions);
				
				// TODO, WTF did they use QFuture* crap?
				//future.reportResult(mimeType);
				m_mime->addMimeType(mimeType);
//				m_factory->m_mimeTypes.append(mimeType.type());
			}
		}
	}
}

QString HighlightDefinitionManager::getHighlightsPath()
{
	return QLatin1String("/usr/share/kde4/apps/katepart/syntax/");
}

