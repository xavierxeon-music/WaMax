#include "helper.h"

#include <Carbon/Carbon.h>

#include <QFileInfo>
#include <QProcess>
#include <QThread>

QString clean(const QString& name)
{
   const int slashIndex = name.indexOf("/", 1);
   const QString colonTest = (-1 != slashIndex) ? name.mid(slashIndex - 1, 1) : "";
   if (":" != colonTest)
      return name;

   const QString front = name.mid(0, slashIndex - 1);
   const QString back = name.mid(slashIndex);

   return "/Volumes/" + front + back;
}

void Helper::launch(const QString& appPath, const QStringList& arguments)
{
   QStringList args;
   for (QString arg : arguments)
   {
      args.append(arg);
   }

   QProcess::startDetached(appPath, args);
   QThread::sleep(1);
}

void Helper::python(const QStringList& arguments)
{
   launch("/opt/homebrew/bin/python3", arguments);
}

Helper::OpenState Helper::openFile(const QString& path, const QString& appName)
{
   QFileInfo info(clean(path));
   if (!info.exists())
      return OpenState::FileNotExist;

   if (!appName.isEmpty())
   {
      QStringList appWindows = getAppWindowTitles(appName);
      if (appWindows.contains(info.baseName()))
      {
         return OpenState::AlreadyOpen;
      }
   }

   launch("/usr/bin/open", {info.absoluteFilePath()});

   return OpenState::Done;
}

void Helper::openApp(const QString& appName)
{
   launch("/usr/bin/open", {"-a", appName});
}

QStringList Helper::getAppWindowTitles(const QString& appName)
{
   CFArrayRef windowList = CGWindowListCopyWindowInfo(kCGWindowListOptionAll, kCGNullWindowID);

   QStringList titles;

   const int windowsCount = CFArrayGetCount(windowList);
   for (int i = 0; i < windowsCount; i++)
   {
      const CFDictionaryRef ref = static_cast<CFDictionaryRef>(CFArrayGetValueAtIndex(windowList, i));

      const CFStringRef ownerRef = static_cast<CFStringRef>(CFDictionaryGetValue(ref, kCGWindowOwnerName));
      const QString owner = QString::fromCFString(ownerRef);
      if (owner != appName)
         continue;

      const CFStringRef nameRef = static_cast<CFStringRef>(CFDictionaryGetValue(ref, kCGWindowName));
      const QString name = QString::fromCFString(nameRef);

      if (!name.isEmpty())
         titles.append(name);
   }

   return titles;
}
