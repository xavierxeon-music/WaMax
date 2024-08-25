#ifndef HelpForMaxH
#define HelpForMaxH

#include <QString>

class HelpForMax
{
public:
   inline HelpForMax();

public:
   inline static QString compileSockerName();
   inline static bool isServerActive();
   inline static void startApplication();
};

#ifndef HelpForMaxHPP
#include "HelpForMax.hpp"
#endif // NOT HelpForMaxHPP

#endif // NOT HelpForMaxH
