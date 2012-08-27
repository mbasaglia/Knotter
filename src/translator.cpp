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
#include "translator.hpp"
#include <QCoreApplication>
#include "resource_loader.hpp"
#include <algorithm>

Translator Translator::object;

Translator::Translator()
    : current ( NULL )
{}

Translator::~Translator()
{
    foreach ( QTranslator* tr, translators.values() )
        delete tr;
}

void Translator::register_default_translation(QString name, QString code)
{
    lang_names[name]=code;
    translators[code]=NULL;
}

void Translator::register_translation(QString name, QString code, QString file)
{
    QTranslator* ntrans = new QTranslator;
    ntrans->load(load::resource_name(DATA_DIR,"translations/"+file));
    lang_names[name]=code;
    translators[code]=ntrans;
}

QString Translator::current_lang_name() const
{
    return lang_names.key(translators.key(current));
}

QString Translator::current_lang_code() const
{
    return translators.key(current);
}

QTranslator *Translator::current_translator() const
{
    return current;
}
static int cmpstr ( const QString & s1, const QString & s2 )
{
    return QString::localeAwareCompare(s1,s2);
}
QStringList Translator::available_languages() const
{
    QStringList sl = lang_names.keys();
    std::sort(sl.begin(),sl.end(),cmpstr);
    return sl;
}

void Translator::change_lang_code(QString code)
{
    /// \todo checking
    QCoreApplication* app = QCoreApplication::instance();
    app->removeTranslator(current);
    current = translators[code];
    app->installTranslator(current);
    emit language_changed();
}

void Translator::change_lang_name(QString name)
{
    change_lang_code(lang_names[name]);
}

