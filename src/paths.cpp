/*  smplayer2, GUI front-end for mplayer2.
    Copyright (C) 2006-2010 Ricardo Villalba <rvm@escomposlinux.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "paths.h"
#include <QLibraryInfo>
#include <QLocale>
#include <QFile>
#include <QRegExp>
#include <QDir>

#ifndef Q_OS_WIN
#include <stdlib.h>
#endif

QString Paths::app_path;
QString Paths::config_path;

void Paths::setAppPath(QString path)
{
    app_path = path;
}

QString Paths::appPath()
{
    return app_path;
}

QString Paths::dataPath()
{
#ifdef DATA_PATH
    QString path = QString(DATA_PATH);

    if (!path.isEmpty())
        return path;
    else
        return appPath();

#else
    return appPath();
#endif
}

QString Paths::translationPath()
{
    return dataPath() + "/translations";
}

QString Paths::docPath()
{
    return dataPath() + "/docs";
}

QString Paths::themesPath()
{
    return dataPath() + "/themes";
}

QString Paths::shortcutsPath()
{
    return dataPath() + "/shortcuts";
}

QString Paths::qtTranslationPath()
{
    return QLibraryInfo::location(QLibraryInfo::TranslationsPath);
}

QString Paths::doc(QString file, QString locale)
{
    if (locale.isEmpty()) {
        locale = QLocale::system().name();
    }

    QString f = docPath() + "/" + locale + "/" + file;
    qDebug("Helper:doc: checking '%s'", f.toUtf8().data());

    if (QFile::exists(f)) return f;

    if (locale.indexOf(QRegExp("_[A-Z]+")) != -1) {
        locale.replace(QRegExp("_[A-Z]+"), "");
        f = docPath() + "/" + locale + "/" + file;
        qDebug("Helper:doc: checking '%s'", f.toUtf8().data());

        if (QFile::exists(f)) return f;
    }

    f = docPath() + "/en/" + file;
    return f;
}

void Paths::setConfigPath(QString path)
{
    config_path = path;
}

QString Paths::configPath()
{
    if (!config_path.isEmpty()) {
        return config_path;
    } else {
#ifdef Q_OS_WIN
        return appPath();
#else
        const char *XDG_CONFIG_HOME = getenv("XDG_CONFIG_HOME");

        if (XDG_CONFIG_HOME != NULL) {
            qDebug("Paths::configPath: XDG_CONFIG_HOME: %s", XDG_CONFIG_HOME);
            return QString(XDG_CONFIG_HOME) + "/smplayer2";
        } else
            return QDir::homePath() + "/.config/smplayer2";

#endif
    }
}

QString Paths::iniPath()
{
    return configPath();
}

QString Paths::subtitleStyleFile()
{
    return configPath() + "/styles.ass";
}
