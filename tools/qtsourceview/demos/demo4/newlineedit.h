#ifndef NEWLINEEDIT_H
#define NEWLINEEDIT_H

#include <QLineEdit>
class QPixmap;
class QToolButton;

class NewLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	NewLineEdit(QWidget *parent );
	void setIcon(QPixmap i);

protected:
	void resizeEvent(QResizeEvent *);

private slots:	
	void updateEditLine();
private:
	QToolButton *clearButton;	
};

#endif
