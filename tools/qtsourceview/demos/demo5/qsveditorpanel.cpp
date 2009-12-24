#include <QPlainTextEdit>

#include <QPainter>
#include <QPaintEvent>
#include <QTextBlock>
#include <QTextLayout>
#include <QScrollBar>

#include "qsvprivateblockdata.h"
#include "qsveditorpanel.h"
#include "qsvtextedit.h"

QsvEditorPanel::QsvEditorPanel(QsvTextEdit *a): QWidget(a)//, m_area(a)
{
	m_edit = a;
	m_modifiedColor = Qt::green;
	m_panelColor = QColor("#FFFFD0");
	m_bookMarkImage = QPixmap(":/images/emblem-important.png");

	setFixedWidth(50);
	connect(m_edit->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(update()));
}

void QsvEditorPanel::paintEvent(QPaintEvent*e)
{
	m_edit->paintPanel(e);
}
