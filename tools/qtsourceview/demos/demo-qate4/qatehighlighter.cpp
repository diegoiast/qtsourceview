#include "qatehighlighter.h"


QateHighlighter::QateHighlighter(QTextDocument *parent) :
    Highlighter(parent)
{
    setMatchBracketList("()[]''\"\"");
}


void QateHighlighter::highlightBlock(const QString &text)
{
    QsvSyntaxHighlighterBase::highlightBlock(text);
    Highlighter::highlightBlock(text);
}


void QateHighlighter::toggleBookmark(QTextBlock &block)
{
    Qate::BlockData *data = getBlockData(block);
	if (data == NULL)
		return;
	data->toggleBookmark();
}


void QateHighlighter::removeModification(QTextBlock &block)
{
    Qate::BlockData *data = getBlockData(block);
	if (data == NULL)
		return;
    data->m_isModified = false;
}


void QateHighlighter::setBlockModified(QTextBlock &block, bool on)
{
    Qate::BlockData *data = getBlockData(block);
	if (data == NULL)
		return;
	data->m_isModified = on;

}

bool QateHighlighter::isBlockModified(QTextBlock &block)
{
    Qate::BlockData *data = getBlockData(block);
	if (data == NULL)
		return false;
	return data->m_isModified;
}


bool QateHighlighter::isBlockBookmarked(QTextBlock &block)
{
    Qate::BlockData *data = getBlockData(block);
	if (data == NULL)
		return false;
	return data->isBookmark();
}


Qate::BlockData::LineFlags QateHighlighter::getBlockFlags(QTextBlock &block)
{
    Qate::BlockData *data = getBlockData(block);
	if (data == NULL)
		return 0;
	return data->m_flags;
}


Qate::BlockData *QateHighlighter::getBlockData(QTextBlock &block)
{
    QTextBlockUserData *userData  = block.userData();
	Qate::BlockData    *blockData = NULL;

    return dynamic_cast<Qate::BlockData*>(userData);
    /*
	if (userData == NULL){
		blockData =  new QsvBlockData();
		block.setUserData(blockData);
	} else {
		blockData = dynamic_cast<QsvBlockData*>(userData);
	}
	return blockData;
    */
}
