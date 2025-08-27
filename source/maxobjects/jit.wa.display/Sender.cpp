#include "Sender.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QStandardPaths>

#include <FileTools.h>

Sender::Sender()
   : QThread(nullptr)
   , bearerToken()
   , sender(nullptr)
   , reply(nullptr)
   , setInfoMap()
   , mode(Mode::Init)
   , active(false)
{
   const QStringList paths = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
   const QString path = paths.at(0) + "/.ApiKeys/flexdisplay.json";

   const QJsonObject apiContent = FileTools::readJson(path);
   QByteArray bearerToken = apiContent["token"].toString().toUtf8();
}

void Sender::run()
{
   active = true;
   while (active)
   {
      if (reply)
      {
         if (reply->isFinished())
         {
            const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            const QByteArray replyContent = reply->readAll();
            reply->deleteLater();

            const QJsonObject content = FileTools::parseBytes(replyContent);
            reply = nullptr;
         }
      }
      else
      {
         if (Mode::Init == mode)
         {
            QNetworkRequest request = createRequest("/api/info");
            reply = sender.post(request, QByteArray());
            mode = Mode::Info;
         }
      }
   }
}

void Sender::sendImage(const QImage& image, const QString& name)
{
   if (Mode::Upload != mode)
      return;
}

QNetworkRequest Sender::createRequest(const QString& endpoint)
{
   static const QString baseUrl("https://schweinesystem.ddns.net/flexdisplay");

   QNetworkRequest request(QUrl(baseUrl + endpoint));
   request.setRawHeader("Authorization", "Bearer " + bearerToken);
   request.setRawHeader("Accept", "application/json");
   return request;
}

void Sender::compileInfo(const QJsonObject& info)
{
   for (QJsonObject::const_iterator iterSet = info.constBegin(); iterSet != info.constEnd(); iterSet++)
   {
      const QString& setId = iterSet.key();
      const QJsonObject setObject = iterSet.value().toObject();

      SetInfo setInfo;
      setInfo.id = setId;

      const QJsonObject imagesObject = setObject["images"].toObject();
      for (QJsonObject::const_iterator iterImage = imagesObject.constBegin(); iterImage != imagesObject.constEnd(); iterImage++)
      {
         const QString& imageId = iterImage.key();
         const QString imageName = iterImage.value().toString();
         setInfo.images.insert(imageName, imageId);
      }
      const QString setName = setObject["name"].toString();
      setInfoMap.insert(setName, setInfo);
   }

   for (SetInfo::Map::const_iterator iterSet = setInfoMap.constBegin(); iterSet != setInfoMap.constEnd(); iterSet++)
   {
      const QString& setName = iterSet.key();
      const SetInfo& setInfo = iterSet.value();
      const QStringList imageNames = setInfo.images.keys();

      //cout << "Set: " << setName.toStdString() << ", Images: " << imageNames.join(", ").toStdString() << endl;
   }
}

void Sender::updateInfo(const QJsonObject& info)
{
}