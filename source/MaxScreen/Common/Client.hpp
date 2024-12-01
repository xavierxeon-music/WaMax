#ifndef ClientHPP
#define ClientHPP

#include "Client.h"

#include <QBuffer>
#include <QImage>

#include <Convertor.h>

inline Client::Client(QObject* parent)
   : QObject(parent)
   , Data()
   , socket(nullptr)
   , publisherImage(true)
{
   socket = new QLocalSocket(this);
   connect(socket, &QLocalSocket::readyRead, this, &Client::slotReceiveData);
}

inline void Client::connectToServer()
{
   const QString socketName = ScreenServer::compileSharedFileName("socket");
   qDebug() << "Client @" << socketName << ScreenServer::isServerActive();

   socket->connectToServer(socketName);
   socket->waitForConnected();

   if (QLocalSocket::ConnectedState != socket->state())
      qDebug() << socket->state() << socket->errorString();
}

inline void Client::sendImage(const QString& fileName)
{
   publisherImage.createFromFile(fileName, getScreenSize());

   static const QByteArray marker("i");
   socket->write(marker);
}

inline void Client::slotReceiveData()
{
   QDataStream stream(socket);

   char marker;
   stream >> marker;

   if ('t' == marker)
   {
      tpMap.load(stream);
   }
   else if ('s' == marker)
   {
      screenSize.load(stream);
      emit signalSizeChanged(screenSize);
   }

   socket->readAll();
}

#endif // NOT ClientHPP
