#include "SuggestModelOutput.h"

Suggest::Model::Output::Output(QObject* parent, Max::RefStructure &structure)
   : Abstract(parent, structure, Max::RefStructure::PatchPart::Output)
{
}
