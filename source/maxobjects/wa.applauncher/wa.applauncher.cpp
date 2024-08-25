#include "wa.applauncher.h"

#include <QDir>
#include <QFileInfo>
#include <QHostInfo>

#include "helper.h"
#include <common.h>

#if defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#include <libgen.h>
#endif

QString AppLauncher::packagePath;

AppLauncher::AppLauncher(const atoms& args)
   : object<AppLauncher>()
   , input(this, "messages")
   , outputA(this, "app output")
   , outputB(this, "request output")
   , openFile(this, "openFile", "open file", Patcher::minBind(this, &AppLauncher::openFileFunction))
   , openApp(this, "openApp", "open application", Patcher::minBind(this, &AppLauncher::openAppFunction))
   , lanch(this, "lanch", "launch application", Patcher::minBind(this, &AppLauncher::launchFunction))
   , python(this, "python", "open python script", Patcher::minBind(this, &AppLauncher::pyhonFunction))
   , package(this, "package", "get the package path", Patcher::minBind(this, &AppLauncher::packagePathFunction))
   , hostname(this, "hostname", "get hostname", Patcher::minBind(this, &AppLauncher::hostnameFunction))
{
}

void AppLauncher::setPackagePath(const char* external_path)
{
   QFileInfo patchInfo(external_path);
   for (QDir dir = patchInfo.dir(); !dir.isRoot(); dir.cdUp())
   {
      const QFileInfoList content = dir.entryInfoList(QDir::Files);
      for (const QFileInfo& contentInfo : content)
      {
         if ("package-info.json" != contentInfo.fileName())
            continue;

         packagePath = contentInfo.dir().absolutePath();
         break;
      }
   }
}

atoms AppLauncher::openFileFunction(const atoms& args, const int inlet)
{
   if (1 > args.size())
      return {};

   const std::string fileName = args[0];
   const std::string appName = (2 == args.size()) ? args[1] : "";
   const Helper::OpenState state = Helper::openFile(QString::fromStdString(fileName), QString::fromStdString(appName));

   if (state == Helper::OpenState::FileNotExist)
      cerr << fileName << " does not exist" << endl;
   else if (state == Helper::OpenState::AlreadyOpen)
      cout << fileName << " already open in " << appName << endl;
   else if (state == Helper::OpenState::Done)
      cout << fileName << " opened in " << appName << endl;

   return {};
}

atoms AppLauncher::openAppFunction(const atoms& args, const int inlet)
{
   if (1 > args.size())
      return {};

   const std::string appName = args[0];
   Helper::openApp(QString::fromStdString(appName));

   return {};
}

atoms AppLauncher::launchFunction(const atoms& args, const int inlet)
{
   if (1 > args.size())
      return {};

   QString appName;
   QStringList argumentList;
   for (size_t i = 0; i < args.size(); i++)
   {
      const QString argument = QString::fromStdString(args[i]);
      if (0 == i)
         appName = argument;
      else
         argumentList.append(argument);
   }

   Helper::launch(appName, argumentList);

   return {};
}

atoms AppLauncher::pyhonFunction(const atoms& args, const int inlet)
{
   if (1 > args.size())
      return {};

   QStringList argumentList;
   for (auto i = 0; i < args.size(); i++)
   {
      const QString argument = QString::fromStdString(args[i]);
      argumentList.append(argument);
   }

   Helper::python(argumentList);

   return {};
}

atoms AppLauncher::packagePathFunction(const atoms& args, const int inlet)
{
   outputB.send(packagePath.toStdString());
   return {};
}

atoms AppLauncher::hostnameFunction(const atoms& args, const int inlet)
{
   outputB.send(QHostInfo::localHostName().toStdString());
   return {};
}

// main

void ext_main(void* moduleRef)
{
   c74::min::wrap_as_max_external<AppLauncher>("AppLauncher", __FILE__, moduleRef);

#if defined(__APPLE__)
   CFURLRef ext_url_ref = CFBundleCopyBundleURL((CFBundleRef)moduleRef);
   CFStringRef mac_path = CFURLCopyFileSystemPath(ext_url_ref, kCFURLPOSIXPathStyle);
   const char* external_path = CFStringGetCStringPtr(mac_path, CFStringGetSystemEncoding());

   AppLauncher::setPackagePath(external_path);

   CFRelease(ext_url_ref);
   CFRelease(mac_path);
#else
   const char* external_path[256];
   GetModuleFileName(moduleRef, (LPCH)external_path, sizeof(external_path));

   AppLauncher::setPackagePath(external_path);
#endif
}
