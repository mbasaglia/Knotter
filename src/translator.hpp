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
#ifndef TRANSLATOR_HPP
#define TRANSLATOR_HPP

#include <QObject>
#include <QTranslator>
#include <QMap>

class Translator : public QObject
{
        Q_OBJECT

    private:
        Translator(); // singleton
        Translator(const Translator&);

        QMap<QString,QString> lang_names; ///< map lang_name -> lang_code
        QMap<QString,QTranslator*> translators; ///< map lang_code -> translator
        QTranslator* current;

    public:
        static Translator object;

        /// load translations files from DATA_DIR/translations
        static void initialize(QString default_code=QString());

        static QString language_name ( QString lang_code, bool issue_warning = false );

        ~Translator();

        void register_default_translation ( QString name, QString code );
        void register_translation ( QString name, QString code, QString file );

        QString current_lang_name() const;
        QString current_lang_code() const;
        QTranslator* current_translator() const;

        QStringList available_languages() const;

    public slots:

        void change_lang_code ( QString code );
        void change_lang_name ( QString name );

    signals:

        void language_changed();

        
};

#endif // TRANSLATOR_HPP
