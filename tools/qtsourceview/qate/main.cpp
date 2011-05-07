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

#define LANGUAGE  "/usr/share/kde4/apps/katepart/syntax/sql.xml"
#define TEST_FILE "../tests/highlight.pas"
void load_text(QString fe, QPlainTextEdit *te );
QSharedPointer<TextEditor::Internal::HighlightDefinition> get_highlighter_definition(QString definitionFileName);

int main( int argc, char* argv[] )
{
	QApplication app( argc, argv );
	QString         definitionFileName = LANGUAGE;
	QSharedPointer<TextEditor::Internal::HighlightDefinition> def = get_highlighter_definition(definitionFileName);
	
	QMainWindow    *mw = new QMainWindow;
	QPlainTextEdit *te = new QPlainTextEdit(mw);
	TextEditor::Internal::Highlighter *hl = new TextEditor::Internal::Highlighter;
	Formats::ApplyToHighlighter(hl);

	te->setFont( QFont("Courier new",10) );
	hl->setParent(te);
	hl->setDocument(te->document());
	hl->setDefaultContext(def->initialContext());
	hl->rehighlight();

	load_text(TEST_FILE, te);
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
