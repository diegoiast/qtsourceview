#ifndef QSVTEXTOPERATIONSWIDGET_H
#define QSVTEXTOPERATIONSWIDGET_H

#include <QObject>
#include <QTextCursor>
#include <QTextDocument>
#include <QColor>

class QTextDocument;
namespace Ui{
	class searchForm ;
}

class QsvTextOperationsWidget : public QObject
{
	Q_OBJECT
public:
	QsvTextOperationsWidget( QWidget *parent );
	void initSearchWidget();

	QFlags<QTextDocument::FindFlag> getSearchFlags();

	virtual QTextCursor getTextCursor();
	virtual void setTextCursor(QTextCursor c);

public slots:
	void showSearch();
	void on_searchText_modified(QString s);


private:
	bool issue_search( const QString &text, QTextCursor newCursor, QFlags<QTextDocument::FindFlag> findOptions  );

	QTextCursor m_searchCursor;
	QTextDocument *m_document;
	QWidget *m_search;
	QWidget *m_replace;
	QWidget *m_gotoLine;

	QColor searchFoundColor;
	QColor searchNotFoundColor;

	Ui::searchForm *searchFormUi;
};

#endif // QSVTEXTOPERATIONSWIDGET_H
