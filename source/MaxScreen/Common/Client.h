#ifndef ClientH
#define ClientH

#include <QObject>

#include <QLocalSocket>

#include "Data.h"

class Client : public QObject, private Data
{
   Q_OBJECT

public:
   Client(QObject* parent);

signals:
   void signalSizeChanged(const Size& screenSize);

public:
   void connectToServer();
   void sendImage(const QImage& image);

private slots:
   void slotReceiveData();

private:
   QLocalSocket* socket;
};

#ifndef ClientHPP
#include "Client.hpp"
#endif // NOT ClientHPP

#endif // NOT ClientH
