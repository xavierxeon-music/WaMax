#ifndef PatchInfoH
#define PatchInfoH

#include <QString>

class PatchInfo
{
public:
   enum class State
   {
      Initial,
      NotInPackage,
      HelpFileOutdated,
      UpToDate
   };

public:
   PatchInfo(const QString& patchPath);

public:
   bool checkState();

public:
   State state = State::Initial;
   QString patchPath;
   QString helpPath;
   QString packagePath;
};

#endif // NOT PatchInfoH
