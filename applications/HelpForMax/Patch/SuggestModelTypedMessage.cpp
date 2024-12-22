#include "SuggestModelTypedMessage.h"

Suggest::Model::TypedMessage::TypedMessage(QObject* parent, Ref::Structure& structure)
   : Abstract(parent, structure, Ref::Structure::PatchPart::MessageTyped)
{
}

void Suggest::Model::TypedMessage::rebuild()
{
   beginResetModel();
   setHorizontalHeaderLabels({"Type", "Active"});

   while (0 < invisibleRootItem()->rowCount())
      invisibleRootItem()->removeRow(0);

   for (const Max::DataType& type : Max::dataTypeList())
   {
      Ref::Structure::MessageTyped& message = structure.messageTypedMap[type];

      QStandardItem* typeItem = new QStandardItem(Max::dataTypeName(type));
      typeItem->setEditable(false);
      typeItem->setData(QVariant::fromValue(type));

      QStandardItem* activeItem = new QStandardItem();
      activeItem->setEditable(false);
      activeItem->setCheckable(false);
      activeItem->setCheckState(message.active ? Qt::Checked : Qt::Unchecked);
      activeItem->setIcon(QIcon(":/DocMessageTyped.svg"));

      invisibleRootItem()->appendRow({typeItem, activeItem});
   }

   endResetModel();
}
