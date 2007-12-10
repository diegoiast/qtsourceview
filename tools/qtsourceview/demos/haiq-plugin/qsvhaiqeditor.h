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
	QStringList fileTypes();
};

#endif //__QSVHAIQEDITOR_H__
