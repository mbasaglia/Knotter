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

#ifndef JSON_STUFF_HPP
#define JSON_STUFF_HPP

#include <QFile>
#include <QVariantMap>
#include <QTextStream>

/**
 * \brief Escape a string in order to write it in a JSON file
 * \param s The string to escaped
 * \return The quoted and escaped string
 */
QString json_escape(QString s);

/**
 * \brief Convert a QVariant into a JSON string
 * \param[out] json The output text stream
 * \param v         The value to be converted
 * \param indent    The indentation level
 */
void json_convert(QTextStream& json, QVariant v, int indent);

/**
 * \brief Read a JSON file into a QVariantMap
 * \pre \c file must be open for reading
 * \param file  The JSON file
 * \return The contents of the file as a QVariantMap
 */
QVariantMap json_read_file(QIODevice& file);
/**
 * \brief Write a QVariantMap in a file as JSON
 * \pre \c file must be open for writing
 * \param[out] file     The JSON file
 * \param      values   Values to write
 */
void json_write_file( QIODevice&file, QVariantMap values );

#endif // JSON_STUFF_HPP
