#include "Client.h"

#include <QCoreApplication>
#include <QTimer>

Client::Client(bool local)
   : QTcpSocket(nullptr)
   , local(local)
   , screenSize()
   , tpMap()
{
   connect(this, &QTcpSocket::stateChanged, this, &Client::slotStateChanged);
   connect(this, &QTcpSocket::readyRead, this, &Client::slotReadData);

   slotTryConnect();
}

void Client::slotTryConnect()
{
   connectToHost(local ? "127.0.0.1" : "192.168.1.106", 6667);
}

void Client::slotStateChanged(QAbstractSocket::SocketState socketState)
{
   if (QAbstractSocket::UnconnectedState != socketState)
      return;

   QTimer::singleShot(5000, this, &Client::slotTryConnect);
}

void Client::slotReadData()
{
   QDataStream stream(this);
   char marker;
   stream >> marker;

   if ('t' == marker)
      tpMap.load(stream);
   else if ('s' == marker)
      screenSize.load(stream);

   readAll();
}

// main function

int main(int argc, char** argv)
{
   QCoreApplication app(argc, argv);

   const bool local = (argc > 1);
   Client client(local);

   return app.exec();
}
