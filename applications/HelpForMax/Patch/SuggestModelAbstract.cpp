#include "SuggestModelAbstract.h"

Suggest::Model::Abstract::Abstract(QObject* parent, Ref::Structure& structure, const Ref::Structure::PatchPart& part)
   : QStandardItemModel(parent)
   , structure(structure)
   , part(part)
{
}
