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

	void setBookmark( bool on )	{ setFlag(Bookmark,on); }
	void toggleBookmark()		{ toggleFlag(Bookmark); }
	void setDebug( bool on )	{ setFlag(Debug,on);    }
	void toggleDebug()		{ toggleFlag(Debug);    }
};	
Q_DECLARE_OPERATORS_FOR_FLAGS(QsvBlockData::LineFlags);

class QsvSyntaxHighlighterBase
{
public:
	QsvSyntaxHighlighterBase();
	virtual ~QsvSyntaxHighlighterBase();

	void setMatchBracketList( const QString &m );
	const QString getMatchBracketList();
	void highlightBlock(const QString &text);
	void setDocument(QTextDocument * document);
	
	virtual QTextBlockUserData* currentBlockUserDataProxy() = 0;
	virtual void setCurrentBlockUserDataProxy(QTextBlockUserData * data) = 0;
protected:
	QString m_matchBracketsList;
};

#endif // QSVSYNTAXHIGHLIGHTERBASE_H
