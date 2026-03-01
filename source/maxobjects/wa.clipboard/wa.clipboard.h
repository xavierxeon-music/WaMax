#ifndef ClipboardH
#define ClipboardH

#include "c74_min.h"
using namespace c74::min;

class Clipboard : public object<Clipboard>
{
public:
   MIN_DESCRIPTION{"copy and paste text to and from the system clipboard"};

public:
   Clipboard(const atoms& args = {});

private:
   atoms sendToClipboardFunction(const atoms& args, const int inlet);
   atoms getFromClipboardFunction(const atoms& args, const int inlet);
   atoms timerFunction(const atoms& args, const int inlet);

private:
   inlet<> input;
   outlet<> content;
   outlet<> status;

   message<> bangMessage;
   message<> doubleClickedMessage;
   message<> textMessage;

   timer<timer_options::defer_delivery> updateTimer;

   std::string buffer;
};

#endif // ClipboardH
