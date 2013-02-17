/*
  methodinvocationdialog.h

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2010-2013 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Volker Krause <volker.krause@kdab.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GAMMARAY_METHODINVOCATIONDIALOG_H
#define GAMMARAY_METHODINVOCATIONDIALOG_H

#include <QDialog>
#include <QMetaEnum>
#include <QMetaObject>
#include <QPointer>

namespace GammaRay {

namespace Ui { class MethodInvocationDialog; }
class MethodArgumentModel;

class MethodInvocationDialog : public QDialog
{
  Q_OBJECT
  public:
    explicit MethodInvocationDialog(QWidget *parent = 0);
    ~MethodInvocationDialog();

    Qt::ConnectionType connectionType() const;

    void setMethod(QObject *object, const QMetaMethod &method);
    void accept();

  private:
    QMetaMethod m_method;
    QPointer<QObject> m_object;
    QScopedPointer<Ui::MethodInvocationDialog> ui;
    MethodArgumentModel *m_argumentModel;
};

}

#endif // GAMMARAY_METHODINVOCATIONDIALOG_H
