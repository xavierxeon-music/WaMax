#include "PackageModel.h"

#include <QDir>
#include <QFileInfo>
#include <QSettings>

#include "FileRef.h"
#include "PackageInfo.h"

Package::Model::Model(QObject* parent, const Info* packageInfo)
   : QStandardItemModel(parent)
   , packageInfo(packageInfo)
{
}

void Package::Model::create()
{
   setHorizontalHeaderLabels({"Folder / Patch Name"});

   using FolderMap = QMap<QString, QStandardItem*>;
   FolderMap folderMap;

   QDir packageDir(packageInfo->getPath() + "/patchers");
   for (const QFileInfo& folderInfo : packageDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name))
   {
      QDir patcherDir(folderInfo.canonicalFilePath());
      for (const QFileInfo& patchFileInfo : patcherDir.entryInfoList(QDir::Files, QDir::Name))
      {
         if (!patchFileInfo.fileName().endsWith(".maxpat"))
            continue;

         if (patchFileInfo.fileName().startsWith("_"))
            continue;

         const QString patchPath = patchFileInfo.absoluteFilePath();
         Patch::Info patchInfo = packageInfo->extractPatchInfo(patchPath);

         QString dirName = patchFileInfo.absolutePath();
         int index = dirName.lastIndexOf("/");
         if (index < 0)
            continue;
         dirName = dirName.mid(index + 1);

         if (!folderMap.contains(dirName))
         {
            QStandardItem* folderItem = new QStandardItem(dirName);
            folderItem->setEditable(false);
            invisibleRootItem()->appendRow(folderItem);

            folderMap[dirName] = folderItem;

            static const QIcon closedIcon(":/TreeClosed.svg");
            folderItem->setIcon(closedIcon);
         }

         QStandardItem* dirItem = folderMap[dirName];

         QStandardItem* patchItem = new QStandardItem(patchInfo.name);
         patchItem->setEditable(false);
         dirItem->appendRow(patchItem);

         patchItem->setData(patchPath, RolePath);
         patchItem->setData(QVariant::fromValue(patchInfo), RoleInfo);
      }
   }

   update();
}

void Package::Model::update()
{
   static const QIcon currentIcon(":/HelpCurrent.svg");
   static const QIcon outdatedIcon(":/HelpOutdated.svg");

   for (int folderRow = 0; folderRow < invisibleRootItem()->rowCount(); folderRow++)
   {
      QStandardItem* folderItem = invisibleRootItem()->child(folderRow, 0);
      for (int patchRow = 0; patchRow < folderItem->rowCount(); patchRow++)
      {
         QStandardItem* patchItem = folderItem->child(patchRow, 0);

         const QString path = patchItem->data(RolePath).toString();
         const Patch::Info patchInfo = patchItem->data(RoleInfo).value<Patch::Info>();
         const QString refPath = File::Ref(nullptr, packageInfo).getFilePath(patchInfo);

         const QDateTime patchTime = QFileInfo(path).lastModified();
         const QDateTime refTime = QFileInfo(refPath).lastModified();

         const bool upToDate = refTime >= patchTime;

         patchItem->setIcon(upToDate ? currentIcon : outdatedIcon);
      }
   }
}

QModelIndex Package::Model::find(const QString& patchFileName) const
{
   for (int folderRow = 0; folderRow < invisibleRootItem()->rowCount(); folderRow++)
   {
      QStandardItem* folderItem = invisibleRootItem()->child(folderRow, 0);

      for (int patchRow = 0; patchRow < folderItem->rowCount(); patchRow++)
      {
         QStandardItem* patchItem = folderItem->child(patchRow, 0);
         if (patchItem->data(RolePath).toString() == patchFileName)
            return patchItem->index();
      }
   }

   return QModelIndex();
}
