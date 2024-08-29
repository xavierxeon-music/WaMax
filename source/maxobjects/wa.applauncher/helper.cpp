#include "helper.h"

#include <QThread>

Helper::Helper()
   : process()
{
}

void Helper::launchDetached(const QString& appPath, const QStringList& arguments)
{
   QStringList args;
   for (QString arg : arguments)
   {
      args.append(arg);
   }

   process.startDetached(appPath, args);
   QThread::sleep(1);
}

QString Helper::clean(const QString& name) const
{
   const int slashIndex = name.indexOf("/", 1);
   const QString colonTest = (-1 != slashIndex) ? name.mid(slashIndex - 1, 1) : "";
   if (":" != colonTest)
      return name;

   const QString front = name.mid(0, slashIndex - 1);
   const QString back = name.mid(slashIndex);

   return "/Volumes/" + front + back;
}
