#include "PatchModelNamedMessage.h"

Patch::Model::NamedMessage::NamedMessage(QObject* parent, Structure* structure)
   : Abstract(parent, structure, Structure::PatchPart::MessageNamed)
{
}

void Patch::Model::NamedMessage::update()
{
   int row = 0;
   for (Structure::AttributesAndMessageNamed::Map::const_iterator it = structure->messageNamedMap.constBegin(); it != structure->messageNamedMap.constEnd(); it++)
   {
      QStandardItem* nameItem = invisibleRootItem()->child(row, 0);
      QStandardItem* typeItem = invisibleRootItem()->child(row, 1);
      QStandardItem* isAttributeItem = invisibleRootItem()->child(row, 2);
      QStandardItem* isMessageItem = invisibleRootItem()->child(row, 3);
      QStandardItem* digestItem = invisibleRootItem()->child(row, 4);

      if (!nameItem)
         continue;

      const Structure::AttributesAndMessageNamed& messageNamed = it.value();

      nameItem->setText(messageNamed.name);
      nameItem->setData(QVariant::fromValue(it));

      typeItem->setText(Structure::dataTypeName(messageNamed.dataType));

      if (0 != (messageNamed.patchParts & Structure::PatchPart::Attribute))
         isAttributeItem->setCheckState(Qt::Checked);
      else
         isAttributeItem->setCheckState(Qt::Unchecked);

      if (0 != (messageNamed.patchParts & Structure::PatchPart::MessageNamed))
         isMessageItem->setCheckState(Qt::Checked);
      else
         isMessageItem->setCheckState(Qt::Unchecked);

      updateDigestItem(digestItem, messageNamed.digest);

      row++;
   }

   emit signalDataEdited();
}

void Patch::Model::NamedMessage::rebuild()
{
   beginResetModel();
   clear();
   setHorizontalHeaderLabels({"Name", "Type", "At", "M", "Digest"});

   for (Structure::AttributesAndMessageNamed::Map::const_iterator it = structure->messageNamedMap.constBegin(); it != structure->messageNamedMap.constEnd(); it++)
   {
      QStandardItem* nameItem = new QStandardItem();
      nameItem->setData(QVariant::fromValue(it));

      QStandardItem* typeItem = new QStandardItem();

      QStandardItem* isAttributeItem = new QStandardItem();
      isAttributeItem->setEditable(false);
      isAttributeItem->setCheckable(true);
      isAttributeItem->setIcon(QIcon(":/DocAttribute.svg"));

      QStandardItem* isMessageItem = new QStandardItem();
      isMessageItem->setEditable(false);
      isMessageItem->setCheckable(true);
      isMessageItem->setIcon(QIcon(":/DocMessageNamed.svg"));

      QStandardItem* digestItem = new QStandardItem();
      digestItem->setEditable(false);

      invisibleRootItem()->appendRow({nameItem, typeItem, isAttributeItem, isMessageItem, digestItem});
   }

   endResetModel();
   update();
}

Patch::Structure::Digest* Patch::Model::NamedMessage::getDigest(const QModelIndex& index)
{
   QStandardItem* nameItem = invisibleRootItem()->child(index.row(), 0);

   Structure::AttributesAndMessageNamed& messageNamed = structure->messageNamedMap[nameItem->text()];
   return &(messageNamed.digest);
}

void Patch::Model::NamedMessage::createBeforeItem(const QModelIndex& index)
{
   Q_UNUSED(index)

   Structure::AttributesAndMessageNamed messageNamed;
   messageNamed.name = "???";

   if (structure->messageNamedMap.contains(messageNamed.name))
      return;

   structure->messageNamedMap.insert(messageNamed.name, messageNamed);
   structure->setDirty();
}

void Patch::Model::NamedMessage::removeItem(const QModelIndex& index)
{
   QStandardItem* nameItem = invisibleRootItem()->child(index.row(), 0);
   Structure::AttributesAndMessageNamed::Map::const_iterator it = nameItem->data().value<Structure::AttributesAndMessageNamed::Map::const_iterator>();

   structure->messageNamedMap.erase(it);
   structure->setDirty();
}

bool Patch::Model::NamedMessage::setData(const QModelIndex& index, const QVariant& value, int role)
{
   QStandardItem* nameItem = invisibleRootItem()->child(index.row(), 0);
   Structure::AttributesAndMessageNamed& messageNamed = structure->messageNamedMap[nameItem->text()];

   qDebug() << nameItem->text() << value.toString();

   const bool result = QStandardItemModel::setData(index, value, role);

   if (Qt::EditRole == role)
   {
      if (0 == index.column())
      {
         messageNamed.name = value.toString();
         structure->repackNamedMessages();
         structure->setDirty();
      }
      else if (1 == index.column())
      {
         messageNamed.dataType = Structure::toDataType(value.toString());
         structure->setDirty();
      }
   }
   else if (Qt::CheckStateRole == role)
   {
      auto setFlag = [&](const Structure::PatchPart& part)
      {
         if (Qt::Checked == value.toInt())
            messageNamed.patchParts |= part;
         else
            messageNamed.patchParts ^= part;
         structure->setDirty();
      };

      if (2 == index.column())
         setFlag(Structure::PatchPart::Attribute);
      else if (3 == index.column())
         setFlag(Structure::PatchPart::MessageNamed);
   }

   emit signalDataEdited();
   return result;
}

Patch::Structure::DataType Patch::Model::NamedMessage::getDataType(const int index)
{
   QStandardItem* nameItem = invisibleRootItem()->child(index, 0);
   const Structure::AttributesAndMessageNamed& messageNamed = structure->messageNamedMap.value(nameItem->text());

   return messageNamed.dataType;
}
