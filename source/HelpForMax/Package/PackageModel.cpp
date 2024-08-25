#include "PackageModel.h"

#include <QDir>
#include <QFileInfo>
#include <QSettings>

#include "PackageInfo.h"

Package::Model::Model(QObject* parent)
   : QStandardItemModel(parent)
{
}

void Package::Model::create(const Info* info)
{
   setHorizontalHeaderLabels({"Folder / Patch Name"});

   using FolderMap = QMap<QString, QStandardItem*>;
   FolderMap folderMap;

   QDir packageDir(info->getPath() + "/patchers");
   for (const QFileInfo& folderInfo : packageDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name))
   {
      QDir patcherDir(folderInfo.canonicalFilePath());
      for (const QFileInfo& patchInfo : patcherDir.entryInfoList(QDir::Files, QDir::Name))
      {
         if (!patchInfo.fileName().endsWith(".maxpat"))
            continue;

         if (patchInfo.fileName().startsWith("_"))
            continue;

         const QString patchPath = patchInfo.absoluteFilePath();
         const QString patchName = patchInfo.fileName().replace(".maxpat", "");

         QString dirName = patchInfo.absolutePath();
         int index = dirName.lastIndexOf("/");
         if (index < 0)
            continue;
         dirName = dirName.mid(index + 1);

         if (!folderMap.contains(dirName))
         {
            QStandardItem* dirItem = new QStandardItem(dirName);
            dirItem->setEditable(false);
            invisibleRootItem()->appendRow(dirItem);

            folderMap[dirName] = dirItem;
         }

         QStandardItem* dirItem = folderMap[dirName];

         QStandardItem* patchItem = new QStandardItem(patchName);
         patchItem->setEditable(false);
         dirItem->appendRow(patchItem);

         patchItem->setData(patchPath);
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
         if (patchItem->data().toString() == patchFileName)
            return patchItem->index();
      }
   }

   return QModelIndex();
}
