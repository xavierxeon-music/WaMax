
#ifndef HelpFileH
#define HelpFileH

// keep order
#include "c74_min.h"
using namespace c74::min;
using namespace c74::min::ui;
#include <CommonQt.h>

#include "PatchInfo.h"

#include <QLocalSocket>

class HelpFile : public object<HelpFile>, public ui_operator<40, 40>
{
public:
   MIN_DESCRIPTION{"open help editor"};

public:
   HelpFile(const atoms& args = {});

private:
   atoms paintFunction(const atoms& args, const int inlet);
   atoms mouseDoubleClickFunction(const atoms& args, const int inlet);
   atoms setFileFunction(const atoms& args, const int inlet);
   atoms timerFunction(const atoms& args, const int inlet);

   void sendData();
   void receiveData();

private:
   QLocalSocket socket;
   PatchInfo currentPatch;

   message<> paint;
   message<> dblclick;
   message<> setfile;
   timer<timer_options::defer_delivery> loopTimer;
};

#endif // NOT HelpFileH
