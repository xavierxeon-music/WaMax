#include "wa.hostname.h"

#include <QHostInfo>
#include <QNetworkInterface>

#include <MaxPatcher.h>

HostName::HostName(const atoms& args)
   : object<HostName>()
   , input(this, "bang")
   , outputHostName(this, "hostname")
   , outputIp(this, "ip list")
   , outputInterface(this, "interface list")
   , bang(this, "bang", "get hostname and ip", Max::Patcher::minBind(this, &HostName::bangFunction))
{
}

atoms HostName::bangFunction(const atoms& args, const int inlet)
{
   const QString hostName = QHostInfo::localHostName();
   outputHostName.send(hostName.toStdString());

   atoms ipList;
   atoms interfaceList;

   const QHostAddress& localhost = QHostAddress(QHostAddress::LocalHost);
   const QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
   for (const QNetworkInterface& interface : interfaces)
   {
      const QString name = interface.humanReadableName();
      const QList<QNetworkAddressEntry> entries = interface.addressEntries();

      for (const QNetworkAddressEntry& entry : entries)
      {
         const QHostAddress& address = entry.ip();
         if (address.protocol() != QAbstractSocket::IPv4Protocol)
            continue;

         if (address == localhost)
            continue;

         const QString ip = address.toString();
         ipList.push_back(ip.toStdString());
         interfaceList.push_back(name.toStdString());
      }
   }

   outputIp.send(ipList);
   outputInterface.send(interfaceList);

   return {};
}

MIN_EXTERNAL(HostName);
