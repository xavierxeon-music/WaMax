#include "SuggestModelArgument.h"

Suggest::Model::Argument::Argument(QObject* parent, Ref::Structure& structure)
   : Abstract(parent, structure, Ref::Structure::PatchPart::Argument)
   , Delegate::DataType::Source()
{
}

Max::DataType Suggest::Model::Argument::getDataType(const int index)
{
   const Ref::Structure::Argument& argument = structure.argumentList.at(index);

   return argument.dataType;
}
