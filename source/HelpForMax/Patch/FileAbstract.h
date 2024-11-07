#ifndef FileAbstractH
#define FileAbstractH

#include "PatchRefStructure.h"

namespace Package
{
   class Info;
}

namespace File
{
   class Abstract
   {
   public:
      Abstract(const Package::Info* info, Patch::RefStructure* structure);

   public:
      virtual void read(const Patch::Info& patchInfo);
      virtual void write(const Patch::Info& patchInfo) = 0;
      virtual QString getFilePath(const Patch::Info& patchInfo) = 0;

   protected:
      const Package::Info* info;
      Patch::RefStructure* structure;
   };
} // namespace File

#endif // NOT FileAbstractH
