#include "FileAbstract.h"

File::Abstract::Abstract(Patch::Structure* structure, const Package::Info* info)
   : structure(structure)
   , info(info)
{
}

void File::Abstract::read(const Patch::Info& patchInfo)
{
   Q_UNUSED(patchInfo)
}
