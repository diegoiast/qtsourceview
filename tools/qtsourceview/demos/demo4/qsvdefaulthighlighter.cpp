#include "qsvdefaulthighlighter.h"

DefaultHighlighter::DefaultHighlighter(QObject *parent) : QSyntaxHighlighter(parent)
{
	setMatchBracketList("()[]''\"\"");
}

void DefaultHighlighter::highlightBlock(const QString &text)
{
	QsvSyntaxHighlighterBase::highlightBlock(text);
}

void DefaultHighlighter::toggleBookmark(QTextBlock &block)
{
	QsvBlockData *data = getBlockData(block);
	if (data == NULL)
		return;
	data->toggleBookmark();
}

void DefaultHighlighter::removeModification(QTextBlock &block)
{
	QsvBlockData *data = getBlockData(block);
	if (data == NULL)
		return;
	data->m_isModified = false;
}

void DefaultHighlighter::setBlockModified(QTextBlock &block, bool on)
{
	QsvBlockData *data = getBlockData(block);
	if (data == NULL)
		return;
	data->m_isModified =  true;
}

bool DefaultHighlighter::isBlockModified(QTextBlock &block)
{
	QsvBlockData *data = getBlockData(block);
	if (data == NULL)
		false;
	return data->m_isModified;
}

bool DefaultHighlighter::isBlockBookmarked(QTextBlock &block)
{
	QsvBlockData *data = getBlockData(block);
	if (data == NULL)
		return false;
	return data->isBookmark();
}

QsvBlockData::LineFlags DefaultHighlighter::getBlockFlags(QTextBlock &block)
{
	QsvBlockData *data = getBlockData(block);
	if (data == NULL)
		return 0;
	return data->m_flags;
}

QsvBlockData* DefaultHighlighter::getBlockData(QTextBlock &block)
{
	QTextBlockUserData *userData  = block.userData();
	QsvBlockData       *blockData = NULL;

	if (userData == NULL){
		blockData =  new QsvBlockData();
		block.setUserData(blockData);
	} else {
		blockData = dynamic_cast<QsvBlockData*>(userData);
	}
	return blockData;
}

