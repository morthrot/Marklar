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

#ifndef MARKLAR_H
#define MARKLAR_H

#include <QtCore>
#include <QtGui>

#ifdef AM_USING_QT5
#include <QtWidgets>
#endif

class Marklar : public QObject {
    Q_OBJECT
private:
    QString _source_path;
    QString _target_path;
    QString _black_list_fn;

    QMenu * _menu;
    QSystemTrayIcon * _tray;

    typedef enum { TrayIdle, TrayFailed, TrayActive } TrayIcon;
    QMap<TrayIcon,QIcon> _icon_map;
public:
    Marklar(const QString & source_path,const QString & target_path,
            const int update_mins = 60,const QString & black_list_fn = "BlackList.txt",
            QObject * parent = NULL);
    ~Marklar(void);
public slots:
    void browseTargetPath(void);
    bool keepAlive(void);

    void performAboutDialog(void);
    void performExitSyncDialog(void);

    void trayMessage(const QString & text,bool fatal = false);
    void trayIcon(const TrayIcon & icon);
    };

#endif // MARKLAR_H
