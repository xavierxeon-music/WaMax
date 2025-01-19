#include "wa.hostname.h"

#include <QHostInfo>
#include <QNetworkInterface>

#include <MaxPatcher.h>

HostName::HostName(const atoms& args)
   : object<HostName>()
   , input(this, "bang")
   , outputHostName(this, "hostname")
   , outputIp(this, "ip")
   , bang(this, "bang", "get hostname and ip", Max::Patcher::minBind(this, &HostName::bangFunction))
{
}

atoms HostName::bangFunction(const atoms& args, const int inlet)
{
   const QString hostName = QHostInfo::localHostName();
   outputHostName.send(hostName.toStdString());

   atoms ipList;
   const QHostAddress& localhost = QHostAddress(QHostAddress::LocalHost);
   for (const QHostAddress& address : QNetworkInterface::allAddresses())
   {
      if (address.protocol() != QAbstractSocket::IPv4Protocol)
         continue;

      if (address == localhost)
         continue;

      ipList.push_back(address.toString());
   }

   outputIp.send(ipList);

   return {};
}

MIN_EXTERNAL(HostName);
