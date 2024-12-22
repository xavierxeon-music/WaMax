#include "SuggestModelTypedMessage.h"

Suggest::Model::TypedMessage::TypedMessage(QObject* parent, Ref::Structure& structure)
   : Abstract(parent, structure, Ref::Structure::PatchPart::MessageTyped)
{
}

void Suggest::Model::TypedMessage::rebuild()
{
}
