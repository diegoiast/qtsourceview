#include <QtPlugin>

#include "qsvhaiqeditorinterface.h"
#include "qsvlangdeffactory.h"
#include "qsvsyntaxhighlighter.h"

#include "lineseditor.h"
#include "qsvlangdef.h"

// Q_EXPORT_PLUGIN2(qsvhaiqeditorinterface,QsvHaiqEditorInterface)


QsvHaiqEditorInterface::QsvHaiqEditorInterface()
{
	m_editor	= new LinesEditor;
	m_langDefinition	= NULL;
}

QsvHaiqEditorInterface::~QsvHaiqEditorInterface()
{
	delete m_editor;
}

QWidget* QsvHaiqEditorInterface::editorWindow()
{
	return m_editor;
}

void QsvHaiqEditorInterface::loadContent(const QString &path)
{
	m_langDefinition = QsvLangDefFactory::getInstanse()->getHighlight( path );
	m_editor->loadFile( path );
	m_editor->getSyntaxHighlighter()->setHighlight( m_langDefinition );

}

void QsvHaiqEditorInterface::saveContent(const QString &path)
{
}

QString QsvHaiqEditorInterface::path()
{
}

void QsvHaiqEditorInterface::setPath(const QString &path_in)
{
}

