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

#ifndef BACKGROUND_IMAGE_HPP
#define BACKGROUND_IMAGE_HPP
#include <QPixmap>

class Background_Image: public QObject
{
    Q_OBJECT
private:
    QPixmap image;
    QPointF pos;
    bool    enabled;
    QString file;
    double  scale;

public:
    Background_Image();

    void render (QPainter *painter) const;

    bool is_enabled () const { return enabled; }
    QString file_name() const { return file; }
    QPointF position() const { return pos; }

    double scale_percent() const { return scale*100; }


public slots:

    void set_scale_percent(double pc);

    void enable ( bool enabled );

    void load_file(QString);

    void set_position ( QPointF position );

signals:
    void changed();
    void moved(QPointF);
};

#endif // BACKGROUND_IMAGE_HPP
