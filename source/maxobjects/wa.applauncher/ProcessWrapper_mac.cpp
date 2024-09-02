#include "ProcessWrapper.h"

#include <Carbon/Carbon.h>

#include <QFileInfo>

void ProcessWrapper::python(const QStringList& arguments)
{
   launch("/opt/homebrew/bin/python3", arguments);
}

ProcessWrapper::ProcessWrapper Helper::openFileWithApp(const QString& path, const QString& appName)
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

   launchDetached("/usr/bin/open", {info.absoluteFilePath()});

   return OpenState::Done;
}

void ProcessWrapper::openApp(const QString& appName)
{
   launchDetached("/usr/bin/open", {"-a", appName});
}

QStringList ProcessWrapper::getAppWindowTitles(const QString& appName)
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
