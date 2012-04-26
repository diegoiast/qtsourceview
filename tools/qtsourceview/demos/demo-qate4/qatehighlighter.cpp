#include "qatehighlighter.h"


QateHighlighter::QateHighlighter(QObject *parent) : QSyntaxHighlighter(parent)
{
}


void QateHighlighter::highlightBlock(const QString &text)
{
}


void QateHighlighter::toggleBookmark(QTextBlock &block)
{
}


void QateHighlighter::removeModification(QTextBlock &block)
{
}


void QateHighlighter::setBlockModified(QTextBlock &block, bool on)
{
}


bool QateHighlighter::isBlockModified(QTextBlock &block)
{
}


bool QateHighlighter::isBlockBookmarked(QTextBlock &block)
{
}


QsvBlockData::LineFlags QateHighlighter::getBlockFlags(QTextBlock &block)
{
}


QsvBlockData *QateHighlighter::getBlockData(QTextBlock &block)
{
}
