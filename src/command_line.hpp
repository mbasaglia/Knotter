/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2014  Mattia Basaglia

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

#ifndef COMMAND_LINE_HPP
#define COMMAND_LINE_HPP

#include <QStringList>

class Command_Line
{
    QStringList args;

    QStringList m_files;
    bool        ui;
    bool        antialias;
    bool        include_graph;
public:
    Command_Line(int argc, char *argv[]);

    QStringList files() const { return m_files; }
    bool load_ui() const { return ui; }

private:
    void license() const;
    void help() const;
    void version() const;


};

#endif // COMMAND_LINE_HPP
