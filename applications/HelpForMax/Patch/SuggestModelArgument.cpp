#include "SuggestModelArgument.h"

Suggest::Model::Argument::Argument(QObject* parent, Max::RefStructure& structure)
   : Abstract(parent, structure, Max::RefStructure::PatchPart::Argument)
   , Delegate::DataType::Source()
{
}

Max::DataType Suggest::Model::Argument::getDataType(const int index)
{
   const Max::RefStructure::Argument& argument = structure.argumentList.at(index);

   return argument.dataType;
}
