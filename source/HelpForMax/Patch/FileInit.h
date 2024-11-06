#ifndef FileInitH
#define FileInitH

#include "FileAbstract.h"

namespace File
{
   class Init : public Abstract
   {
   public:
      Init(Patch::Structure* structure, const Package::Info* info);

   public:
      void write(const Patch::Info& patchInfo) override;
      QString getFilePath(const Patch::Info& patchInfo) override;
   };
} // namespace File

#endif // NOT FileInitH
