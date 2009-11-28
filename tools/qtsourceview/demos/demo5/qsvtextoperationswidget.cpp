#include "qsvtextoperationswidget.h"

#include <QLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QTextDocument>

#include <QDebug>

QsvTextOperationsWidget::QsvTextOperationsWidget( QWidget *parent )
	: QObject(parent)
{
	m_gotoLine = NULL;
	m_search   = NULL;
	m_replace  = NULL;
	m_document = NULL;

	QTextEdit *t = qobject_cast<QTextEdit*>(parent);
	if (t) {
		m_document = t->document();
	}
	else {
		QPlainTextEdit *pt = qobject_cast<QPlainTextEdit*>(parent);
		if (pt) {
			m_document = pt->document();
		}
	}

	m_search = new QWidget(parent);
	QHBoxLayout *layout   = new QHBoxLayout(m_search);
	QLabel      *label    = new QLabel(m_search);
	QLineEdit   *lineEdit = new QLineEdit(m_search);
	QPushButton *button   = new QPushButton(m_search);

	label->setObjectName("searchTextLabel");
	label->setText(tr("Search"));
	lineEdit->setObjectName("searchText");
	button->setObjectName("searchButton");
	button->setText(tr("Close"));
	connect(lineEdit,SIGNAL(textChanged(QString)),this,SLOT(on_searchText_modified(QString)));

	layout->addWidget(label);
	layout->addWidget(lineEdit);
	layout->addWidget(button);
	m_search->hide();
	m_search->setAutoFillBackground(true);
	m_search->setLayout(layout);
}

void QsvTextOperationsWidget::showSearch()
{
	m_search->setVisible(!m_search->isVisible());
	if (!m_search->isVisible())
		return;
	QWidget *parent = qobject_cast<QWidget*>(this->parent());

	QLineEdit *lineEdit = m_search->findChild<QLineEdit*>("searchText");
	if (!lineEdit) {
		// something here is fucked up, really nice
		return;
	}
	lineEdit->setFocus();
	m_searchCursor = getTextCursor();

	QRect r = parent->rect();
	r.adjust( 10, 0, -10, 0 );
	r.setHeight( m_search->height() );
	r.moveBottom( parent->rect().height() - 10 );
	m_search->setGeometry( r );
}

void QsvTextOperationsWidget::on_searchText_modified(QString s)
{
	issue_search( s, m_searchCursor, getSearchFlags() );
}

QTextCursor	QsvTextOperationsWidget::getTextCursor()
{
	QTextCursor searchCursor;
	QTextEdit *t = qobject_cast<QTextEdit*>(parent());
	if (t) {
		searchCursor = t->textCursor();
	} else {
		QPlainTextEdit *pt = qobject_cast<QPlainTextEdit*>(parent());
		if (pt) {
			searchCursor = pt->textCursor();
		}
	}
	return searchCursor;
}

void	QsvTextOperationsWidget::setTextCursor(QTextCursor c)
{
	QTextCursor searchCursor;
	QTextEdit *t = qobject_cast<QTextEdit*>(parent());
	if (t) {
		t->setTextCursor(c);
	} else {
		QPlainTextEdit *pt = qobject_cast<QPlainTextEdit*>(parent());
		if (pt) {
			pt->setTextCursor(c);
		}
	}
}

bool	QsvTextOperationsWidget::issue_search( const QString &text, QTextCursor newCursor, QFlags<QTextDocument::FindFlag> findOptions  )
{
	QTextCursor c = m_document->find( text, newCursor, findOptions );
	bool found = ! c.isNull();
	QLineEdit *lineEdit = m_search->findChild<QLineEdit*>("searchText");
	if (!lineEdit) {
		// something here is fucked up, really nice
		return false;
	}

	if (!found) {
		//lets try again, from the start
		c.movePosition(findOptions.testFlag(QTextDocument::FindBackward)? QTextCursor::End : QTextCursor::Start);
		c = m_document->find( lineEdit->text(), c, findOptions );
		found = ! c.isNull();
	}

	if (found) {
//		QPalette ok = this->palette();
//		ok.setColor( QPalette::Base, searchFoundColor );
//		ui_findWidget.searchText->setPalette( ok );
		setTextCursor( c );
	} else 	{
//		QPalette bad = this->palette();
//		bad.setColor( QPalette::Base, searchNotFoundColor );
//		ui_findWidget.searchText->setPalette( bad );
		setTextCursor( m_searchCursor );
	}
	return found;
}


QFlags<QTextDocument::FindFlag> QsvTextOperationsWidget::getSearchFlags()
{
	QFlags<QTextDocument::FindFlag> f;

	/*if (ui_findWidget.caseSensitive->isChecked())
		f = f | QTextDocument::FindCaseSensitively;

	if (ui_findWidget.wholeWords->isChecked())
		f = f | QTextDocument::FindWholeWords;
*/
	return f;
}
