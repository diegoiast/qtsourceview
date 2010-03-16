#ifndef QSVTEXTOPERATIONSWIDGET_H
#define QSVTEXTOPERATIONSWIDGET_H

// this is done to shut up warnings inside Qt Creator
// if you remove it, the whole project gets marked with warnings
// as it thinks QObject is not defined. WTF.
class QObject;
class QString;
class QTextCursor;

#include <QObject>
#include <QString>
#include <QTextCursor>
#include <QTextDocument>
#include <QColor>

class QWidget;
class QTextDocument;
class QLineEdit;

namespace Ui{
	class searchForm;
	class replaceForm;
}

class QsvTextEdit;

class QsvTextOperationsWidget : public QObject
{
	Q_OBJECT
	friend class QsvTextEdit;
public:
	QsvTextOperationsWidget( QWidget *parent );
	void initSearchWidget();
	void initReplaceWidget();

	QFlags<QTextDocument::FindFlag> getSearchFlags();

	virtual QTextCursor getTextCursor();
	virtual void setTextCursor(QTextCursor c);

public slots:
	void showSearch();
	void showReplace();
	void showBottomWidget(QWidget* w=NULL);
	void on_searchText_modified(QString s);
	void on_replaceText_modified(QString s);
	void searchNext();
	void searchPrevious();
	void searchPrev(){ searchPrevious(); };
	void adjustBottomWidget();

protected:
	bool eventFilter(QObject *obj, QEvent *event);

private:
	bool issue_search( const QString &text, QTextCursor newCursor, QFlags<QTextDocument::FindFlag> findOptions, QLineEdit *l );

	QTextCursor m_searchCursor;
	QTextDocument *m_document;
	QWidget *m_search;
	QWidget *m_replace;
	QWidget *m_gotoLine;

	QColor searchFoundColor;
	QColor searchNotFoundColor;

	Ui::searchForm *searchFormUi;
	Ui::replaceForm *replaceFormUi;
};

#endif // QSVTEXTOPERATIONSWIDGET_H
