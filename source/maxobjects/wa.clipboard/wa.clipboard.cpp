#include "wa.clipboard.h"

#include <MaxPatcher.h>

#include "SystemClipboard.h"

Clipboard::Clipboard(const atoms& args)
   : object<Clipboard>()
   , input(this, "content")
   , content(this, "content")
   , status(this, "status")
   , bangMessage(this, "bang", "read from clipboard", Max::Patcher::minBind(this, &Clipboard::getFromClipboardFunction))
   , doubleClickedMessage(this, "dblclick", "read from clipboard", Max::Patcher::minBind(this, &Clipboard::getFromClipboardFunction))
   , textMessage(this, "set", "copy to clipboard", Max::Patcher::minBind(this, &Clipboard::sendToClipboardFunction))
   , updateTimer{this, Max::Patcher::minBind(this, &Clipboard::timerFunction)}
   , buffer()
{
   updateTimer.delay(1000);
}

atoms Clipboard::sendToClipboardFunction(const atoms& args, const int inlet)
{
   std::string text = args[0];
   for (auto i = 1; i < args.size(); i++)
   {
      const std::string part = args[i];
      text = text + " " + part;
   }

   if (text.empty())
      return {};

   SystemClipboard::write(text);
   return {};
}

atoms Clipboard::getFromClipboardFunction(const atoms& args, const int inlet)
{
   content.send(buffer);
   status.send(0);
   return {};
}

atoms Clipboard::timerFunction(const atoms& args, const int inlet)
{
   std::string text = SystemClipboard::read();
   if (buffer != text)
   {
      buffer = text;
      status.send(1);
   }

   updateTimer.delay(100);
   return {};
}

MIN_EXTERNAL(Clipboard);
