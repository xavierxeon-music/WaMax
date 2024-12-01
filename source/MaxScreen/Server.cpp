#include "Server.h"

#include <QApplication>
#include <QHostInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkInterface>
#include <QTimer>

#include <Convertor.h>
#include <LocalServer.h>

using ScreenServer = LocalServer<"MaxScreen">;

Server::Server(QObject* parent)
   : QLocalServer(parent)
   , socket(nullptr)
   , stackId(0)
   , rainbow(300)
   , screenSize()
   , tpMap()
   , imageDisplay(nullptr)
   , imageBuffer()
   , imageSize(-1)
{
   QTimer* colorTimer = new QTimer(this);
   connect(colorTimer, &QTimer::timeout, this, &Server::slotChangeColor);
   colorTimer->start(100);

   QTimer* updateTimer = new QTimer(this);
   connect(updateTimer, &QTimer::timeout, this, &Server::sendTouchPointUpdates);
   updateTimer->start(100);

   connect(this, &QLocalServer::newConnection, this, &Server::slotNewConnection);
   const QString socketName = ScreenServer::compileSocketName();
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
   socket = nextPendingConnection();
   connect(socket, &QLocalSocket::disconnected, this, &Server::slotSocketClosed);
   connect(socket, &QLocalSocket::readyRead, this, &Server::slotSocketRead);

   stackId = 1;
   emit signalStackIdChanged();

   QImage image(64, 64, QImage::Format_RGB32);
   image.fill(QColor(255, 255, 255));
   if (imageDisplay)
      imageDisplay->setBuffer(image);

   sendWindowSize();
}

void Server::slotSocketClosed()
{
   socket = nullptr;

   stackId = 0;
   emit signalStackIdChanged();
}

void Server::slotSocketRead()
{
   imageBuffer.append(socket->readAll());
   if (imageSize < 0)
   {
      Convertor<qsizetype> convertor;
      std::memcpy(convertor.bytes, imageBuffer.constData(), sizeof(qsizetype));

      imageBuffer = imageBuffer.mid(sizeof(qsizetype));
      imageSize = convertor.data;

      //qDebug() << imageSize;
   }

   if (imageBuffer.size() >= imageSize)
   {
      QByteArray imageData = imageBuffer.left(imageSize);
      imageBuffer = imageBuffer.mid(imageSize);
      imageSize = -1;

      QImage image = QImage::fromData(imageData);
      if (imageDisplay && !image.isNull())
         imageDisplay->setBuffer(image);
   }
}

void Server::slotChangeColor()
{
   rainbow.advanceColor();
   emit signalColorChanged();
}

void Server::sendTouchPointUpdates()
{
   if (socket.isNull())
      return;

   if (tpMap.isEmpty())
      return;

   QDataStream stream(socket);
   tpMap.dump(stream);
}

void Server::sendWindowSize()
{
   if (socket.isNull())
      return;

   QDataStream stream(socket);
   screenSize.dump(stream);
}
