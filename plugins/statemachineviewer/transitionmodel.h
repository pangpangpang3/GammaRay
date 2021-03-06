/*
  transitionmodel.h

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2010-2015 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Stephen Kelly <stephen.kelly@kdab.com>

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

#ifndef GAMMARAY_STATEMACHINEVIEWER_TRANSITIONMODEL_H
#define GAMMARAY_STATEMACHINEVIEWER_TRANSITIONMODEL_H

#include <core/objectmodelbase.h>

class QAbstractState;

namespace GammaRay {

class TransitionModelPrivate;

class TransitionModel : public ObjectModelBase<QAbstractItemModel>
{
  public:
    explicit TransitionModel(QObject *parent = 0);
    ~TransitionModel();
    void setState(QAbstractState *state);
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int rowCount (const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;

  protected:
    Q_DECLARE_PRIVATE(TransitionModel)
    TransitionModelPrivate *const d_ptr;
};

}

#endif
