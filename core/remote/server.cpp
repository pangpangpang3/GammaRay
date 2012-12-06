#include "server.h"
#include "probe.h"

#include <network/protocol.h>
#include <network/message.h>

#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>

using namespace GammaRay;

Server::Server(QObject *parent) : 
  Endpoint(parent),
  m_tcpServer(new QTcpServer(this)),
  m_nextAddress(Protocol::InvalidObjectAddress + 1),
  m_myAddress(Protocol::InvalidObjectAddress +1 )
{
  connect(m_tcpServer, SIGNAL(newConnection()), SLOT(newConnection()));
  m_tcpServer->listen(QHostAddress::Any, defaultPort());
}

Server::~Server()
{
}

Server* Server::instance()
{
  Q_ASSERT(s_instance);
  return static_cast<Server*>(s_instance);
}


void Server::newConnection()
{
  if (isConnected()) {
    qDebug() << Q_FUNC_INFO << "connected already, refusing incoming connection.";
    m_tcpServer->nextPendingConnection()->close();
    return;
  }

  qDebug() << Q_FUNC_INFO;
  setDevice(m_tcpServer->nextPendingConnection());

  // send greeting message for protocol version check
  {
    Message msg(m_myAddress);
    msg.stream() << Protocol::ServerVersion << Protocol::version();
    stream() << msg;
  }

  {
    Message msg(m_myAddress);
    msg.stream() << Protocol::ObjectMapReply << objectAddresses();
    stream() << msg;
  }
}

void Server::messageReceived(const Message& msg)
{
  dispatchMessage(msg);
}

Protocol::ObjectAddress Server::registerObject(const QString& objectName, QObject* receiver, const char* messageHandlerName)
{
  registerObjectInternal(objectName, ++m_nextAddress);
  Q_ASSERT(m_nextAddress);
  registerMessageHandlerInternal(m_nextAddress, receiver, messageHandlerName);
  m_objectToNameMap.insert(receiver, objectName);
  connect(receiver, SIGNAL(destroyed(QObject*)), SLOT(objectDestroyed(QObject*)));

  if (isConnected()) {
    Message msg(m_myAddress);
    msg.stream() << Protocol::ObjectAdded << objectName << m_nextAddress;
    stream() << msg;
  }

  return m_nextAddress;
}

void Server::objectDestroyed(QObject* object)
{
  const QString objectName = m_objectToNameMap.value(object);
  m_objectToNameMap.remove(object);
  const Protocol::ObjectAddress addr = objectAddress(objectName);
  m_messageHandlers.remove(addr);
  unregisterObjectInternal(objectName);

  if (isConnected()) {
    Message msg(m_myAddress);
    msg.stream() << Protocol::ObjectRemoved << objectName;
    stream() << msg;
  }
}


#include "server.moc"