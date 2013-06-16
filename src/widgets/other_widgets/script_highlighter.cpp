/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2013  Mattia Basaglia

Knotter is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Knotter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "script_highlighter.hpp"
#include <QSettings>

Script_Highlighter::Script_Highlighter(QObject *parent) :
    QSyntaxHighlighter(parent)
{
    keywords << "break" <<
                "case" <<
                "catch" <<
                "continue" <<
                "default" <<
                "delete" <<
                "do" <<
                "else" <<
                "finally" <<
                "for" <<
                "function" <<
                "if" <<
                "in" <<
                "instanceof" <<
                "new" <<
                "return" <<
                "switch" <<
                "this" <<
                "throw" <<
                "try" <<
                "typeof" <<
                "var" <<
                "void" <<
                "while" <<
                "with";

    keywords << "true" << "false" << "null" << "undefined";
}

void Script_Highlighter::style_from_file(QString file)
{

    QSettings settings( file, QSettings::IniFormat, 0 );

    foreach ( const QString& key,  settings.childGroups() )
    {
        settings.beginGroup(key);
        QTextCharFormat format;
        format.setForeground ( QColor ( settings.value("foreground","black").toString() ) );
        format.setBackground ( QColor ( settings.value("background","white").toString() ) );
        format.setFontWeight ( settings.value("weight",QFont::Normal).toInt() );
        format.setFontItalic ( settings.value("italic",false).toBool() );
        format.setFontUnderline ( settings.value("underline",false).toBool() );
        format.setFontOverline ( settings.value("overline",false).toBool() );
        format.setFontStrikeOut ( settings.value("strikeout",false).toBool() );
        settings.endGroup();
        m_style[key] = format;
    }
}

void Script_Highlighter::highlightBlock(const QString &text)
{

    int state = previousBlockState();

    int i = 0;
    while ( i < text.size() )
    {
        int old_i = i;
        QChar c = text[i];
        if ( state == 1 || ( c == '/' && i+1 < text.size() && text[i+1] == '*' ) )
        {
            state = 1; // comment
            int end = text.indexOf("*/",i+2);
            if ( end == -1 )
                end = text.size();
            else
            {
                state = 0;
                end += 2;
            }
            setFormat(i,end-i,m_style["comment"]);
            i = end;
        }
        else if ( c.isSpace() )
        {
            setFormat(i,1,m_style["whitespace"]);
            i++;
        }
        else if ( c.isDigit() || c == '.' )
        {
            QRegExp number_literal("[0-9]*(\\.[0-9]*([eE][+-]?[0-9]+)?)?");
            text.indexOf(number_literal,i);
            if ( c == '.' && number_literal.matchedLength() == 1)
                setFormat(i,1,m_style["operator"]);
            else
                setFormat(i,number_literal.matchedLength(),m_style["number"]);
            i += number_literal.matchedLength();
        }
        else if ( c.isLetter() || c == '_' )
        {
            QRegExp identifier("[a-zA-Z0-9_]+");
            text.indexOf(identifier,i);
            if ( keywords.contains(text.mid(i,identifier.matchedLength())))
                setFormat(i,identifier.matchedLength(),m_style["keyword"]);
            else
                setFormat(i,identifier.matchedLength(),m_style["identifier"]);
            i += identifier.matchedLength();
        }
        else if ( c == '"' || c == '\'' )
        {
            int j = i+1;
            for ( ; j < text.size(); j++ )
            {
                if ( text[j] == c )
                {
                    j++;
                    break;
                }
                else if ( text[j] == '\\' )
                {
                    setFormat(i,j-i,m_style["string"]);
                    setFormat(j,2,m_style["escape"]);
                    j++;
                    i = j+1;
                }
            }
            setFormat(i,j-i,m_style["string"]);
            i = j;
        }
        else if ( c == '/' && i+1 < text.size() && text[i+1] == '/' )
        {
            int nl = text.indexOf('\n',i);
            if ( nl == -1 )
                nl = text.size();
            setFormat(i,nl-i,m_style["comment"]);
            i = nl;
        }
        else
        {
            setFormat(i,1,m_style["operator"]);
            i++;
        }

        if ( i <= old_i )
            i = old_i+1;
    }

    setCurrentBlockState(state);
}
