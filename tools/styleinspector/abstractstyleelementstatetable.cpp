/*
  abstractstyleelementstatetable.cpp

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2012 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
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

#include "abstractstyleelementstatetable.h"
#include "styleoption.h"

#include <qpainter.h>

using namespace GammaRay;

AbstractStyleElementStateTable::AbstractStyleElementStateTable(QObject* parent): AbstractStyleElementModel(parent), m_cellWidth(64), m_cellHeight(64)
{
}

int AbstractStyleElementStateTable::doColumnCount() const
{
  return 1 + StyleOption::stateCount();
}

QVariant AbstractStyleElementStateTable::doData(int row, int column, int role) const
{
  Q_UNUSED(row);
  if (role == Qt::SizeHintRole && column > 0)
    return QSize(cellWidth() + 4, cellHeight() + 4);
  return QVariant();
}

QVariant AbstractStyleElementStateTable::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (section > 0 && orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return StyleOption::stateDisplayName(section - 1);
  return QAbstractItemModel::headerData(section, orientation, role);
}

void AbstractStyleElementStateTable::drawTransparencyBackground(QPainter* painter, const QRect& rect) const
{
  QPixmap bgPattern(16,16);
  bgPattern.fill(Qt::lightGray);
  QPainter bgPainter(&bgPattern);
  bgPainter.fillRect(8, 0, 8, 8, Qt::gray);
  bgPainter.fillRect(0, 8, 8, 8, Qt::gray);

  QBrush bgBrush;
  bgBrush.setTexture(bgPattern);
  painter->fillRect(rect, bgBrush);
}

int AbstractStyleElementStateTable::cellWidth() const
{
  return m_cellWidth;
}

int AbstractStyleElementStateTable::cellHeight() const
{
  return m_cellHeight;
}

void AbstractStyleElementStateTable::setCellWidth(int width)
{
  m_cellWidth = width;
  if (rowCount() > 0)
    emit dataChanged(index(0, 0), index(doRowCount() - 1, doColumnCount() - 1) );
}

void AbstractStyleElementStateTable::setCellHeight(int height)
{
  m_cellHeight = height;
  if (rowCount() > 0)
    emit dataChanged(index(0, 0), index(doRowCount() - 1, doColumnCount() - 1));
}

#include "abstractstyleelementstatetable.moc"
