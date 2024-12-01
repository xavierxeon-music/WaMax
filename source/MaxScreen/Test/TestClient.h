#ifndef TestClientH
#define TestClientH

#include <QObject>

#include <QLocalSocket>

#include "Data.h"

namespace Test
{
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
} // namespace Test

#endif // NOT TestClientH
