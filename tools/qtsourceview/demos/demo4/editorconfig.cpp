//#include <QFont>
#include <QTimer>
#include <QDir>
#include <QDialog>
#include <QPushButton>
#include <QFontDialog>

#include "editorconfig.h"
#include "qsvlangdef.h"
#include "qsvlangdeffactory.h"
#include "qsvsyntaxhighlighter.h"

#include <QDebug>

EditorConfig * EditorConfig::instance = NULL;

EditorConfig *EditorConfig::getInstance()
{
	if (EditorConfig::instance == NULL)
		EditorConfig::instance = new EditorConfig;
		
	return EditorConfig::instance ;
}

void EditorConfig::applyConfiguration( EditorConfigData c, LinesEditor *editor )
{
	qDebug("applying configuration");
	//textEdit->setAutoBrackets( c.autoBrackets );
	editor->setDisplayCurrentLine( c.markCurrentLine );
	//currentConfig.showLineNumbers
	editor->getPanel()->setVisible( c.showLineNumbers );
	editor->setDisplayWhiteSpaces( c.showWhiteSpaces );
	editor->setDisplatMatchingBrackets( c.matchBrackes );
	editor->setMatchingString( c.matchBrackesList );
	
	//currentConfig.tabSize		= sbTabSize->value();
	editor->document()->setDefaultFont( c.currentFont );
	editor->getPanel()->setFont( c.currentFont );
	
	//currentConfig.currentColorScheme= colorSchemes[0];

	editor->adjustMarginWidgets();
	editor->update();
	editor->viewport()->update();	
}
	
EditorConfig::EditorConfig()
{
	dialog = new QDialog;
	ui.setupUi(dialog);

	connect( ui.buttonBox	, SIGNAL(clicked(QAbstractButton *))	, this, SLOT(on_buttonBox_clicked(QAbstractButton*)));
	connect( ui.btnChooseFont	, SIGNAL(clicked())			, this, SLOT(on_btnChooseFont_clicked()));
	connect( ui.tabWidget	, SIGNAL(currentChanged(int))		, this, SLOT(on_tabWidget_currentChanged(int)));
	
	// set configuration will be done on display	
	currentConfig = getDefaultConfiguration();
	highlight = NULL;
}

void EditorConfig::showConfigDialog()
{
	// the construction of the syntax highlighter must be postponded
	// to the last possible moment - so the programmer
	// will ask to load the colors directory
	if (!highlight)
	{
		QsvLangDef *langDefinition = QsvLangDefFactory::getInstanse()->getHighlight("1.cpp");
		//QsvColorDefFactory defColors = colorSchemes[0];
		highlight = new QsvSyntaxHighlighter( ui.sampleEdit, (colorSchemes[0]), langDefinition );		
	}
	
	setConfiguration( currentConfig );
//	QTimer::singleShot( 1000, sampleEdit,SLOT(adjustMarginWidgets()));
	dialog->show();
	ui.sampleEdit->adjustMarginWidgets();
}

void EditorConfig::closeConfigDialog()
{
	// this is treated as "abort"
	dialog->close();
}


void EditorConfig::loadColorsDirectory( QString directory )
{
	if (directory.isEmpty())
		directory = QDir::currentPath();
	QDir dir(directory, "*.xml");

	QStringList files = dir.entryList(QDir::Files | QDir::NoSymLinks);
	int fileCount =	files.count();

	if (fileCount == 0)
	{
		qDebug( "%s %d - warning: no color definitions found at directory: %s", __FILE__, __LINE__, qPrintable(directory) );
		return;
	}

	for (int i = 0; i < fileCount; ++i)
	{
		QsvColorDefFactory *c = new QsvColorDefFactory( directory + "/" + files[i] );
		colorSchemes << c;
	}
}

#ifdef WIN32
#	define DEFAULT_FONT_NAME "Courier New"
#	define DEFAULT_FONT_SIZE 10
#else
#	define DEFAULT_FONT_NAME "Monospace"
#	define DEFAULT_FONT_SIZE 10
#endif

EditorConfigData EditorConfig::getCurrentConfiguration()
{
	return currentConfig;
}

EditorConfigData  EditorConfig::getDefaultConfiguration()
{
	EditorConfigData  c;
	c.autoBrackets		= true;
	c.markCurrentLine	= true;
	c.showLineNumbers	= true;
	c.showWhiteSpaces	= true;
	c.matchBrackes		= true;
	c.tabSize		= 8;
	c.matchBrackesList	= "()[]{}";
	c.currentFont		= QFont( DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE );
	if (colorSchemes.isEmpty())
		currentConfig.currentColorScheme = NULL;
	else		
		currentConfig.currentColorScheme = colorSchemes[0];
	
	return c;
}

