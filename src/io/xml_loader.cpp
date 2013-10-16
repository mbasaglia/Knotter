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

#include "xml_loader.hpp"
#include "xml_loader_v2.hpp"
#include "xml_loader_v3.hpp"
#include "xml_loader_v4.hpp"
#include <QBuffer>

typedef XML_Loader_v4 XML_Loader_current;

void import_xml_style(QString style, Graph& graph)
{
    QByteArray output(style.toUtf8());
    QBuffer buffer(&output);
    buffer.open(QIODevice::ReadOnly);
    XML_Loader_current().load_style(&buffer,&graph);
}


bool import_xml(QIODevice &file, Graph& graph)
{
    if ( !file.isOpen() && !file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return false;
    }

    XML_Loader_current xml;

    if ( xml.load(&file,&graph) )
        return true;
    else if ( XML_Loader_v3::supports_version(xml.version()) )
    {
        file.seek(0);
        XML_Loader_v3 xml3;
        return xml3.load(&file,&graph);
    }
    else if ( XML_Loader_v2::supports_version(xml.version()) )
    {
        file.seek(0);
        XML_Loader_v2 xml2;

        if ( !xml2.load(&file) )
            return false;

        xml2.get_graph(graph);
        return true;
    }

    return false;
}
