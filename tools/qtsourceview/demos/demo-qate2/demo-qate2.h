#include <QObject>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QFile>
#include <QIODevice>
#include <QDir>
#include <QToolBar>

#include "context.h"
#include "highlighter.h"
#include "highlightdefinition.h"
#include "qate/highlightdefinitionmanager.h"
#include "qate/defaultcolors.h"

#ifdef __WIN32
#   include <windows.h>
#   define LANGUAGE  QDir::homePath() + "\\AppData\\Roaming\\Nokia\\qtcreator\\generic-highlighter\\cpp.xml"
#else
#   define LANGUAGE  "/usr/share/kde4/apps/katepart/syntax/cpp.xml"
#endif
#define TEST_FILE "demos/demo6/demo6.cpp"

/* some examples for different formats inside comments */
// fixme todo  ###
/// doxygen comment
/// \brief bla bla bala

void load_text(QString fe, QPlainTextEdit *te );
QSharedPointer<TextEditor::Internal::HighlightDefinition> get_highlighter_definition(QString definitionFileName);


class Demo2MainWindow: public QMainWindow {
	Q_OBJECT
protected:
	QPlainTextEdit *textEditor;
	
	Qate::MimeDatabase *mimes;
	Qate::HighlightDefinitionManager *hl_manager;
	TextEditor::Internal::Highlighter *highlighter;
	QSharedPointer<TextEditor::Internal::HighlightDefinition>  highlight_definition;
public:
	Demo2MainWindow(QWidget *parent=NULL) : QMainWindow(parent)
	{
		createMainGUI();
		
		mimes       = new Qate::MimeDatabase();
		highlighter = new TextEditor::Internal::Highlighter(textEditor->document());
		Qate::DefaultColors::ApplyToHighlighter(highlighter);
		hl_manager = Qate::HighlightDefinitionManager::instance();
		connect(hl_manager,SIGNAL(mimeTypesRegistered()), this, SLOT(onDefinitionsMetaDataReady()));
		hl_manager->setMimeDatabase(mimes);
		hl_manager->registerMimeTypes();
	}
	
	void createMainGUI()
	{
		QToolBar *b = addToolBar("main");
		QAction  *a;
		
		b->setMovable(false);
		a = b->addAction(tr("&New") , this, SLOT(onNew()));
		a = b->addAction(tr("&Open"), this, SLOT(onOpen()));
		a = b->addAction(tr("&Save"), this, SLOT(onSave()));
		b->addSeparator();
		a = b->addAction(tr("&Quit"));
		
		textEditor = new QPlainTextEdit;
		textEditor->setFont(QFont("Courier new",10));
		textEditor->setFrameStyle(QFrame::NoFrame);
		setCentralWidget(textEditor);
	}
	
public slots:
	void onDefinitionsMetaDataReady()
	{
		const char* FILE = "PHP (HTML)";
		highlight_definition = hl_manager->definition(hl_manager->definitionIdByName(FILE));
		if (highlight_definition.isNull()) {
			qDebug("No definition found for %s", FILE);
			return;
		}
		highlighter->setDefaultContext(highlight_definition->initialContext());
	}
	
	void onNew()
	{
		qDebug("New");
	}
	void onOpen()
	{
		qDebug("Open");
	}
	void onSave()
	{
		qDebug("Save");
	}
};
