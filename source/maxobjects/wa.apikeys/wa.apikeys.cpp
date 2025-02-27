#include "wa.apikeys.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QStandardPaths>

#include <MaxPatcher.h>

ApiKeys::ApiKeys(const atoms& args)
   : object<ApiKeys>()
   , input(this, "content")
   , query(this, "query")
   , content(this, "content", "dictionary")
   , bangMessage(this, "bang", "output dict", Max::Patcher::minBind(this, &ApiKeys::contentFunction))
   , queryMessage(this, "query", "query dict", Max::Patcher::minBind(this, &ApiKeys::queryFunction))
   , apiContent()
   , dictApi(symbol(true))
{
   if (args.size() > 0)
   {
      const std::string what = args[0];
      readApiKeys(QString::fromStdString(what));
   }
}

atoms ApiKeys::contentFunction(const atoms& args, const int inlet)
{
   content.send("dictionary", dictApi.name());
   return {};
}

atoms ApiKeys::queryFunction(const atoms& args, const int inlet)
{
   const std::string key = args[0];
   const QString qkey = QString::fromStdString(key);
   if (!apiContent.contains(qkey))
      return {};

   const QString qvalue = apiContent[qkey].toString();
   const std::string value = qvalue.toStdString();
   query.send(value);

   return {};
}

void ApiKeys::readApiKeys(const QString& what)
{
   const QStringList paths = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
   const QString path = paths.at(0) + "/.ApiKeys/" + what + ".json";

   QFile file(path);
   if (!file.open(QIODevice::ReadOnly))
      return;

   const QByteArray data = file.readAll();
   file.close();

   QJsonParseError error;
   const QJsonDocument doc = QJsonDocument::fromJson(data, &error);
   if (error.error != QJsonParseError::NoError)
      return;

   apiContent = doc.object();
   for (QJsonObject::const_iterator it = apiContent.constBegin(); it != apiContent.constEnd(); it++)
   {
      const QString qkey = it.key();
      const QString qvalue = it.value().toString();

      const std::string key = qkey.toStdString();
      const std::string value = qvalue.toStdString();

      dictApi[key] = value;
   }
}

MIN_EXTERNAL(ApiKeys);
