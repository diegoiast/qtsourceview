#ifndef QSVTEXTOPERATIONSWIDGET_H
#define QSVTEXTOPERATIONSWIDGET_H

#include <QObject>
#include <QTextCursor>
#include <QTextDocument>

class QTextDocument;

class QsvTextOperationsWidget : public QObject
{
	Q_OBJECT
public:
	QsvTextOperationsWidget( QWidget *parent );
	QFlags<QTextDocument::FindFlag> getSearchFlags();

public slots:
	void showSearch();
	void on_searchText_modified(QString s);

	QTextCursor getTextCursor();
	void setTextCursor(QTextCursor c);

private:
	bool issue_search( const QString &text, QTextCursor newCursor, QFlags<QTextDocument::FindFlag> findOptions  );

	QTextCursor m_searchCursor;
	QTextDocument *m_document;
	QWidget *m_search;
	QWidget *m_replace;
	QWidget *m_gotoLine;
};

#endif // QSVTEXTOPERATIONSWIDGET_H
