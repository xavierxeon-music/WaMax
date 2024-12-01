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
{
   socket = new QLocalSocket(this);
   connect(socket, &QLocalSocket::readyRead, this, &Client::slotReceiveData);
}

inline void Client::connectToServer()
{
   const QString socketName = ScreenServer::compileSocketName();
   qDebug() << "Client @" << socketName << ScreenServer::isServerActive();

   socket->connectToServer(socketName);
   socket->waitForConnected();

   if (QLocalSocket::ConnectedState != socket->state())
      qDebug() << socket->state() << socket->errorString();
}

inline void Client::sendImage(const QImage& image)
{
   QByteArray block;
   {
      QBuffer writer(&block);
      image.save(&writer, "PNG");
   }

   Convertor<qsizetype> convertor;
   convertor.data = block.size();
   //cout << "image size " << convertor.data << endl;

   socket->write(convertor.bytes, sizeof(qsizetype));
   socket->write(block);
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
