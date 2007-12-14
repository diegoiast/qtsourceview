// Qt includes
#include <QtPlugin>
#include <QTextCursor>

// HaiQ incluudes
#include <haiqcommon.h>

// QtSourceView inclues
#include "qsvlangdef.h"
#include "qsvlangdeffactory.h"
#include "qsvsyntaxhighlighter.h"
#include "lineseditor.h"
#include "editorconfig.h"

// plugin includes
#include "qsvhaiqeditorinterface.h"

QsvHaiqEditorInterface::QsvHaiqEditorInterface()
{
	m_editor = new LinesEditor;
	m_syntaxHighlighter	=	NULL;
	m_langDefinition	=	NULL;
	m_colorScheme		=	NULL;
}

QsvHaiqEditorInterface::~QsvHaiqEditorInterface()
{
	//delete m_editor;
	//don't delete it please :)
}

QWidget* QsvHaiqEditorInterface::editorWindow()
{
	return m_editor;
}

int QsvHaiqEditorInterface::currentLineNumber()
{
	QTextCursor c = m_editor->textCursor();
	return c.blockNumber();
}

int QsvHaiqEditorInterface::currentColumnNumber()
{
	QTextCursor c = m_editor->textCursor();
	return c.position() - c.block().position();
}

bool QsvHaiqEditorInterface::isModified()
{
	return m_editor->document()->isModified();
}

void QsvHaiqEditorInterface::setModified(bool val)
{
	return m_editor->document()->setModified(val);
}


void QsvHaiqEditorInterface::loadContent(const QString &fileName)
{
	m_editor->loadFile(fileName);
	
	// setup the syntax highlighter
	m_langDefinition = QsvLangDefFactory::getInstanse()->getHighlight(fileName);
	m_colorScheme = EditorConfig::getInstance()->getCurrentConfiguration().currentColorScheme;
	
	if (!m_syntaxHighlighter)
	{
		m_syntaxHighlighter = new QsvSyntaxHighlighter( m_editor, m_colorScheme, m_langDefinition );
		m_editor->setSyntaxHighlighter( m_syntaxHighlighter );
	}
	else
	{
		m_syntaxHighlighter->setHighlight( m_langDefinition );
		m_syntaxHighlighter->setColorsDef( m_colorScheme );
		m_syntaxHighlighter->rehighlight();
	}
}

void QsvHaiqEditorInterface::saveContent(const QString &fileName)
{
	write_text_file(fileName,m_editor->toPlainText());
}

QString QsvHaiqEditorInterface::path()
{
	return m_path;
}

void QsvHaiqEditorInterface::setPath(const QString &path_in)
{
	m_path = path_in;
}

QString QsvHaiqEditorInterface::text()
{
	return m_editor->toPlainText();
}

QString QsvHaiqEditorInterface::selectedText()
{
	QTextCursor c = m_editor->textCursor();
	return c.selectedText();
}

void QsvHaiqEditorInterface::ensureCursorVisible()
{
	m_editor->ensureCursorVisible();
}

QString QsvHaiqEditorInterface::currentLine()
{
	QTextCursor c = m_editor->textCursor();
	return c.block().text();
}

bool QsvHaiqEditorInterface::backspace()
{
	QTextCursor c = m_editor->textCursor();
	c.deletePreviousChar();
	m_editor->setTextCursor( c );
	
	// WTF?
	return true;
}

void QsvHaiqEditorInterface::deleteCharacter()
{
	QTextCursor c = m_editor->textCursor();
	c.deleteChar();
	m_editor->setTextCursor( c );
}

void QsvHaiqEditorInterface::removeSelectedText()
{
	// if has selection...?
	deleteCharacter();
}

void QsvHaiqEditorInterface::undo()
{
	m_editor->undo();
}

void QsvHaiqEditorInterface::redo()
{
	m_editor->redo();
}

void QsvHaiqEditorInterface::cut()
{
	m_editor->cut();
}

void QsvHaiqEditorInterface::copy()
{
	m_editor->copy();
}

void QsvHaiqEditorInterface::paste()
{
	m_editor->paste();
}

void QsvHaiqEditorInterface::selectAll()
{
	m_editor->selectAll();
}

void QsvHaiqEditorInterface::findInFile()
{
	m_editor->showFindWidget();
}

void QsvHaiqEditorInterface::findAgain()
{
	m_editor->findNext();
}

void QsvHaiqEditorInterface::replaceInFile()
{
	m_editor->showReplaceWidget();
}
