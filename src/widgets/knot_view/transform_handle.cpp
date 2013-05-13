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

#include "transform_handle.hpp"
#include "resource_manager.hpp"


QSvgRenderer Transform_Handle::scale_rest;
QSvgRenderer Transform_Handle::scale_active;
QSvgRenderer Transform_Handle::rotate_rest;
QSvgRenderer Transform_Handle::rotate_active;
double Transform_Handle::m_image_size = 24;
bool Transform_Handle::images_initialized = false;

Transform_Handle::Transform_Handle(Mode mode, int image_angle)
    : m_mode(mode), m_image_angle(image_angle)
{
    if ( !images_initialized )
    {
        images_initialized = true;

        scale_rest.load(Resource_Manager::data("img/handle_scale_rest.svg"));
        scale_active.load(Resource_Manager::data("img/handle_scale_active.svg"));
        rotate_rest.load(Resource_Manager::data("img/handle_rotate_rest.svg"));
        rotate_active.load(Resource_Manager::data("img/handle_rotate_active.svg"));
    }
}

void Transform_Handle::set_mode(Transform_Handle::Mode mode)
{
    m_mode = mode;
}

void Transform_Handle::set_image_angle(int angle)
{
    m_image_angle = angle;
}

void Transform_Handle::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QSvgRenderer* rend;
    if ( m_mode == ROTATE )
        rend = highlighted ? &rotate_active : &rotate_rest;
    else
        rend = highlighted ? &scale_active : &scale_rest;

    painter->rotate(-m_image_angle);
    painter->translate(-m_image_size/2,-m_image_size/2);
    rend->render(painter,boundingRect());

}
