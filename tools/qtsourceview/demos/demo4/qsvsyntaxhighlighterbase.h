#ifndef QSVSYNTAXHIGHLIGHTERBASE_H
#define QSVSYNTAXHIGHLIGHTERBASE_H

#include <QTextBlockUserData>

class MatchData {
public:
	QChar matchedChar;
	int position;
};

class QsvBlockData : public QTextBlockUserData {
public:
	enum LineFlag{
		Bookmark = 1,
		Debug = 2,
		Executing = 4,
		CompileError = 8,
		SyntaxError = 16
	};
	Q_DECLARE_FLAGS(LineFlags,LineFlag)
//	not using QObject.
//	Q_FLAGS(LineFlags)

	QList<MatchData> matches;
	bool m_isModified;
	LineFlags m_flags;

	QsvBlockData(){
		m_isModified = false;
		m_flags      = 0;
	}
	
	void setFlag( LineFlags f, bool on)
	{
		if (on)
			m_flags |= f;
		else
			m_flags &= !f;
	}
	
	void toggleFlag( LineFlag f  )
	{
		setFlag(f,!m_flags.testFlag(f));
	}
	
	bool testFlag( LineFlag f )
	{
		return m_flags.testFlag(f);
	}

	bool isModified()		{ return m_isModified; }
	void setBookmark( bool on )	{ setFlag(Bookmark,on); }
	void toggleBookmark()		{ toggleFlag(Bookmark); }
	bool isBookmark()		{ return m_flags.testFlag(Bookmark); }
	void setDebug( bool on )	{ setFlag(Debug,on);    }
	bool isDebug()			{ return m_flags.testFlag(Debug); }
	void toggleDebug()		{ toggleFlag(Debug);    }
};	
Q_DECLARE_OPERATORS_FOR_FLAGS(QsvBlockData::LineFlags);

class QsvSyntaxHighlighterBase
{
public:
	QsvSyntaxHighlighterBase();
	virtual ~QsvSyntaxHighlighterBase();
	
	virtual void toggleBookmark(QTextBlock &block) = 0;
	virtual void removeModification(QTextBlock &block) = 0;
	virtual void setBlockModified(QTextBlock &block, bool on) = 0;
	virtual bool isBlockModified(QTextBlock &block) = 0;
	virtual bool isBlockBookmarked(QTextBlock &block) = 0;
	virtual QsvBlockData::LineFlags getBlockFlags(QTextBlock &block) = 0;

	void setMatchBracketList( const QString &m );
	const QString getMatchBracketList();
	void highlightBlock(const QString &text);
	void setTextDocument(QTextDocument * document);

protected:
	QString m_matchBracketsList;
};

#endif // QSVSYNTAXHIGHLIGHTERBASE_H
