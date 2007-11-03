#include <QPainter>
#include <QPaintEvent>
#include <QTextEdit>
#include <QTextBlock>
#include <QTextLayout>
#include <QScrollBar>

#include "samplepanel.h"
#include "lineseditor.h"

SamplePanel::SamplePanel(LinesEditor *a): QWidget(a)//, m_area(a)
{
	edit = a;
	setFixedWidth(50);

	connect(edit->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(update()));
}
	
void SamplePanel::paintEvent(QPaintEvent*)
{
	QPainter p( this );
	QRect r = geometry();
	
	p.fillRect( r, panelColor );
	
	int contentsY = edit->verticalScrollBar()->value();
	qreal pageBottom = contentsY + edit->viewport()->height();
	int m_lineNumber = 1;
	const QFontMetrics fm = fontMetrics();
	const int ascent = fontMetrics().ascent() +1;
	
	for ( QTextBlock block = edit->document()->begin(); block.isValid(); block = block.next(), m_lineNumber++ )
	{
		QTextLayout* layout = block.layout();
		const QRectF boundingRect = layout->boundingRect();
		QPointF position = layout->position();
		if ( position.y() +boundingRect.height() < contentsY )
			continue;
		if ( position.y() > pageBottom )
			break;
		
		const QString txt = QString::number( m_lineNumber );
		
		p.drawText( width() -fm.width( txt ) - 2, qRound( position.y() ) -contentsY +ascent, txt ); // -fm.width( "0" ) is an ampty place/indent 
	}
}
