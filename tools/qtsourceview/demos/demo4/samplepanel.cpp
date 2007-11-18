#include <QPainter>
#include <QPaintEvent>
#include <QTextEdit>
#include <QTextBlock>
#include <QTextLayout>
#include <QScrollBar>

#include "privateblockdata.h"
#include "samplepanel.h"
#include "lineseditor.h"

SamplePanel::SamplePanel(LinesEditor *a): QWidget(a)//, m_area(a)
{
	edit = a;
	setFixedWidth(50);

	bookMarkImage = QPixmap(":/images/bookmark.png");
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
	QTextBlock currentBlock= edit->textCursor().block();
	
	for ( QTextBlock block = edit->document()->begin(); block.isValid(); block = block.next(), m_lineNumber++ )
	{
		QTextLayout* layout = block.layout();
		const QRectF boundingRect = layout->boundingRect();
		QPointF position = layout->position();
		PrivateBlockData *data = dynamic_cast<PrivateBlockData*>( block.userData() );

		if ( position.y() +boundingRect.height() < contentsY )
			continue;
		if ( position.y() > pageBottom )
			break;
		
		const QString txt = QString::number( m_lineNumber );
		
		if (block == currentBlock)
		{
			QFont f = p.font();
			f.setBold( true );
			p.setFont( f );
		}
		p.drawText( width() -fm.width( txt ) - 7, qRound( position.y() ) -contentsY +ascent, txt ); // -fm.width( "0" ) is an ampty place/indent 
		if (block == currentBlock)
		{
			QFont f = p.font();
			f.setBold( false );
			p.setFont( f );
		}
		
		if (data)
		{
			if (data->m_isBookmark)
				p.drawPixmap( 2, qRound(position.y() -contentsY +ascent - bookMarkImage.height()), bookMarkImage ); 
				
			if (data->m_isModified)
				p.fillRect( width()- 3, qRound(position.y()-contentsY), 2, qRound(boundingRect.height()), Qt::green );
		}
	}
}
