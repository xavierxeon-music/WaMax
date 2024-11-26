#include "MaxScreen.h"

#include <QGuiApplication>
#include <QHostInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkInterface>
#include <QQmlApplicationEngine>
#include <QTimer>

#include "ImageDisplay.h"

MaxScreen::MaxScreen(QObject* parent)
   : QTcpServer(parent)
   , socket(nullptr)
   , stackId(0)
   , rainbow(300)
   , screenSize(0, 0)
{
   QTimer* colorTimer = new QTimer(this);
   connect(colorTimer, &QTimer::timeout, this, &MaxScreen::slotChangeColor);
   colorTimer->start(100);

   connect(this, &QTcpServer::newConnection, this, &MaxScreen::slotNewConnection);
   listen(QHostAddress::Any, getPort());
}

int MaxScreen::getStackId() const
{
   return stackId;
}

QColor MaxScreen::getBgColor() const
{
   return Rainbow::getColor().darker(400);
}

QString MaxScreen::getHostName() const
{
   const QString host = QHostInfo::localHostName();
   if ("localhost" != host)
   {
      const QString domain = QHostInfo::localDomainName();
      if (!domain.isEmpty())
         return host + domain;

      if (!host.endsWith(".local"))
         return host + ".local";

      return host;
   }

   const QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
   for (const QHostAddress& address : addresses)
   {
      if (address.isLoopback())
         continue;

      if (address.protocol() != QAbstractSocket::IPv4Protocol)
         continue;

      if (!address.isGlobal())
         continue;

      return address.toString();
   }

   return host;
}

quint32 MaxScreen::getPort() const
{
   return 6667;
}

void MaxScreen::setWindowSize(int width, int height)
{
   screenSize = QSize(width, height);
   sendWindowSize();
}

void MaxScreen::touchPointsUpdated(const QList<QObject*>& touchPoints)
{
   static const QStringList propertyNames = {
      "pointId",
      //"pressed",
      "x",
      "y",
      //"pressure",
      //"velocity",
      //"startX",
      //"startY"
      //
   };

   QJsonArray data;
   for (QObject* touchPoint : touchPoints)
   {
      QJsonObject tp;
      for (const QString& propertyName : propertyNames)
      {
         const QVariant& value = touchPoint->property(propertyName.toUtf8());
         tp[propertyName] = value.toJsonValue();
      }

      data.append(tp);
   }

   QJsonObject object;
   object["payload"] = "TouchPoints";
   object["data"] = data;

   sendPackage(object);
}

void MaxScreen::slotNewConnection()
{
   socket = nextPendingConnection();
   connect(socket, &QTcpSocket::disconnected, this, &MaxScreen::slotSocketClosed);

   stackId = 1;
   emit signalStackIdChanged();

   sendWindowSize();
}

void MaxScreen::slotSocketClosed()
{
   socket = nullptr;

   stackId = 0;
   emit signalStackIdChanged();
}

void MaxScreen::slotSetDisplay(const QImage& image)
{
   stackId = 1;
   emit signalStackIdChanged();

   ImageDisplay::push(image);
}

void MaxScreen::slotResetDisplay()
{
   stackId = 0;
   emit signalStackIdChanged();
}

void MaxScreen::slotChangeColor()
{
   rainbow.advanceColor();
   emit signalColorChanged();
}

void MaxScreen::sendWindowSize()
{
   QJsonObject object;
   object["payload"] = "ScreenSize";
   object["width"] = screenSize.width();
   object["height"] = screenSize.height();

   sendPackage(object);
}

void MaxScreen::sendPackage(const QJsonObject& object)
{
   if (socket.isNull())
      return;

   QJsonDocument doc(object);
   socket->write(doc.toJson(QJsonDocument::Compact));
}
