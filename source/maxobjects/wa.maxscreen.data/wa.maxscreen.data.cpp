#include "wa.maxscreen.data.h"

#include <MaxPatcher.h>

MaxScreenData::MaxScreenData(const atoms& args)
   : object<MaxScreenData>()
   , doubleClickMessage{this, "dblclick", Max::Patcher::minBind(this, &MaxScreenData::doubleClickFunction)}
{
}

atoms MaxScreenData::doubleClickFunction(const atoms& args, const int inlet)
{
   cout << "double click" << endl;
   return {};
}

MIN_EXTERNAL(MaxScreenData);
