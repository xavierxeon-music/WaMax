#include "SuggestModelNamedMessage.h"

Suggest::Model::NamedMessage::NamedMessage(QObject* parent, Max::RefStructure& structure)
   : Abstract(parent, structure, Max::RefStructure::PatchPart::MessageNamed)

{
}

Max::DataType Suggest::Model::NamedMessage::getDataType(const int index)
{
   QStandardItem* nameItem = invisibleRootItem()->child(index, 0);
   const Max::RefStructure::AttributesAndMessageNamed& messageNamed = structure.messageNamedMap.value(nameItem->text());

   return messageNamed.dataType;
}
