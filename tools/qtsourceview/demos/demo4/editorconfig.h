#ifndef __EDITORCONFIG_H__
#define __EDITORCONFIG_H__

#include <QObject>
#include <QList>
#include "ui_configdialog.h"
#include "qsvcolordeffactory.h"
#include "lineseditor.h"

class QFont;
class QString;
class QDialog;
class QsvColorDefFactory;
class QsvSyntaxHighlighter;

struct EditorConfigData
{
	bool			autoBrackets;
	bool			markCurrentLine;
	bool			showLineNumbers;
	bool			showWhiteSpaces;
	bool			matchBrackes;
	int			tabSize;
	QString			matchBrackesList;
	QFont			currentFont;
	QsvColorDefFactory	*currentColorScheme;	
};

class EditorConfig : public QObject
{
	Q_OBJECT
	
public:
	static EditorConfig *getInstance();
	static void applyConfiguration( EditorConfigData c, LinesEditor *editor );
	
	void showConfigDialog();
	void closeConfigDialog();
	void loadColorsDirectory( QString directory );
	
	EditorConfigData getCurrentConfiguration();
	EditorConfigData getDefaultConfiguration();
	EditorConfigData getUserConfiguration();
	void setConfiguration( EditorConfigData c );
	
	void applyCurrentConfiguration( LinesEditor *editor );
	void updateConfiguration();

public slots:
	void on_buttonBox_clicked( QAbstractButton * button );
	void on_btnChooseFont_clicked();
	void on_tabWidget_currentChanged(int index);

signals:
	void configurationModified();

private:
	EditorConfig();
	
	static EditorConfig *instance;
	QList<QsvColorDefFactory*> colorSchemes;
	QsvSyntaxHighlighter	*highlight;

	QDialog *dialog;
	Ui::ConfigDialog ui;
	EditorConfigData currentConfig;
};

#endif // __EDITORCONFIG_H__
