#ifndef PRIVATEBLOCKDATA_H
#define PRIVATEBLOCKDATA_H

#include <QTextBlockUserData>

class PrivateBlockData : public QTextBlockUserData
{
public:
	PrivateBlockData();
//private:
	bool m_isCurrentDebugLine;
	bool m_isBookmark;
	bool m_isModified;
};

#endif
