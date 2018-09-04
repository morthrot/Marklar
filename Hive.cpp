/*

Copyright (c) 2018 Д.С. Андреев

Этот файл — часть Marklar.

Marklar - свободная программа: вы можете перераспространять ее и/или
изменять ее на условиях Стандартной общественной лицензии GNU в том виде,
в каком она была опубликована Фондом свободного программного обеспечения;
либо версии 3 лицензии, либо (по вашему выбору) любой более поздней
версии.

Marklar распространяется в надежде, что она будет полезной,
но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА
или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной
общественной лицензии GNU.

Вы должны были получить копию Стандартной общественной лицензии GNU
вместе с этой программой. Если это не так, см.
<http://www.gnu.org/licenses/>.)


This file is part of Marklar.

Marklar is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Marklar is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Composer.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "Hive.h"

Hive::Hive(QObject * parent) : QObject(parent) {
    _contents.clear();
    _exceptions.clear();
    _last_error.clear();
    }

QString Hive::lastError(void) {
    return _last_error;
    }

bool Hive::loadExceptions(const QString & fn) {
    if(fn.isEmpty() == true) { return false; }

    QFile file(fn);
    if(file.open(QIODevice::ReadOnly) == false) {
        _last_error = QString("Ошибка открытия файла исключений \"%1\" для чтения").arg( QDir::toNativeSeparators(fn) );
        return false;
        }

    _exceptions.clear();
    while(file.atEnd() == false) {
        QString line = QString::fromUtf8(file.readLine()).trimmed();
        if(line.isEmpty() == true) { continue; }
        if(line.startsWith("#") == true) { continue; }

        _exceptions.insert( QRegExp(line) );
        }

    file.close();
    return true;
    }

bool Hive::matchException(const QString & text) {
    if(text.isEmpty() == true) { return false; }
    Q_FOREACH(const QRegExp &rx, _exceptions) { if(rx.indexIn(text) != -1) { return true; } }
    return false;
    }

bool Hive::load(const QString & fn) {
    if(fn.isEmpty() == true) { return false; }

    QFile file(fn);
    if(file.open(QIODevice::ReadOnly) == false) {
        _last_error = QString("Ошибка открытия для чтения файла \"%1\"").arg( QDir::toNativeSeparators(fn) );
        return false;
        }

    QDataStream ds(&file);
    ds.setVersion(QDataStream::Qt_4_7);
    ds >> _contents;

    file.close();
    return true;
    }

bool Hive::save(const QString & fn) {
    if(fn.isEmpty() == true) { return false; }

    QFile file(fn);
    if(file.open(QIODevice::WriteOnly|QIODevice::Truncate) == false) {
        _last_error = QString("Ошибка открытия для записи файла \"%1\"").arg( QDir::toNativeSeparators(fn) );
        return false;
        }

    QDataStream ds(&file);
    ds.setVersion(QDataStream::Qt_4_7);
    ds << _contents;

    file.close();
    return true;
    }

void Hive::clear(void) {
    _contents.clear();
    }

bool Hive::appendFolderToHive(QString base_name,QString folder_name) {
    if(base_name.isEmpty() == true) { return false; }

    if(matchException(folder_name) == true) { return true; }

    if(base_name.endsWith('/') == false) { base_name = base_name.append('/'); }
    if(folder_name.isEmpty() == false && folder_name.endsWith('/') == false) { folder_name = folder_name.append('/'); }

    QString full_name = base_name + folder_name;

    QDir folder(full_name);
    if(folder.exists() == false) {
        _last_error = QString("Каталог \"%1\" не существует").arg( QDir::toNativeSeparators(full_name) );
        return false;
        }

    QStringList files = folder.entryList(QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot,QDir::Name);
    for(int i=0; i<files.count(); i++) {
        bool append_result = appendFileToHive(base_name,folder_name + files.at(i));
        if(append_result == false) { return false; }
        }

    QStringList folders = folder.entryList(QDir::Dirs|QDir::NoSymLinks|QDir::NoDotAndDotDot,QDir::Name);
    for(int i=0; i<folders.count(); i++) {
        bool append_result = appendFolderToHive(base_name,folder_name + folders.at(i));
        if(append_result == false) { return false; }
        }

    return true;
    }

bool Hive::appendFileToHive(const QString & base_name,const QString & file_name) {
    if(file_name.isEmpty() == true) { return false; }

    if(matchException(file_name) == true) { return true; }

    if(_contents.contains(file_name) == true) {
        _last_error = QString("Дублирующееся имя файла \"%1\"").arg( (QDir::toNativeSeparators(file_name)) );
        return false;
        }

    QString full_name = base_name + file_name;

    QFile file(full_name);
    if(file.exists() == false) {
        _last_error = QString("Файл \"%1\" не существует").arg( QDir::toNativeSeparators(full_name) );
        return false;
        }

    bool open_result = file.open(QIODevice::ReadOnly);
    if(open_result == false) {
        _last_error = QString("Ошибка открытия файла \"%1\" для чтения").arg( QDir::toNativeSeparators(full_name) );
        return false;
        }

    QCryptographicHash hash(QCryptographicHash::Md5);
    while(file.atEnd() == false) { hash.addData( file.read(1 * 1024 * 1024) ); }
    file.close();

    _contents[file_name] = hash.result();
    return true;
    }

QStringList Hive::changedFiles(const Hive & other) {
    QStringList result;

    QStringList keys = _contents.keys();
    for(int i=0; i<keys.count(); i++) {
        QString key = keys.at(i);
        if(key.isEmpty() == true) { continue; }

        if(_contents.contains(key) == false) { continue; }
        if(other._contents.contains(key) == false) { continue; }

        if(_contents.value(key) == other._contents.value(key)) { continue; }

        result.append(key);
        }

    return result;
    }

QStringList Hive::removedFiles(const Hive & other) {
    QStringList result;

    QStringList keys = _contents.keys();
    for(int i=0; i<keys.count(); i++) {
        QString key = keys.at(i);
        if(key.isEmpty() == true) { continue; }

        if(_contents.contains(key) == false) { continue; }
        if(other._contents.contains(key) == true) { continue; }

        result.append(key);
        }

    return result;
    }
