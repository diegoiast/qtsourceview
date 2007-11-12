#ifndef __SAMPLEPANEL_H__
#define __SAMPLEPANEL_H__

#include <QWidget>
class LinesEditor;
class QTextEdit;

class SamplePanel : public QWidget
{
	Q_OBJECT
public:
	SamplePanel(LinesEditor *a);
private:
	void paintEvent(QPaintEvent*);
	QTextEdit *edit;
	QColor panelColor;
	QPixmap bookMarkImage;
friend class LinesEditor;
};

#endif // __SAMPLEPANEL_H__
