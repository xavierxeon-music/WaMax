#include "Server.h"

#include <QApplication>
#include <QHostInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkInterface>
#include <QTimer>

#include <Convertor.h>

Server::Server(QObject* parent)
   : QLocalServer(parent)
   , Data()
   , socketList()
   , stackId(0)
   , rainbow(300)
   , imageDisplay(nullptr)
{
   QTimer* colorTimer = new QTimer(this);
   connect(colorTimer, &QTimer::timeout, this, &Server::slotChangeColor);
   colorTimer->start(100);

   QTimer* updateTimer = new QTimer(this);
   connect(updateTimer, &QTimer::timeout, this, &Server::sendTouchPointUpdates);
   updateTimer->start(100);

   connect(this, &QLocalServer::newConnection, this, &Server::slotNewConnection);
   const QString socketName = ScreenServer::socketName();
   if (QFile::exists(socketName))
      QFile::remove(socketName);

   qDebug() << "Server @" << socketName;
   listen(socketName);
}

int Server::getStackId() const
{
   return stackId;
}

QColor Server::getBgColor() const
{
   return Rainbow::getColor().darker(400);
}

void Server::setWindowSize(int width, int height)
{
   screenSize.update(width, height);
   sendWindowSize();
}

void Server::touchPointsUpdated(const QList<QObject*>& touchPoints)
{
   tpMap.startUpdates();
   for (QObject* touchPoint : touchPoints)
   {
      tpMap.update(touchPoint);
   }
}

void Server::toogleFullScreen()
{
   emit signalToolgeFullScreen();
}

void Server::setImageDisplay(QObject* displayObject)
{
   imageDisplay = qobject_cast<ImageDisplay*>(displayObject);
}

void Server::slotNewConnection()
{
   QLocalSocket* socket = nextPendingConnection();
   connect(socket, &QLocalSocket::disconnected, this, &Server::slotSocketClosed);
   connect(socket, &QLocalSocket::readyRead, this, &Server::slotSocketRead);

   socketList.append(socket);

   stackId = 1;
   emit signalStackIdChanged();

   sendWindowSize();
}

void Server::slotSocketClosed()
{
   QLocalSocket* source = qobject_cast<QLocalSocket*>(sender());

   socketList.removeAll(source);

   if (socketList.isEmpty())
   {
      stackId = 0;
      emit signalStackIdChanged();
   }
}

void Server::slotSocketRead()
{
   QLocalSocket* source = qobject_cast<QLocalSocket*>(sender());

   source->readAll();

   if (imageDisplay)
      imageDisplay->attach();
}

void Server::slotChangeColor()
{
   rainbow.advanceColor();
   emit signalColorChanged();
}

void Server::sendTouchPointUpdates()
{
   if (tpMap.isEmpty())
      return;

   for (SocketPointer& socket : socketList)
   {
      if (socket.isNull())
         continue;

      QDataStream stream(socket);
      tpMap.dump(stream);
   }
}

void Server::sendWindowSize()
{
   for (SocketPointer& socket : socketList)
   {
      if (socket.isNull())
         continue;

      if (imageDisplay)
         imageDisplay->detach();

      QDataStream stream(socket);
      screenSize.dump(stream);
   }
}
