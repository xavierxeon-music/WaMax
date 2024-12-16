#ifndef ClientHPP
#define ClientHPP

#include "Client.h"

#include <QBuffer>
#include <QImage>

#include <Convertor.h>

#include "Marker.h"

inline Client::Client(QObject* parent)
   : QObject(parent)
   , Data()
   , socket(nullptr)
   , memoryPublisher(true)
   , image()
{
   socket = new QLocalSocket(this);
   connect(socket, &QLocalSocket::readyRead, this, &Client::slotReceiveData);
}

inline void Client::connectToServer()
{
   const QString socketName = ScreenServer::socketName();
   qDebug() << "Client @" << socketName << ScreenServer::isServerActive();

   socket->connectToServer(socketName);
   socket->waitForConnected();

   if (QLocalSocket::ConnectedState != socket->state())
      qDebug() << socket->state() << socket->errorString();
}

inline void Client::sendImage(const QString& fileName)
{
   QImage localImage(fileName);
   if (localImage.isNull() || !getImageSize().isValid())
      return;

   localImage = localImage.scaled(getImageSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

   int xOffset = (localImage.width() < image.width()) ? 0.5 * (image.width() - localImage.width()) : 0;
   int yOffset = (localImage.height() < image.height()) ? 0.5 * (image.height() - localImage.height()) : 0;

   for (int x = 0; x < localImage.width(); x++)
   {
      for (int y = 0; y < localImage.height(); y++)
      {
         const QColor color = localImage.pixelColor(x, y);
         image.setPixelColor(x + xOffset, y, color);
      }
   }

   socket->write(&Marker::Image, 1);
}

inline void Client::slotReceiveData()
{
   QDataStream stream(socket);

   char marker;
   stream >> marker;

   if (Marker::TouchPoint == marker)
   {
      tpMap.load(stream);
      emit signalTouchPointsChanged(tpMap);
   }
   else if (Marker::ScreenSize == marker)
   {
      screenSize.load(stream);
      image = memoryPublisher.createNew(screenSize.getWidth(), screenSize.getHeight());
      image.fill(QColor(0, 0, 0, 0));

      emit signalSizeChanged(screenSize);
   }

   socket->readAll();
}

#endif // NOT ClientHPP
