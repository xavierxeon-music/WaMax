#include "SuggestModelOutput.h"

Suggest::Model::Output::Output(QObject* parent, Ref::Structure& structure)
   : Abstract(parent, structure, Ref::Structure::PatchPart::Output)
{
}

void Suggest::Model::Output::rebuild()
{
}
