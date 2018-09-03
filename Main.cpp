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

#include <QtCore>
#include <QtGui>

#ifdef AM_USING_QT5
#include <QtWidgets>
#endif

#include "Marklar.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
#ifdef AM_USING_QT4
    QTextCodec::setCodecForCStrings( QTextCodec::codecForName("UTF-8") );
#endif
    app.setWindowIcon(QIcon(":/icon.png"));

    QStringList args = app.arguments();

    if(args.count() < 3) {
        QString app_name = QDir::fromNativeSeparators(args.at(0)).split("/",QString::SkipEmptyParts).last();
        QMessageBox::critical(NULL,"Параметры запуска",QString("<b>%1</b> &lt;исходный путь&gt; &lt;путь архивов&gt; [интервал, мин] [имя файла исключений]").arg(app_name),"Выход");
        return EXIT_FAILURE;
        }

    QString source_path = QDir::fromNativeSeparators(args.at(1));
    QString target_path = QDir::fromNativeSeparators(args.at(2));
    int update_mins = args.count() > 3 ? args.at(3).toInt() : 60;
    QString black_list_fn = args.count() > 4 ? QDir::fromNativeSeparators(args.at(4)) : "BlackList.txt";

    Marklar marklar(source_path,target_path,update_mins,black_list_fn);

    return app.exec();
    return EXIT_SUCCESS;
    }
