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

#ifndef HIVE_H
#define HIVE_H

#include <QtCore>

class Hive : public QObject {
    Q_OBJECT
private:
    QMap<QString,QByteArray> _contents;
    QString _last_error;
public:
    Hive(QObject * parent = NULL);
    ~Hive(void);
public slots:
    QString lastError(void);

    bool load(const QString & fn);
    bool save(const QString & fn);
    void clear(void);

    bool appendFolderToHive(QString base_name,QString folder_name = "");
    bool appendFileToHive(const QString & base_name,const QString & file_name);

    QStringList changedFiles(const Hive & other);
    QStringList removedFiles(const Hive & other);
    };

#endif // HIVE_H
