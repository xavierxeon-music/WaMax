#include "TestClient.h"

#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <HelpForMax.h>

TestClient::TestClient()
   : QDialog(nullptr)
   , socketMap()
{
   setupUi(this);

   selectTree->setHeaderLabels({"Patch"});
   selectTree->setRootIsDecorated(false);

   auto addItem = [&](const QString& patchPath)
   {
      QFileInfo patchInfo(patchPath);
      const QString patchName = patchInfo.fileName().replace(".maxpat", "");

      QTreeWidgetItem* item = new QTreeWidgetItem(selectTree);
      item->setText(0, patchName);
      item->setData(0, Qt::UserRole + 1, patchPath);
   };

   const QString packageDir = "/Users/waspe/GitHub/MusicProjects/WaMaxPackage";
   addItem(packageDir + "/patchers/hardware/wa.grid.pot.maxpat");
   addItem(packageDir + "/patchers/audio/wa.wave_terrain~.maxpat");
   addItem(packageDir + "/patchers/mixer/wa.channel_strip.master~.maxpat");

   connect(selectTree, &QTreeWidget::currentItemChanged, this, &TestClient::slotSelectItemChanged);
}

void TestClient::slotSelectItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
   Q_UNUSED(previous)

   const QString path = current->data(0, Qt::UserRole + 1).toString();

   sendData(path);
}

void TestClient::sendData(const QString& patchPath)
{
   if (!socketMap.contains(patchPath))
   {
      QLocalSocket* socket = new QLocalSocket(this);
      socketMap.insert(patchPath, socket);

      auto receiveFunction = std::bind(&TestClient::receiveData, this, socket);
      connect(socket, &QLocalSocket::readyRead, receiveFunction);

      if (!HelpForMax::isServerActive())
         HelpForMax::startApplication();

      socket->connectToServer(HelpForMax::compileSockerName());
      socket->waitForConnected();
   }

   QLocalSocket* socket = socketMap[patchPath];
   socket->waitForConnected();

   QJsonObject object;
   object["patch"] = patchPath;

   QJsonDocument doc(object);
   socket->write(doc.toJson(QJsonDocument::Compact));
}

void TestClient::receiveData(QLocalSocket* socket)
{
   const QJsonDocument doc = QJsonDocument::fromJson(socket->readAll());
   const QJsonObject object = doc.object();

   //const QString path = object["path"].toString();

   resultEdit->append(doc.toJson());
}
