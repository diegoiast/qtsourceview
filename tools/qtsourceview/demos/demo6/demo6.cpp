#include <QApplication>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QFile>
#include <QIODevice>

#include "context.h"
#include "highlighter.h"
#include "highlightdefinition.h"
#include "qate/highlightdefinitionmanager.h"
#include "qate/defaultcolors.h"

#define LANGUAGE  "/usr/share/kde4/apps/katepart/syntax/cpp.xml"
#define TEST_FILE "demos/demo6/demo6.cpp"

// fixme todo  ###
/// doxygen comment
/// \brief bla bla bala

void load_text(QString fe, QPlainTextEdit *te );
QSharedPointer<TextEditor::Internal::HighlightDefinition> get_highlighter_definition(QString definitionFileName);

int main( int argc, char* argv[] )
{
	QApplication app( argc, argv );
	QMainWindow    *main_window = new QMainWindow;
	QPlainTextEdit *text_editor;

	Qate::MimeDatabase                                        *mimes;
	Qate::HighlightDefinitionManager                          *hl_manager;
	TextEditor::Internal::Highlighter                         *highlight;
	QSharedPointer<TextEditor::Internal::HighlightDefinition>  highlight_definition;
	QSharedPointer<TextEditor::Internal::Context>              context;
	
	// create the main widget
	text_editor = new QPlainTextEdit(main_window);
	highlight   = new TextEditor::Internal::Highlighter(text_editor->document());
	Qate::DefaultColors::ApplyToHighlighter(highlight);
	text_editor->setFont( QFont("Courier new",10) );
	
	// create the highlighters manager
	hl_manager = Qate::HighlightDefinitionManager::instance();

	// here is where the magic starts:
	//
	// The first option is to ask the manager for a specific file:
	//
	//	highlight_definition = hl_manager->definition(LANGUAGE);
	//
	// This will load the forward declarations and includes, and will work perfectly.
	// The downside, is that you still need to know the exact path, and if your
	// definitions are spread in several dirs - this will be a pain.
	//
	// The recommended way is to ask for a definition by name, or by mimetype
	// by calling definitionIdByAnyMimeType(). This example uses the former.
	//
	// Notes: 
	//  - if the syntax you loaded contains several includes or forward
	//    and you do not see them, this means that 
	//    Qate::HighlightDefinitionManager::registerMimeTypes() failed
	//  - if your application needs a single definition, you can there is 
	//    not need to setup the mime database, nor register the mime types
	//    see the first example.
	
#if 0 
	highlight_definition = hl_manager->definition(LANGUAGE);
#else
	mimes = new Qate::MimeDatabase();
	hl_manager->setMimeDatabase(mimes);
	hl_manager->registerMimeTypes();
	
	// ugly - but, let the highlight manager build the mime DB
	// in  real life, you should wait for the signal definitionsMetaDataReady()
	sleep(1);  
	highlight_definition = hl_manager->definition( hl_manager->definitionIdByName("C++") );
#endif
	if (!highlight_definition.isNull()) {
		context = highlight_definition->initialContext();
	}
	highlight->setDefaultContext(context);

	load_text(TEST_FILE, text_editor);
	main_window->setWindowTitle("Kate syntax highter test");
	main_window->setCentralWidget(text_editor);
	main_window->show();
	return app.exec();
	
	Q_UNUSED(highlight);
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

