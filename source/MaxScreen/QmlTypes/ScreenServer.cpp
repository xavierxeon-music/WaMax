#include "ScreenServer.h"

#include <QApplication>
#include <QHostInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkInterface>
#include <QTimer>

#include <Convertor.h>
#include <LocalServer.h>

using MaxScreenServer = LocalServer<"MaxScreen">;

ScreenServer::ScreenServer(QObject* parent)
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
   connect(colorTimer, &QTimer::timeout, this, &ScreenServer::slotChangeColor);
   colorTimer->start(100);

   QTimer* updateTimer = new QTimer(this);
   connect(updateTimer, &QTimer::timeout, this, &ScreenServer::sendTouchPointUpdates);
   updateTimer->start(100);

   connect(this, &QLocalServer::newConnection, this, &ScreenServer::slotNewConnection);
   const QString socketName = MaxScreenServer::compileSocketName();
   qDebug() << "Server @" << socketName;
   listen(socketName);
}

int ScreenServer::getStackId() const
{
   return stackId;
}

QColor ScreenServer::getBgColor() const
{
   return Rainbow::getColor().darker(400);
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

void ScreenServer::setImageDisplay(QObject* displayObject)
{
   imageDisplay = qobject_cast<ImageDisplay*>(displayObject);
}

void ScreenServer::slotNewConnection()
{
   socket = nextPendingConnection();
   connect(socket, &QLocalSocket::disconnected, this, &ScreenServer::slotSocketClosed);
   connect(socket, &QLocalSocket::readyRead, this, &ScreenServer::slotSocketRead);

   stackId = 1;
   emit signalStackIdChanged();

   QImage image(64, 64, QImage::Format_RGB32);
   image.fill(QColor(255, 255, 255));
   if (imageDisplay)
      imageDisplay->setBuffer(image);

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
      if (imageDisplay && !image.isNull())
         imageDisplay->setBuffer(image);
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
