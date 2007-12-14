#ifndef __QSVHAIQEDITOR_H__
#define __QSVHAIQEDITOR_H__

#include <QDebug>
#include <QObject>
#include <haiqeditorplugin.h>

class QsvHaiqEditorPlugin : public QObject, public HaiQEditorPlugin
{
	Q_OBJECT
	Q_INTERFACES(HaiQEditorPlugin)
public:
	QsvHaiqEditorPlugin();
	virtual ~QsvHaiqEditorPlugin();
	
	void initialize(HaiQCoreInterface *core_in);
	QString name();
	QString description();
	QString authors();
	QString lastModified();
	double loadOrder();
	
	HaiQEditorInterface *newInterface();
	void activate(HaiQEditorInterface *interface);
	void deactivate();
	QStringList fileTypes();
	
private slots:
	void on_actionConfigQtSourceView_triggered();
	
private:
	HaiQCoreInterface	*m_core;
	QAction			*m_actionConfigQtSourceView;
};

#endif //__QSVHAIQEDITOR_H__
