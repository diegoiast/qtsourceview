#ifndef __SAMPLEPANEL_H__
#define __SAMPLEPANEL_H__


#include <QWidget>
class QTextEdit;
class QPlainTextEdit;

class QsvTextEdit;

class QsvEditorPanel : public QWidget
{
	Q_OBJECT
public:
	QsvEditorPanel(QsvTextEdit *editor);
private:
	void paintEvent(QPaintEvent*);
	QsvTextEdit *m_edit;
	QColor m_panelColor;
	QColor m_modifiedColor;
	QPixmap m_bookMarkImage;
friend class QsvTextEdit;
};

#endif // __SAMPLEPANEL_H__
