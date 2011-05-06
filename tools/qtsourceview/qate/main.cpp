#include <QApplication>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QFile>
#include <QIODevice>

#include "formats.h"
#include "highlighter.h"
#include "highlightdefinition.h"
#include "highlighterexception.h"
#include "qate/highlightdefinitionhandler-v2.h"


void load_text(QString fe, QPlainTextEdit *te );
QSharedPointer<TextEditor::Internal::HighlightDefinition> get_highlighter_definition(QString definitionFileName);

int main( int argc, char* argv[] )
{
	QApplication app( argc, argv );
	QString         definitionFileName = "/usr/share/kde4/apps/katepart/syntax/sql.xml";
	QSharedPointer<TextEditor::Internal::HighlightDefinition> def = get_highlighter_definition(definitionFileName);
	
	QMainWindow    *mw = new QMainWindow;
	QPlainTextEdit *te = new QPlainTextEdit(mw);
	TextEditor::Internal::Highlighter *hl = new TextEditor::Internal::Highlighter;

	hl->configureFormat(TextEditor::Internal::Highlighter::Normal,   Formats::instance().charFormat() );
	hl->configureFormat(TextEditor::Internal::Highlighter::Keyword,  Formats::instance().keywordFormat() );
	hl->configureFormat(TextEditor::Internal::Highlighter::Decimal,  Formats::instance().decimalFormat() );
	hl->configureFormat(TextEditor::Internal::Highlighter::Comment,  Formats::instance().commentFormat() );
	hl->configureFormat(TextEditor::Internal::Highlighter::Function, Formats::instance().functionFormat() );

	hl->setParent(te);
	hl->setDocument(te->document());
	hl->setDefaultContext(def->initialContext());
	hl->rehighlight();


//	load_text(__FILE__, te);
	load_text("/home/elcuco/test.sql", te);
	mw->setWindowTitle("Kate syntax highter test");
	mw->setCentralWidget(te);
	mw->show();
	return app.exec();
	
	Q_UNUSED(hl);
}

void load_text(QString fe, QPlainTextEdit *te )
{
	QFile f(fe);
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	QString s = f.readAll();
	te->setPlainText(s);
	te->setLineWrapMode(QPlainTextEdit::NoWrap);
}

// stollen from QSharedPointer<HighlightDefinition> Manager::definition(const QString &id)
QSharedPointer<TextEditor::Internal::HighlightDefinition> get_highlighter_definition(QString definitionFileName)
{
	QFile definitionFile(definitionFileName);
	if (!definitionFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return QSharedPointer<TextEditor::Internal::HighlightDefinition>();
	
	QSharedPointer<TextEditor::Internal::HighlightDefinition> definition(new TextEditor::Internal::HighlightDefinition);
	TextEditor::Internal::HighlightDefinitionHandlerV2 handler(definition);
	
	QXmlInputSource source(&definitionFile);
	QXmlSimpleReader reader;
	reader.setContentHandler(&handler);
	//m_isBuilding.insert(id);
	try {
		reader.parse(source);
	} catch (TextEditor::Internal::HighlighterException &) {
		definition.clear();
	}
	//m_isBuilding.remove(id);
	definitionFile.close();
	
//	m_definitions.insert(id, definition);
	return definition;
}
