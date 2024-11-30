#include "ScreenServer.h"

#include <QApplication>
#include <QHostInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkInterface>
#include <QTimer>

#include "Convertor.h"
#include "ImageDisplay.h"
#include "MainWindow.h"

ScreenServer::ScreenServer(QObject* parent)
   : QTcpServer(parent)
   , socket(nullptr)
   , stackId(0)
   , rainbow(300)
   , screenSize()
   , tpMap()
   , imageBuffer()
   , imageSize(-1)
{
   QTimer* colorTimer = new QTimer(this);
   connect(colorTimer, &QTimer::timeout, this, &ScreenServer::slotChangeColor);
   colorTimer->start(100);

   QTimer* updateTimer = new QTimer(this);
   connect(updateTimer, &QTimer::timeout, this, &ScreenServer::sendTouchPointUpdates);
   updateTimer->start(100);

   connect(this, &QTcpServer::newConnection, this, &ScreenServer::slotNewConnection);
   listen(QHostAddress::Any, getPort());
}

int ScreenServer::getStackId() const
{
   return stackId;
}

QColor ScreenServer::getBgColor() const
{
   return Rainbow::getColor().darker(400);
}

QString ScreenServer::getHostName() const
{
   const QString host = QHostInfo::localHostName();
   if ("localhost" != host)
   {
      const QString domain = QHostInfo::localDomainName();
      if (!domain.isEmpty())
         return host + domain;

      if (!host.endsWith(".local"))
         return host + ".local";

      return host;
   }

   const QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
   for (const QHostAddress& address : addresses)
   {
      if (address.isLoopback())
         continue;

      if (address.protocol() != QAbstractSocket::IPv4Protocol)
         continue;

      if (!address.isGlobal())
         continue;

      return address.toString();
   }

   return host;
}

quint32 ScreenServer::getPort() const
{
   return 6667;
}

void ScreenServer::setWindowSize(int width, int height)
{
   screenSize.update(width, height);
   sendWindowSize();
}

void ScreenServer::touchPointsUpdated(const QList<QObject*>& touchPoints)
{
   tpMap.startUpdates();
   for (QObject* touchPoint : touchPoints)
   {
      tpMap.update(touchPoint);
   }
}

void ScreenServer::toogleFullScreen()
{
   emit signalToolgeFullScreen();
}

void ScreenServer::slotNewConnection()
{
   socket = nextPendingConnection();
   connect(socket, &QTcpSocket::disconnected, this, &ScreenServer::slotSocketClosed);
   connect(socket, &QTcpSocket::readyRead, this, &ScreenServer::slotSocketRead);

   stackId = 1;
   emit signalStackIdChanged();

   QImage image(64, 64, QImage::Format_RGB32);
   image.fill(QColor(255, 255, 255));
   ImageDisplay::push(image);

   sendWindowSize();
}

void ScreenServer::slotSocketClosed()
{
   socket = nullptr;

   stackId = 0;
   emit signalStackIdChanged();
}

void ScreenServer::slotSocketRead()
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
      if (!image.isNull())
         ImageDisplay::push(image);
   }
}

void ScreenServer::slotChangeColor()
{
   rainbow.advanceColor();
   emit signalColorChanged();
}

void ScreenServer::sendTouchPointUpdates()
{
   if (socket.isNull())
      return;

   if (tpMap.isEmpty())
      return;

   QDataStream stream(socket);
   tpMap.dump(stream);
}

void ScreenServer::sendWindowSize()
{
   if (socket.isNull())
      return;

   QDataStream stream(socket);
   screenSize.dump(stream);
}
