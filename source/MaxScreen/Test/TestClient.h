#ifndef TestClientH
#define TestClientH

#include <QObject>

#include <QLocalSocket>
#include <QSharedMemory>

#include "ScreenSize.h"
#include "TouchPoint.h"

namespace Test
{
   class Client : public QObject
   {
      Q_OBJECT

   public:
      Client(QObject* parent);

   signals:
      void signalSizeChanged(const ScreenSize& screenSize);

   public:
      void connectToServer();
      void sendImage(const QImage& image);

   private slots:
      void slotReceiveData();

   private:
      QLocalSocket* socket;
      ScreenSize screenSize;
      TouchPoint::Map tpMap;
      QSharedMemory imageMemory;
   };
} // namespace Test

#endif // NOT TestClientH
