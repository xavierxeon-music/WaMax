#include "SuggestModelAbstract.h"

Suggest::Model::Abstract::Abstract(QObject* parent, Patch::RefStructure* structure, const Patch::RefStructure::PatchPart& part)
   : QStandardItemModel(parent)
   , structure(structure)
   , part(part)
{
}
