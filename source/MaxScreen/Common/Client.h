#ifndef ClientH
#define ClientH

#include <QObject>

#include <QLocalSocket>

#include "Data.h"
#include "SharedImage.h"

class Client : public QObject, public Data
{
   Q_OBJECT

public:
   Client(QObject* parent);

signals:
   void signalSizeChanged(const ImageSize& imageSize);
   void signalTouchPointsChanged(const TouchPoint::Map& touchPointMap);

public:
   void connectToServer();
   void sendImage(const QString& fileName);

private slots:
   void slotReceiveData();

private:
   QLocalSocket* socket;
   SharedImage memoryPublisher;
   QImage image;
};

#ifndef ClientHPP
#include "Client.hpp"
#endif // NOT ClientHPP

#endif // NOT ClientH
