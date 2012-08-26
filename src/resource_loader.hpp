/**

\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012  Mattia Basaglia

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
#ifndef resource_loader_HPP
#define resource_loader_HPP

#include <QIcon>
#include <QUrl>

namespace load {

QIcon icon ( QString name );

/**
    \brief Search resource path
    \param base_dir  exprected installation directory
    \param name      resource file name relative to base_dir
*/
QString resource_name ( QString base_dir, QString name );

QUrl resource_url ( QString base_dir, QString name );

} // namespace icon
#endif // resource_loader_HPP
