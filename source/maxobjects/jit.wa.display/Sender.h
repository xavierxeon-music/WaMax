#ifndef SenderH
#define SenderH

#include <QThread>

#include <QNetworkAccessManager>

class Sender : public QThread
{
   //Q_OBJECT

public:
   Sender();

public:
   void run() override;
   void sendImage(const QImage& image, const QString& name);

private:
   enum class Mode
   {
      Wait,
      Init,
      Info,
      Upload
   };

   struct SetInfo
   {
      QString id;
      QMap<QString, QString> images; // name vs id

      using Map = QMap<QString, SetInfo>; // name vs set
   };

private:
   QNetworkRequest createRequest(const QString& endpoint);
   void compileInfo(const QJsonObject& info);
   void updateInfo(const QJsonObject& info);

private:
   QByteArray bearerToken;

   QNetworkAccessManager sender;
   QNetworkReply* reply;

   SetInfo::Map setInfoMap;
   Mode mode;

   bool active;
};

#endif // NOT SenderH
