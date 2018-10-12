/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (info@qt.nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#ifndef QATE_DEFAULT_COLORS_H
#define QATE_DEFAULT_COLORS_H

#include <QTextCharFormat>
#include <QMap>

namespace TextEditor{
	namespace Internal {
		class Highlighter;
	}
}

class QTextEdit;
class QPlainTextEdit;

namespace Qate {


#define FormatNormal        "Normal"
#define FormatKeyword       "Keyword"
#define FormatDataType      "DataType"
#define FormatDecimal       "DecVal"
#define FormatBaseN         "BaseN"
#define FormatFloat         "Float"
#define FormatChar          "Char"
#define FormatString        "String"
#define FormatComment       "Comment"
#define FormatAlert         "Alert"
#define FormatError         "Error"
#define FormatFunction      "Function"
#define FormatRegionMarker  "RegionMarker"
#define FormatOthers        "Others"

#define EditorBackgroundColor              "background-color"
#define EditorCodeFolding                  "code-folding"
#define EditorBracketMatching              "bracket-matching"
#define EditorCurrentLine                  "current-line"
#define EditorIconBorder                   "icon-border"
#define EditorIndentationLine              "indentation-line"
#define EditorLineNumbers                  "line-numbers"
#define EditorCurrentLineNumber            "current-line-number"
#define EditorMarkBookmark                 "mark-bookmark"
#define EditorMarkBreakpointActive         "mark-breakpoint-active"
#define EditorMarkBreakpointReached        "mark-breakpoint-reached"
#define EditorMarkBreakpointDisabled       "mark-breakpoint-disabled"
#define EditorMarkExecution                "mark-execution"
#define EditorMarkWarning                  "mark-warning"
#define EditorMarkError                    "mark-error"
#define EditorModifiedLines                "modified-lines"
#define EditorReplaceHighlight             "replace-highlight"
#define EditorSavedLines                   "saved-lines"
#define EditorSearchHighlight              "search-highlight"
#define EditorSelection                    "selection"
#define EditorSeparator                    "separator"
#define EditorSpellChecking                "spell-checking"
#define EditorTabMarker                    "tab-marker"
#define EditorTemplateBackground           "template-background"
#define EditorTemplatePlaceholder          "template-placeholder"
#define EditorTemplateFocusedPlaceholder   "template-focused-placeholder"
#define EditorTemplateReadOnlyPlaceholder  "template-read-only-placeholder"
#define EditorWordWrapMarker               "word-wrap-marker"

class Theme
{
private:
	QMap<QString, QTextCharFormat> mFormats;
	QMap<QString, QColor> mEditorColors;
	QTextCharFormat mDefault;

public:
    static Theme &defaultColors();

    void load(const QString &fileName);
    void loadTextStyles(const QJsonObject &textStyles);
    void loadTextColors(const QJsonObject &editorColors);
    void applyToHighlighter(TextEditor::Internal::Highlighter *hl) const;
    void applyToEditor(QPlainTextEdit* editor) const;

    inline QColor getEditorColor(const char* name) const { return  mEditorColors.value(name); }
    inline QTextCharFormat getFormat( const char* name) const { return  mFormats.value(name, mDefault); }

    Theme();
    void setupDefaultColors();
};

}

#endif // QATE_DEFAULT_COLORS_H
