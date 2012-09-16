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
#ifndef ERROR_RECOVERY_HPP
#define ERROR_RECOVERY_HPP

#include "knotgraph.hpp"

/**
    \brief Quick and dirty class used to save a backup file on error
*/
class ErrorRecovery
{
    private:
        static void sigdying(int sig);

    public:
        static QList<KnotGraph*> recover;
        static void initialize();
        static void insert(KnotGraph*);
        static void remove(KnotGraph*);
};

#endif // ERROR_RECOVERY_HPP
