#include "PatchModelHeader.h"

Patch::Model::Header::Header(QObject* parent, RefStructure* structure)
   : Abstract(parent, structure, RefStructure::PatchPart::Header)
{
}

void Patch::Model::Header::update()
{
   QStandardItem* typeItem = invisibleRootItem()->child(0, 0);
   QStandardItem* digestItem = invisibleRootItem()->child(0, 1);

   typeItem->setText(RefStructure::patchTypeName(structure->header.patcherType));
   updateDigestItem(digestItem, structure->header.digest);

   emit signalDataEdited();
}

void Patch::Model::Header::rebuild()
{
   beginResetModel();
   clear();
   setHorizontalHeaderLabels({"PatchType", "Digest"});

   QStandardItem* typeItem = new QStandardItem();

   QStandardItem* digestItem = new QStandardItem();
   digestItem->setEditable(false);

   invisibleRootItem()->appendRow({typeItem, digestItem});
   endResetModel();

   update();
}

Patch::RefStructure::Digest* Patch::Model::Header::getDigest(const QModelIndex& index)
{
   Q_UNUSED(index)

   return &(structure->header.digest);
}

bool Patch::Model::Header::setData(const QModelIndex& index, const QVariant& value, int role)
{
   const bool result = QStandardItemModel::setData(index, value, role);
   if (Qt::EditRole == role)
   {
      if (0 == index.column())
      {
         RefStructure::PatchType patchType = RefStructure::toPatchType(value.toString());
         structure->header.patcherType = patchType;
         structure->setDirty();
      }
   }

   return result;
}

Patch::RefStructure::PatchType Patch::Model::Header::getPatchType(const int index)
{
   Q_UNUSED(index)

   return structure->header.patcherType;
}
