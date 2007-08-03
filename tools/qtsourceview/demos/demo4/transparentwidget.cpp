#include <QPainter>
#include "transparentwidget.h"


TransparentWidget::TransparentWidget( QWidget *parent, qreal t )
:QWidget(parent)
{
	widgetTransparency = t;
}

qreal TransparentWidget::getWidgetTransparency()
{
	return widgetTransparency;
}

void TransparentWidget::setWidgetTransparency( qreal t )
{
	widgetTransparency = t;
	update();
}

void TransparentWidget::paintEvent(QPaintEvent*)
{
	QPainter p( this );
	p.setOpacity( widgetTransparency );
	p.fillRect(rect(), palette().background() );
}