EditorConfigData EditorConfig::getUserConfiguration()
{
	EditorConfigData c;
	c.autoBrackets		= ui.cbAutoBrackets->isChecked();
	c.markCurrentLine	= ui.cbMarkCurrentLine->isChecked();
	c.showLineNumbers	= ui.cbShowLineNumbers->isChecked();
	c.showWhiteSpaces	= ui.cbShowWhiteSpaces->isChecked();
	c.matchBrackes		= ui.cbMatchBrackets->isChecked();
	c.matchBrackesList	= ui.leMatchCraketsList->text();
	c.tabSize		= ui.sbTabSize->value();
	c.currentFont		= ui.labelFontPreview->font();
	
	c.tabSize		= 8;
	c.matchBrackesList	= "()[]{}";
	c.currentFont		= QFont( DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE );
	if (colorSchemes.isEmpty())
		currentConfig.currentColorScheme = NULL;
	else		
		currentConfig.currentColorScheme = colorSchemes[0];
	
	return c;
}

void EditorConfig::setConfiguration( EditorConfigData c  )
{
	ui.cbAutoBrackets->setChecked( c.autoBrackets );
	ui.cbMarkCurrentLine->setChecked( c.markCurrentLine );
	ui.cbShowLineNumbers->setChecked( c.showLineNumbers );
	ui.cbShowWhiteSpaces->setChecked( c.showWhiteSpaces );
	ui.cbMatchBrackets->setChecked( c.matchBrackes );
	//ui.cbMatchBrackets->setChecked ( c.tabSize	 );

	ui.leMatchCraketsList->setText( c.matchBrackesList );
	ui.sbTabSize->setValue( c.tabSize );
	ui.labelFontPreview->setText( c.currentFont.toString() );
	ui.labelFontPreview->setFont( c.currentFont );
	//ui.c.currentFont		= QFont( DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE );
}

void EditorConfig::applyCurrentConfiguration( LinesEditor *editor ) 
{
	applyConfiguration( currentConfig, editor );
}

void EditorConfig::updateConfiguration()
{
	currentConfig.autoBrackets	= ui.cbAutoBrackets->isChecked();
	currentConfig.markCurrentLine	= ui.cbMarkCurrentLine->isChecked();
	currentConfig.showLineNumbers	= ui.cbShowLineNumbers->isChecked();
	currentConfig.showWhiteSpaces	= ui.cbShowWhiteSpaces->isChecked();
	currentConfig.matchBrackes	= ui.cbMatchBrackets->isChecked();
	currentConfig.matchBrackesList	= ui.leMatchCraketsList->text();
	currentConfig.tabSize		= ui.sbTabSize->value();
	currentConfig.currentFont	= ui.labelFontPreview->font();

	// TODO
	if (colorSchemes.isEmpty())
		currentConfig.currentColorScheme = NULL;
	else		
		currentConfig.currentColorScheme = colorSchemes[0];
}

void EditorConfig::on_buttonBox_clicked( QAbstractButton * button )
{
	QPushButton  *b = qobject_cast<QPushButton*>(button);
	
	if (!b)
	{
		// this should not happen
		qDebug( "%s %d something funny is happenning", __FILE__, __LINE__ );
		return;
	}
	
	if (b == ui.buttonBox->button(QDialogButtonBox::Ok))
	{
		// set the configuration internally and emit signal
		updateConfiguration();
		emit( configurationModified() );
		dialog->close();		
	} 
	else if (b == ui.buttonBox->button(QDialogButtonBox::Apply))
	{
		// set the configuration internally and emit signal
		updateConfiguration();
		applyCurrentConfiguration( ui.sampleEdit );
		emit( configurationModified() );
	}
	else if (b == ui.buttonBox->button(QDialogButtonBox::Cancel))
	{
		// lets abort
		dialog->close();
	}
	else if (b == ui.buttonBox->button(QDialogButtonBox::RestoreDefaults))
	{
		// restore default values
		setConfiguration( getDefaultConfiguration() );
	}
}

void EditorConfig::on_btnChooseFont_clicked()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok, ui.labelFontPreview->font(), dialog );
	if (!ok)
		return;
	ui.labelFontPreview->setText( font.toString() );
	ui.labelFontPreview->setFont( font );
}

void EditorConfig::on_tabWidget_currentChanged(int index)
{
	qDebug("tab changed - %d", index);
	// TODO why does the editor not show the lines widget 
	// when not applying configuration...?
	if (index == 1)
	{
		applyConfiguration( getCurrentConfiguration(), ui.sampleEdit );
	}
}
