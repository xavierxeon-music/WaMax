#include "SuggestModelNamedMessage.h"

Suggest::Model::NamedMessage::NamedMessage(QObject* parent, Patch::RefStructure* structure)
   : Abstract(parent, structure, Patch::RefStructure::PatchPart::MessageNamed)

{
}

Max::DataType Suggest::Model::NamedMessage::getDataType(const int index)
{
   QStandardItem* nameItem = invisibleRootItem()->child(index, 0);
   const Patch::RefStructure::AttributesAndMessageNamed& messageNamed = structure->messageNamedMap.value(nameItem->text());

   return messageNamed.dataType;
}
