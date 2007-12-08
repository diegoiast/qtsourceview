#ifndef __QSVHAIQEDITOR_H__
#define __QSVHAIQEDITOR_H__

#include <QDebug>
#include <QObject>
#include <haiqeditorplugin.h>

class QsvHaiqEditor : public QObject, public HaiQEditorPlugin
{
	Q_OBJECT
	Q_INTERFACES(HaiQEditorPlugin)
	HaiQCoreInterface *core;
public:
	QsvHaiqEditor();
	virtual ~QsvHaiqEditor();
	
	void initialize(HaiQCoreInterface *core_in);
	QString name();
	QString description();
	QString authors();
	QString lastModified();
	double loadOrder();
	
	HaiQEditorInterface *newInterface();
	QStringList fileTypes();
};

#endif
