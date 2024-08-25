#include "FileAbstract.h"

File::Abstract::Abstract(Patch::Structure* structure, const Package::Info* info)
   : structure(structure)
   , info(info)
{
}

void File::Abstract::read(const QString& patchName)
{
   Q_UNUSED(patchName)
}
