#include "TestClient.h"

#include <QBuffer>
#include <QImage>

#include <Convertor.h>
#include <LocalServer.h>

using MaxScreenServer = LocalServer<"MaxScreen">;

Test::Client::Client(QObject* parent)
   : QObject(parent)
   , socket(nullptr)
{
   socket = new QLocalSocket(this);
   connect(socket, &QLocalSocket::readyRead, this, &Client::slotReceiveData);
}

void Test::Client::connectToServer()
{
   const QString socketName = MaxScreenServer::compileSocketName();
   qDebug() << "Client @" << socketName << MaxScreenServer::isServerActive();

   socket->connectToServer(socketName);
   socket->waitForConnected();

   if (QLocalSocket::ConnectedState != socket->state())
      qDebug() << socket->state() << socket->errorString();
}

void Test::Client::sendImage(const QImage& image)
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

void Test::Client::slotReceiveData()
{
}
