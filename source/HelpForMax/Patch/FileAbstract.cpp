#include "FileAbstract.h"

File::Abstract::Abstract(const Package::Info* info, Patch::RefStructure* structure)
   : info(info)
   , structure(structure)
{
}

void File::Abstract::read(const Patch::Info& patchInfo)
{
   Q_UNUSED(patchInfo)
}
