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

#ifndef SCRIPT_HIGHLIGHTER_HPP
#define SCRIPT_HIGHLIGHTER_HPP

#include <QSyntaxHighlighter>

class Script_Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    typedef QMap<QString,QTextCharFormat> Style;

private:
    Style m_style;
    QStringList keywords;

public:
    explicit Script_Highlighter(QObject *parent = 0);

    void style_from_file(QString file);
protected:
    void highlightBlock(const QString &text);

};

#endif // SCRIPT_HIGHLIGHTER_HPP
