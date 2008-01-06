#ifndef __SAMPLEPANEL_H__
#define __SAMPLEPANEL_H__

#if QT_VERSION >= 0x040400
#	define	QTextEditorControl	QPlainTextEdit
#	warning	"Using QPlainTextEdit as the text editor control"
#else
#	define	QTextEditorControl	QTextEdit
#endif

#include <QWidget>
class LinesEditor;
class QTextEdit;
class QTextEditorControl;

class SamplePanel : public QWidget
{
	Q_OBJECT
public:
	SamplePanel(LinesEditor *a);
private:
	void paintEvent(QPaintEvent*);
	QTextEditorControl *m_edit;
	QColor m_panelColor;
	QColor m_modifiedColor;
	QPixmap m_bookMarkImage;
friend class LinesEditor;
};

#endif // __SAMPLEPANEL_H__
