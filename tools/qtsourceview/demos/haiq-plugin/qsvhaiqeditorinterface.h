#ifndef __QSV_HAIQ_EDITORINTERFACE_H__
#define __QSV_HAIQ_EDITORINTERFACE_H__

#include <haiqeditorinterface.h>

class LinesEditor;
class QsvLangDef;

class QsvHaiqEditorInterface: public HaiQEditorInterface
{
private:
public:
	QsvHaiqEditorInterface();
	virtual ~QsvHaiqEditorInterface();
	
	virtual QWidget *editorWindow();
	virtual void loadContent(const QString &path);
	virtual void saveContent(const QString &path);
	virtual QString path();
	virtual void setPath(const QString &path_in);
	
// 	virtual int currentLineNumber();
// 	virtual int currentColumnNumber();
// 	virtual bool isModified();
// 	virtual void setModified(bool val);
	virtual QString text();
// 	virtual QString textForFind();
// 	virtual int replaceAll(const QString &str, const QString &str2);
// 	virtual QString selectedText();
// 	virtual void gotoLine(int line_number,bool highlight=false);
// 	virtual void ensureCursorVisible();
// 	virtual QString currentLine();
// 	virtual int position(int where=AT_CURSOR);
// 	virtual void setPosition(int pos);
// 	virtual void selectText(int startpos,int endpos);
// 	virtual bool backspace();
// 	virtual void deleteCharacter();
// 	virtual void removeSelectedText();
// 	virtual void insertText(const QString &txt);
// 	virtual QRect cursorRect();
	
	/*
	virtual void undo();
	virtual void redo();
	virtual void cut();
	virtual void copy();
	virtual void paste();
	virtual void selectAll();
	virtual void processKey(QKeyEvent event);
	virtual bool emitKeyOnlyMode();
	virtual void setEmitKeyOnlyMode(bool val);
	virtual void setTagsToHighlight(const QSet<QString> &tags);
	virtual void setHighlightingMode(bool val);
	virtual bool highlightingMode();
	virtual void setNumTabSpaces(int num);
	virtual void editSettings();
	
	virtual void setMarkers(QList<haiq_marker> markers);
	virtual QList<haiq_marker> markers();
	virtual void addMarker(const haiq_marker &marker);
	virtual void removeMarker(const QString & marker_id);
	virtual void removeMarkersOfType(int marker_type);
	virtual void toggle_marker(int marker_type);
signals:
	void cursorPositionChanged();
	void modificationChanged();
	void wordActivated();
	void keyPressed(const QKeyEvent &); 
	void contextMenu(const QPoint &);
	void closeMe();
	void pathChanged();
	void findInFile();
*/
private:
	QString		m_path;
	LinesEditor	*m_editor;
	QsvLangDef	*m_langDefinition;
};

#endif //__QSV_HAIQ_EDITORINTERFACE_H__
