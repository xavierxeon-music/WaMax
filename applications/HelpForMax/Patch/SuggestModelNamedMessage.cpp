#include "SuggestModelNamedMessage.h"

Suggest::Model::NamedMessage::NamedMessage(QObject* parent, Ref::Structure& structure)
   : Abstract(parent, structure, Ref::Structure::PatchPart::MessageNamed)

{
}

Max::DataType Suggest::Model::NamedMessage::getDataType(const int index)
{
   QStandardItem* nameItem = invisibleRootItem()->child(index, 0);
   const Ref::Structure::AttributesAndMessageNamed& messageNamed = structure.messageNamedMap.value(nameItem->text());

   return messageNamed.dataType;
}
