#include "SuggestModelTypedMessage.h"

Suggest::Model::TypedMessage::TypedMessage(QObject* parent, Max::RefStructure &structure)
   : Abstract(parent, structure, Max::RefStructure::PatchPart::MessageTyped)
{
}
