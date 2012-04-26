#include <QMainWindow>

class QPlainTextEdit;
class QsvTextEdit;

class Demo4MainWindow: public QMainWindow {
	Q_OBJECT
protected:
	QsvTextEdit *textEditor;
/*
	Qate::MimeDatabase *mimes;
	Qate::HighlightDefinitionManager *hl_manager;
	TextEditor::Internal::Highlighter *highlighter;
	TextEditor::Internal::Context dummyContext;
	QSharedPointer<TextEditor::Internal::Context> pdummyContext;
	QSharedPointer<TextEditor::Internal::HighlightDefinition>  highlight_definition;
*/
	// this is a hack, otherwise the app crashes
	bool highlightReady;
public:
	Demo4MainWindow(QWidget *parent=NULL);
	void createMainGUI();

public slots:
	void onDefinitionsMetaDataReady();
	void onNew();
	void onOpen();
	bool onSave();
	void onQuit();
	void loadTextFile(QString fileName);
	//QString findDefinitionId(const Qate::MimeType &mimeType, bool considerParents) const;
	//Qate::MimeType getMimeByExt(const QString &fileName);
};
