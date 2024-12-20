#include "SuggestModelArgument.h"

Suggest::Model::Argument::Argument(QObject* parent, Patch::RefStructure* structure)
   : Abstract(parent, structure, Patch::RefStructure::PatchPart::Argument)
   , Delegate::DataType::Source()
{
}

Max::DataType Suggest::Model::Argument::getDataType(const int index)
{
   const Patch::RefStructure::Argument& argument = structure->argumentList.at(index);

   return argument.dataType;
}
