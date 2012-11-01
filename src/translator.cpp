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
#include <QDir>
#include <QDebug>
#include <QLocale>
#include <ciso646>

Translator Translator::object;

Translator::Translator()
    : current ( NULL )
{}

void Translator::initialize(QString default_code)
{
    if ( !default_code.isEmpty() )
    {
        QString name = language_name(default_code,true);
        if ( !name.isEmpty() )
            object.register_default_translation(name,default_code);
    }

    QDir translations = load::resource_name(DATA_DIR,"translations" );
    QStringList translation_files = translations.entryList(QStringList()<<"*.qm");

    QRegExp re("[^_]+_([^.]+)\\.qm");
    foreach ( QString file, translation_files )
    {
        if ( re.exactMatch(file) )
        {
            QString code = re.cap(1);
            QString name = language_name(code,true);
            if ( !name.isEmpty() )
                object.register_translation(name,code,file);
        }
        else
            qWarning() << tr("Warning:")
                       << tr("Unrecognised translation file name pattern: %1").arg(file);
    }

}

QString Translator::language_name(QString lang_code, bool issue_warning)
{
    QLocale lang_loc = QLocale(lang_code);
    QString name;

    #if HAS_QT_4_8
        name = lang_loc.nativeLanguageName(); // native name
    #else
        name = QLocale::languageToString(lang_loc.language()); // English name
    #endif

    if ( !name.isEmpty() )
        name[0] = name[0].toUpper();
    else if ( issue_warning )
        qWarning() << tr("Warning:")
                   << tr("Unrecognised language code: %1").arg(lang_code);

    return name;
}

void Translator::load_system_default()
{
    object.change_lang_code(QLocale::system().name());
}

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
    return QString::localeAwareCompare(s1,s2) < 0;
}
QStringList Translator::available_languages() const
{
    QStringList sl = lang_names.keys();
    std::sort(sl.begin(),sl.end(),cmpstr);
    return sl;
}

void Translator::change_lang_code(QString code)
{
    if ( !translators.contains(code) )
    {
        QString base_code = code.left(code.lastIndexOf('_')); // en_US -> en
        bool found = false;
        foreach ( QString installed_code, translators.keys() )
        {
            if ( installed_code.left(installed_code.lastIndexOf('_')) == base_code )
            {
                code = installed_code;
                found = true;
                break;
            }
        }
        if ( not found )
        {
            qWarning() << tr("Warning:") << tr("Language code %1 is not loaded").arg(code);
            return;
        }
    }

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

