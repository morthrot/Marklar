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

#include "Marklar.h"

#include "Hive.h"

Marklar::Marklar(const QString & source_path,const QString & target_path,const int update_mins,const QString & black_list_fn,QObject * parent) : QObject(parent) {
    _source_path = source_path;
    _target_path = target_path;
    _black_list_fn = black_list_fn;

    if(_source_path.isEmpty() == false && _source_path.endsWith("/") == false) { _source_path += "/"; }
    if(_target_path.isEmpty() == false && _target_path.endsWith("/") == false) { _target_path += "/"; }

    _icon_map[TrayIdle] = QIcon(":/idle.png");
    _icon_map[TrayFailed] = QIcon(":/failed.png");
    _icon_map[TrayActive] = QIcon(":/active.png");

    _menu = new QMenu;
    _menu->addAction("Показать архив",this,SLOT(browseTargetPath()));
    _menu->addAction("Архивировать сейчас",this,SLOT(keepAlive()));
    _menu->addSeparator();
    _menu->addAction("О программе...",this,SLOT(performAboutDialog()));
    _menu->addAction("Выход...",this,SLOT(performExitSyncDialog()));

    _tray = new QSystemTrayIcon(this);
    _tray->setIcon(_icon_map.value(TrayIdle));
    _tray->setContextMenu(_menu);
    _tray->setToolTip("Marklar");
    _tray->show();

    unsigned int effective_mins = (update_mins > 2 ? update_mins : 60);

    QTimer * timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(keepAlive()));
    timer->start(effective_mins * 60 * 1000);

    trayMessage( QString("Запуск архивирования \"%1\" в \"%2\" каждые %3 минут(а,ы)")
                 .arg( QDir::toNativeSeparators(_source_path) )
                 .arg( QDir::toNativeSeparators(_target_path) )
                 .arg( effective_mins ) );

    QTimer::singleShot(4096,this,SLOT(keepAlive()));
    }

Marklar::~Marklar(void) {
    _menu->deleteLater();
    }

void Marklar::browseTargetPath(void) {
    if(_target_path.isEmpty() == true) { return; }
    else { QDesktopServices::openUrl( QUrl(_target_path,QUrl::TolerantMode) ); }
    }

bool Marklar::keepAlive(void) {
    if(_source_path.isEmpty() == true) { return false; }
    if(_target_path.isEmpty() == true) { return false; }

    trayIcon(TrayActive);

    if(QDir(_source_path).exists() == false) {
        trayIcon(TrayFailed);
        trayMessage(QString("Исходный путь \"%1\" не существует").arg(QDir::toNativeSeparators(_source_path)),true);
        return false;
        }

    if(QDir(_target_path).exists() == false) {
        trayIcon(TrayFailed);
        trayMessage(QString("Путь архивов \"%1\" не существует").arg(QDir::toNativeSeparators(_target_path)),true);
        return false;
        }

    QString hive_fn = _target_path + "Marklar.hive";
    Hive source_hive, target_hive;

    if(target_hive.load(hive_fn) == false) { trayMessage("Инициализация нового каталога"); }

    source_hive.loadExceptions(_black_list_fn);
    if(source_hive.appendFolderToHive(_source_path) == false) {
        trayIcon(TrayFailed);
        trayMessage(source_hive.lastError(),true);
        return false;
        }

    QStringList changed_files;
    changed_files.append( source_hive.changedFiles(target_hive) );
    changed_files.append( source_hive.removedFiles(target_hive) );

    if(changed_files.isEmpty() == false) {
        QString backup_folder = _target_path + QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss/");
        if(QDir(backup_folder).mkpath(".") == false) {
            trayIcon(TrayFailed);
            trayMessage( QString("Ошибка создания архива \"%1\"").arg( QDir::toNativeSeparators(backup_folder) ), true );
            return false;
            }

        for(int i=0; i<changed_files.count(); i++) {
            QString fn = changed_files.at(i);
            QString source_fn = _source_path + fn;
            QString target_fn = backup_folder + fn;

            QFileInfo(target_fn).dir().mkpath(".");

            if(QFile::copy(source_fn,target_fn) == false) {
                trayIcon(TrayFailed);
                trayMessage( QString("Ошибка архивирования \"%1\" в \"%2\"")
                             .arg( QDir::toNativeSeparators(source_fn) )
                             .arg( QDir::toNativeSeparators(target_fn) ),
                             true );
                return false;
                }
            }

        trayMessage( QString("Архивировано %1 файл(а,ов)").arg(changed_files.count()) );
        }

    if(source_hive.save(hive_fn) == false) {
        trayIcon(TrayFailed);
        trayMessage(source_hive.lastError(),true);
        return false;
        }

    trayIcon(TrayIdle);
    return true;
    }

void Marklar::performAboutDialog(void) {
    QMessageBox box;
    box.setWindowTitle("О программе");
    box.setIcon(QMessageBox::Information);
    box.setText("Программа резервного копирования <b>\"Marklar\"</b>");
    box.setInformativeText("Copyright &copy; 2018 Д.С. Андреев");

    QFile license_file(":/LICENSE");
    if(license_file.open(QIODevice::ReadOnly) == true) {
        box.setDetailedText( QString::fromUtf8(license_file.readAll()) );
        license_file.close();
        }

    box.exec();
    }

void Marklar::performExitSyncDialog(void) {
    QMessageBox box;
    box.setWindowTitle("Выход");
    box.setIcon(QMessageBox::Question);
    box.setText("Выход из Marklar");
    box.setInformativeText("Произвести сохранение перед выходом?");
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::Yes);

    int result = box.exec();
    if(result == QMessageBox::Cancel) { return; }

    if(result == QMessageBox::Yes) {
        bool save_ok = keepAlive();
        if(save_ok == false) { return; }
        }

    qApp->quit();
    }

void Marklar::trayMessage(const QString & text,bool fatal) {
    _tray->showMessage("Marklar", text, fatal ? QSystemTrayIcon::Critical : QSystemTrayIcon::Information, 2048);
    }

void Marklar::trayIcon(const TrayIcon & icon) {
    if(_icon_map.contains(icon) == false) { return; }

    const QIcon & value = _icon_map.value(icon);
    if(value.isNull() == true) { return; }

    _tray->setIcon(value);
    }
