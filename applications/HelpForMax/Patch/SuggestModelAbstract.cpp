#include "SuggestModelAbstract.h"

Suggest::Model::Abstract::Abstract(QObject* parent, Max::RefStructure& structure, const Max::RefStructure::PatchPart& part)
   : QStandardItemModel(parent)
   , structure(structure)
   , part(part)
{
}
