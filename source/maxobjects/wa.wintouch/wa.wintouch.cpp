#include "wa.wintouch.h"

#include <windows.h>

#include <QAbstractEventDispatcher>
#include <QByteArray>
#include <QCoreApplication>

#include <MaxPatcher.h>

// see https://learn.microsoft.com/en-us/windows/win32/wintouch/wm-touchdown
// see https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-msg
// see https://learn.microsoft.com/en-us/windows/win32/wintouch/getting-started-with-multi-touch-messages

WinTouch::WinTouch(const atoms& args)
   : object<WinTouch>()
   , QAbstractNativeEventFilter()
   , inputMessage(this, "message")
   , outputTouch(this, "touch")
   , rect(this, "rect", "client rectangle", Max::Patcher::minBind(this, &WinTouch::rectFunction))
{
   QAbstractEventDispatcher::instance()->installNativeEventFilter(this);
}

atoms WinTouch::rectFunction(const atoms& args, const int inlet)
{
   if (4 == args.size())
   {
      const int x = args[0];
      const int y = args[1];
      const int width = args[2];
      const int height = args[3];

      cout << "RECT " << x << " " << y << " " << width << " " << height << endl;
   }
   return {};
}

bool WinTouch::nativeEventFilter(const QByteArray& eventType, void* message, qintptr*)
{
   MSG* msg = static_cast<MSG*>(message);
   switch (msg->message)
   {
      case WM_GESTURE:
         cout << "WM_GESTURE" << endl;
         break;
      case WM_TOUCH:
         cout << "WM_TOUCH" << endl;
         break;
      default:
         cout << "message " << msg->message << endl;
         break;
   }

   return false;
}

MIN_EXTERNAL(WinTouch);
