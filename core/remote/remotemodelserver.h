/*
  remotemodelserver.h

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2013-2015 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Volker Krause <volker.krause@kdab.com>

  Licensees holding valid commercial KDAB GammaRay licenses may use this file in
  accordance with GammaRay Commercial License Agreement provided with the Software.

  Contact info@kdab.com if any conditions of this licensing are not clear to you.

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

#ifndef GAMMARAY_REMOTEMODELSERVER_H
#define GAMMARAY_REMOTEMODELSERVER_H

#include <common/protocol.h>

#include <QObject>
#include <QPointer>

class QBuffer;
class QAbstractItemModel;

namespace GammaRay {

class Message;

/** Provides the server-side interface for a QAbstractItemModel to be used from a separate process. */
class RemoteModelServer : public QObject
{
  Q_OBJECT
  public:
    /** Registers a new model server object with name @p objectName (must be unique). */
    explicit RemoteModelServer(const QString &objectName, QObject *parent = 0);
    ~RemoteModelServer();

    /** Returns the source model. */
    QAbstractItemModel* model() const;
    /** Set the source model for this model server instance. */
    void setModel(QAbstractItemModel *model);

  public slots:
    void newRequest(const GammaRay::Message &msg);
    /** Notifications about an object on the client side (un)monitoring this object.
     *  If no one is watching, we don't send out any change notification to reduce network traffice.
     */
    void modelMonitored(bool monitored = false);

  private:
    void connectModel();
    void disconnectModel();
    void sendAddRemoveMessage(Protocol::MessageType type, const QModelIndex &parent, int start, int end);
    void sendMoveMessage(Protocol::MessageType type, const Protocol::ModelIndex &sourceParent, int sourceStart, int sourceEnd, const Protocol::ModelIndex &destinationParent, int destinationIndex);
    QMap< int, QVariant > filterItemData(const QMap< int, QVariant >& data) const;
    bool canSerialize(const QVariant &value) const;

    // unit test hooks
    static void (*s_registerServerCallback)();
    void registerServer();
    virtual bool isConnected() const;
    virtual void sendMessage(const Message &msg) const;
    friend class FakeRemoteModelServer;

  private slots:
    void dataChanged(const QModelIndex &begin, const QModelIndex &end);
    void headerDataChanged(Qt::Orientation orientation, int first, int last);
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void rowsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationRow);
    void rowsMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationRow);
    void rowsRemoved(const QModelIndex &parent, int start, int end);
    void columnsInserted(const QModelIndex &parent, int start, int end);
    void columnsMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationColumn);
    void columnsRemoved(const QModelIndex &parent, int start, int end);
    void layoutChanged();
    void modelReset();

    void modelDeleted();

  private:
    QPointer<QAbstractItemModel> m_model;
    // those two are used for canSerialize, since recreating the QBuffer is somewhat expensive,
    // especially since being a QObject triggers all kind of GammaRay internals
    QByteArray m_dummyData;
    QBuffer *m_dummyBuffer;
    // converted model indexes from aboutToBeX signals, needed in cases where the operation changes
    // the serialized index (move to sub-tree of source parent for example)
    // as operations can occur nested, we need to have a stack for this
    QList<Protocol::ModelIndex> m_preOpIndexes;
    Protocol::ObjectAddress m_myAddress;
    bool m_monitored;
};

}

#endif
