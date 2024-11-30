#ifndef TestClientH
#define TestClientH

#include <QObject>

#include <QLocalSocket>

namespace Test
{
   class Client : public QObject
   {
      Q_OBJECT

   public:
      Client(QObject* parent);

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
