/*

Copyright (c) 2018 Д.С. Андреев

Этот файл — часть Marklar.

Banderol - свободная программа: вы можете перераспространять ее и/или
изменять ее на условиях Стандартной общественной лицензии GNU в том виде,
в каком она была опубликована Фондом свободного программного обеспечения;
либо версии 3 лицензии, либо (по вашему выбору) любой более поздней
версии.

Banderol распространяется в надежде, что она будет полезной,
но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА
или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной
общественной лицензии GNU.

Вы должны были получить копию Стандартной общественной лицензии GNU
вместе с этой программой. Если это не так, см.
<http://www.gnu.org/licenses/>.)


This file is part of Marklar.

Banderol is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Banderol is distributed in the hope that it will be useful,
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
        QMessageBox::critical(NULL,"РџР°СЂР°РјРµС‚СЂС‹ Р·Р°РїСѓСЃРєР°",QString("<b>%1</b> &lt;РёСЃС…РѕРґРЅС‹Р№ РїСѓС‚СЊ&gt; &lt;РїСѓС‚СЊ Р°СЂС…РёРІРѕРІ&gt; [РёРЅС‚РµСЂРІР°Р», РјРёРЅ]").arg(app_name),"Р’С‹С…РѕРґ");
        return EXIT_FAILURE;
        }

    QString source_path = QDir::fromNativeSeparators(args.at(1));
    QString target_path = QDir::fromNativeSeparators(args.at(2));
    int update_mins = args.count() > 3 ? args.at(3).toInt() : 60;

    Marklar marklar(source_path,target_path,update_mins);

    return app.exec();
    return EXIT_SUCCESS;
    }
