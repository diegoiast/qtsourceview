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

const char* FormatNormal       = "Normal";
const char* FormatKeyword      = "Keyword";
const char* FormatDataType     = "DataType";
const char* FormatDecimal      = "DecVal";
const char* FormatBaseN        = "BaseN";
const char* FormatFloat        = "Float";
const char* FormatChar         = "Char";
const char* FormatString       = "String";
const char* FormatComment      = "Comment";
const char* FormatAlert        = "Alert";
const char* FormatError        = "Error";
const char* FormatFunction     = "Function";
const char* FormatRegionMarker = "RegionMarker";
const char* FormatOthers       = "Others";

const char* EditorBackgroundColor             = "background-color";
const char* EditorCodeFolding                 = "code-folding";
const char* EditorBracketMatching             = "bracket-matching";
const char* EditorCurrentLine                 = "current-line";
const char* EditorIconBorder                  = "icon-border";
const char* EditorIndentationLine             = "indentation-line";
const char* EditorLineNumbers                 = "line-numbers";
const char* EditorCurrentLineNumber           = "current-line-number";
const char* EditorMarkBookmark                = "mark-bookmark";
const char* EditorMarkBreakpointActive        = "mark-breakpoint-active";
const char* EditorMarkBreakpointReached       = "mark-breakpoint-reached";
const char* EditorMarkBreakpointDisabled      = "mark-breakpoint-disabled";
const char* EditorMarkExecution               = "mark-execution";
const char* EditorMarkWarning                 = "mark-warning";
const char* EditorMarkError                   = "mark-error";
const char* EditorModifiedLines               = "modified-lines";
const char* EditorReplaceHighlight            = "replace-highlight";
const char* EditorSavedLines                 = "saved-lines";
const char* EditorSearchHighlight             = "search-highlight";
const char* EditorSelection                   = "selection";
const char* EditorSeparator                   = "separator";
const char* EditorSpellChecking               = "spell-checking";
const char* EditorTabMarker                   = "tab-marker";
const char* EditorTemplateBackground          = "template-background";
const char* EditorTemplatePlaceholder         = "template-placeholder";
const char* EditorTemplateFocusedPlaceholder  = "template-focused-placeholder";
const char* EditorTemplateReadOnlyPlaceholder = "template-read-only-placeholder";
const char *EditorWordWrapMarker              = "word-wrap-marker";

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
