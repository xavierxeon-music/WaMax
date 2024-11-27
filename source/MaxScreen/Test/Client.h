#ifndef ClientH
#define ClientH

#include <QTcpSocket>

#include "ScreenSize.h"
#include "TouchPoint.h"

class Client : public QTcpSocket
{
   Q_OBJECT

public:
   Client(bool local = false);

private slots:
   void slotTryConnect();
   void slotStateChanged(QAbstractSocket::SocketState socketState);
   void slotReadData();

private:
   bool local;
   ScreenSize screenSize;
   TouchPoint::Map tpMap;
};

#endif // NOT ClientH
