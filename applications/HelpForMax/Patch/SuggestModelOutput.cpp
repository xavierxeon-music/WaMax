#include "SuggestModelOutput.h"

Suggest::Model::Output::Output(QObject* parent, Patch::RefStructure* structure)
   : Abstract(parent, structure, Patch::RefStructure::PatchPart::Output)
{
}
