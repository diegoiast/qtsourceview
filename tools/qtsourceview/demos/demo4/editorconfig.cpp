#include <QTimer>
#include <QDir>
#include <QDialog>
#include <QPushButton>
#include <QFontDialog>

#include "editorconfig.h"
#include "colorsmodel.h"
#include "qsvlangdef.h"
#include "qsvcolordef.h"
#include "qsvlangdeffactory.h"
#include "qsvsyntaxhighlighter.h"

#include <QDebug>

#ifdef WIN32
#	define DEFAULT_FONT_NAME "Courier New"
#	define DEFAULT_FONT_SIZE 10
#else
#	define DEFAULT_FONT_NAME "Monospace"
#	define DEFAULT_FONT_SIZE 10
#endif

EditorConfig * EditorConfig::instance = NULL;

EditorConfig::EditorConfig()
{
	dialog = new QDialog;
	ui.setupUi(dialog);

	// set configuration will be done on display	
	highlight = NULL;
	colorsModel = NULL;
	currentConfig = getDefaultConfiguration();	

	connect( ui.buttonBox	, SIGNAL(clicked(QAbstractButton *))	, this, SLOT(on_buttonBox_clicked(QAbstractButton*)));
	connect( ui.btnChooseFont, SIGNAL(clicked())			, this, SLOT(on_btnChooseFont_clicked()));
	connect( ui.tabWidget	, SIGNAL(currentChanged(int))		, this, SLOT(on_tabWidget_currentChanged(int)));
	connect( ui.colorsCombo	, SIGNAL(currentIndexChanged(int))	, this, SLOT(on_colorsCombo_currentIndexChanged(int)));
}

EditorConfig *EditorConfig::getInstance()
{
	if (EditorConfig::instance == NULL)
		EditorConfig::instance = new EditorConfig;
		
	return EditorConfig::instance;
}

void EditorConfig::showConfigDialog()
{
	// the construction of the syntax highlighter must be postponded
	// to the last possible moment - so the programmer
	// will be able to load the colors directory manually
	if (!highlight)
	{
		QsvLangDef *langDefinition = QsvLangDefFactory::getInstanse()->getHighlight("1.cpp");
		highlight = new QsvSyntaxHighlighter( ui.sampleEdit, colorSchemes[0], langDefinition );		
	}
	
	dialog->show();
	setConfiguration( currentConfig );
	updateConfiguration();
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
	
	delete colorsModel;
	
	// TODO: how to clean the model...?
	//ui.comboBox->setModel( 0 );
	colorsModel = new ColorsModel( &colorSchemes, this );
	ui.colorsCombo->setModel( colorsModel );
}


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
		c.currentColorScheme = NULL;
	else
		c.currentColorScheme = colorSchemes[0];
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
		c.currentColorScheme = NULL;
	else		
		c.currentColorScheme = colorSchemes[ ui.colorsCombo->currentIndex() ];
	
	return c;
}

void EditorConfig::setConfiguration( EditorConfigData c )
{
	currentConfig = c;
}

void EditorConfig::applyConfiguration( EditorConfigData c, LinesEditor *editor )
{
	//textEdit->setAutoBrackets( c.autoBrackets );
	editor->setDisplayCurrentLine( c.markCurrentLine );
	editor->getPanel()->setVisible( c.showLineNumbers );
	editor->setDisplayWhiteSpaces( c.showWhiteSpaces );
	editor->setDisplatMatchingBrackets( c.matchBrackes );
	editor->setMatchingString( c.matchBrackesList );
	
	//currentConfig.tabSize         = sbTabSize->value();
	editor->document()->setDefaultFont( c.currentFont );
	editor->getPanel()->setFont( c.currentFont );
	
	if (c.currentColorScheme == NULL )
		qDebug("%s %d - Warning - no color scheme found!", __FILE__, __LINE__ );
	else
	{
		QPalette p( editor->palette() );
		p.setColor( QPalette::Base, c.currentColorScheme->getColorDef("dsWidgetBackground").getBackground() );
		editor->setPalette( p );
		editor->setTextColor( c.currentColorScheme->getColorDef("dsNormal").getColor() );
		editor->setItemColor( LinesPanel, c.currentColorScheme->getColorDef("dsWidgetLinesPanel").getBackground() );
		editor->setItemColor( CurrentLine, c.currentColorScheme->getColorDef("dsWidgetCurLine").getBackground() );
		
		// TODO
		// how about syntax highlighter?
	}
	
	editor->adjustMarginWidgets();
	editor->update();
	editor->viewport()->update();
}

void EditorConfig::updateConfiguration()
{
	// set the values on the first tab
	ui.cbAutoBrackets->setChecked( currentConfig.autoBrackets );
	ui.cbMarkCurrentLine->setChecked( currentConfig.markCurrentLine );
	ui.cbShowLineNumbers->setChecked( currentConfig.showLineNumbers );
	ui.cbShowWhiteSpaces->setChecked( currentConfig.showWhiteSpaces );
	ui.cbMatchBrackets->setChecked( currentConfig.matchBrackes );
	ui.cbMatchBrackets->setChecked ( currentConfig.tabSize	 );

	ui.leMatchCraketsList->setText( currentConfig.matchBrackesList );
	ui.sbTabSize->setValue( currentConfig.tabSize );
	ui.labelFontPreview->setText( currentConfig.currentFont.toString() );
	ui.labelFontPreview->setFont( currentConfig.currentFont );

	// TODO set the color configuration combo box	
	if (currentConfig.currentColorScheme)
	{
		highlight->setColorsDef( currentConfig.currentColorScheme );
		highlight->rehighlight();
	}
	applyConfiguration( currentConfig, ui.sampleEdit );
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
		//updateConfiguration();
		emit( configurationModified() );
		dialog->close();
	} 
	else if (b == ui.buttonBox->button(QDialogButtonBox::Apply))
	{
		// set the configuration internally and emit signal
		//updateConfiguration();
		//applyCurrentConfiguration( ui.sampleEdit );
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
		updateConfiguration();
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
	if (index != 1)
		return;

	EditorConfigData c = getUserConfiguration();
	applyConfiguration( c , ui.sampleEdit );
}

void EditorConfig::on_colorsCombo_currentIndexChanged( int index )
{
	
	EditorConfigData c = getUserConfiguration();

	applyConfiguration( c , ui.sampleEdit );	
	if (highlight)
	{
		highlight->setColorsDef( c.currentColorScheme );
		highlight->rehighlight();
	}

	// remove gcc warnings
	(void)(index);
}
