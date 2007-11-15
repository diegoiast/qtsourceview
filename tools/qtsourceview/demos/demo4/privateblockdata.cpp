#include "privateblockdata.h"

PrivateBlockData::PrivateBlockData(  ) 
	: QTextBlockUserData()
{
	// TODO
	m_isCurrentDebugLine = false;
	m_isBookmark = false;
	m_isBreakpoint = false;
}
