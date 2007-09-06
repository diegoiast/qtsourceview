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
	setupUi(dialog);

	connect( buttonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(on_buttonBox_clicked(QAbstractButton*)));
	connect( btnChooseFont, SIGNAL(clicked()), this, SLOT(on_btnChooseFont_clicked()));
	connect( tabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_tabWidget_currentChanged(int)) );
	
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
		highlight = new QsvSyntaxHighlighter( sampleEdit, (colorSchemes[0]), langDefinition );		
	}
	
	setConfiguration( currentConfig );
//	QTimer::singleShot( 1000, sampleEdit,SLOT(adjustMarginWidgets()));
	dialog->show();
	sampleEdit->adjustMarginWidgets();
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

void EditorConfigData::getUserConfiguration()
{
	EditorConfigData c;
	c.autoBrackets		= cbAutoBrackets->isChecked();
	c.markCurrentLine	= cbMarkCurrentLine->isChecked();
	c.showLineNumbers	= cbShowLineNumbers->isChecked();
	c.showWhiteSpaces	= cbShowWhiteSpaces->isChecked();
	c.matchBrackes		= cbMatchBrackets->isChecked();
	c.matchBrackesList	= leMatchCraketsList->text();
	c.tabSize		= sbTabSize->value();
	c.currentFont		= labelFontPreview->font();
	
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
	cbAutoBrackets->setChecked( c.autoBrackets );
	cbMarkCurrentLine->setChecked( c.markCurrentLine );
	cbShowLineNumbers->setChecked( c.showLineNumbers );
	cbShowWhiteSpaces->setChecked( c.showWhiteSpaces );
	cbMatchBrackets->setChecked( c.matchBrackes );
	//cbMatchBrackets->setChecked ( c.tabSize	 );

	leMatchCraketsList->setText( c.matchBrackesList );
	sbTabSize->setValue( c.tabSize );
	labelFontPreview->setText( c.currentFont.toString() );
	labelFontPreview->setFont( c.currentFont );
	//c.currentFont		= QFont( DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE );
}

void EditorConfig::applyCurrentConfiguration( LinesEditor *editor ) 
{
	applyConfiguration( currentConfig, editor );
}

void EditorConfig::updateConfiguration()
{
	currentConfig.autoBrackets	= cbAutoBrackets->isChecked();
	currentConfig.markCurrentLine	= cbMarkCurrentLine->isChecked();
	currentConfig.showLineNumbers	= cbShowLineNumbers->isChecked();
	currentConfig.showWhiteSpaces	= cbShowWhiteSpaces->isChecked();
	currentConfig.matchBrackes	= cbMatchBrackets->isChecked();
	currentConfig.matchBrackesList	= leMatchCraketsList->text();
	currentConfig.tabSize		= sbTabSize->value();
	currentConfig.currentFont	= labelFontPreview->font();

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
	
	if (b == buttonBox->button(QDialogButtonBox::Ok))
	{
		// set the configuration internally and emit signal
		updateConfiguration();
		emit( configurationModified() );
		dialog->close();		
	} 
	else if (b == buttonBox->button(QDialogButtonBox::Apply))
	{
		// set the configuration internally and emit signal
		updateConfiguration();
		applyCurrentConfiguration( sampleEdit );
		emit( configurationModified() );
	}
	else if (b == buttonBox->button(QDialogButtonBox::Cancel))
	{
		// lets abort
		dialog->close();
	}
	else if (b == buttonBox->button(QDialogButtonBox::RestoreDefaults))
	{
		// restore default values
		setConfiguration( getDefaultConfiguration() );
	}
}

void EditorConfig::on_btnChooseFont_clicked()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok, labelFontPreview->font(), dialog );
	if (!ok)
		return;
	labelFontPreview->setText( font.toString() );
	labelFontPreview->setFont( font );
}

void EditorConfig::on_tabWidget_currentChanged(int index)
{
	// TODO why does the editor not show the lines widget 
	// when not applying configuration...?
	if (index == 1)
	{
		//applyCurrentConfiguration( sampleEdit );	
		applyConfiguration( getCurrentConfiguration(), sampleEdit );
	}

}
