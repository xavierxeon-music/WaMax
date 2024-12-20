#include "SuggestModelTypedMessage.h"

Suggest::Model::TypedMessage::TypedMessage(QObject* parent, Patch::RefStructure* structure)
   : Abstract(parent, structure, Patch::RefStructure::PatchPart::MessageTyped)
{
}
