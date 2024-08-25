#ifndef CurrentH
#define CurrentH

#include <QString>

class Current
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
   Current(const QString& patchPath);

public:
   bool checkState();

public:
   State state = State::Initial;
   QString patchPath;
   QString helpPath;
   QString packagePath;
};

#endif // NOT CurrentH
