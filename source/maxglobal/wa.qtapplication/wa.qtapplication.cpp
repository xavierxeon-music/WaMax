#include "wa.qtapplication.h"

#include <QApplication>

#include <MaxPatcher.h>

#include "ext_proto_win.h"

QtApplication::QtApplication(const atoms& args)
   : object<QtApplication>()
   , eventLoopTimer{this, Max::Patcher::minBind(this, &QtApplication::timerFunction)}
{
#ifdef WIN_VERSION
   HWND handle = c74::max::main_get_client();
   RegisterTouchWindow(handle, 0);
#endif // WIN_VERSION

   cout << "qt application created" << endl;

   eventLoopTimer.delay(100);
}

QtApplication::~QtApplication()
{
   eventLoopTimer.stop();
}

atoms QtApplication::timerFunction(const atoms& args, const int inlet)
{
   QApplication::processEvents();

   eventLoopTimer.delay(1);
   return {};
}

// main function

void ext_main(void* moduleRef)
{
   if (!QApplication::instance())
   {
      static int __argc = 0;

      static char name[] = "foo";
      static char* __argv[] = {name};

      new QApplication(__argc, __argv);
   }

   c74::min::wrap_as_max_external<QtApplication>("QtApplication", __FILE__, moduleRef);
}
