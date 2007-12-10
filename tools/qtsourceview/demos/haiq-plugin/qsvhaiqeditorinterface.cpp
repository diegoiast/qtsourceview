#include <QtPlugin>
#include <haiqcommon.h>

// plugin includes
#include "qsvhaiqeditorinterface.h"

// library inclues
#include "lineseditor.h"
#include "qsvlangdeffactory.h"
#include "qsvsyntaxhighlighter.h"
#include "qsvlangdef.h"



QsvHaiqEditorInterface::QsvHaiqEditorInterface()
{
	m_editor		= new LinesEditor;
	m_langDefinition	= NULL;
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

void QsvHaiqEditorInterface::loadContent(const QString &path)
{
	m_langDefinition = QsvLangDefFactory::getInstanse()->getHighlight( path );
	m_editor->loadFile( path );
	//m_editor->getSyntaxHighlighter()->setHighlight( m_langDefinition );
	m_path = path;
}

void QsvHaiqEditorInterface::saveContent(const QString &path)
{
	write_text_file(path,m_editor->toPlainText());
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
