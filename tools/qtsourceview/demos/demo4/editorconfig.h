#ifndef __EDITORCONFIG_H__
#define __EDITORCONFIG_H__

#include <QObject>
#include <QList>
#include "ui_configdialog.h"
#include "qsvcolordeffactory.h"

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

class EditorConfig : public QObject, public Ui::ConfigDialog
{
	Q_OBJECT
	
public:
	static EditorConfig *getInstance();
	void showConfigDialog();
	void loadColorsDirectory( QString directory );
	EditorConfigData getDefaultConfiguration();
	void setConfiguration( EditorConfigData c );
	
public slots:
	void on_buttonBox_clicked( QAbstractButton * button );
	void on_btnChooseFont_clicked();
	
private:
	EditorConfig();
	
	static EditorConfig *instance;
	QList<QsvColorDefFactory*> colorSchemes;
	QsvSyntaxHighlighter	*highlight;
	QDialog *dialog;
	EditorConfigData currentConfig;
};

#endif // __EDITORCONFIG_H__
