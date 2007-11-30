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
class ColorsModel;

struct EditorConfigData
{
	bool			autoBrackets;
	bool			markCurrentLine;
	bool			showLineNumbers;
	bool			showWhiteSpaces;
	bool			showMargins;
	bool			matchBrackes;
	bool			lineWrapping;
	int			tabSize;
	int			marginsWidth;
	QString			matchBrackesList;
	QFont			currentFont;
	QsvColorDefFactory	*currentColorScheme;
};

class EditorConfig : public QObject
{
	Q_OBJECT
	
	EditorConfig();
	
public:
	static EditorConfig *getInstance();
		
	void showConfigDialog();
	void closeConfigDialog();
	void loadColorsDirectory( QString directory );
	
	// gets the currently saved configuration
	// does not modify object status
	EditorConfigData getCurrentConfiguration();
	
	// gets a default set of configuration
	// does not modify object status
	EditorConfigData getDefaultConfiguration();
	
	// gets the configuration which the user is working on, 
	// by reading from the gui
	// does not modify object status
	EditorConfigData getUserConfiguration();
	
	// sets the current configuration to c
	// does not modify the GUI
	void setConfiguration( EditorConfigData c );
	
	// enforce a configuration set on an editor
	// modifies object status
	static void applyConfiguration( EditorConfigData c, LinesEditor *editor );

	// updates the GUI to refelct the new configuration
	void updateConfiguration();

public slots:
	void on_buttonBox_clicked( QAbstractButton * button );
	void on_btnChooseFont_clicked();
	void on_tabWidget_currentChanged(int index);
	void on_colorsCombo_currentIndexChanged(int index);

signals:
	void configurationModified();

private:
	static EditorConfig *instance;
	QList<QsvColorDefFactory*> colorSchemes;
	//QsvSyntaxHighlighter	*highlight;

	QDialog *dialog;
	ColorsModel *colorsModel;
	Ui::ConfigDialog ui;
	EditorConfigData currentConfig;
};

#endif // __EDITORCONFIG_H__
