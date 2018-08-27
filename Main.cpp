/*

Copyright (c) 2018 �.�. �������

���� ���� � ����� Marklar.

Banderol - ��������� ���������: �� ������ ������������������ �� �/���
�������� �� �� �������� ����������� ������������ �������� GNU � ��� ����,
� ����� ��� ���� ������������ ������ ���������� ������������ �����������;
���� ������ 3 ��������, ���� (�� ������ ������) ����� ����� �������
������.

Banderol ���������������� � �������, ��� ��� ����� ��������,
�� ���� ������ ��������; ���� ��� ������� �������� ��������� ����
��� ����������� ��� ������������ �����. ��������� ��. � �����������
������������ �������� GNU.

�� ������ ���� �������� ����� ����������� ������������ �������� GNU
������ � ���� ����������. ���� ��� �� ���, ��.
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
        QMessageBox::critical(NULL,"Параметры запуска",QString("<b>%1</b> &lt;исходный путь&gt; &lt;путь архивов&gt; [интервал, мин]").arg(app_name),"Выход");
        return EXIT_FAILURE;
        }

    QString source_path = QDir::fromNativeSeparators(args.at(1));
    QString target_path = QDir::fromNativeSeparators(args.at(2));
    int update_mins = args.count() > 3 ? args.at(3).toInt() : 60;

    Marklar marklar(source_path,target_path,update_mins);

    return app.exec();
    return EXIT_SUCCESS;
    }
