#ifndef __TRANSPARENTWIDGET_H__
#define __TRANSPARENTWIDGET_H__

#include <QWidget>

class TransparentWidget: public QWidget
{
public:
	TransparentWidget( QWidget *parent, qreal t=1 );
	qreal getWidgetTransparency();
	void setWidgetTransparency( qreal );
	
protected:
	void paintEvent(QPaintEvent*);
	
private:
	qreal widgetTransparency;
};

#endif // __TRANSPARENTWIDGET_H__
